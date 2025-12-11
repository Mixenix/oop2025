#include "AI.hpp"
#include <future>
#include <algorithm>

AI::AI(Environment* env) : environment(env) {}

void AI::updateKnowledge() {
    knowledgeBase.clear();
    for (auto& plat : environment->platforms) {
        if (!plat->isConnectedToAI()) continue;
        auto sensors = plat->getModules<SensorModule>();
        for (auto& s : sensors) {
            const auto& readings = s->getReadings();
            for (const auto& coord : readings) {
                knowledgeBase.insert(coord);
            }
        }
    }
}


void AI::updateKnowledgeMT() {
    knowledgeBase.clear();

    size_t count = environment->platforms.size();
    unsigned int nThreads = std::thread::hardware_concurrency();
    if (nThreads == 0) nThreads = 2;
    size_t chunkSize = (count + nThreads - 1) / nThreads;

    Vector<std::future<std::vector<Coordinate>>> futures;

    auto worker = [&](size_t start, size_t end) {
        std::vector<Coordinate> localReadings;
        for (size_t i = start; i < end; ++i) {
            auto& plat = environment->platforms[i];
            if (!plat->isConnectedToAI()) continue;
            auto sensors = plat->getModules<SensorModule>();
            for (auto& s : sensors) {
                const auto& readings = s->getReadings();
                for (const auto& coord : readings) localReadings.push_back(coord);
            }
        }
        return localReadings;
        };

    for (size_t i = 0; i < count; i += chunkSize) {
        size_t end = std::min(i + chunkSize, count);
        futures.pushBack(std::async(std::launch::async, worker, i, end));
    }


    for (auto& f : futures) {
        auto vec = f.get();
        for (const auto& c : vec) knowledgeBase.insert(c);
    }
}

void AI::commandMobileUnits() {
    for (auto& plat : environment->platforms) {
        auto mobile = std::dynamic_pointer_cast<MobilePlatform>(plat);
        if (!mobile) continue;
        if (mobile->getEnergyLevel() < 50.0) {
            Coordinate charger = environment->findNearestCharger(mobile->getPosition());
            if (charger.getX() != -1) {
                mobile->move(*environment, charger);
                continue;
            }
        }
        double minDist = 1e9;
        Coordinate target = mobile->getPosition();
        bool foundTarget = false;
        for (const auto& k : knowledgeBase) {
            double d = mobile->getPosition().distanceTo(k);
            if (d < minDist) {
                minDist = d;
                target = k;
                foundTarget = true;
            }
        }
        if (foundTarget) mobile->move(*environment, target);
    }
}


void AI::neutralizeAllIntruders() {
    updateKnowledge();
    commandMobileUnits();

    for (auto& platform : environment->platforms) {
        if (!platform->isConnectedToAI()) continue;

        auto weapons = platform->getModules<WeaponModule>();
        for (auto& weapon : weapons) {
            if (!weapon->isEnabled()) weapon->activate(platform->getEnergyLevel());

            for (auto& intruder : environment->intruders) {
                if (intruder->isNeutralized()) continue;
                if (knowledgeBase.find(intruder->getPosition()) != knowledgeBase.end()) {
                    weapon->neutralize(intruder.get(), platform->getPosition());
                }
            }
        }
    }
    environment->refreshGrid();
}


void AI::neutralizeAllIntrudersMT() {
    updateKnowledgeMT();
    commandMobileUnits();

    size_t count = environment->platforms.size();
    if (count == 0) return;

    unsigned int nThreads = std::thread::hardware_concurrency();
    if (nThreads == 0) nThreads = 2;
    size_t chunkSize = (count + nThreads - 1) / nThreads;

    Vector<std::future<void>> futures;

    auto worker = [&](size_t start, size_t end) {
        for (size_t i = start; i < end; ++i) {
            auto& platform = environment->platforms[i];
            if (!platform->isConnectedToAI()) continue;

            auto weapons = platform->getModules<WeaponModule>();
            for (auto& weapon : weapons) {
                if (!weapon->isEnabled()) weapon->activate(platform->getEnergyLevel());

                for (auto& intruder : environment->intruders) {
                    if (intruder->isNeutralized()) continue;
                    if (knowledgeBase.find(intruder->getPosition()) != knowledgeBase.end()) {
                        weapon->neutralize(intruder.get(), platform->getPosition());
                    }
                }
            }
        }
        };

    for (size_t i = 0; i < count; i += chunkSize) {
        size_t end = std::min(i + chunkSize, count);
        futures.pushBack(std::async(std::launch::async, worker, i, end));
    }

    for (auto& f : futures) f.wait();

    environment->refreshGrid();
}