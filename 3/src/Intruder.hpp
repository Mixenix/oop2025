/**
 * @file Intruder.hpp
 * @brief Defines Intruder class and behavior algorithms.
 */

#pragma once
#include "Utils.hpp"

class Environment;

/**
 * @class BehaviorAlgorithm
 * @brief Interface (Strategy Pattern) for intruder movement logic.
 */
class BehaviorAlgorithm {
public:
    virtual ~BehaviorAlgorithm() = default;

    /**
     * @brief Calculates the next move.
     * @param env Reference to environment.
     * @param currentPos Current position of the intruder.
     * @return Coordinate The new calculated position.
     */
    virtual Coordinate getNextCoordinate(const Environment& env, const Coordinate& currentPos) = 0;

    /**
     * @brief Gets the name of the behavior.
     * @return std::string Name (e.g., "Knight", "Random").
     */
    virtual std::string getName() const = 0;
};

/**
 * @class Intruder
 * @brief Represents a hostile entity in the system.
 */
class Intruder {
    Coordinate position;
    std::shared_ptr<BehaviorAlgorithm> behavior;
    std::atomic<bool> neutralized;
public:
    /**
     * @brief Constructs an Intruder.
     * @param pos Initial position.
     * @param beh Behavior strategy.
     */
    Intruder(Coordinate pos, std::shared_ptr<BehaviorAlgorithm> beh);

    Coordinate getPosition() const;
    void setPosition(Coordinate c);

    /**
     * @brief Checks if intruder is active.
     * @return true if neutralized (dead/captured).
     */
    bool isNeutralized() const;

    /**
     * @brief Sets state to neutralized.
     */
    void neutralize();

    /**
     * @brief Executes one movement step based on behavior.
     * @param env Environment context.
     */
    void move(const Environment& env);

    /**
     * @brief Gets the name of the active behavior.
     * @return std::string Behavior name.
     */
    std::string getBehaviorName() const { return behavior->getName(); }
};

/**
 * @class RandomBehavior
 * @brief Intruder moves to a random adjacent empty cell.
 */
class RandomBehavior : public BehaviorAlgorithm {
public:
    Coordinate getNextCoordinate(const Environment& env, const Coordinate& currentPos) override;
    std::string getName() const override { return "Random"; }
};

/**
 * @class KnightBehavior
 * @brief Intruder moves like a Chess Knight (L-shape).
 */
class KnightBehavior : public BehaviorAlgorithm {
public:
    Coordinate getNextCoordinate(const Environment& env, const Coordinate& currentPos) override;
    std::string getName() const override { return "Knight"; }
};