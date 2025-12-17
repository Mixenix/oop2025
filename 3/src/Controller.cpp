/**
 * @file Controller.cpp
 * @brief Implementation of the main application logic and command parsing.
 */

#include "Controller.hpp"

Controller::Controller() {
    env = std::make_unique<Environment>(10, 10);
    view = std::make_unique<ConsoleView>();
    ai = std::make_unique<AI>(env.get());

    auto p1 = std::make_shared<StationaryPlatform>(Coordinate(2, 2));
    p1->installModule(std::make_shared<WeaponModule>(5.0));
    p1->installModule(std::make_shared<NetworkModule>(5.0));
    env->platforms.pushBack(p1);

    env->setCellType({ 0,0 }, GridCellType::CHARGER);
    env->refreshGrid();
}

void Controller::help() {
    view->message("Commands:");
    view->message("  step                      - Advance simulation 1 step");
    view->message("  auto <n>                  - Advance simulation n steps");
    view->message("  add_intruder <x> <y> <t>  - Add intruder (t: 0=Rand, 1=Knight)");
    view->message("  add_plat <x> <y> <type>   - Add platform (0=Stat, 1=Mob)");
    view->message("  add_mod <pid> <type>      - Add module (0=Net, 1=Sens, 2=Wep)");
    view->message("  add_obs <x> <y>           - Add obstacle");
    view->message("  add_char <x> <y>          - Add charger");
    view->message("  resize <w> <h>            - Resize grid");
    view->message("  toggle <pid> <mid>        - Toggle module power");
    view->message("  info                      - Show summary stats");
    view->message("  info_type <id>            - Show specific (0=Plat, 1=Intr, 2=Net, 3=Sens, 4=Wep, 5=Grid)");
    view->message("  quit                      - Exit");
}

void Controller::showTypeInfo(int type) {
    std::cout << "\n=== DETAILED REPORT (Type " << type << ") ===\n";

    if (type == 0) {
        if (env->platforms.empty()) std::cout << "No Platforms active.\n";
        for (size_t i = 0; i < env->platforms.size(); ++i) {
            auto& p = env->platforms[i];
            std::cout << "ID " << i << " [" << p->getDescription() << "]\n"
                << "   Position: (" << p->getPosition().getX() << "," << p->getPosition().getY() << ")\n"
                << "   Energy:   " << p->getEnergyLevel() << "\n"
                << "   Modules:  " << p->getAllModules().size() << "\n"
                << "   AI Link:  " << (p->isConnectedToAI() ? "ONLINE" : "OFFLINE") << "\n"
                << "--------------------------------\n";
        }
    }
    else if (type == 1) { // intruders
        if (env->intruders.empty()) std::cout << "No Intruders detected.\n";
        for (size_t i = 0; i < env->intruders.size(); ++i) {
            auto& intruder = env->intruders[i];
            std::cout << "ID " << i << " [" << intruder->getBehaviorName() << "]\n"
                << "   Position: (" << intruder->getPosition().getX() << "," << intruder->getPosition().getY() << ")\n"
                << "   Status:   " << (intruder->isNeutralized() ? "NEUTRALIZED" : "ACTIVE") << "\n"
                << "--------------------------------\n";
        }
    }
    else if (type == 2) {   // net mods
        int count = 0;
        for (size_t i = 0; i < env->platforms.size(); ++i) {
            auto nets = env->platforms[i]->getModules<NetworkModule>();
            for (auto& n : nets) {
                count++;
                std::cout << "NetMod @ Platform " << i << "\n"
                    << "   State:    " << (n->isEnabled() ? "ON" : "OFF") << "\n"
                    << "   Peers:    " << n->getSessions().size() << " direct neighbors\n"
                    << "   Routing:  " << n->getRoutingTable().routes.size() << " known paths\n"
                    << "--------------------------------\n";
            }
        }
        if (count == 0) std::cout << "No Network Modules installed.\n";
    }
    else if (type == 3) { // sensor mods
        int count = 0;
        for (size_t i = 0; i < env->platforms.size(); ++i) {
            auto sensors = env->platforms[i]->getModules<SensorModule>();
            for (auto& s : sensors) {
                count++;
                std::cout << "Sensor @ Platform " << i << "\n"
                    << "   State:    " << (s->isEnabled() ? "ON" : "OFF") << "\n"
                    << "   Detecting:" << s->getReadings().size() << " targets currently tracked\n"
                    << "--------------------------------\n";
            }
        }
        if (count == 0) std::cout << "No Sensor Modules installed.\n";
    }
    else if (type == 4) { // weapon mods
        int count = 0;
        for (size_t i = 0; i < env->platforms.size(); ++i) {
            auto weps = env->platforms[i]->getModules<WeaponModule>();
            for (auto& w : weps) {
                count++;
                std::cout << "Weapon @ Platform " << i << "\n"
                    << "   State:    " << (w->isEnabled() ? "ON" : "OFF") << "\n"
                    << "   Power:    High Consumption (20.0/tick)\n"
                    << "--------------------------------\n";
            }
        }
        if (count == 0) std::cout << "No Weapon Modules installed.\n";
    }
    else if (type == 5) {  // grid
        std::cout << "--- Chargers ---\n";
        bool foundC = false;
        for (int y = 0; y < env->getHeight(); ++y) {
            for (int x = 0; x < env->getWidth(); ++x) {
                if (env->getCellType({ x,y }) == GridCellType::CHARGER) {
                    std::cout << "   Charger  @ (" << x << "," << y << ")\n";
                    foundC = true;
                }
            }
        }
        if (!foundC) std::cout << "   None.\n";

        std::cout << "--- Obstacles ---\n";
        bool foundO = false;
        for (int y = 0; y < env->getHeight(); ++y) {
            for (int x = 0; x < env->getWidth(); ++x) {
                if (env->getCellType({ x,y }) == GridCellType::OBSTACLE) {
                    std::cout << "   Obstacle @ (" << x << "," << y << ")\n";
                    foundO = true;
                }
            }
        }
        if (!foundO) std::cout << "   None.\n";
    }
    else {
        std::cout << "Invalid Type ID. Use 0-5.\n";
    }
    std::cout << "================================\n";
}

void Controller::run() {
    view->message("Security System Started.");
    while (true) {
        help();
        view->display(*env);
        std::string cmd = view->getCommand();

        if (cmd == "quit") break;
        else if (cmd == "step") {
            processStep();
            view->message("Step complete.");
        }
        else if (cmd == "auto") {
            int n;
            if (std::cin >> n) {
                for (int i = 0; i < n; ++i) processStep();
                view->message("Auto-run complete.");
            }
            else clearInput();
        }
        else if (cmd == "add_intruder") {
            int x, y, type;
            if (std::cin >> x >> y >> type) {
                std::shared_ptr<BehaviorAlgorithm> beh;
                if (type == 1) beh = std::make_shared<KnightBehavior>();
                else beh = std::make_shared<RandomBehavior>();
                env->intruders.pushBack(std::make_shared<Intruder>(Coordinate(x, y), beh));
                env->refreshGrid();
                view->message("Intruder added.");
            }
            else clearInput();
        }
        else if (cmd == "add_obs") {
            int x, y;
            if (std::cin >> x >> y) {
                env->setCellType({ x,y }, GridCellType::OBSTACLE);
                env->refreshGrid();
                view->message("Obstacle added.");
            }
            else clearInput();
        }
        else if (cmd == "add_char") {
            int x, y;
            if (std::cin >> x >> y) {
                env->setCellType({ x,y }, GridCellType::CHARGER);
                env->refreshGrid();
                view->message("Charger added.");
            }
            else clearInput();
        }
        else if (cmd == "resize") {
            int w, h;
            if (std::cin >> w >> h && w > 0 && h > 0) {
                env->resize(w, h);
                env->refreshGrid();
                view->message("Environment resized.");
            }
            else clearInput();
        }
        else if (cmd == "add_plat") {
            int x, y, type;
            if (std::cin >> x >> y >> type) {
                std::shared_ptr<Platform> p;
                if (type == 0) p = std::make_shared<StationaryPlatform>(Coordinate(x, y));
                else p = std::make_shared<MobilePlatform>(Coordinate(x, y), 2.0);
                env->platforms.pushBack(p);
                env->refreshGrid();
                view->message("Platform added.");
            }
            else clearInput();
        }
        else if (cmd == "add_mod") {
            size_t pid; int type;
            if (std::cin >> pid >> type) {
                if (pid < env->platforms.size()) {
                    std::shared_ptr<Module> m;
                    if (type == 0) m = std::make_shared<NetworkModule>(5.0);
                    else if (type == 1) m = std::make_shared<SensorModule>(4.0, SensorType::OPTICAL);
                    else m = std::make_shared<WeaponModule>(5.0);

                    if (env->platforms[pid]->installModule(m)) view->message("Module installed.");
                    else view->message("Failed (No slots).");
                }
                else view->message("Invalid platform ID.");
            }
            else clearInput();
        }
        else if (cmd == "toggle") {
            size_t pid, mid;
            if (std::cin >> pid >> mid) {
                if (pid < env->platforms.size()) {
                    auto& mods = env->platforms[pid]->getAllModules();
                    if (mid < mods.size()) {
                        if (mods[mid]->isEnabled()) mods[mid]->deactivate();
                        else mods[mid]->activate(env->platforms[pid]->getEnergyLevel());
                        view->message("Module toggled.");
                    }
                }
            }
            else clearInput();
        }
        else if (cmd == "info") {
            std::cout << "\n--- Status Report ---\n";
            std::cout << "Intruders: " << env->intruders.size() << "\n";
            for (size_t i = 0; i < env->platforms.size(); ++i) {
                auto& p = env->platforms[i];
                std::cout << "Plat[" << i << "] " << p->getDescription()
                    << " @(" << p->getPosition().getX() << "," << p->getPosition().getY() << ")"
                    << " Energy: " << p->getEnergyLevel();
                if (env->getCellType(p->getPosition()) == GridCellType::CHARGER) std::cout << " [CHARGING]";
                std::cout << "\n";

                auto& mods = p->getAllModules();
                for (size_t j = 0; j < mods.size(); ++j) {
                    std::cout << "  - Mod[" << j << "] " << mods[j]->getType()
                        << " Active: " << (mods[j]->isEnabled() ? "YES" : "NO") << "\n";
                }
            }
        }
        else if (cmd == "info_type") {
            int typeID;
            if (std::cin >> typeID) {
                showTypeInfo(typeID);
            }
            else clearInput();
        }
        else {
            view->message("Unknown command. Try 'info' or 'step'.");
            clearInput();
        }
    }
}

void Controller::processStep() {
    for (auto& i : env->intruders) i->move(*env);
    for (auto& p : env->platforms) p->tick(*env);
    ai->neutralizeAllIntruders();
    env->refreshGrid();
}

void Controller::clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    view->message("Invalid input.");
}