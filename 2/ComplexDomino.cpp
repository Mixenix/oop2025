#include "ComplexDomino.h"
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;


void ComplexDomino::resize(size_t newCapacity) {
	if (newCapacity <= capacity) return;

	// Увеличивать capacity в 1.5-2 раза для уменьшения реаллокаций
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
			// Создаем пустую группу с предварительным размером
			size = 0;
			capacity = sz;
			if (capacity > 0) {
				arr = new SimpleDomino[capacity];
			}
			else {
				arr = nullptr;
			}
		}
		else if (each > 0) {
			// Создаем полный набор
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
		// Случайные домино
		if (sz > 0) {
			capacity = sz;
			size = sz;
			arr = new SimpleDomino[capacity];

			unsigned int seed = static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count());
			mt19937 generator(seed);
			uniform_int_distribution<int> distribution(1, 6);

			for (size_t i = 0; i < sz; i++) {
				int val1 = distribution(generator);
				int val2 = distribution(generator);
				arr[i] = SimpleDomino(val1, val2, false);
			}
		}
		else {
			
			size = 0;
			capacity = 0;
			arr = nullptr;
			throw std::logic_error("Wrong size!");
		}
	}
}
ComplexDomino::ComplexDomino(const ComplexDomino& other) : size(other.size), capacity(other.capacity) {
	arr = new SimpleDomino[capacity];
	for (size_t i = 0; i < size; i++) {
		arr[i] = other.arr[i];
	}
}

ComplexDomino& ComplexDomino::operator=(const ComplexDomino& other) {
	if (this != &other) {
		delete[] arr;  // Освобождаем старую память

		size = other.size;
		capacity = other.capacity;

		if (capacity > 0) {
			arr = new SimpleDomino[capacity];
			for (size_t i = 0; i < size; i++) {
				arr[i] = other.arr[i];  // Копируем элементы
			}
		}
		else {
			arr = nullptr;
		}
	}
	return *this;
}

ComplexDomino& ComplexDomino::operator=(ComplexDomino& other) noexcept {
	if (this != &other) {
		delete[] arr;

		size = other.size;
		capacity = other.capacity;
		arr = other.arr;

		// Обнуляем other, чтобы деструктор не удалил память
		other.arr = nullptr;
		other.size = 0;
		other.capacity = 0;
	}
	return *this;
}

ComplexDomino::ComplexDomino(ComplexDomino&& other) noexcept : size(other.size), capacity(other.capacity), arr(other.arr) {
	other.arr = nullptr;
	other.size = 0;
	other.capacity = 0;
}

ComplexDomino::~ComplexDomino() {
	delete[] arr;
}

SimpleDomino ComplexDomino::operator[](int index) const {
	try {
		if (index < 0 or index >= size) { throw out_of_range("Invalid index"); }
		return arr[index];
	}
	catch (out_of_range& ex) {
		cerr << ex.what() << endl;
		throw;
	}
}

ostream& operator<<(ostream& os, const ComplexDomino& dom) {
	for (size_t i = 0; i < dom.size; i++) {
		os << i << ": ";
		os << "LeftValue = " << dom[i].getVal1() << "  " << "RightValue = " << dom[i].getVal2() << endl;
	}
	return os;
}

ComplexDomino& ComplexDomino::operator+=(const SimpleDomino& dom) {
	if (capacity <= size) { this->resize(capacity + 2); }
	this->arr[size] = dom;
	size++;
	return *this;
}

void ComplexDomino::addRandom() {
	SimpleDomino tmp(1, 1, true);
	if (capacity <= size) { this->resize(capacity + 2); }
	arr[size] = tmp;
	size++;
}

void ComplexDomino::removeRandom() {
	if (size == 0) {
		cout << "No dominoes to remove!" << endl;
		return;
	}

	unsigned int seed = static_cast<unsigned int>(chrono::system_clock::now().time_since_epoch().count());
	mt19937 generator(seed);
	uniform_int_distribution<int> distribution(0, static_cast<int>(size) - 1);
	int rndIndex = (distribution(generator));
	//arr.erase(arr.begin() + rndIndex);
	for (int i = rndIndex; i < size - 1; i++) {
		arr[i] = arr[i + 1];
	}
	arr[size - 1] == NULL;
	size--;
}

void ComplexDomino::remove(int val1, int val2) {
	for (size_t i = 0; i < size; i++) {
		if ((val1 == arr[i].getVal1() && val2 == arr[i].getVal2()) ||
			(val1 == arr[i].getVal2() && val2 == arr[i].getVal1())) {
			//arr.erase(arr.begin() + i);
			for (int j = i; j < size - 1; j++) {
				arr[j] = arr[j + 1];
			}
			arr[size - 1] == NULL;
			size--;
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
	//arr.erase(arr.begin() + index);
	for (int j = index; j < size - 1; j++) {
		arr[j] = arr[j + 1];
	}
	arr[size - 1] == SimpleDomino();
	size--;
}

void ComplexDomino::sortBySumAscending() {
	//sort(arr.begin(), arr.end();
	sort(arr, arr + size, [](const SimpleDomino& a, const SimpleDomino& b) {
		return a < b;
		});
}

ComplexDomino ComplexDomino::subSeq(int value) {
	ComplexDomino res(false, 0, -1);
	size_t writeIndex = 0;

	for (size_t i = 0; i < size; i++) {
		if (arr[i].getVal1() == value || arr[i].getVal2() == value) {
			res += arr[i];
		}
		else {
			// Сохраняем элементы, которые не подходят
			arr[writeIndex++] = arr[i];
		}
	}

	size = writeIndex;
	return res;
}

string ComplexDomino::ascii(int index) {
	if (index < 0 || index >= static_cast<int>(size)) {
		throw out_of_range("Invalid index in ascii()");
	}
	return arr[index].ascii();
}

size_t ComplexDomino::getSize() const {
	return size;
}
