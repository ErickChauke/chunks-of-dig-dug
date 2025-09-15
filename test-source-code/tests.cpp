#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../game-source-code/Coordinate.h"

TEST_CASE("Coordinate Basic Operations") {
    SUBCASE("Construction and access") {
        Coordinate pos(5, 10);
        CHECK(pos.row == 5);
        CHECK(pos.col == 10);
    }
    
    SUBCASE("Addition") {
        Coordinate pos1(1, 2);
        Coordinate pos2(3, 4);
        Coordinate result = pos1 + pos2;
        CHECK(result.row == 4);
        CHECK(result.col == 6);
    }
    
    SUBCASE("Bounds checking") {
        Coordinate valid(10, 15);
        Coordinate invalid(-1, 5);
        CHECK(valid.isWithinBounds() == true);
        CHECK(invalid.isWithinBounds() == false);
    }
}
