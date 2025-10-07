#include "ComplexDomino.h"
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

void ComplexDomino::resize(size_t newCapacity) {
    if (newCapacity <= capacity) return;

    size_t betterCapacity = max(newCapacity, capacity * 2);
    SimpleDomino* newArr = new SimpleDomino[betterCapacity];

    for (size_t i = 0; i < size; i++) {
        newArr[i] = arr[i];
    }

    delete[] arr;
    arr = newArr;
    capacity = betterCapacity;
}

ComplexDomino::ComplexDomino(bool rnd, size_t sz, int each) {
    if (!rnd) {
        if (each == -1) {
            size = 0;
            capacity = sz;
            arr = (capacity > 0) ? new SimpleDomino[capacity] : nullptr;
        }
        else if (each > 0) {
            size_t totalSize = each * 36;
            capacity = totalSize;
            size = totalSize;

            if (totalSize > 0) {
                arr = new SimpleDomino[capacity];
                size_t index = 0;
                for (int copy = 0; copy < each; copy++) {
                    for (int val1 = 1; val1 <= 6; val1++) {
                        for (int val2 = 1; val2 <= 6; val2++) {
                            arr[index++] = SimpleDomino(val1, val2, false);
                        }
                    }
                }
            }
            else {
                arr = nullptr;
            }
        }
    }
    else {
        if (sz > 0) {
            capacity = sz;
            size = sz;
            arr = new SimpleDomino[capacity];

            unsigned int seed = static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count());
            mt19937 generator(seed);
            uniform_int_distribution<int> distribution(1, 6);

            for (size_t i = 0; i < sz; i++) {
                arr[i] = SimpleDomino(distribution(generator), distribution(generator), false);
            }
        }
        else {
            throw logic_error("Wrong size!");
        }
    }
}

ComplexDomino::ComplexDomino(const ComplexDomino& other) : size(other.size), capacity(other.capacity) {
    if (capacity > 0) {
        arr = new SimpleDomino[capacity];
        for (size_t i = 0; i < size; i++) {
            arr[i] = other.arr[i];
        }
    }
    else {
        arr = nullptr;
    }
}

ComplexDomino& ComplexDomino::operator=(const ComplexDomino& other) {
    if (this != &other) {
        delete[] arr;

        size = other.size;
        capacity = other.capacity;

        if (capacity > 0) {
            arr = new SimpleDomino[capacity];
            for (size_t i = 0; i < size; i++) {
                arr[i] = other.arr[i];
            }
        }
        else {
            arr = nullptr;
        }
    }
    return *this;
}

ComplexDomino& ComplexDomino::operator=(ComplexDomino&& other) noexcept {
    if (this != &other) {
        delete[] arr;

        size = other.size;
        capacity = other.capacity;
        arr = other.arr;

        other.arr = nullptr;
        other.size = 0;
        other.capacity = 0;
    }
    return *this;
}

ComplexDomino::ComplexDomino(ComplexDomino&& other) noexcept
    : size(other.size), capacity(other.capacity), arr(other.arr) {
    other.arr = nullptr;
    other.size = 0;
    other.capacity = 0;
}

ComplexDomino::~ComplexDomino() {
    delete[] arr;
}

SimpleDomino& ComplexDomino::operator[](int index) {
    if (index < 0 || index >= static_cast<int>(size)) {
        throw out_of_range("Invalid index");
    }
    return arr[index];
}

const SimpleDomino& ComplexDomino::operator[](int index) const {
    if (index < 0 || index >= static_cast<int>(size)) {
        throw out_of_range("Invalid index");
    }
    return arr[index];
}

ostream& operator<<(ostream& os, const ComplexDomino& dom) {
    for (size_t i = 0; i < dom.size; i++) {
        os << i << ": LeftValue = " << dom[i].getVal1()
            << "  RightValue = " << dom[i].getVal2() << endl;
    }
    return os;
}

ComplexDomino& ComplexDomino::operator+=(const SimpleDomino& dom) {
    if (size >= capacity) {
        resize(capacity + 10);
    }
    arr[size++] = dom;
    return *this;
}

ComplexDomino& ComplexDomino::operator!() {
    sort(arr, arr + size, [](const SimpleDomino& a, const SimpleDomino& b) {
        return a < b;
        });
    return *this;
}

ComplexDomino ComplexDomino::operator()(int value) const {
    ComplexDomino result(false, 0, -1);
    ComplexDomino temp(*this);

    size_t writeIndex = 0;
    for (size_t i = 0; i < temp.size; i++) {
        if (temp.arr[i].getVal1() == value || temp.arr[i].getVal2() == value) {
            result += temp.arr[i];
        }
        else {
            temp.arr[writeIndex++] = temp.arr[i];
        }
    }
    temp.size = writeIndex;

    *const_cast<ComplexDomino*>(this) = temp;
    return result;
}

size_t ComplexDomino::getSize() const {
    return size;
}

void ComplexDomino::addRandom() {
    *this += SimpleDomino(1, 1, true);
}

void ComplexDomino::removeRandom() {
    if (size == 0) {
        cout << "No dominoes to remove!" << endl;
        return;
    }

    unsigned int seed = static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count());
    mt19937 generator(seed);
    uniform_int_distribution<int> distribution(0, static_cast<int>(size) - 1);
    int rndIndex = distribution(generator);

    removeByIndex(rndIndex);
}

void ComplexDomino::remove(int val1, int val2) {
    for (size_t i = 0; i < size; i++) {
        if ((arr[i].getVal1() == val1 && arr[i].getVal2() == val2) ||
            (arr[i].getVal1() == val2 && arr[i].getVal2() == val1)) {
            removeByIndex(i);
            return;
        }
    }
    cout << "Domino not found!" << endl;
}

void ComplexDomino::removeByIndex(int index) {
    if (index < 0 || index >= static_cast<int>(size)) {
        cout << "Invalid index!" << endl;
        return;
    }

    for (int i = index; i < static_cast<int>(size) - 1; i++) {
        arr[i] = arr[i + 1];
    }
    size--;
}

void ComplexDomino::sortBySumAscending() {
    !(*this);
}

ComplexDomino ComplexDomino::subSeq(int value) {
    return (*this)(value);
}

string ComplexDomino::ascii(int index) {
    if (index < 0 || index >= static_cast<int>(size)) {
        throw out_of_range("Invalid index");
    }
    return arr[index].ascii();
}