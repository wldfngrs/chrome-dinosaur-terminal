#ifndef PLAYER_H
#define PLAYER_H

#include <ncurses.h>

#include <string>
#include <vector>

#include "game.h"

struct Player {
  std::vector<vec2i> pos;
  std::string disp_char{"       ____ "
                        "      /0___}"
                        ",     ||    "
                        "|\\   / |    "
                        "| \\_/  =    "
                        " \\     |    "
                        "  \\ _  /    "
                        "  |/ [\\     "
                        "__[\\_______"};

  std::vector<vec2i> crouch_pos;

  std::string crouch_disp_char{",         ____ "
                               "|\\_______/0___}"
                               " \\       /     "
                               "  \\  _  /=     "
                               "   |/ [\\       "
                               "___[\\__________"};
  bool crouched;
  bool up;

  int air_time;
  int max_air_time;

  int score;

  rect bounds;
};

void initPlayerPosition(WINDOW *window, Player *player);
void clearPlayerPosition(WINDOW *window, Player *player);
void movePlayerUp(WINDOW *window, Player *player);
void movePlayerDown(WINDOW *window, Player *player);
void crouchPlayer(WINDOW *window, Player *player);
void playerFeetAnimation(WINDOW *window, Player *player);
void playerDead();

#endif
