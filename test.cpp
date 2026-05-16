// Copyright 2026 Aanya Bharti

#define BOOST_TEST_MODULE SokobanTests

#include <sstream>

#include <boost/test/included/unit_test.hpp>

#include "Sokoban.hpp"

using SB::Direction;
using SB::Sokoban;

BOOST_AUTO_TEST_CASE(basic_movement) {
    std::string level =
        "5 5\n"
        "#####\n"
        "#   #\n"
        "# @ #\n"
        "#   #\n"
        "#####\n";

    std::istringstream input(level);
    Sokoban game;
    input >> game;

    auto before = game.playerLoc();
    game.movePlayer(Direction::Down);
    auto after = game.playerLoc();

    BOOST_CHECK(after != before);
}

BOOST_AUTO_TEST_CASE(wall_blocks) {
    std::string level =
        "5 5\n"
        "#####\n"
        "#@  #\n"
        "#   #\n"
        "#   #\n"
        "#####\n";

    std::istringstream input(level);
    Sokoban game;
    input >> game;

    auto before = game.playerLoc();
    game.movePlayer(Direction::Up);
    auto after = game.playerLoc();

    BOOST_CHECK(after == before);
}

BOOST_AUTO_TEST_CASE(box_push) {
    std::string level =
        "6 5\n"
        "######\n"
        "#    #\n"
        "# @A #\n"
        "#    #\n"
        "######\n";

    std::istringstream input(level);
    Sokoban game;
    input >> game;

    auto before = game.playerLoc();
    game.movePlayer(Direction::Right);
    auto after = game.playerLoc();

    BOOST_CHECK(after != before);
}

BOOST_AUTO_TEST_CASE(victory_condition) {
    std::string level =
        "5 5\n"
        "#####\n"
        "#   #\n"
        "# @1#\n"
        "#   #\n"
        "#####\n";

    std::istringstream input(level);
    Sokoban game;
    input >> game;

    BOOST_CHECK(game.isWon());
}
