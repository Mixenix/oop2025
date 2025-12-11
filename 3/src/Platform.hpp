/**
 * @file Platform.hpp
 * @brief Defines the Platform hierarchy (Base, Stationary, Mobile).
 */

#pragma once
#include "Utils.hpp"
#include "Module.hpp"

class Environment;

/**
 * @class Platform
 * @brief Base class for hosting modules and managing energy.
 */
class Platform {
protected:
    Coordinate position;
    std::string description;
    double energyLevel;
    int totalSlots;
    Vector<std::shared_ptr<Module>> modules;
public:
    /**
     * @brief Constructs a Platform.
     * @param pos Initial position.
     * @param desc Text description.
     * @param energy Starting energy.
     * @param slots Module capacity.
     */
    Platform(Coordinate pos, std::string desc, double energy, int slots);
    virtual ~Platform() = default;

    Coordinate getPosition() const;
    void setPosition(Coordinate c);
    double getEnergyLevel() const;
    const std::string& getDescription() const;

    /**
     * @brief Installs a module if slots allow.
     * @param m Module pointer.
     * @return true if installed.
     */
    bool installModule(std::shared_ptr<Module> m);

    /**
     * @brief Helper to retrieve modules of a specific type.
     * @tparam T The class type (e.g. NetworkModule).
     * @return Vector of pointers.
     */
    template<typename T> Vector<std::shared_ptr<T>> getModules() {
        Vector<std::shared_ptr<T>> res;
        for (auto& m : modules) if (auto casted = std::dynamic_pointer_cast<T>(m)) res.pushBack(casted);
        return res;
    }
    const Vector<std::shared_ptr<Module>>& getAllModules() const;

    /**
     * @brief Simulation tick: consumes energy, updates modules, recharges.
     * @param env Environment.
     */
    void tick(const Environment& env);

    /**
     * @brief Checks if platform can reach the AI logic.
     * @return true if connected.
     */
    virtual bool isConnectedToAI() const = 0;

    /**
     * @brief Moves the platform (if mobile).
     * @param env Environment for collision checking.
     * @param target Destination.
     */
    virtual void move(const Environment& env, Coordinate target) {}
};

/**
 * @class StationaryPlatform
 * @brief Immobile platform, assumed permanently connected to AI.
 */
class StationaryPlatform : public Platform {
public:
    StationaryPlatform(Coordinate pos);
    bool isConnectedToAI() const override;
};

/**
 * @class MobilePlatform
 * @brief Mobile platform with speed and limited energy.
 */
class MobilePlatform : public Platform {
    double speed;
public:
    MobilePlatform(Coordinate pos, double s);

    /**
     * @brief Checks connection via NetworkModule.
     * @return true if network is active and has sessions.
     */
    bool isConnectedToAI() const override;

    /**
     * @brief Performs a greedy movement step towards target.
     * @param env Environment.
     * @param target Target coordinate.
     */
    void move(const Environment& env, Coordinate target) override;
};