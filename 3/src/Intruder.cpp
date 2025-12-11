/**
 * @file Intruder.cpp
 * @brief Implementation of Intruder and behaviors.
 */

#include "Intruder.hpp"
#include "Environment.hpp"

Intruder::Intruder(Coordinate pos, std::shared_ptr<BehaviorAlgorithm> beh)
    : position(pos), behavior(beh), neutralized(false) {
}

Coordinate Intruder::getPosition() const { return position; }
void Intruder::setPosition(Coordinate c) { position = c; }
bool Intruder::isNeutralized() const { return neutralized.load(); }
void Intruder::neutralize() { neutralized.store(true); }

void Intruder::move(const Environment& env) {
    if (neutralized.load()) return;
    position = behavior->getNextCoordinate(env, position);
}

Coordinate RandomBehavior::getNextCoordinate(const Environment& env, const Coordinate& currentPos) {
    int dx = (rand() % 3) - 1; int dy = (rand() % 3) - 1;
    Coordinate next(currentPos.getX() + dx, currentPos.getY() + dy);
    if (env.getCellType(next) == GridCellType::EMPTY) return next;
    return currentPos;
}

Coordinate KnightBehavior::getNextCoordinate(const Environment& env, const Coordinate& currentPos) {
    const int moves[8][2] = {
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1}
    };

    Vector<Coordinate> validMoves;

    for (const auto& move : moves) {
        Coordinate next(currentPos.getX() + move[0], currentPos.getY() + move[1]);
        if (env.getCellType(next) == GridCellType::EMPTY) {
            validMoves.pushBack(next);
        }
    }

    if (!validMoves.empty()) {
        int idx = rand() % validMoves.size();
        return validMoves[idx];
    }
    return currentPos;
}