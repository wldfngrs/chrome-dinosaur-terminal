#include <unistd.h>

#include "player.h"

/* MAKE THE PLAYABLE CHARACTER SWITCHABLE FROM A MENU */

/* pos[i] is 'co' to disp_char[i] */
void initPlayerPosition(WINDOW *game_wnd, Player *player) { /* {x,y} */
  int y_start = 11;
  player->pos = {
      {10, y_start},     {11, y_start},     {12, y_start},
      {13, y_start},     {14, y_start},     {15, y_start},
      {16, y_start},     {17, y_start},     {18, y_start},
      {19, y_start},     {20, y_start},     {21, y_start},
      {10, y_start + 1}, {11, y_start + 1}, {12, y_start + 1},
      {13, y_start + 1}, {14, y_start + 1}, {15, y_start + 1},
      {16, y_start + 1}, {17, y_start + 1}, {18, y_start + 1},
      {19, y_start + 1}, {20, y_start + 1}, {21, y_start + 1},
      {10, y_start + 2}, {11, y_start + 2}, {12, y_start + 2},
      {13, y_start + 2}, {14, y_start + 2}, {15, y_start + 2},
      {16, y_start + 2}, {17, y_start + 2}, {18, y_start + 2},
      {19, y_start + 2}, {20, y_start + 2}, {21, y_start + 2},
      {10, y_start + 3}, {11, y_start + 3}, {12, y_start + 3},
      {13, y_start + 3}, {14, y_start + 3}, {15, y_start + 3},
      {16, y_start + 3}, {17, y_start + 3}, {18, y_start + 3},
      {19, y_start + 3}, {20, y_start + 3}, {21, y_start + 3},
      {10, y_start + 4}, {11, y_start + 4}, {12, y_start + 4},
      {13, y_start + 4}, {14, y_start + 4}, {15, y_start + 4},
      {16, y_start + 4}, {17, y_start + 4}, {18, y_start + 4},
      {19, y_start + 4}, {20, y_start + 4}, {21, y_start + 4},
      {10, y_start + 5}, {11, y_start + 5}, {12, y_start + 5},
      {13, y_start + 5}, {14, y_start + 5}, {15, y_start + 5},
      {16, y_start + 5}, {17, y_start + 5}, {18, y_start + 5},
      {19, y_start + 5}, {20, y_start + 5}, {21, y_start + 5},
      {10, y_start + 6}, {11, y_start + 6}, {12, y_start + 6},
      {13, y_start + 6}, {14, y_start + 6}, {15, y_start + 6},
      {16, y_start + 6}, {17, y_start + 6}, {18, y_start + 6},
      {19, y_start + 6}, {20, y_start + 6}, {21, y_start + 6},
      {10, y_start + 7}, {11, y_start + 7}, {12, y_start + 7},
      {13, y_start + 7}, {14, y_start + 7}, {15, y_start + 7},
      {16, y_start + 7}, {17, y_start + 7}, {18, y_start + 7},
      {19, y_start + 7}, {20, y_start + 7}, {21, y_start + 7},
      {10, y_start + 8}, {11, y_start + 8}, {12, y_start + 8},
      {13, y_start + 8}, {14, y_start + 8}, {15, y_start + 8},
      {16, y_start + 8}, {17, y_start + 8}, {18, y_start + 8},
      {19, y_start + 8}, {20, y_start + 8}, {21, y_start + 8},
  };

  for (size_t i = 0; i < player->disp_char.size(); i++) {
    mvwaddch(game_wnd, player->pos[i].y, player->pos[i].x,
             static_cast<unsigned>(player->disp_char[i]));
  }

  player->crouched = false;
}

/*
void playerDead() {
        player->disp_char[3] = '"';
        player->disp_char[4] = '"';
}*/

void clearPlayerPosition(WINDOW *game_wnd, Player *player) {
  if (player->crouched) {
    for (size_t i = 0; i < player->crouch_disp_char.size(); i++) {
      if (player->crouch_pos[i].y == 19) {
        mvwaddch(game_wnd, player->crouch_pos[i].y, player->crouch_pos[i].x,
                 '_');
      } else {
        mvwaddch(game_wnd, player->crouch_pos[i].y, player->crouch_pos[i].x,
                 ' ');
      }
    }
    player->crouched = false;

    return;
  }

  for (size_t i = 0; i < player->disp_char.size(); i++) {
    if (player->pos[i].y == 19) {
      mvwaddch(game_wnd, player->pos[i].y, player->pos[i].x, '_');
    } else {
      mvwaddch(game_wnd, player->pos[i].y, player->pos[i].x, ' ');
    }
  }
}

void movePlayerDown(WINDOW *game_wnd, Player *player) {
  clearPlayerPosition(game_wnd, player);

  player->disp_char = "       ____ "
                      "      /0___}"
                      ",     ||    "
                      "|\\   / |    "
                      "| \\_/  =    "
                      " \\     |    "
                      "  \\ _  /    "
                      "  |/ [\\     "
                      "__[\\________";

  for (size_t i = 0; i < player->disp_char.size(); i++) {
    player->pos[i].y += 8;
    mvwaddch(game_wnd, player->pos[i].y, player->pos[i].x,
             static_cast<unsigned>(player->disp_char[i]));
  }
}

void movePlayerUp(WINDOW *game_wnd, Player *player) {
  clearPlayerPosition(game_wnd, player);

  player->disp_char = "       ____ "
                      "      /^___}"
                      ",     ||    "
                      "|\\   / |    "
                      "| \\_/  =    "
                      " \\     |    "
                      "  \\ _  /    "
                      "  |/ |/     "
                      "  [\\ [\\     ";

  for (size_t i = 0; i < player->disp_char.size(); i++) {
    player->pos[i].y -= 8;
    mvwaddch(game_wnd, player->pos[i].y, player->pos[i].x,
             static_cast<unsigned>(player->disp_char[i]));
  }
  flushinp();
}

void crouchPlayer(WINDOW *game_wnd, Player *player) {
  clearPlayerPosition(game_wnd, player);

  int y_start = 14;
  player->crouch_pos = {
      {10, y_start},     {11, y_start},     {12, y_start},
      {13, y_start},     {14, y_start},     {15, y_start},
      {16, y_start},     {17, y_start},     {18, y_start},
      {19, y_start},     {20, y_start},     {21, y_start},
      {22, y_start},     {23, y_start},     {24, y_start},
      {10, y_start + 1}, {11, y_start + 1}, {12, y_start + 1},
      {13, y_start + 1}, {14, y_start + 1}, {15, y_start + 1},
      {16, y_start + 1}, {17, y_start + 1}, {18, y_start + 1},
      {19, y_start + 1}, {20, y_start + 1}, {21, y_start + 1},
      {22, y_start + 1}, {23, y_start + 1}, {24, y_start + 1},
      {10, y_start + 2}, {11, y_start + 2}, {12, y_start + 2},
      {13, y_start + 2}, {14, y_start + 2}, {15, y_start + 2},
      {16, y_start + 2}, {17, y_start + 2}, {18, y_start + 2},
      {19, y_start + 2}, {20, y_start + 2}, {21, y_start + 2},
      {22, y_start + 2}, {23, y_start + 2}, {24, y_start + 2},
      {10, y_start + 3}, {11, y_start + 3}, {12, y_start + 3},
      {13, y_start + 3}, {14, y_start + 3}, {15, y_start + 3},
      {16, y_start + 3}, {17, y_start + 3}, {18, y_start + 3},
      {19, y_start + 3}, {20, y_start + 3}, {21, y_start + 3},
      {22, y_start + 3}, {23, y_start + 3}, {24, y_start + 3},
      {10, y_start + 4}, {11, y_start + 4}, {12, y_start + 4},
      {13, y_start + 4}, {14, y_start + 4}, {15, y_start + 4},
      {16, y_start + 4}, {17, y_start + 4}, {18, y_start + 4},
      {19, y_start + 4}, {20, y_start + 4}, {21, y_start + 4},
      {22, y_start + 4}, {23, y_start + 4}, {24, y_start + 4},
      {10, y_start + 5}, {11, y_start + 5}, {12, y_start + 5},
      {13, y_start + 5}, {14, y_start + 5}, {15, y_start + 5},
      {16, y_start + 5}, {17, y_start + 5}, {18, y_start + 5},
      {19, y_start + 5}, {20, y_start + 5}, {21, y_start + 5},
      {22, y_start + 5}, {23, y_start + 5}, {24, y_start + 5},
  };

  for (size_t i = 0; i < player->crouch_disp_char.size(); i++) {
    mvwaddch(game_wnd, player->crouch_pos[i].y, player->crouch_pos[i].x,
             static_cast<unsigned>(player->crouch_disp_char[i]));
  }

  player->crouched = true;
}

void playerFeetAnimation(WINDOW *game_wnd, Player *player) {
  if (player->crouched) {
    if (player->crouch_disp_char[63] == '|' &&
        player->crouch_disp_char[64] == '/') {
      player->crouch_disp_char[63] = '[';
      player->crouch_disp_char[64] = '\\';
      player->crouch_disp_char[78] = '_';
      player->crouch_disp_char[79] = '_';
    } else if (player->crouch_disp_char[63] == '[' &&
               player->crouch_disp_char[64] == '\\') {
      player->crouch_disp_char[63] = '|';
      player->crouch_disp_char[64] = '/';
      player->crouch_disp_char[78] = '[';
      player->crouch_disp_char[79] = '\\';
    }

    if (player->crouch_disp_char[66] == '[' &&
        player->crouch_disp_char[67] == '\\') {
      player->crouch_disp_char[66] = '|';
      player->crouch_disp_char[67] = '/';
      player->crouch_disp_char[81] = '[';
      player->crouch_disp_char[82] = '\\';
    } else if (player->crouch_disp_char[66] == '|' &&
               player->crouch_disp_char[67] == '/') {
      player->crouch_disp_char[66] = '[';
      player->crouch_disp_char[67] = '\\';
      player->crouch_disp_char[81] = '_';
      player->crouch_disp_char[82] = '_';
    }

    for (size_t i = 63; i < player->crouch_disp_char.size(); i++) {
      mvwaddch(game_wnd, player->crouch_pos[i].y, player->crouch_pos[i].x,
               static_cast<unsigned>(player->crouch_disp_char[i]));
    }

    return;
  }

  if (player->disp_char[86] == '|' && player->disp_char[87] == '/') {
    player->disp_char[86] = '[';
    player->disp_char[87] = '\\';
    player->disp_char[98] = '_';
    player->disp_char[99] = '_';
  } else if (player->disp_char[86] == '[' && player->disp_char[87] == '\\') {
    player->disp_char[86] = '|';
    player->disp_char[87] = '/';
    player->disp_char[98] = '[';
    player->disp_char[99] = '\\';
  }

  if (player->disp_char[89] == '[' && player->disp_char[90] == '\\') {
    player->disp_char[89] = '|';
    player->disp_char[90] = '/';
    player->disp_char[101] = '[';
    player->disp_char[102] = '\\';
  } else if (player->disp_char[89] == '|' && player->disp_char[90] == '/') {
    player->disp_char[89] = '[';
    player->disp_char[90] = '\\';
    player->disp_char[101] = '_';
    player->disp_char[102] = '_';
  }

  for (size_t i = 86; i < player->disp_char.size(); i++) {
    mvwaddch(game_wnd, player->pos[i].y, player->pos[i].x,
             static_cast<unsigned>(player->disp_char[i]));
  }
}
