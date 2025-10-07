#include "SimpleDomino.h"
#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>

using namespace std;

SimpleDomino::SimpleDomino(int val1, int val2, bool rnd) {
    if (rnd) {
        setRandomValues();
    }
    else {
        value1 = val1;
        value2 = val2;
    }
}

void SimpleDomino::setRandomValues() {
    unsigned int seed = static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count());
    mt19937 generator(seed);
    uniform_int_distribution<int> distribution(1, 6);
    value1 = distribution(generator);
    value2 = distribution(generator);
}

void SimpleDomino::operator!() {
    setRandomValues();
}

SimpleDomino& SimpleDomino::operator~() {
    swap(value1, value2);
    return *this;
}

bool SimpleDomino::operator==(const SimpleDomino& domToCompare) const {
    return (value1 == domToCompare.value1 && value2 == domToCompare.value2) ||
        (value1 == domToCompare.value2 && value2 == domToCompare.value1);
}

int SimpleDomino::getVal1() const {
    return value1;
}

int SimpleDomino::getVal2() const {
    return value2;
}

void SimpleDomino::setVal1(int v) {
    if (v < 1 || v > 6) throw out_of_range("Value must be between 1 and 6");
    value1 = v;
}

void SimpleDomino::setVal2(int v) {
    if (v < 1 || v > 6) throw out_of_range("Value must be between 1 and 6");
    value2 = v;
}

void SimpleDomino::print() {
    cout << value1 << " and " << value2 << endl;
}

string SimpleDomino::drawHalf(int value) const {
    switch (value) {
    case 0:
        return "     \n"
            "     \n"
            "     \n";
    case 1:
        return "     \n"
            "  •  \n"
            "     \n";
    case 2:
        return "•    \n"
            "     \n"
            "    •\n";
    case 3:
        return "•    \n"
            "  •  \n"
            "    •\n";
    case 4:
        return "•   •\n"
            "     \n"
            "•   •\n";
    case 5:
        return "•   •\n"
            "  •  \n"
            "•   •\n";
    case 6:
        return "•   •\n"
            "•   •\n"
            "•   •\n";
    default:
        return "     \n"
            "  ?  \n"
            "     \n";
    }
}

string SimpleDomino::ascii() const {
    string leftHalf = drawHalf(value1);
    string rightHalf = drawHalf(value2);

    string result;
    result += to_string(value1) + " " + to_string(value2) + "\n";
    result += "┌─────┬─────┐\n";

    size_t pos = 0;
    for (int i = 0; i < 3; ++i) {
        size_t leftEnd = leftHalf.find('\n', pos);
        size_t rightEnd = rightHalf.find('\n', pos);

        string leftLine = leftHalf.substr(pos, leftEnd - pos);
        string rightLine = rightHalf.substr(pos, rightEnd - pos);

        result += "│" + leftLine + "│" + rightLine + "│\n";
        pos = leftEnd + 1;
    }

    result += "└─────┴─────┘\n";
    return result;
}

ostream& operator<<(ostream& os, const SimpleDomino& dom) {
    os << "Current state:\n" << "LeftValue = " << dom.value1 << "\n" << "RightValue = " << dom.value2 << endl;
    return os;
}

istream& operator>>(istream& is, SimpleDomino& dom) {
    int val1, val2;
    is >> val1 >> val2;

    int oldVal1 = dom.value1;
    int oldVal2 = dom.value2;

    try {
        dom.setVal1(val1);
        dom.setVal2(val2);
    }
    catch (const out_of_range& e) {
        dom.value1 = oldVal1;
        dom.value2 = oldVal2;
        throw;
    }

    return is;
}

bool SimpleDomino::operator<(const SimpleDomino& dom) const {
    int sum1 = value1 + value2;
    int sum2 = dom.value1 + dom.value2;
    return sum1 < sum2;
}