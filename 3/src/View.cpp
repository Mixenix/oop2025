/**
 * @file View.cpp
 * @brief Implementation of ConsoleView.
 */

#include "View.hpp"

void ConsoleView::display(const Environment& env) {
    std::cout << "\n--- Environment Map (" << env.getWidth() << "x" << env.getHeight() << ") ---\n";
    for (int y = 0; y < env.getHeight(); ++y) {
        std::cout << std::setw(2) << y << " | ";
        for (int x = 0; x < env.getWidth(); ++x) {
            switch (env.getCellType({ x,y })) {
            case GridCellType::EMPTY: std::cout << ". "; break;
            case GridCellType::OBSTACLE: std::cout << "# "; break;
            case GridCellType::PLATFORM: std::cout << "P "; break;
            case GridCellType::INTRUDER: std::cout << "I "; break;
            case GridCellType::CHARGER: std::cout << "C "; break;
            }
        }
        std::cout << "\n";
    }
}
void ConsoleView::message(const std::string& msg) { std::cout << "[System]: " << msg << std::endl; }
std::string ConsoleView::getCommand() {
    std::cout << "> ";
    std::string cmd;
    std::cin >> cmd;
    return cmd;
}