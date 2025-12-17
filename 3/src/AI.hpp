#pragma once
#include "Environment.hpp"
#include "Utils.hpp"
#include <set>
#include <mutex>

class AI {
    Environment* environment;
    std::set<Coordinate> knowledgeBase;
    std::mutex kbMutex;

public:
    AI(Environment* env);

    void updateKnowledge();
    void updateKnowledgeMT();
    void commandMobileUnits();
    void neutralizeAllIntruders();
    void neutralizeAllIntrudersMT();
};