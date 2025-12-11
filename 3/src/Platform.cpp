/**
 * @file Platform.cpp
 * @brief Implementation of Platform logic (Tick, Move, Install).
 */

#include "Platform.hpp"
#include "Environment.hpp" 

Platform::Platform(Coordinate pos, std::string desc, double energy, int slots)
    : position(pos), description(desc), energyLevel(energy), totalSlots(slots) {
}

Coordinate Platform::getPosition() const { return position; }
void Platform::setPosition(Coordinate c) { position = c; }
double Platform::getEnergyLevel() const { return energyLevel; }
const std::string& Platform::getDescription() const { return description; }

bool Platform::installModule(std::shared_ptr<Module> m) {
    if (modules.size() + m->getSlotsRequired() <= totalSlots) {
        modules.pushBack(m);
        return true;
    }
    return false;
}

const Vector<std::shared_ptr<Module>>& Platform::getAllModules() const { return modules; }

void Platform::tick(const Environment& env) {
    if (env.getCellType(position) == GridCellType::CHARGER) {
        energyLevel += 50.0;
        if (energyLevel > 2000.0) energyLevel = 2000.0;
    }

    for (auto& m : modules) {
        double c = m->getConsumption();
        if (energyLevel >= c) {
            energyLevel -= c;
            m->update(env, position);
        }
        else { energyLevel = 0; m->deactivate(); }
    }
}

StationaryPlatform::StationaryPlatform(Coordinate pos) : Platform(pos, "Stationary", 1000.0, 5) {}
bool StationaryPlatform::isConnectedToAI() const {
    return true;
}

MobilePlatform::MobilePlatform(Coordinate pos, double s) : Platform(pos, "Mobile", 300.0, 3), speed(s) {}
bool MobilePlatform::isConnectedToAI() const {
    for (auto& m : modules) {
        if (auto net = std::dynamic_pointer_cast<NetworkModule>(m)) {
            if (net->isEnabled() && !net->getSessions().empty()) return true;
        }
    }
    return false;
}

void MobilePlatform::move(const Environment& env, Coordinate target) {
    if (energyLevel < 10) return;
    if (position == target) return;
    int dx = (target.getX() > position.getX()) ? 1 : ((target.getX() < position.getX()) ? -1 : 0);
    int dy = (target.getY() > position.getY()) ? 1 : ((target.getY() < position.getY()) ? -1 : 0);

    Coordinate next(position.getX() + dx, position.getY() + dy);

    GridCellType cell = env.getCellType(next);
    if (cell == GridCellType::EMPTY || cell == GridCellType::CHARGER || cell == GridCellType::INTRUDER) {
        position = next;
        energyLevel -= 2.0;
    }
}