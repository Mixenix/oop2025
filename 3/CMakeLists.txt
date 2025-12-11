cmake_minimum_required(VERSION 3.14)
project(SecuritySystemSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/Z7) 
else()
    add_compile_options(-g)
endif()

include_directories(src)

set(CORE_SOURCES
    src/Utils.hpp
    src/Module.cpp src/Module.hpp
    src/Platform.cpp src/Platform.hpp
    src/Intruder.cpp src/Intruder.hpp
    src/Environment.cpp src/Environment.hpp
    src/AI.cpp src/AI.hpp
    src/View.cpp src/View.hpp
    src/Controller.cpp src/Controller.hpp
)

add_library(SecurityLib ${CORE_SOURCES})


add_executable(SecuritySim src/main.cpp)
target_link_libraries(SecuritySim PRIVATE SecurityLib)


add_executable(SecurityBenchmark tests/Benchmark.cpp)
target_link_libraries(SecurityBenchmark PRIVATE SecurityLib)


include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(SecurityTests 
    tests/TestUtils.cpp
    tests/TestLogic.cpp
)

target_link_libraries(SecurityTests PRIVATE SecurityLib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(SecurityTests)