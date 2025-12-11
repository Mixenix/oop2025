/**
 * @file Environment.hpp
 * @brief Defines the simulation grid environment.
 */

#pragma once
#include "Utils.hpp"
#include "Platform.hpp"
#include "Intruder.hpp"

 /**
  * @class Environment
  * @brief Manages the grid, platforms, and intruders.
  *
  * Acts as the "World" for the simulation. It handles thread-safe resizing
  * and grid cell queries.
  */
class Environment {
    int width;
    int height;
    Vector<Vector<GridCellType>> grid;

public:
    Vector<std::shared_ptr<Platform>> platforms;
    Vector<std::shared_ptr<Intruder>> intruders;
    std::mutex envMutex;

    /**
     * @brief Constructs environment with dimensions.
     * @param w Width.
     * @param h Height.
     */
    Environment(int w, int h);

    /**
     * @brief Resizes the simulation grid.
     * @param w New Width.
     * @param h New Height.
     */
    void resize(int w, int h);

    int getWidth() const;
    int getHeight() const;

    /**
     * @brief Gets the type of a specific cell.
     * @param c Coordinate to check.
     * @return GridCellType The type (OBSTACLE if out of bounds).
     */
    GridCellType getCellType(Coordinate c) const;

    /**
     * @brief Manually sets a cell type (e.g. adding obstacles).
     * @param c Coordinate to set.
     * @param t Type to apply.
     */
    void setCellType(Coordinate c, GridCellType t);

    /**
     * @brief Updates the grid representation based on object positions.
     *
     * Clears mobile entities from the grid map and re-places them
     * based on their current Coordinates.
     */
    void refreshGrid();

    /**
     * @brief Locates the nearest charger to a given position.
     * @param pos Starting position.
     * @return Coordinate of the nearest charger or (-1,-1) if none.
     */
    Coordinate findNearestCharger(Coordinate pos) const;
};