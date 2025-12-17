#include "Controller.hpp"
#include <ctime>

#ifndef RUN_TESTS 
int main() {
    srand(static_cast<unsigned int>(time(0)));
    Controller app;
    app.run();
    return 0;
}
#endif