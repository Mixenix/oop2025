// CMakeLab2.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <random>

/**
 * @brief Displays menu for testing SimpleDomino class
 *
 * Provides interactive interface for creating and manipulating
 * individual domino tiles with various operations.
 */
void simpleDominoMenu();

/**
 * @brief Displays menu for testing ComplexDomino class
 *
 * Provides interactive interface for managing collections of
 * domino tiles with group operations and manipulations.
 */
void complexDominoMenu();