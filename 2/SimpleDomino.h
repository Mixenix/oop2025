#ifndef SIMPLEDOMINO_H
#define SIMPLEDOMINO_H

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <stdexcept>

/**
 * @class SimpleDomino
 * @brief Represents a single domino tile with two values
 *
 * The SimpleDomino class represents a domino tile with two integer values
 * ranging from 1 to 6. It provides functionality for creating, manipulating,
 * and displaying domino tiles.
 */
class SimpleDomino {
private:
    int value1; ///< First value of the domino (1-6)
    int value2; ///< Second value of the domino (1-6)

public:
    /**
     * @brief Default constructor
     * @param val1 First value of the domino (default: 1)
     * @param val2 Second value of the domino (default: 1)
     * @param rnd If true, creates domino with random values (default: false)
     */
    SimpleDomino(int val1 = 1, int val2 = 1, bool rnd = false);

    /**
     * @brief Gets the first value of the domino
     * @return First value (1-6)
     */
    int getVal1() const;

    /**
     * @brief Gets the second value of the domino
     * @return Second value (1-6)
     */
    int getVal2() const;

    /**
     * @brief Sets the first value of the domino
     * @param v Value to set (must be between 1-6)
     * @throws std::out_of_range if value is outside valid range
     */
    void setVal1(int v);

    /**
     * @brief Sets the second value of the domino
     * @param v Value to set (must be between 1-6)
     * @throws std::out_of_range if value is outside valid range
     */
    void setVal2(int v);

    /**
     * @brief Generates random values for the domino
     */
    void setRandomValues();

    /**
     * @brief Randomizes the domino values using operator overloading
     */
    void operator!();

    /**
     * @brief Flips the domino (swaps the two values)
     * @return Reference to the flipped domino
     */
    SimpleDomino& operator~();

    /**
     * @brief Compares two dominoes for equality
     * @param domToCompare Domino to compare with
     * @return true if dominoes have the same values (order doesn't matter), false otherwise
     */
    bool operator==(const SimpleDomino& domToCompare) const;

    /**
     * @brief Compares two dominoes by their sum of values
     * @param dom Domino to compare with
     * @return true if this domino's sum is less than the other domino's sum
     */
    bool operator<(const SimpleDomino& dom) const;

    /**
     * @brief Prints domino values to standard output
     */
    void print();

    /**
     * @brief Generates ASCII art representation of the domino
     * @return String containing ASCII art
     */
    std::string ascii() const;

    /**
     * @brief Outputs domino state to output stream
     * @param os Output stream
     * @param dom Domino to output
     * @return Reference to output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const SimpleDomino& dom);

    /**
     * @brief Inputs domino values from input stream
     * @param is Input stream
     * @param dom Domino to input values into
     * @return Reference to input stream
     * @throws std::out_of_range if input values are outside valid range
     */
    friend std::istream& operator>>(std::istream& is, SimpleDomino& dom);

private:
    /**
     * @brief Draws one half of the domino for ASCII art
     * @param value Value to draw (1-6)
     * @return String representation of the domino half
     */
    std::string drawHalf(int value) const;
};

#endif