#include <ctime>
#include <ncurses.h>
#include <string>
#include <unistd.h>

#include "game.h"
#include "objects.h"
#include "player.h"

WINDOW *main_wnd;
WINDOW *game_wnd;

rect game_area;
rect screen_area;
rect dirt_area;

bool isCollided(rect a, rect b) {
  bool a1 = (a.top() < b.bot() - b.top());
  bool b1 = (a.bot() - a.top() > b.top());
  bool c1 = (a.right() - a.left() > b.left());
  bool d1 = (a.left() < b.right() - b.left());

  return (a1 && b1 && c1 && d1);
}

int init() {
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  main_wnd = initscr();

  cbreak();
  noecho();
  clear();
  refresh();

  curs_set(0);

  start_color();

  screen_area = {{0, 0}, {80, 24}};

  game_wnd = newwin(screen_area.height() - 2, screen_area.width() - 2,
                    screen_area.top() + 1, screen_area.left() + 1);

  main_wnd = newwin(screen_area.height(), screen_area.width(), 0, 0);

  game_area = {{0, 0}, {screen_area.width() - 2, screen_area.height() - 2}};

  dirt_area = {{0, 19}, {screen_area.width() - 2, screen_area.height() - 2}};

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_WHITE);

  /* Enable function keys */
  keypad(main_wnd, true);
  keypad(game_wnd, true);

  nodelay(main_wnd, true);
  nodelay(game_wnd, true);

  if (!has_colors()) {
    endwin();
    printf("ERROR: Terminal does not support color.\n");
    exit(1);
  }

  return 0;
}

bool run(int *max_score) {
  ObjectField field;
  DirtField df;

  Player player;
  player.up = false;
  player.score = 0;

  int tick = 0;

  int speed_tick = 0;
  int speed_tick_cap = 2; /* Increment or decrement to control obstacl speed */

  int dirt_tick = 0;
  const int dirt_tick_cap = 2;

  player.air_time = 0;
  player.max_air_time =
      58; /* Increment or decrement to control player air time */

  int feet_tick = 0;

  int in_char = 0;

  bool game_over = false;

  field.setBounds(game_area);
  df.setBounds(dirt_area);

  // frame around screen
  wattron(main_wnd, A_BOLD);
  box(main_wnd, 0, 0);
  wattroff(main_wnd, A_BOLD);

  // initial draw
  wrefresh(main_wnd);
  wrefresh(game_wnd);

  // mvwprintw(main_wnd, 0, 0, "press SPACE to start...");

  // Seed dirt field
  df.seed();
  wattroff(game_wnd, A_BOLD);
  df.update(game_wnd);
  wattron(game_wnd, A_BOLD);

  wattron(game_wnd, A_BOLD);

  // Draw dino to terminal
  initPlayerPosition(game_wnd, &player);
  mvwhline(game_wnd, screen_area.bot() - 5, 0, '_', screen_area.width() - 2);
  wrefresh(game_wnd);

  while (1) {
    in_char = wgetch(main_wnd);
    in_char = tolower(in_char);

    if (player.air_time == player.max_air_time && player.up) {
      player.air_time = 0;
      player.up = false;

      movePlayerDown(game_wnd, &player);
    }

    switch (in_char) {
    case KEY_UP:
    case ' ':
    case 'w':
      if (!player.up) {
        movePlayerUp(game_wnd, &player);
        player.up = true;
      }
      break;
    case KEY_DOWN:
    case 's':
      if (player.up) { /* Cancel jump */
        player.air_time = 0;
        player.up = false;

        movePlayerDown(game_wnd, &player);
        break;
      }

      crouchPlayer(game_wnd, &player);
      break;
    default:
      break;
    }

    size_t p_length = player.disp_char.size();
    size_t c_length = player.crouch_disp_char.size();
    if (player.crouched) {
      player.bounds = {{player.crouch_pos[0].x, player.crouch_pos[0].y},
                       {player.crouch_pos[c_length - 1].x,
                        player.crouch_pos[c_length - 1].y}};
    } else {
      player.bounds = {
          {player.pos[0].x, player.pos[0].y},
          {player.pos[p_length - 1].x, player.pos[p_length - 1].y}};
    }

    if (tick >= 200) {
      speed_tick++;
      if (speed_tick == speed_tick_cap) {
        speed_tick = 0;
        if (field.update(game_wnd, player.bounds, player.score)) {
          /* Collision occured; game over*/
          game_over = true;
        };
      }
    }

    if (dirt_tick == dirt_tick_cap) {
      dirt_tick = 0;
      wattroff(game_wnd, A_BOLD);
      df.update(game_wnd);
      wattron(game_wnd, A_BOLD);
    }

    if (feet_tick == 17) {
      feet_tick = 0;
      if (!player.up)
        playerFeetAnimation(game_wnd, &player);
    }

    wrefresh(main_wnd);
    wrefresh(game_wnd);

    if (game_over) {
      *max_score = player.score > *max_score ? player.score : *max_score;
      // Print game over message and prompt user for input
      nodelay(game_wnd, false);
      int input;
      // wattron(game_wnd, A_BLINK); Uncomment for blinking "GAME OVER" effect
      mvwprintw(game_wnd, 9, 35, "GAME OVER");
      // wattroff(game_wnd, A_BLINK); Uncomment for blinking "GAME OVER" effect
      mvwprintw(game_wnd, 11, 29, "press 'q' to exit");
      mvwprintw(game_wnd, 12, 29, "press 'r' to retry");
      wrefresh(game_wnd);
      usleep(50000); /* Gotta let the gamer read the info */
      flushinp();

      while (1) {
      input = wgetch(game_wnd);
      input = tolower(input);

        switch (input) {
        case 'q':
          return false; // quit game
        case 'r':
          return true;
        }
      }
    }

    if (player.up)
      player.air_time++;

    if (tick % 5 == 0) {
      // increase player score
      player.score += 1;
    }

    mvwprintw(game_wnd, 0, 58, "HIGH SCORE: %8d", *max_score);
    mvwprintw(game_wnd, 1, 55, "CURRENT SCORE: %8d", player.score);
    wrefresh(game_wnd);

    feet_tick++;
    dirt_tick++;
    tick++;

    if (player.score > 1500) {
      speed_tick_cap = 1;
      player.max_air_time = 43;
    }

    usleep(10000);
  }
}

void close() { endwin(); }
