/**
 * @file Utils.hpp
 * @brief fundamental utility classes, templates, and enums for the Security System Simulation.
 */

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <memory>
#include <atomic>
#include <mutex>
#include <future>
#include <thread>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <queue>
#include <set>

 /**
  * @enum GridCellType
  * @brief Represents the content of a specific cell in the environment grid.
  */
enum class GridCellType {
    EMPTY,
    OBSTACLE,
    PLATFORM,
    INTRUDER,
    CHARGER
};

/**
 * @enum SensorType
 * @brief Defines the technology used by a Sensor Module.
 */
enum class SensorType {
    OPTICAL,
    XRAY
};

/**
 * @class Coordinate
 * @brief Represents a 2D point in the simulation grid.
 */
class Coordinate {
    int x;
    int y;
public:
    /**
     * @brief Constructs a Coordinate.
     * @param x X-axis position.
     * @param y Y-axis position.
     */
    Coordinate(int x = 0, int y = 0) : x(x), y(y) {}

    /**
     * @brief Gets X coordinate.
     * @return int X value.
     */
    int getX() const { return x; }

    /**
     * @brief Gets Y coordinate.
     * @return int Y value.
     */
    int getY() const { return y; }

    /**
     * @brief Sets X coordinate.
     * @param val New X value.
     */
    void setX(int val) { x = val; }

    /**
     * @brief Sets Y coordinate.
     * @param val New Y value.
     */
    void setY(int val) { y = val; }

    /**
     * @brief Equality operator.
     * @param other Coordinate to compare.
     * @return true if x and y match.
     */
    bool operator==(const Coordinate& other) const { return x == other.x && y == other.y; }

    /**
     * @brief Inequality operator.
     * @param other Coordinate to compare.
     * @return true if coordinates differ.
     */
    bool operator!=(const Coordinate& other) const { return !(*this == other); }

    /**
     * @brief Less-than operator for sorting and set storage.
     * @param other Coordinate to compare.
     * @return true if this comes before other in ordering.
     */
    bool operator<(const Coordinate& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    /**
     * @brief Calculates Euclidean distance to another coordinate.
     * @param other Target coordinate.
     * @return double Distance value.
     */
    double distanceTo(const Coordinate& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
    }
};

/**
 * @class Vector
 * @brief A custom dynamic array implementation (Template Class).
 * @tparam T Type of elements stored.
 */
template <typename T>
class Vector {
private:
    T* data;
    size_t capacity;
    size_t count;

    /**
     * @brief Resizes the internal storage.
     * @param new_capacity The new size of the allocated block.
     */
    void reallocate(size_t new_capacity) {
        T* new_block = new T[new_capacity];
        if (new_capacity < count) count = new_capacity;
        for (size_t i = 0; i < count; ++i) new_block[i] = std::move(data[i]);
        delete[] data;
        data = new_block;
        capacity = new_capacity;
    }

public:
    /**
     * @class Iterator
     * @brief Random access iterator for the Vector class.
     */
    class Iterator {
        T* ptr;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        Iterator(T* p) : ptr(p) {}
        reference operator*() const { return *ptr; }
        pointer operator->() { return ptr; }
        Iterator& operator++() { ptr++; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const Iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
        Iterator operator+(int n) const { return Iterator(ptr + n); }
        Iterator operator-(int n) const { return Iterator(ptr - n); }
        ptrdiff_t operator-(const Iterator& other) const { return ptr - other.ptr; }
    };

    /**
     * @brief Default constructor.
     */
    Vector() : data(nullptr), capacity(0), count(0) {}

    /**
     * @brief Destructor. Releases memory.
     */
    ~Vector() { delete[] data; }

    /**
     * @brief Copy constructor.
     * @param other Vector to copy.
     */
    Vector(const Vector& other) : data(new T[other.capacity]), capacity(other.capacity), count(other.count) {
        for (size_t i = 0; i < count; ++i) data[i] = other.data[i];
    }

    /**
     * @brief Copy assignment operator.
     * @param other Vector to copy.
     * @return Reference to self.
     */
    Vector& operator=(const Vector& other) {
        if (this != &other) { delete[] data; count = other.count; capacity = other.capacity; data = new T[capacity]; for (size_t i = 0; i < count; ++i) data[i] = other.data[i]; }
        return *this;
    }

    /**
     * @brief Move constructor.
     * @param other Vector to move.
     */
    Vector(Vector&& other) noexcept : data(other.data), capacity(other.capacity), count(other.count) { other.data = nullptr; other.capacity = 0; other.count = 0; }

    /**
     * @brief Move assignment operator.
     * @param other Vector to move.
     * @return Reference to self.
     */
    Vector& operator=(Vector&& other) noexcept { if (this != &other) { delete[] data; data = other.data; capacity = other.capacity; count = other.count; other.data = nullptr; other.capacity = 0; other.count = 0; } return *this; }

    /**
     * @brief Adds an element to the end of the vector.
     * @param value Element to add.
     */
    void pushBack(const T& value) { if (count >= capacity) reallocate(capacity == 0 ? 4 : capacity * 2); data[count++] = value; }

    /**
     * @brief Adds an element to the end of the vector (Move semantics).
     * @param value Element to add.
     */
    void pushBack(T&& value) { if (count >= capacity) reallocate(capacity == 0 ? 4 : capacity * 2); data[count++] = std::move(value); }

    /**
     * @brief Removes an element at a specific index.
     * @param index Index of element to remove.
     * @throws std::out_of_range if index is invalid.
     */
    void remove(size_t index) { if (index >= count) throw std::out_of_range("Index out of range"); for (size_t i = index; i < count - 1; ++i) data[i] = std::move(data[i + 1]); count--; }

    /**
     * @brief Clears all elements from the vector.
     */
    void clear() { count = 0; }

    /**
     * @brief Access element at index.
     * @param index Index to access.
     * @return Reference to element.
     */
    T& operator[](size_t index) { return data[index]; }

    /**
     * @brief Access const element at index.
     * @param index Index to access.
     * @return Const reference to element.
     */
    const T& operator[](size_t index) const { return data[index]; }

    /**
     * @brief Gets current number of elements.
     * @return size_t Count.
     */
    size_t size() const { return count; }

    /**
     * @brief Checks if vector is empty.
     * @return true if empty.
     */
    bool empty() const { return count == 0; }

    Iterator begin() { return Iterator(data); }
    Iterator end() { return Iterator(data + count); }
    const Iterator begin() const { return Iterator(data); }
    const Iterator end() const { return Iterator(data + count); }
};

/**
 * @struct EnvironmentInfo
 * @brief Holder for data returned by sensors.
 */
struct EnvironmentInfo { std::string data; };