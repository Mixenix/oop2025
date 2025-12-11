/**
 * @file Controller.hpp
 * @brief Defines the Controller class (MVC).
 */

#pragma once
#include "Utils.hpp"
#include "Environment.hpp"
#include "View.hpp"
#include "AI.hpp"

 /**
  * @class Controller
  * @brief Manages the main simulation loop and user input.
  */
class Controller {
    std::unique_ptr<Environment> env;
    std::unique_ptr<IView> view;
    std::unique_ptr<AI> ai;
public:
    Controller();
    void help();
    void run();
private:
    void processStep();
    void clearInput();
    void showTypeInfo(int type);
};