/**
 * @file Environment.cpp
 * @brief Implementation of Environment logic.
 */

#include "Environment.hpp"

Environment::Environment(int w, int h) : width(w), height(h) {
    resize(w, h);
}

void Environment::resize(int w, int h) {
    std::lock_guard<std::mutex> lock(envMutex);
    width = w;
    height = h;
    grid = Vector<Vector<GridCellType>>();
    for (int i = 0; i < h; ++i) {
        Vector<GridCellType> row;
        for (int j = 0; j < w; ++j) row.pushBack(GridCellType::EMPTY);
        grid.pushBack(std::move(row));
    }
}

int Environment::getWidth() const { return width; }
int Environment::getHeight() const { return height; }

GridCellType Environment::getCellType(Coordinate c) const {
    if (c.getX() < 0 || c.getX() >= width || c.getY() < 0 || c.getY() >= height) return GridCellType::OBSTACLE;
    return grid[c.getY()][c.getX()];
}

void Environment::setCellType(Coordinate c, GridCellType t) {
    if (c.getX() >= 0 && c.getX() < width && c.getY() >= 0 && c.getY() < height) grid[c.getY()][c.getX()] = t;
}

void Environment::refreshGrid() {
    std::lock_guard<std::mutex> lock(envMutex);
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            if (grid[y][x] != GridCellType::OBSTACLE && grid[y][x] != GridCellType::CHARGER)
                grid[y][x] = GridCellType::EMPTY;

    for (auto& p : platforms) {
        if (p->getPosition().getX() < width && p->getPosition().getY() < height)
            grid[p->getPosition().getY()][p->getPosition().getX()] = GridCellType::PLATFORM;
    }
    for (auto& i : intruders) {
        if (!i->isNeutralized() && i->getPosition().getX() < width && i->getPosition().getY() < height)
            grid[i->getPosition().getY()][i->getPosition().getX()] = GridCellType::INTRUDER;
    }
}

Coordinate Environment::findNearestCharger(Coordinate pos) const {
    double minDist = 1e9;
    Coordinate target(-1, -1);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == GridCellType::CHARGER) {
                double d = pos.distanceTo({ x,y });
                if (d < minDist) {
                    minDist = d;
                    target = { x,y };
                }
            }
        }
    }
    return target;
}