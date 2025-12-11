/**
 * @file Module.cpp
 * @brief Implementation of module logic, including BFS routing and sensing.
 */

#include "Module.hpp"
#include "Environment.hpp"
#include "Intruder.hpp"
#include "Platform.hpp"

Module::Module(double eA, double eP, int slots)
    : energyActive(eA), energyPassive(eP), slotsRequired(slots), active(false) {
}

bool Module::activate(double availableEnergy) {
    if (availableEnergy >= energyActive) { active = true; return true; }
    return false;
}
void Module::deactivate() { active = false; }
bool Module::isEnabled() const { return active; }
int Module::getSlotsRequired() const { return slotsRequired; } 
double Module::getConsumption() const { return active ? energyActive : energyPassive; }
void Module::update(const Environment& env, Coordinate myPos) {}

//net mod//////////////////////////////////////
NetworkModule::NetworkModule(double r) : Module(5.0, 1.0, 1), range(r) {}
std::string NetworkModule::getType() const { return "Network"; }

bool NetworkModule::isConnected(NetworkModule* other) {
    for (auto& s : activeSessions) if (s.neighbor == other) return true;
    return false;
}
void NetworkModule::addSession(NetworkModule* other) {
    if (!isConnected(other)) activeSessions.pushBack({ other, 1.0 });
}
const Vector<Session>& NetworkModule::getSessions() const { return activeSessions; }
const CommunicationTable& NetworkModule::getRoutingTable() const { return commTable; }

void NetworkModule::update(const Environment& env, Coordinate myPos) {
    if (!active) { activeSessions.clear(); commTable.clear(); return; }
    activeSessions.clear();
    for (const auto& platform : env.platforms) {
        double dist = myPos.distanceTo(platform->getPosition());
        if (dist <= range && dist > 0.001) {
            auto netMods = platform->getModules<NetworkModule>();
            for (auto& remoteNet : netMods) {
                if (remoteNet->isEnabled()) addSession(remoteNet.get());
            }
        }
    }
    commTable.clear();
    struct BfsNode { NetworkModule* current; NetworkModule* firstHop; int dist; };
    std::queue<BfsNode> q;
    std::set<NetworkModule*> visited;
    visited.insert(this);
    for (auto& session : activeSessions) {
        q.push({ session.neighbor, session.neighbor, 1 });
        visited.insert(session.neighbor);
        commTable.addRoute(session.neighbor, session.neighbor, 1);
    }
    while (!q.empty()) {
        BfsNode node = q.front(); q.pop();
        for (auto& subSession : node.current->getSessions()) {
            if (visited.find(subSession.neighbor) == visited.end()) {
                visited.insert(subSession.neighbor);
                commTable.addRoute(subSession.neighbor, node.firstHop, node.dist + 1);
                q.push({ subSession.neighbor, node.firstHop, node.dist + 1 });
            }
        }
    }
}
//net mod//////////////////////////////////////

// sensor mod /////////////////////////////////////////////////////////////////////////////////////
SensorModule::SensorModule(double r, SensorType t) : Module(3.0, 0.5, 1), range(r), type(t) {}
std::string SensorModule::getType() const { return "Sensor"; }

const Vector<Coordinate>& SensorModule::getReadings() const { return detectedIntruders; }

void SensorModule::update(const Environment& env, Coordinate myPos) {
    detectedIntruders.clear();
    if (!active) return;

    for (auto& intruder : env.intruders) {
        if (intruder->isNeutralized()) continue;
        if (myPos.distanceTo(intruder->getPosition()) <= range) {
            detectedIntruders.pushBack(intruder->getPosition());
        }
    }
}
// sensor mod /////////////////////////////////////////////////////////////////////////////////////

// weapon mod///////////////////
WeaponModule::WeaponModule(double r) : Module(20.0, 2.0, 2), range(r), chargingTime(5.0), isCharging(false) {}
std::string WeaponModule::getType() const { return "Weapon"; }
void WeaponModule::switchToCharging() { isCharging = true; }
void WeaponModule::switchFromCharging() { isCharging = false; }

bool WeaponModule::neutralize(Intruder* intruder, Coordinate myPos) {
    if (!active) return false;
    if (myPos.distanceTo(intruder->getPosition()) <= range) {
        intruder->neutralize();
        return true;
    }
    return false;
}
// weapon mod///////////////////