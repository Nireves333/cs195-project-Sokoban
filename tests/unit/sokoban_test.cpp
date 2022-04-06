#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <time.h>
#include "doctest.h"
#include "../../src/sokoban.hpp"
using Direction = Sokoban::Direction;

TEST_CASE("returns the board") {
    std::vector<std::vector<std::string>> levels = {{
        "#####",
        "#@$.#",
        "#####",
    }};

    Sokoban soko(levels);
    CHECK(soko.board() == levels.at(0));
}

TEST_CASE("should return the current level") {
    Sokoban soko({{
        "#####",
        "#@$.#",
        "#####",
    }});
    CHECK(soko.level() == 0);
}

TEST_CASE("should be solved when all boxes are on goals") {
    Sokoban soko({{
        "#####",
        "# @*#",
        "#####",
    }});
    CHECK(soko.solved());
}

TEST_CASE("should not be solved when player is on goal") {
    Sokoban soko({{
        "#####",
        "#$ +#",
        "#####",
    }});
    CHECK_FALSE(soko.solved());
}

TEST_CASE("should not be solved when empty goal cell is present") {
    Sokoban soko({{
        "#####",
        "#@$.#",
        "#####",
    }});
    CHECK_FALSE(soko.solved());
}


TEST_SUITE("Test cases for move() - player only") {

    TEST_CASE("should not mutate the board when move is invalid") {
        Sokoban soko({{
            "####",
            "#@ #",
            "####",
        }});
        std::vector<std::string> expected = {
            "####",
            "#@ #",
            "####",
        };
        CHECK_FALSE(soko.move(Direction::U));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player down onto a goal cell") {
        Sokoban soko({{
            "#####",
            "# @ #",
            "# . #",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#   #",
            "# + #",
            "#####",
        };
        CHECK(soko.move(Direction::D));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player off of goal cell to the left") {
        Sokoban soko({{
            "#####",
            "#  +#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "# @.#",
            "#####",
        };
        CHECK(soko.move(Direction::L));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move left then up") {
        Sokoban soko({{
            "####",
            "#  #",
            "# @#",
            "####",
        }});
        std::vector<std::string> expected = {
            "####",
            "#@ #",
            "#  #",
            "####",
        };
        CHECK(soko.move(Direction::L));
        CHECK(soko.move(Direction::U));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move right then down") {
        Sokoban soko({{
            "####",
            "#@ #",
            "#  #",
            "####",
        }});
        std::vector<std::string> expected = {
            "####",
            "#  #",
            "# @#",
            "####",
        };
        CHECK(soko.move(Direction::R));
        CHECK(soko.move(Direction::D));
        CHECK(soko.board() == expected);
    }
}


TEST_SUITE("Test cases for move() - player and box pushing") {

    TEST_CASE("should move player and box to the right on an empty cell") {
        Sokoban soko({{
            "#####",
            "#@$ #",
            "#   #",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "# @$#",
            "#   #",
            "#####",
        };
        CHECK(soko.move(Direction::R));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player and box-on-goal to the left on an empty cell") {
        Sokoban soko({{
            "#####",
            "# *@#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#$+ #",
            "#####",
        };
        CHECK(soko.move(Direction::L));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player-on-goal and box-on-goal up on an empty cell") {
        Sokoban soko({{
            "###",
            "# #",
            "#*#",
            "#+#",
            "###",
        }});
        std::vector<std::string> expected = {
            "###",
            "#$#",
            "#+#",
            "#.#",
            "###",
        };
        CHECK(soko.move(Direction::U));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player-on-goal and box-on-goal down on a goal") {
        Sokoban soko({{
            "###",
            "#+#",
            "#*#",
            "#.#",
            "###",
        }});
        std::vector<std::string> expected = {
            "###",
            "#.#",
            "#+#",
            "#*#",
            "###",
        };
        CHECK(soko.move(Direction::D));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should not mutate the board when move is invalid") {
        Sokoban soko({{
            "####",
            "#+$#",
            "####",
        }});
        std::vector<std::string> expected = {
            "####",
            "#+$#",
            "####",
        };
        CHECK_FALSE(soko.move(Direction::R));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should not mutate the board when move is invalid") {
        Sokoban soko({{
            "###",
            "#@#",
            "#$#",
            "#$#",
            "###",
        }});
        std::vector<std::string> expected = {
            "###",
            "#@#",
            "#$#",
            "#$#",
            "###",
        };
        CHECK_FALSE(soko.move(Direction::D));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move box to a goal with Left-Up sequence") {
        Sokoban soko({{
            "####",
            "#. #",
            "#$ #",
            "# @#",
            "####",
        }});

        std::vector<std::string> expected = {
            "####",
            "#* #",
            "#@ #",
            "#  #",
            "####",
        };
        CHECK(soko.move(Direction::L));
        CHECK(soko.move(Direction::U));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move box to a goal with Down-Right sequence") {
        Sokoban soko({{
            "#####",
            "#@  #",
            "# $.#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#   #",
            "# @*#",
            "#####",
        };
        CHECK(soko.move(Direction::D));
        CHECK(soko.move(Direction::R));
        CHECK(soko.board() == expected);
    }
}


TEST_SUITE("Test cases for undo()") {

    TEST_CASE("should move player down onto a goal cell, then revert to original") {
        Sokoban soko({{
            "#####",
            "# @ #",
            "# . #",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "# @ #",
            "# . #",
            "#####",
        };
        CHECK(soko.move(Direction::D));
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player off of goal cell to the left, then revert to original") {
        Sokoban soko({{
            "#####",
            "#  +#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#  +#",
            "#####",
        };
        CHECK(soko.move(Direction::L));
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move left then up, then revert to original") {
        Sokoban soko({{
            "####",
            "#  #",
            "# @#",
            "####",
        }});
        std::vector<std::string> expected = {
            "####",
            "#  #",
            "# @#",
            "####",
        };
        CHECK(soko.move(Direction::L));
        CHECK(soko.move(Direction::U));
        CHECK(soko.undo());
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move right then down, then undo last move") {
        Sokoban soko({{
            "####",
            "#@ #",
            "#  #",
            "####",
        }});
        std::vector<std::string> expected = {
            "####",
            "# @#",
            "#  #",
            "####",
        };
        CHECK(soko.move(Direction::R));
        CHECK(soko.move(Direction::D));
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player and box to the right on an empty cell, then revert to original") {
        Sokoban soko({{
            "#####",
            "#@$ #",
            "#   #",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#@$ #",
            "#   #",
            "#####",
        };
        CHECK(soko.move(Direction::R));
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player and box-on-goal to the left on an empty cell, then revert to original") {
        Sokoban soko({{
            "#####",
            "# *@#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "# *@#",
            "#####",
        };
        CHECK(soko.move(Direction::L));
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player-on-goal and box-on-goal up on an empty cell, then revert to original") {
        Sokoban soko({{
            "###",
            "# #",
            "#*#",
            "#+#",
            "###",
        }});
        std::vector<std::string> expected = {
            "###",
            "# #",
            "#*#",
            "#+#",
            "###",
        };
        CHECK(soko.move(Direction::U));
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player-on-goal and box-on-goal down on a goal, then revert to original") {
        Sokoban soko({{
            "###",
            "#+#",
            "#*#",
            "#.#",
            "###",
        }});
        std::vector<std::string> expected = {
            "###",
            "#+#",
            "#*#",
            "#.#",
            "###",
        };
        CHECK(soko.move(Direction::D));
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move box to a goal with Left-Up sequence, then revert to original") {
        Sokoban soko({{
            "####",
            "#. #",
            "#$ #",
            "# @#",
            "####",
        }});
        std::vector<std::string> expected = {
            "####",
            "#. #",
            "#$ #",
            "# @#",
            "####",
        };
        CHECK(soko.move(Direction::L));
        CHECK(soko.move(Direction::U));
        CHECK(soko.undo());
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move box to a goal with Down-Right sequence, then revert to original") {
        Sokoban soko({{
            "#####",
            "#@  #",
            "# $.#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#@  #",
            "# $.#",
            "#####",
        };
        CHECK(soko.move(Direction::D));
        CHECK(soko.move(Direction::R));
        CHECK(soko.undo());
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should return false for an invalid undo()") {
        Sokoban soko({{
            "#####",
            "#@  #",
            "# $.#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#@  #",
            "# $.#",
            "#####",
        };
        CHECK_FALSE(soko.move(Direction::U));
        CHECK_FALSE(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("After RLR-undo, player should have not pulled the box") {
        Sokoban soko({{
            "#####",
            "#@$.#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#@ *#",
            "#####",
        };
        CHECK(soko.move(Direction::R));
        CHECK(soko.move(Direction::L));
        CHECK(soko.move(Direction::R));
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("After RLR-undo-undo, player should vacate an empty cell") {
        Sokoban soko({{
            "#####",
            "#@$.#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "# @*#",
            "#####",
        };
        CHECK(soko.move(Direction::R));
        CHECK(soko.move(Direction::L));
        CHECK(soko.move(Direction::R));
        CHECK(soko.undo());
        CHECK(soko.undo());
        CHECK(soko.board() == expected);
    }
}


TEST_SUITE("Test cases for redo()") {

    TEST_CASE("should return false for an invalid redo()") {
        Sokoban soko({{
            "#####",
            "#   #",
            "#@$.#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#@  #",
            "# $.#",
            "#####",
        };
        CHECK(soko.move(Direction::U));
        CHECK_FALSE(soko.redo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should redo the move after undo") {
        Sokoban soko({{
            "#####",
            "#@  #",
            "# $.#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#   #",
            "#@$.#",
            "#####",
        };
        CHECK(soko.move(Direction::D));
        CHECK(soko.undo());
        CHECK(soko.redo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should return false after move-undo-move") {
        Sokoban soko({{
            "######",
            "#@$. #",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "# @* #",
            "######",
        };
        CHECK(soko.move(Direction::R));
        CHECK(soko.undo());
        CHECK(soko.move(Direction::R));
        CHECK_FALSE(soko.redo());
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should do multiple moves, undo(), and redo()") {
        Sokoban soko({{
            "######",
            "#    #",
            "#+*. #",
            "#    #",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#    #",
            "#.*. #",
            "#   @#",
            "######",
        };
        CHECK(soko.move(Direction::U));
        CHECK(soko.move(Direction::R));
        CHECK(soko.move(Direction::R));
        CHECK(soko.move(Direction::D));
        CHECK(soko.move(Direction::L));
        CHECK(soko.undo());
        CHECK(soko.undo());
        CHECK(soko.undo());
        CHECK(soko.redo());
        CHECK(soko.redo());
        CHECK(soko.move(Direction::D));
        CHECK_FALSE(soko.redo());
        CHECK(soko.move(Direction::R));
        CHECK(soko.board() == expected);
    }
}

TEST_SUITE("Test cases for the bfs version of move(y, x)") {

    TEST_CASE("should move player to bottom right corder of board") {
        Sokoban soko({{
            "######",
            "#+*. #",
            "#    #",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#.*. #",
            "#   @#",
            "######",
        };
        CHECK(soko.move(2, 4));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player to top right corner of board") {
        Sokoban soko({{
            "######",
            "#    #",
            "#+*. #",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#   @#",
            "#.*. #",
            "######",
        };
        CHECK(soko.move(1, 4));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player to bottom left corner of board") {
        Sokoban soko({{
            "######",
            "#   @#",
            "#.*. #",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#    #",
            "#+*. #",
            "######",
        };
        CHECK(soko.move(2, 1));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should move player to top left corder of board") {
        Sokoban soko({{
            "######",
            "#.*. #",
            "#   @#",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#+*. #",
            "#    #",
            "######",
        };
        CHECK(soko.move(1, 1));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("player should move around the obstacle") {
        Sokoban soko({{
            "######",
            "#    #",
            "#.*. #",
            "# $ @#",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#    #",
            "#+*. #",
            "# $  #",
            "######",
        };
        CHECK(soko.move(2, 1));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("player should move to the valid adjacent cell") {
        Sokoban soko({{
            "######",
            "#    #",
            "#.*. #",
            "# $@ #",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#    #",
            "#.*. #",
            "#$@  #",
            "######",
        };
        CHECK(soko.move(3, 2));
        soko.print_board();
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should not move player when the destination is the current position") {
        Sokoban soko({{
            "######",
            "#@#*.#",
            "#    #",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#@#*.#",
            "#    #",
            "######",
        };
        CHECK_FALSE(soko.move(1, 1));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should not mutate board when the specified destination is out-of-bounds") {
        Sokoban soko({{
            "#####",
            "#+*.#",
            "#####",
        }});
        std::vector<std::string> expected = {
            "#####",
            "#+*.#",
            "#####",
        };
        CHECK_FALSE(soko.move(5, 6));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should not move player to a destination with a box") {
        Sokoban soko({{
            "######",
            "#.*. #",
            "#   @#",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#.*. #",
            "#   @#",
            "######",
        };
        CHECK_FALSE(soko.move(1, 2));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should not move player to a destination with a wall") {
        Sokoban soko({{
            "######",
            "#.*. #",
            "#   @#",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#.*. #",
            "#   @#",
            "######",
        };
        CHECK_FALSE(soko.move(0, 0));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should not move player beyond the barricaded boxes") {
        Sokoban soko({{
            "######",
            "#@*. #",
            "# $  #",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#@*. #",
            "# $  #",
            "######",
        };
        CHECK_FALSE(soko.move(2, 3));
        CHECK(soko.board() == expected);
    }

    TEST_CASE("should not move player beyond the barricaded wall and box") {
        Sokoban soko({{
            "######",
            "#@#*.#",
            "# $  #",
            "######",
        }});
        std::vector<std::string> expected = {
            "######",
            "#@#*.#",
            "# $  #",
            "######",
        };
        CHECK_FALSE(soko.move(2, 3));
        CHECK(soko.board() == expected);
    }

}