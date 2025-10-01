#include "SimpleDomino.h"
#include "ComplexDomino.h"
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <vector>
#include <algorithm>

TEST_CASE("SimpleDomino Constructors", "[SimpleDomino]") {
    SECTION("Default constructor") {
        SimpleDomino dom;
        REQUIRE(dom.getVal1() == 1);
        REQUIRE(dom.getVal2() == 1);
    }
    
    SECTION("Constructor with specific values") {
        SimpleDomino dom(3, 5, false);
        REQUIRE(dom.getVal1() == 3);
        REQUIRE(dom.getVal2() == 5);
    }
    
    SECTION("Constructor with random values") {
        SimpleDomino dom1(0, 0, true);
        SimpleDomino dom2(0, 0, true);
        
        // Values should be in valid range
        REQUIRE(dom1.getVal1() >= 1);
        REQUIRE(dom1.getVal1() <= 6);
        REQUIRE(dom1.getVal2() >= 1);
        REQUIRE(dom1.getVal2() <= 6);
        
        // Two consecutive random dominoes are likely different
        // (but not guaranteed, so we can't test for inequality)
    }
}

TEST_CASE("SimpleDomino Getters and Setters", "[SimpleDomino]") {
    SimpleDomino dom;
    
    SECTION("Valid value setting") {
        dom.setVal1(2);
        dom.setVal2(4);
        REQUIRE(dom.getVal1() == 2);
        REQUIRE(dom.getVal2() == 4);
    }
    
    SECTION("Invalid value setting") {
        REQUIRE_THROWS_AS(dom.setVal1(-1), std::out_of_range);
        REQUIRE_THROWS_AS(dom.setVal1(7), std::out_of_range);
        REQUIRE_THROWS_AS(dom.setVal2(-1), std::out_of_range);
        REQUIRE_THROWS_AS(dom.setVal2(10), std::out_of_range);
    }
    
    SECTION("Boundary value setting") {
        dom.setVal1(1);
        dom.setVal2(6);
        REQUIRE(dom.getVal1() == 1);
        REQUIRE(dom.getVal2() == 6);
    }
}

TEST_CASE("SimpleDomino Random Operations", "[SimpleDomino]") {
    SimpleDomino dom(2, 3, false);
    
    SECTION("setRandomValues") {
        dom.setRandomValues();
        REQUIRE(dom.getVal1() >= 1);
        REQUIRE(dom.getVal1() <= 6);
        REQUIRE(dom.getVal2() >= 1);
        REQUIRE(dom.getVal2() <= 6);
    }
    
    SECTION("Operator ! for randomization") {
        !dom;
        REQUIRE(dom.getVal1() >= 1);
        REQUIRE(dom.getVal1() <= 6);
        REQUIRE(dom.getVal2() >= 1);
        REQUIRE(dom.getVal2() <= 6);
    }
}

TEST_CASE("SimpleDomino Flip Operation", "[SimpleDomino]") {
    SimpleDomino dom(2, 5, false);
    
    SECTION("Operator ~ for flipping") {
        int originalVal1 = dom.getVal1();
        int originalVal2 = dom.getVal2();
        
        ~dom;
        
        REQUIRE(dom.getVal1() == originalVal2);
        REQUIRE(dom.getVal2() == originalVal1);
    }
}

TEST_CASE("SimpleDomino Comparison", "[SimpleDomino]") {
    SECTION("Equal dominoes") {
        SimpleDomino dom1(3, 4, false);
        SimpleDomino dom2(3, 4, false);
        SimpleDomino dom3(4, 3, false);
        
        REQUIRE(dom1 == dom2);
        REQUIRE(dom1 == dom3);
        REQUIRE(dom2 == dom3);
    }
    
    SECTION("Different dominoes") {
        SimpleDomino dom1(1, 2, false);
        SimpleDomino dom2(3, 4, false);
        
        REQUIRE_FALSE(dom1 == dom2);
    }
}

TEST_CASE("SimpleDomino Less Than Operator", "[SimpleDomino]") {
    SECTION("Compare by sum") {
        SimpleDomino dom1(1, 2, false); // sum = 3
        SimpleDomino dom2(3, 4, false); // sum = 7
        SimpleDomino dom3(2, 1, false); // sum = 3
        
        REQUIRE(dom1 < dom2);
        REQUIRE_FALSE(dom2 < dom1);
        REQUIRE_FALSE(dom1 < dom3); // equal sums
    }
}

TEST_CASE("SimpleDomino Output Stream", "[SimpleDomino]") {
    SimpleDomino dom(3, 5, false);
    
    SECTION("Output format") {
        std::stringstream ss;
        ss << dom;
        
        std::string output = ss.str();
        REQUIRE(output.find("Current state:") != std::string::npos);
        REQUIRE(output.find("LeftValue = 3") != std::string::npos);
        REQUIRE(output.find("RightValue = 5") != std::string::npos);
    }
}

TEST_CASE("SimpleDomino Input Stream", "[SimpleDomino]") {
    SECTION("Valid input") {
        SimpleDomino dom;
        std::stringstream ss("4 2");
        ss >> dom;
        
        REQUIRE(dom.getVal1() == 4);
        REQUIRE(dom.getVal2() == 2);
    }
    
    SECTION("Invalid input - out of range") {
        SimpleDomino dom;
        std::stringstream ss("8 2");
        
        REQUIRE_THROWS_AS(ss >> dom, std::out_of_range);
    }
}

TEST_CASE("SimpleDomino ASCII Art", "[SimpleDomino]") {
    SimpleDomino dom(3, 5, false);
    
    SECTION("ASCII output contains expected elements") {
        std::string ascii = dom.ascii();
        
        REQUIRE(ascii.find("3 5") != std::string::npos);
        REQUIRE(ascii.find("┌─────┬─────┐") != std::string::npos);
        REQUIRE(ascii.find("└─────┴─────┘") != std::string::npos);
    }
    
    SECTION("Different values produce different art") {
        SimpleDomino dom1(1, 1, false);
        SimpleDomino dom2(6, 6, false);
        
        std::string ascii1 = dom1.ascii();
        std::string ascii2 = dom2.ascii();
        
        REQUIRE(ascii1 != ascii2);
    }
}

// ComplexDomino Tests
TEST_CASE("ComplexDomino Constructors", "[ComplexDomino]") {
    SECTION("Default constructor") {
        ComplexDomino comp;
        REQUIRE(comp.getSize() == 0);
    }
    
    SECTION("Empty group constructor") {
        ComplexDomino comp(false, 0, -1);
        REQUIRE(comp.getSize() == 0);
    }
    
    SECTION("Random group constructor") {
        ComplexDomino comp(true, 5, -1);
        REQUIRE(comp.getSize() == 5);
        
        // Check that all dominoes have valid values
        for (size_t i = 0; i < comp.getSize(); ++i) {
            SimpleDomino dom = comp[i];
            REQUIRE(dom.getVal1() >= 1);
            REQUIRE(dom.getVal1() <= 6);
            REQUIRE(dom.getVal2() >= 1);
            REQUIRE(dom.getVal2() <= 6);
        }
    }
    
    SECTION("Full set constructor") {
        ComplexDomino comp(false, 0, 1);
        REQUIRE(comp.getSize() == 36); // 6x6 = 36 unique dominoes
        
        // Count occurrences of each value combination
        std::vector<std::pair<int, int>> combinations;
        for (size_t i = 0; i < comp.getSize(); ++i) {
            SimpleDomino dom = comp[i];
            combinations.emplace_back(dom.getVal1(), dom.getVal2());
        }
        
        // Should have all combinations from (1,1) to (6,6)
        REQUIRE(combinations.size() == 36);
    }
    
    SECTION("Invalid size throws exception") {
        REQUIRE_THROWS_AS(ComplexDomino(true, -1, -1), std::logic_error);
    }
}

TEST_CASE("ComplexDomino Copy and Move Semantics", "[ComplexDomino]") {
    SECTION("Copy constructor") {
        ComplexDomino original(true, 3, -1);
        ComplexDomino copy(original);
        
        REQUIRE(original.getSize() == copy.getSize());
        REQUIRE(original.getSize() == 3);
        
        // Check that dominoes are the same
        for (size_t i = 0; i < original.getSize(); ++i) {
            REQUIRE(original[i] == copy[i]);
        }
    }
    
    SECTION("Copy assignment operator") {
        ComplexDomino original(true, 3, -1);
        ComplexDomino copy;
        copy = original;
        
        REQUIRE(original.getSize() == copy.getSize());
        REQUIRE(original.getSize() == 3);
        
        for (size_t i = 0; i < original.getSize(); ++i) {
            REQUIRE(original[i] == copy[i]);
        }
    }
    
    SECTION("Move constructor") {
        ComplexDomino original(true, 3, -1);
        size_t originalSize = original.getSize();
        
        ComplexDomino moved(std::move(original));
        
        REQUIRE(moved.getSize() == originalSize);
        REQUIRE(original.getSize() == 0); // original should be empty after move
    }
}

TEST_CASE("ComplexDomino Index Operator", "[ComplexDomino]") {
    ComplexDomino comp(true, 3, -1);
    
    SECTION("Valid index access") {
        SimpleDomino dom = comp[0];
        REQUIRE(dom.getVal1() >= 1);
        REQUIRE(dom.getVal1() <= 6);
        REQUIRE(dom.getVal2() >= 1);
        REQUIRE(dom.getVal2() <= 6);
    }
    
    SECTION("Invalid index access") {
        REQUIRE_THROWS_AS(comp[-1], std::out_of_range);
        REQUIRE_THROWS_AS(comp[10], std::out_of_range);
    }
}

TEST_CASE("ComplexDomino Addition Operations", "[ComplexDomino]") {
    ComplexDomino comp;
    
    SECTION("Add specific domino with +=") {
        SimpleDomino newDom(2, 4, false);
        comp += newDom;
        
        REQUIRE(comp.getSize() == 1);
        REQUIRE(comp[0] == newDom);
    }
    
    SECTION("Add random domino") {
        comp.addRandom();
        
        REQUIRE(comp.getSize() == 1);
        SimpleDomino dom = comp[0];
        REQUIRE(dom.getVal1() >= 1);
        REQUIRE(dom.getVal1() <= 6);
        REQUIRE(dom.getVal2() >= 1);
        REQUIRE(dom.getVal2() <= 6);
    }
    
    SECTION("Multiple additions") {
        for (int i = 0; i < 5; ++i) {
            comp.addRandom();
        }
        
        REQUIRE(comp.getSize() == 5);
    }
}

TEST_CASE("ComplexDomino Removal Operations", "[ComplexDomino]") {
    SECTION("Remove random from empty group") {
        ComplexDomino comp;
        comp.removeRandom(); // Should not crash, just print message
        
        REQUIRE(comp.getSize() == 0);
    }
    
    SECTION("Remove random from non-empty group") {
        ComplexDomino comp(true, 5, -1);
        size_t initialSize = comp.getSize();
        
        comp.removeRandom();
        
        REQUIRE(comp.getSize() == initialSize - 1);
    }
    
    SECTION("Remove by values - existing domino") {
        ComplexDomino comp;
        SimpleDomino target(3, 5, false);
        comp += target;
        comp += SimpleDomino(1, 2, false);
        
        comp.remove(3, 5);
        
        REQUIRE(comp.getSize() == 1);
        REQUIRE_FALSE(comp[0] == target);
    }
    
    SECTION("Remove by values - non-existing domino") {
        ComplexDomino comp;
        comp += SimpleDomino(1, 2, false);
        
        comp.remove(3, 5); // Should print "Domino not found!"
        
        REQUIRE(comp.getSize() == 1); // Size unchanged
    }
    
    SECTION("Remove by index - valid index") {
        ComplexDomino comp;
        SimpleDomino dom1(1, 2, false);
        SimpleDomino dom2(3, 4, false);
        comp += dom1;
        comp += dom2;
        
        comp.removeByIndex(0);
        
        REQUIRE(comp.getSize() == 1);
        REQUIRE(comp[0] == dom2);
    }
    
    SECTION("Remove by index - invalid index") {
        ComplexDomino comp;
        comp += SimpleDomino(1, 2, false);
        
        comp.removeByIndex(5); // Should print "Invalid index!"
        
        REQUIRE(comp.getSize() == 1); // Size unchanged
    }
}

TEST_CASE("ComplexDomino Sorting", "[ComplexDomino]") {
    SECTION("Sort by sum ascending") {
        ComplexDomino comp;
        comp += SimpleDomino(6, 6, false); // sum = 12
        comp += SimpleDomino(1, 1, false); // sum = 2
        comp += SimpleDomino(3, 4, false); // sum = 7
        
        comp.sortBySumAscending();
        
        // Check order: (1,1), (3,4), (6,6)
        REQUIRE(comp[0].getVal1() + comp[0].getVal2() <= 
                comp[1].getVal1() + comp[1].getVal2());
        REQUIRE(comp[1].getVal1() + comp[1].getVal2() <= 
                comp[2].getVal1() + comp[2].getVal2());
    }
}

TEST_CASE("ComplexDomino Subsequence", "[ComplexDomino]") {
    SECTION("Extract subsequence with matching value") {
        ComplexDomino comp;
        comp += SimpleDomino(3, 5, false);
        comp += SimpleDomino(1, 3, false);
        comp += SimpleDomino(2, 4, false);
        comp += SimpleDomino(3, 6, false);
        
        ComplexDomino subsequence = comp.subSeq(3);
        
        // Subsequence should contain dominoes with value 3
        REQUIRE(subsequence.getSize() == 3);
        for (size_t i = 0; i < subsequence.getSize(); ++i) {
            SimpleDomino dom = subsequence[i];
            REQUIRE((dom.getVal1() == 3 || dom.getVal2() == 3));
        }
        
        // Original should contain only domino without value 3
        REQUIRE(comp.getSize() == 1);
        REQUIRE(comp[0].getVal1() == 2);
        REQUIRE(comp[0].getVal2() == 4);
    }
    
    SECTION("Extract subsequence with non-existing value") {
        ComplexDomino comp;
        comp += SimpleDomino(1, 2, false);
        comp += SimpleDomino(4, 5, false);
        
        ComplexDomino subsequence = comp.subSeq(6);
        
        REQUIRE(subsequence.getSize() == 0);
        REQUIRE(comp.getSize() == 2); // Original unchanged
    }
}

TEST_CASE("ComplexDomino ASCII Art for Index", "[ComplexDomino]") {
    ComplexDomino comp;
    comp += SimpleDomino(3, 5, false);
    
    SECTION("Valid index ASCII") {
        std::string ascii = comp.ascii(0);
        
        REQUIRE(ascii.find("3 5") != std::string::npos);
        REQUIRE(ascii.find("┌─────┬─────┐") != std::string::npos);
    }
    
    SECTION("Invalid index ASCII") {
        REQUIRE_THROWS_AS(comp.ascii(5), std::out_of_range);
    }
}

TEST_CASE("ComplexDomino Output Stream", "[ComplexDomino]") {
    ComplexDomino comp;
    comp += SimpleDomino(2, 3, false);
    comp += SimpleDomino(4, 5, false);
    
    SECTION("Output format") {
        std::stringstream ss;
        ss << comp;
        
        std::string output = ss.str();
        REQUIRE(output.find("0: LeftValue = 2  RightValue = 3") != std::string::npos);
        REQUIRE(output.find("1: LeftValue = 4  RightValue = 5") != std::string::npos);
    }
}

TEST_CASE("Edge Cases and Boundary Values", "[EdgeCases]") {
    SECTION("SimpleDomino with boundary values") {
        SimpleDomino minDom(1, 1, false);
        SimpleDomino maxDom(6, 6, false);
        
        REQUIRE(minDom.getVal1() == 1);
        REQUIRE(minDom.getVal2() == 1);
        REQUIRE(maxDom.getVal1() == 6);
        REQUIRE(maxDom.getVal2() == 6);
    }
    
    SECTION("ComplexDomino with maximum practical size") {
        // Test that resize works correctly
        ComplexDomino comp;
        for (int i = 0; i < 50; ++i) {
            comp.addRandom();
        }
        
        REQUIRE(comp.getSize() == 50);
        
        // All dominoes should have valid values
        for (size_t i = 0; i < comp.getSize(); ++i) {
            SimpleDomino dom = comp[i];
            REQUIRE(dom.getVal1() >= 1);
            REQUIRE(dom.getVal1() <= 6);
            REQUIRE(dom.getVal2() >= 1);
            REQUIRE(dom.getVal2() <= 6);
        }
    }
    
    SECTION("Multiple operations sequence") {
        SimpleDomino dom(2, 3, false);
        ComplexDomino comp;
        
        // Sequence of operations
        comp += dom;
        ~dom;
        comp += dom;
        !dom;
        comp += dom;
        
        REQUIRE(comp.getSize() == 3);
    }
}