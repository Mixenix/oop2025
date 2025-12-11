/**
 * @file View.hpp
 * @brief Defines the View interface and Console implementation (MVC).
 */

#pragma once
#include "Utils.hpp"
#include "Environment.hpp"

 /**
  * @class IView
  * @brief Interface for displaying simulation state.
  */
class IView {
public:
    virtual void display(const Environment& env) = 0;
    virtual void message(const std::string& msg) = 0;
    virtual std::string getCommand() = 0;
    virtual ~IView() = default;
};

/**
 * @class ConsoleView
 * @brief Text-based implementation of the view.
 */
class ConsoleView : public IView {
public:
    void display(const Environment& env) override;
    void message(const std::string& msg) override;
    std::string getCommand() override;
};