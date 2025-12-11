/**
 * @file Module.hpp
 * @brief Defines the base Module class and specific implementations (Network, Sensor, Weapon).
 */

#pragma once
#include "Utils.hpp"

class Environment;
class Intruder;
class NetworkModule;

/**
 * @struct Session
 * @brief Represents a direct link between two Network Modules.
 */
struct Session {
    NetworkModule* neighbor;
    double linkQuality;
};

/**
 * @struct Route
 * @brief Represents a path in the network routing table.
 */
struct Route {
    NetworkModule* target;
    NetworkModule* nextHop;
    int hopCount;
};

/**
 * @class Module
 * @brief Abstract base class for all platform add-ons.
 */
class Module {
protected:
    double energyActive;
    double energyPassive;
    bool active;
    int slotsRequired;
public:
    /**
     * @brief Constructs a module.
     * @param eA Energy consumption when active.
     * @param eP Energy consumption when idle.
     * @param slots Number of platform slots used.
     */
    Module(double eA, double eP, int slots);
    virtual ~Module() = default;

    /**
     * @brief Attempts to activate the module.
     * @param availableEnergy Current energy of the platform.
     * @return true if activation successful.
     */
    virtual bool activate(double availableEnergy);

    /**
     * @brief Deactivates the module.
     */
    virtual void deactivate();

    /**
     * @brief Performs periodic logic (Scanning, Routing, etc.).
     * @param env The environment context.
     * @param myPos The position of the parent platform.
     */
    virtual void update(const Environment& env, Coordinate myPos);

    bool isEnabled() const;
    int getSlotsRequired() const;
    double getConsumption() const;

    /**
     * @brief Gets the type name of the module.
     * @return std::string Type name.
     */
    virtual std::string getType() const = 0;
};

/**
 * @class CommunicationTable
 * @brief Stores routing information for the Network Module.
 */
class CommunicationTable {
public:
    Vector<Route> routes;
    void clear() { routes.clear(); }
    void addRoute(NetworkModule* target, NetworkModule* nextHop, int hops) {
        routes.pushBack({ target, nextHop, hops });
    }
};

/**
 * @class NetworkModule
 * @brief Handles connectivity, neighbor discovery, and routing (BFS).
 */
class NetworkModule : public Module {
    double range;
    Vector<Session> activeSessions;
    CommunicationTable commTable;
    bool isConnected(NetworkModule* other);
public:
    NetworkModule(double r);
    std::string getType() const override;

    /**
     * @brief Scans for neighbors and builds routing table via BFS.
     * @param env Environment.
     * @param myPos Position.
     */
    void update(const Environment& env, Coordinate myPos) override;

    const Vector<Session>& getSessions() const;
    const CommunicationTable& getRoutingTable() const;
    void addSession(NetworkModule* other);
};

/**
 * @class SensorModule
 * @brief Detects intruders within range (Fog of War).
 */
class SensorModule : public Module {
    double range;
    SensorType type;
    Vector<Coordinate> detectedIntruders;
public:
    SensorModule(double r, SensorType t);
    std::string getType() const override;

    /**
     * @brief Scans environment for intruders and populates internal list.
     * @param env Environment.
     * @param myPos Position.
     */
    void update(const Environment& env, Coordinate myPos) override;

    /**
     * @brief Returns list of currently seen intruder coordinates.
     * @return Vector<Coordinate> List of targets.
     */
    const Vector<Coordinate>& getReadings() const;
};

/**
 * @class WeaponModule
 * @brief Neutralizes intruders.
 */
class WeaponModule : public Module {
    double range;
    double chargingTime;
    bool isCharging;
public:
    WeaponModule(double r);
    std::string getType() const override;
    void switchToCharging();
    void switchFromCharging();

    /**
     * @brief Attempts to neutralize an intruder.
     * @param intruder Target pointer.
     * @param myPos Firing position.
     * @return true if successful.
     */
    bool neutralize(Intruder* intruder, Coordinate myPos);
};