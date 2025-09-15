#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../game-source-code/Coordinate.h"
#include "../game-source-code/BlockGrid.h"
#include "../game-source-code/Player.h"

TEST_CASE("Terrain Management")
{
    BlockGrid terrain;

    SUBCASE("Default map initialization")
    {
        CHECK(terrain.isLocationBlocked(Coordinate(0, 0)) == false);
        CHECK(terrain.isLocationBlocked(Coordinate(0, 15)) == false);
        CHECK(terrain.isLocationBlocked(Coordinate(19, 15)) == false);
        CHECK(terrain.isLocationBlocked(Coordinate(10, 0)) == false);
        CHECK(terrain.isLocationBlocked(Coordinate(10, 29)) == false);

        CHECK(terrain.isLocationBlocked(Coordinate(5, 5)) == true);
    }

    SUBCASE("Tunnel digging")
    {
        Coordinate digPos(8, 8);

        CHECK(terrain.isLocationBlocked(digPos) == true);

        terrain.clearPassageAt(digPos);
        CHECK(terrain.isLocationBlocked(digPos) == false);
    }
}

TEST_CASE("Player Movement")
{
    Player player(Coordinate(1, 1));
    BlockGrid terrain;

    SUBCASE("Initial state")
    {
        CHECK(player.getPosition().row == 1);
        CHECK(player.getPosition().col == 1);
        CHECK(player.isActive() == true);
        CHECK(player.getTunnelsCreated() == 0);
    }

    SUBCASE("Movement in clear areas")
    {
        terrain.clearPassageAt(Coordinate(2, 1));
        terrain.clearPassageAt(Coordinate(1, 2));

        bool movedDown = player.moveInDirection(Direction::DOWN, terrain);
        CHECK(movedDown == true);
        CHECK(player.getPosition().row == 2);
        CHECK(player.getPosition().col == 1);

        bool movedRight = player.moveInDirection(Direction::RIGHT, terrain);
        CHECK(movedRight == true);
        CHECK(player.getPosition().row == 2);
        CHECK(player.getPosition().col == 2);
    }

    SUBCASE("Tunnel digging")
    {
        player.reset(Coordinate(5, 5));
        int initialTunnels = player.getTunnelsCreated();

        Coordinate earthPos(5, 6);
        CHECK(terrain.isLocationBlocked(earthPos) == true);

        bool dugTunnel = player.moveInDirection(Direction::RIGHT, terrain);
        CHECK(dugTunnel == true);
        CHECK(player.getPosition().col == 6);
        CHECK(terrain.isLocationBlocked(earthPos) == false);
        CHECK(player.getTunnelsCreated() == initialTunnels + 1);
    }
}

TEST_CASE("Boundary Restrictions")
{
    Player player(Coordinate(0, 0));
    BlockGrid terrain;

    SUBCASE("Top boundary")
    {
        bool movedUp = player.moveInDirection(Direction::UP, terrain);
        CHECK(movedUp == false);
        CHECK(player.getPosition().row == 0);
    }

    SUBCASE("Corner boundaries")
    {
        player.reset(Coordinate(0, 0));
        CHECK(player.moveInDirection(Direction::UP, terrain) == false);
        CHECK(player.moveInDirection(Direction::LEFT, terrain) == false);

        player.reset(Coordinate(19, 29));
        CHECK(player.moveInDirection(Direction::DOWN, terrain) == false);
        CHECK(player.moveInDirection(Direction::RIGHT, terrain) == false);
    }
}

TEST_CASE("Screen Environment")
{
    SUBCASE("Screen dimensions")
    {
        const int CELL_SIZE = 40;
        const int SCREEN_WIDTH = 1200;
        const int SCREEN_HEIGHT = 800;

        CHECK(SCREEN_WIDTH / CELL_SIZE == Coordinate::WORLD_COLS);
        CHECK(SCREEN_HEIGHT / CELL_SIZE == Coordinate::WORLD_ROWS);
    }
}
