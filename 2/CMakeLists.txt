cmake_minimum_required (VERSION 3.8)

project ("CMakeLab2")

# Включение coverage для Visual Studio
option(ENABLE_COVERAGE "Enable code coverage" ON)

if(ENABLE_COVERAGE)
    if(MSVC)
        message(STATUS "Building with Visual Studio code coverage")
        # Флаги для code coverage в Visual Studio
        add_compile_options("/DEBUG")
        add_compile_options("/Z7")
        add_link_options("/DEBUG")
        string(APPEND CMAKE_EXE_LINKER_FLAGS " /PROFILE")
    else()
        message(STATUS "Building with GCC/Clang code coverage")
        add_compile_options("--coverage" "-fprofile-arcs" "-ftest-coverage")
        add_link_options("--coverage" "-fprofile-arcs")
    endif()
endif()

# Локальная версия Catch2
add_subdirectory(lib/Catch2)

# Основное приложение
add_executable (CMakeLab2 
    CMakeLab2.cpp 
    CMakeLab2.h
    SimpleDomino.h SimpleDomino.cpp
    ComplexDomino.h ComplexDomino.cpp
)

# Тестовое приложение
add_executable (Tests 
    tests.cpp
    SimpleDomino.h SimpleDomino.cpp
    ComplexDomino.h ComplexDomino.cpp
)

target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeLab2 PROPERTY CXX_STANDARD 20)
  set_property(TARGET Tests PROPERTY CXX_STANDARD 20)
endif()

# Цель для генерации coverage отчета (только для Visual Studio)
if(ENABLE_COVERAGE AND MSVC)
    find_program(COVERAGE_TOOL "CodeCoverage.exe")
    if(COVERAGE_TOOL)
        add_custom_target(coverage
            COMMAND powershell -Command "
                $$coverageDir = \"${CMAKE_BINARY_DIR}/coverage\"
                if (Test-Path $$coverageDir) { Remove-Item -Recurse -Force $$coverageDir }
                New-Item -ItemType Directory -Path $$coverageDir | Out-Null
                
                $$testExe = \"${CMAKE_BINARY_DIR}/$<CONFIG>/Tests.exe\"
                $$coverageFile = \"$$coverageDir/coverage.coverage\"
                $$reportDir = \"$$coverageDir/report\"
                
                Write-Host 'Running tests with code coverage...'
                & '${COVERAGE_TOOL}' collect /output:$$coverageFile $$testExe
                
                Write-Host 'Generating report...'
                & '${COVERAGE_TOOL}' analyze /output:$$reportDir $$coverageFile
                
                Write-Host \"Coverage report generated in: $$reportDir\"
                if (Test-Path \"$$reportDir/index.html\") { 
                    Start-Process \"$$reportDir/index.html\" 
                }
            "
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS Tests
            COMMENT "Running tests with code coverage and generating report"
        )
    endif()
endif()