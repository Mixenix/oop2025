#ifndef COMPLEXDOMINO_H
#define COMPLEXDOMINO_H

#include "SimpleDomino.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>

/**
 * @class ComplexDomino
 * @brief Represents a collection of domino tiles
 *
 * The ComplexDomino class manages a dynamic array of SimpleDomino objects.
 * It provides functionality for creating, manipulating, and managing groups
 * of domino tiles with various operations.
 */
class ComplexDomino {
private:
    size_t size = 0; ///< Current number of dominoes in the collection
    size_t capacity = 0; ///< Current capacity of the dynamic array
    SimpleDomino* arr = nullptr; ///< Dynamic array storing dominoes

    /**
     * @brief Resizes the internal array to accommodate more elements
     * @param newCapacity New capacity for the array
     */
    void resize(size_t newCapacity);

public:
    /**
     * @brief Constructs a new ComplexDomino object
     * @param rnd If true, creates random dominoes (default: false)
     * @param sz Number of dominoes to create (used when rnd=true)
     * @param each Number of copies for each domino combination (used when rnd=false and each>0)
     * @throws std::logic_error if invalid parameters are provided
     */
    ComplexDomino(bool rnd = false, size_t sz = 0, int each = -1);

    /**
     * @brief Copy constructor
     * @param other ComplexDomino to copy from
     */
    ComplexDomino(const ComplexDomino& other);

    /**
     * @brief Move constructor
     * @param other ComplexDomino to move from
     */
    ComplexDomino(ComplexDomino&& other) noexcept;

    /**
     * @brief Destructor
     */
    ~ComplexDomino();

    /**
     * @brief Copy assignment operator
     * @param other ComplexDomino to copy from
     * @return Reference to this object
     */
    ComplexDomino& operator=(const ComplexDomino& other);

    /**
     * @brief Move assignment operator
     * @param other ComplexDomino to move from
     * @return Reference to this object
     */
    ComplexDomino& operator=(ComplexDomino& other) noexcept;

    /**
     * @brief Accesses domino at specified index
     * @param index Index of domino to access
     * @return Copy of domino at specified index
     * @throws std::out_of_range if index is invalid
     */
    SimpleDomino operator[](int index) const;

    /**
     * @brief Gets the current number of dominoes in the collection
     * @return Number of dominoes
     */
    size_t getSize() const;

    /**
     * @brief Adds a domino to the collection
     * @param dom Domino to add
     * @return Reference to this object
     */
    ComplexDomino& operator+=(const SimpleDomino& dom);

    /**
     * @brief Adds a random domino to the collection
     */
    void addRandom();

    /**
     * @brief Removes a random domino from the collection
     */
    void removeRandom();

    /**
     * @brief Removes domino with specified values from the collection
     * @param val1 First value of domino to remove
     * @param val2 Second value of domino to remove
     */
    void remove(int val1, int val2);

    /**
     * @brief Removes domino at specified index from the collection
     * @param index Index of domino to remove
     */
    void removeByIndex(int index);

    /**
     * @brief Sorts dominoes by ascending sum of their values
     */
    void sortBySumAscending();

    /**
     * @brief Extracts subsequence of dominoes containing specified value
     * @param value Value to search for in dominoes
     * @return New ComplexDomino containing matching dominoes
     */
    ComplexDomino subSeq(int value);

    /**
     * @brief Gets ASCII art representation of domino at specified index
     * @param index Index of domino
     * @return ASCII art string
     * @throws std::out_of_range if index is invalid
     */
    std::string ascii(int index);

    /**
     * @brief Outputs all dominoes in the collection to output stream
     * @param os Output stream
     * @param dom ComplexDomino to output
     * @return Reference to output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const ComplexDomino& dom);
};

#endif