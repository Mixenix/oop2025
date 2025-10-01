#include "SimpleDomino.h"
#include <random>
#include <chrono>
#include <algorithm>
#include <iostream>

using namespace std;


SimpleDomino::SimpleDomino(int val1, int val2, bool rnd) {
	if (rnd == true) {
		unsigned int seed = static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count());
		mt19937 generator(seed);
		uniform_int_distribution<int> distribution(1, 6);
		value1 = (distribution(generator));
		value2 = (distribution(generator));
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
	value1 = (distribution(generator));
	value2 = (distribution(generator));
}

void SimpleDomino::operator!() {
	unsigned int seed = static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count());
	mt19937 generator(seed);
	uniform_int_distribution<int> distribution(1, 6);
	value1 = (distribution(generator));
	value2 = (distribution(generator));
}

SimpleDomino SimpleDomino::operator~() {
	int t = value2;
	value2 = value1;
	value1 = t;
	return *this;
}

bool SimpleDomino::operator == (const SimpleDomino& domToCompare) const {
	if ((domToCompare.getVal1() == value1 && domToCompare.getVal2() == value2) ||
		(domToCompare.getVal1() == value2 && domToCompare.getVal2() == value1)) {
		return true;
	}
	else {
		return false;
	}
}

int SimpleDomino::getVal1() const {
	return value1;
}

int SimpleDomino::getVal2() const {
	return value2;
}

void SimpleDomino::setVal1(int v) {
	if (v < 0 || v > 6) throw out_of_range("Value must be 0 < x <= 6");
	value1 = v;
}

void SimpleDomino::setVal2(int v) {
	if (v < 0 || v > 6) throw out_of_range("Value must be 0 < x <= 6");
	value2 = v;
}

void SimpleDomino::print() {
	cout << value1 << "and" << value2 << endl;
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
	vector<string> leftLines;
	vector<string> rightLines;

	string leftHalf = drawHalf(value1);
	string rightHalf = drawHalf(value2);

	size_t pos = 0;
	while (pos < leftHalf.length()) {
		size_t newPos = leftHalf.find('\n', pos);
		if (newPos == string::npos) break;
		leftLines.push_back(leftHalf.substr(pos, newPos - pos));
		pos = newPos + 1;
	}

	pos = 0;
	while (pos < rightHalf.length()) {
		size_t newPos = rightHalf.find('\n', pos);
		if (newPos == string::npos) break;
		rightLines.push_back(rightHalf.substr(pos, newPos - pos));
		pos = newPos + 1;
	}

	string result;
	result += to_string(value1) + " " + to_string(value2) + "\n";
	result += "┌─────┬─────┐\n";

	for (size_t i = 0; i < leftLines.size(); ++i) {
		result += "│" + leftLines[i] + "│" + rightLines[i] + "│\n";
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
	is >> val1;
	is >> val2;
	dom.setVal1(val1);
	dom.setVal2(val2);
	return is;
}

bool SimpleDomino::operator<(const SimpleDomino& dom) const {
	int sum1 = value1 + value2;
	int sum2 = dom.getVal1() + dom.getVal2();
	return sum1 < sum2;
}