#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <random>

#include "game.h"
#include "obstacles.h"

int tick = 0;

///////////////////////////// OBJECT CLASS IMPLEMENTATION
////////////////////////////////

Obstacle::Obstacle(std::vector<vec2i> pos, std::string disp_char, int type)
    : m_positions(pos), m_disp_char(disp_char), m_type(type), m_fielded(false) {
}

vec2i Obstacle::getLastPos() const {
  return m_positions[m_disp_char.size() - 1];
}

std::vector<vec2i> Obstacle::getPos() const { return m_positions; }

std::string Obstacle::getDispChar() const { return m_disp_char; }

void Obstacle::fieldedToTrue() { m_fielded = true; }

void Obstacle::update(WINDOW *game_wnd) {
  for (size_t i = 0; i < m_disp_char.size(); i++) {
    if (m_positions[i].y == 19) {
      mvwaddch(game_wnd, m_positions[i].y, m_positions[i].x, '_');
    } else {
      mvwaddch(game_wnd, m_positions[i].y, m_positions[i].x, ' ');
    }
  }

  render(game_wnd);
  fieldedToTrue(); /* Object is now in the object field/game window */
}

void Obstacle::render(WINDOW *game_wnd) {
  if (m_type == 6 && tick >= 20) {
    tick = 0;
    if (m_disp_char[5] == ',') {
      m_disp_char = "         "
                    "         "
                    "  _____  "
                    ">|_   _\\="
                    "   | /   "
                    "   |/    "
                    "___'_____";

    } else if (m_disp_char[5] == ' ') {

      m_disp_char = "     ,   "
                    "    /|   "
                    "  _/ |_  "
                    ">|_____\\="
                    "         "
                    "         "
                    "_________";
    }
  } else if (m_type == 5 && tick >= 20) {
    tick = 0;
    if (m_disp_char[5] == ',') {
      m_disp_char = "         "
                    "         "
                    "  _____  "
                    ">|_   _\\="
                    "   | /   "
                    "   |/    "
                    "   '     ";

    } else if (m_disp_char[5] == ' ') {

      m_disp_char = "     ,   "
                    "    /|   "
                    "  _/ |_  "
                    ">|_____\\="
                    "         "
                    "         "
                    "         ";
    }
  }

  for (size_t i = 0; i < m_disp_char.size(); i++) {
    m_positions[i].x -= 1;
    mvwaddch(game_wnd, m_positions[i].y, m_positions[i].x,
             static_cast<unsigned>(m_disp_char[i]));
  }
  tick++;
}

bool Obstacle::getFielded() { return m_fielded; }

///////////////////////////// DIRT CLASS IMPLEMENTATION
////////////////////////////////

Dirt::Dirt(int x, int y, char disp_char) {
  m_position.x = x;
  m_position.y = y;
  m_disp_char = disp_char;
}

void Dirt::update(WINDOW *game_wnd) {
  mvwaddch(game_wnd, m_position.y, m_position.x, static_cast<unsigned>(' '));
  m_position.x -= 1;
  mvwaddch(game_wnd, m_position.y, m_position.x,
           static_cast<unsigned>(m_disp_char));
}

vec2i Dirt::getPos() const { return m_position; }

///////////////////////////// DIRT FIELD CLASS IMPLEMENTATION
////////////////////////////////

void DirtField::update(WINDOW *game_wnd) {
  for (auto dirt_index{m_dirt_field.begin()}; dirt_index != m_dirt_field.end();
       ++dirt_index) {
    auto &dirt = *dirt_index;
    if (dirt.getPos().x < m_dirt_bounds.left()) {
      m_dirt_field.erase(dirt_index);
    }

    dirt.update(game_wnd);
  }

  std::random_device rd;  /* Obtain random number from hardware */
  std::mt19937 gen(rd()); /* Seed the generator */
  std::uniform_int_distribution<> distr(20, 22);
  std::uniform_int_distribution<> dirt_distribution(0, 4);

  auto y_cord = distr(gen);

  auto dirt_type = dirt_distribution(gen);

  if (dirt_type == 0) {
    Dirt dirt(m_dirt_bounds.width(), y_cord, '.');
    m_dirt_field.push_back(dirt);
  } else if (dirt_type == 1) {
    Dirt dirt(m_dirt_bounds.width(), y_cord, '-');
    m_dirt_field.push_back(dirt);
  } else if (dirt_type == 2) {
    Dirt dirt(m_dirt_bounds.width(), y_cord, '_');
    m_dirt_field.push_back(dirt);
  } else if (dirt_type == 3) {
    Dirt dirt(m_dirt_bounds.width(), y_cord, '\'');
    m_dirt_field.push_back(dirt);
  } else {
    Dirt dirt(m_dirt_bounds.width(), y_cord, '`');
    m_dirt_field.push_back(dirt);
  }
}

void DirtField::setBounds(rect a) { m_dirt_bounds = a; }

void DirtField::seed() {
  std::random_device rd; /* Obtain random number from hardware */
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(20, 22);
  std::uniform_int_distribution<> distr2(0, 78);
  std::uniform_int_distribution<> dirt_distribution(0, 4);

  for (size_t i = 0; i < 100; i++) {

    auto dirt_type = dirt_distribution(gen);
    auto y_cord = distr(gen);
    auto x_cord = distr2(gen);

    if (dirt_type == 0) {
      Dirt dirt(x_cord, y_cord, '.');
      m_dirt_field.push_back(dirt);
    } else if (dirt_type == 1) {
      Dirt dirt(x_cord, y_cord, '-');
      m_dirt_field.push_back(dirt);
    } else if (dirt_type == 2) {
      Dirt dirt(x_cord, y_cord, '_');
      m_dirt_field.push_back(dirt);
    } else if (dirt_type == 3) {
      Dirt dirt(x_cord, y_cord, '\'');
      m_dirt_field.push_back(dirt);
    } else {
      Dirt dirt(x_cord, y_cord, '`');
      m_dirt_field.push_back(dirt);
    }
  }
}

rect DirtField::getBounds() { return m_dirt_bounds; }

///////////////////////////// OBJECT FIELD CLASS IMPLEMENTATION
////////////////////////////////

ObstacleField::ObstacleField() : m_rd(), m_gen(m_rd()) {}

rect ObstacleField::getBounds() { return m_field_bounds; }

void ObstacleField::setBounds(rect a) { m_field_bounds = a; }

bool ObstacleField::update(WINDOW *game_wnd, rect player_rect,
                           int player_score) {
  int dist_index = rand() % 2;

  /* Update existing objects */
  for (auto current_point{m_object_set.begin()};
       current_point != m_object_set.end(); ++current_point) {

    auto &current_object = *current_point;

    if (current_object.getLastPos().x < m_field_bounds.left()) {
      m_object_set.erase(current_point);
    }

    if (current_object.getFielded() || current_point == m_object_set.begin()) {
      current_object.update(game_wnd);

      std::vector<vec2i> obj_pos = current_object.getPos();
      size_t length = current_object.getDispChar().size();
      rect object_rect = {{obj_pos[0].x, obj_pos[0].y},
                          {obj_pos[length - 1].x, obj_pos[length - 1].y}};

      if (isCollided(player_rect, object_rect)) {
        return true;
      }
    } else {
      auto previous_object = *(std::prev(current_point));

      int diff =
          abs(previous_object.getLastPos().x - current_object.getLastPos().x);
      if (dist_index == 0) {
        if (diff >= 60) {
          current_object.update(game_wnd);
        }
      }
    }
  }

  if (m_object_set.size() < 7) {
    int i = player_score > 500 ? 6 : 4;

    /* Spawn a new object */
    std::uniform_int_distribution<> distr(0, i);

    /* generate random number between 0 and 6 */
    auto object_type = distr(m_gen);
    rect fb = getBounds();

    std::string dc; /* Display characters */

    int y_start;

    switch (object_type) {
    case 0: {
      y_start = 16;
      std::vector<vec2i> p{
          {fb.bounds.x - 6, y_start},     {fb.bounds.x - 5, y_start},
          {fb.bounds.x - 4, y_start},     {fb.bounds.x - 3, y_start},
          {fb.bounds.x - 2, y_start},     {fb.bounds.x - 1, y_start},
          {fb.bounds.x, y_start},         {fb.bounds.x - 6, y_start + 1},
          {fb.bounds.x - 5, y_start + 1}, {fb.bounds.x - 4, y_start + 1},
          {fb.bounds.x - 3, y_start + 1}, {fb.bounds.x - 2, y_start + 1},
          {fb.bounds.x - 1, y_start + 1}, {fb.bounds.x, y_start + 1},
          {fb.bounds.x - 6, y_start + 2}, {fb.bounds.x - 5, y_start + 2},
          {fb.bounds.x - 4, y_start + 2}, {fb.bounds.x - 3, y_start + 2},
          {fb.bounds.x - 2, y_start + 2}, {fb.bounds.x - 1, y_start + 2},
          {fb.bounds.x, y_start + 2},     {fb.bounds.x - 6, y_start + 3},
          {fb.bounds.x - 5, y_start + 3}, {fb.bounds.x - 4, y_start + 3},
          {fb.bounds.x - 3, y_start + 3}, {fb.bounds.x - 2, y_start + 3},
          {fb.bounds.x - 1, y_start + 3}, {fb.bounds.x, y_start + 3}};

      dc = "   _   "
           "  | |  "
           " _| |_ "
           "|_____|";

      Obstacle obj0{p, dc, 0};
      m_object_set.push_back(obj0);
      break;
    }

    case 1: {
      y_start = 15;
      std::vector<vec2i> p{
          {fb.bounds.x - 6, y_start},     {fb.bounds.x - 5, y_start},
          {fb.bounds.x - 4, y_start},     {fb.bounds.x - 3, y_start},
          {fb.bounds.x - 2, y_start},     {fb.bounds.x - 1, y_start},
          {fb.bounds.x, y_start},         {fb.bounds.x - 6, y_start + 1},
          {fb.bounds.x - 5, y_start + 1}, {fb.bounds.x - 4, y_start + 1},
          {fb.bounds.x - 3, y_start + 1}, {fb.bounds.x - 2, y_start + 1},
          {fb.bounds.x - 1, y_start + 1}, {fb.bounds.x, y_start + 1},
          {fb.bounds.x - 6, y_start + 2}, {fb.bounds.x - 5, y_start + 2},
          {fb.bounds.x - 4, y_start + 2}, {fb.bounds.x - 3, y_start + 2},
          {fb.bounds.x - 2, y_start + 2}, {fb.bounds.x - 1, y_start + 2},
          {fb.bounds.x, y_start + 2},     {fb.bounds.x - 6, y_start + 3},
          {fb.bounds.x - 5, y_start + 3}, {fb.bounds.x - 4, y_start + 3},
          {fb.bounds.x - 3, y_start + 3}, {fb.bounds.x - 2, y_start + 3},
          {fb.bounds.x - 1, y_start + 3}, {fb.bounds.x, y_start + 3},
          {fb.bounds.x - 6, y_start + 4}, {fb.bounds.x - 5, y_start + 4},
          {fb.bounds.x - 4, y_start + 4}, {fb.bounds.x - 3, y_start + 4},
          {fb.bounds.x - 2, y_start + 4}, {fb.bounds.x - 1, y_start + 4},
          {fb.bounds.x, y_start + 4}};

      dc = " _   _ "
           "| |_| |"
           "|_   _|"
           "  | |  "
           "__|_|__";

      Obstacle obj1{p, dc, 1};
      m_object_set.push_back(obj1);
      break;
    }

    case 2: {
      y_start = 13;
      std::vector<vec2i> p{
          {fb.bounds.x - 10, y_start},     {fb.bounds.x - 9, y_start},
          {fb.bounds.x - 8, y_start},      {fb.bounds.x - 7, y_start},
          {fb.bounds.x - 6, y_start},      {fb.bounds.x - 5, y_start},
          {fb.bounds.x - 4, y_start},      {fb.bounds.x - 3, y_start},
          {fb.bounds.x - 2, y_start},      {fb.bounds.x - 1, y_start},
          {fb.bounds.x, y_start},          {fb.bounds.x - 10, y_start + 1},
          {fb.bounds.x - 9, y_start + 1},  {fb.bounds.x - 8, y_start + 1},
          {fb.bounds.x - 7, y_start + 1},  {fb.bounds.x - 6, y_start + 1},
          {fb.bounds.x - 5, y_start + 1},  {fb.bounds.x - 4, y_start + 1},
          {fb.bounds.x - 3, y_start + 1},  {fb.bounds.x - 2, y_start + 1},
          {fb.bounds.x - 1, y_start + 1},  {fb.bounds.x, y_start + 1},
          {fb.bounds.x - 10, y_start + 2}, {fb.bounds.x - 9, y_start + 2},
          {fb.bounds.x - 8, y_start + 2},  {fb.bounds.x - 7, y_start + 2},
          {fb.bounds.x - 6, y_start + 2},  {fb.bounds.x - 5, y_start + 2},
          {fb.bounds.x - 4, y_start + 2},  {fb.bounds.x - 3, y_start + 2},
          {fb.bounds.x - 2, y_start + 2},  {fb.bounds.x - 1, y_start + 2},
          {fb.bounds.x, y_start + 2},      {fb.bounds.x - 10, y_start + 3},
          {fb.bounds.x - 9, y_start + 3},  {fb.bounds.x - 8, y_start + 3},
          {fb.bounds.x - 7, y_start + 3},  {fb.bounds.x - 6, y_start + 3},
          {fb.bounds.x - 5, y_start + 3},  {fb.bounds.x - 4, y_start + 3},
          {fb.bounds.x - 3, y_start + 3},  {fb.bounds.x - 2, y_start + 3},
          {fb.bounds.x - 1, y_start + 3},  {fb.bounds.x, y_start + 3},
          {fb.bounds.x - 10, y_start + 4}, {fb.bounds.x - 9, y_start + 4},
          {fb.bounds.x - 8, y_start + 4},  {fb.bounds.x - 7, y_start + 4},
          {fb.bounds.x - 6, y_start + 4},  {fb.bounds.x - 5, y_start + 4},
          {fb.bounds.x - 4, y_start + 4},  {fb.bounds.x - 3, y_start + 4},
          {fb.bounds.x - 2, y_start + 4},  {fb.bounds.x - 1, y_start + 4},
          {fb.bounds.x, y_start + 4},      {fb.bounds.x - 10, y_start + 5},
          {fb.bounds.x - 9, y_start + 5},  {fb.bounds.x - 8, y_start + 5},
          {fb.bounds.x - 7, y_start + 5},  {fb.bounds.x - 6, y_start + 5},
          {fb.bounds.x - 5, y_start + 5},  {fb.bounds.x - 4, y_start + 5},
          {fb.bounds.x - 3, y_start + 5},  {fb.bounds.x - 2, y_start + 5},
          {fb.bounds.x - 1, y_start + 5},  {fb.bounds.x, y_start + 5},
          {fb.bounds.x - 10, y_start + 6}, {fb.bounds.x - 9, y_start + 6},
          {fb.bounds.x - 8, y_start + 6},  {fb.bounds.x - 7, y_start + 6},
          {fb.bounds.x - 6, y_start + 6},  {fb.bounds.x - 5, y_start + 6},
          {fb.bounds.x - 4, y_start + 6},  {fb.bounds.x - 3, y_start + 6},
          {fb.bounds.x - 2, y_start + 6},  {fb.bounds.x - 1, y_start + 6},
          {fb.bounds.x, y_start + 6}};

      dc = "     _     "
           "    | |    "
           " _  | |  _ "
           "| |_| |_| |"
           "|___   ___|"
           "    | |    "
           "____|_|____";

      Obstacle obj2{p, dc, 2};
      m_object_set.push_back(obj2);
      break;
    }

    case 3: {
      y_start = 16;
      std::vector<vec2i> p{
          {fb.bounds.x - 10, y_start},     {fb.bounds.x - 9, y_start},
          {fb.bounds.x - 8, y_start},      {fb.bounds.x - 7, y_start},
          {fb.bounds.x - 6, y_start},      {fb.bounds.x - 5, y_start},
          {fb.bounds.x - 4, y_start},      {fb.bounds.x - 3, y_start},
          {fb.bounds.x - 2, y_start},      {fb.bounds.x - 1, y_start},
          {fb.bounds.x, y_start},          {fb.bounds.x - 10, y_start + 1},
          {fb.bounds.x - 9, y_start + 1},  {fb.bounds.x - 8, y_start + 1},
          {fb.bounds.x - 7, y_start + 1},  {fb.bounds.x - 6, y_start + 1},
          {fb.bounds.x - 5, y_start + 1},  {fb.bounds.x - 4, y_start + 1},
          {fb.bounds.x - 3, y_start + 1},  {fb.bounds.x - 2, y_start + 1},
          {fb.bounds.x - 1, y_start + 1},  {fb.bounds.x, y_start + 1},
          {fb.bounds.x - 10, y_start + 2}, {fb.bounds.x - 9, y_start + 2},
          {fb.bounds.x - 8, y_start + 2},  {fb.bounds.x - 7, y_start + 2},
          {fb.bounds.x - 6, y_start + 2},  {fb.bounds.x - 5, y_start + 2},
          {fb.bounds.x - 4, y_start + 2},  {fb.bounds.x - 3, y_start + 2},
          {fb.bounds.x - 2, y_start + 2},  {fb.bounds.x - 1, y_start + 2},
          {fb.bounds.x, y_start + 2},      {fb.bounds.x - 10, y_start + 3},
          {fb.bounds.x - 9, y_start + 3},  {fb.bounds.x - 8, y_start + 3},
          {fb.bounds.x - 7, y_start + 3},  {fb.bounds.x - 6, y_start + 3},
          {fb.bounds.x - 5, y_start + 3},  {fb.bounds.x - 4, y_start + 3},
          {fb.bounds.x - 3, y_start + 3},  {fb.bounds.x - 2, y_start + 3},
          {fb.bounds.x - 1, y_start + 3},  {fb.bounds.x, y_start + 3}};

      dc = "     _     "
           " _  | |  _ "
           "| |_| |_| |"
           "|_________|";

      Obstacle obj3{p, dc, 3};
      m_object_set.push_back(obj3);
      break;
    }

    case 4: {
      y_start = 14;
      std::vector<vec2i> p{
          {fb.bounds.x - 6, y_start},     {fb.bounds.x - 5, y_start},
          {fb.bounds.x - 4, y_start},     {fb.bounds.x - 3, y_start},
          {fb.bounds.x - 2, y_start},     {fb.bounds.x - 1, y_start},
          {fb.bounds.x, y_start},         {fb.bounds.x - 6, y_start + 1},
          {fb.bounds.x - 5, y_start + 1}, {fb.bounds.x - 4, y_start + 1},
          {fb.bounds.x - 3, y_start + 1}, {fb.bounds.x - 2, y_start + 1},
          {fb.bounds.x - 1, y_start + 1}, {fb.bounds.x, y_start + 1},
          {fb.bounds.x - 6, y_start + 2}, {fb.bounds.x - 5, y_start + 2},
          {fb.bounds.x - 4, y_start + 2}, {fb.bounds.x - 3, y_start + 2},
          {fb.bounds.x - 2, y_start + 2}, {fb.bounds.x - 1, y_start + 2},
          {fb.bounds.x, y_start + 2},     {fb.bounds.x - 6, y_start + 3},
          {fb.bounds.x - 5, y_start + 3}, {fb.bounds.x - 4, y_start + 3},
          {fb.bounds.x - 3, y_start + 3}, {fb.bounds.x - 2, y_start + 3},
          {fb.bounds.x - 1, y_start + 3}, {fb.bounds.x, y_start + 3},
          {fb.bounds.x - 6, y_start + 4}, {fb.bounds.x - 5, y_start + 4},
          {fb.bounds.x - 4, y_start + 4}, {fb.bounds.x - 3, y_start + 4},
          {fb.bounds.x - 2, y_start + 4}, {fb.bounds.x - 1, y_start + 4},
          {fb.bounds.x, y_start + 4},     {fb.bounds.x - 6, y_start + 5},
          {fb.bounds.x - 5, y_start + 5}, {fb.bounds.x - 4, y_start + 5},
          {fb.bounds.x - 3, y_start + 5}, {fb.bounds.x - 2, y_start + 5},
          {fb.bounds.x - 1, y_start + 5}, {fb.bounds.x, y_start + 5}};

      dc = " _   _ "
           "| | | |"
           "| |_| |"
           "|_   _|"
           "  | |  "
           "__|_|__";

      Obstacle obj4{p, dc, 4};
      m_object_set.push_back(obj4);
      break;
    }

    case 5: {
      y_start = 7;
      std::vector<vec2i> p{
          {fb.bounds.x - 8, y_start},     {fb.bounds.x - 7, y_start},
          {fb.bounds.x - 6, y_start},     {fb.bounds.x - 5, y_start},
          {fb.bounds.x - 4, y_start},     {fb.bounds.x - 3, y_start},
          {fb.bounds.x - 2, y_start},     {fb.bounds.x - 1, y_start},
          {fb.bounds.x, y_start},         {fb.bounds.x - 8, y_start + 1},
          {fb.bounds.x - 7, y_start + 1}, {fb.bounds.x - 6, y_start + 1},
          {fb.bounds.x - 5, y_start + 1}, {fb.bounds.x - 4, y_start + 1},
          {fb.bounds.x - 3, y_start + 1}, {fb.bounds.x - 2, y_start + 1},
          {fb.bounds.x - 1, y_start + 1}, {fb.bounds.x, y_start + 1},
          {fb.bounds.x - 8, y_start + 2}, {fb.bounds.x - 7, y_start + 2},
          {fb.bounds.x - 6, y_start + 2}, {fb.bounds.x - 5, y_start + 2},
          {fb.bounds.x - 4, y_start + 2}, {fb.bounds.x - 3, y_start + 2},
          {fb.bounds.x - 2, y_start + 2}, {fb.bounds.x - 1, y_start + 2},
          {fb.bounds.x, y_start + 2},     {fb.bounds.x - 8, y_start + 3},
          {fb.bounds.x - 7, y_start + 3}, {fb.bounds.x - 6, y_start + 3},
          {fb.bounds.x - 5, y_start + 3}, {fb.bounds.x - 4, y_start + 3},
          {fb.bounds.x - 3, y_start + 3}, {fb.bounds.x - 2, y_start + 3},
          {fb.bounds.x - 1, y_start + 3}, {fb.bounds.x, y_start + 3},
          {fb.bounds.x - 8, y_start + 4}, {fb.bounds.x - 7, y_start + 4},
          {fb.bounds.x - 6, y_start + 4}, {fb.bounds.x - 5, y_start + 4},
          {fb.bounds.x - 4, y_start + 4}, {fb.bounds.x - 3, y_start + 4},
          {fb.bounds.x - 2, y_start + 4}, {fb.bounds.x - 1, y_start + 4},
          {fb.bounds.x, y_start + 4},     {fb.bounds.x - 8, y_start + 5},
          {fb.bounds.x - 7, y_start + 5}, {fb.bounds.x - 6, y_start + 5},
          {fb.bounds.x - 5, y_start + 5}, {fb.bounds.x - 4, y_start + 5},
          {fb.bounds.x - 3, y_start + 5}, {fb.bounds.x - 2, y_start + 5},
          {fb.bounds.x - 1, y_start + 5}, {fb.bounds.x, y_start + 5},
          {fb.bounds.x - 8, y_start + 6}, {fb.bounds.x - 7, y_start + 6},
          {fb.bounds.x - 6, y_start + 6}, {fb.bounds.x - 5, y_start + 6},
          {fb.bounds.x - 4, y_start + 6}, {fb.bounds.x - 3, y_start + 6},
          {fb.bounds.x - 2, y_start + 6}, {fb.bounds.x - 1, y_start + 6},
          {fb.bounds.x, y_start + 6},
      };

      dc = "         "
           "         "
           "  _____  "
           ">|_   _\\="
           "   | /   "
           "   |/    "
           "   '     ";

      Obstacle obj5{p, dc, 5};
      m_object_set.push_back(obj5);
      break;
    }

    case 6: {
      y_start = 13;
      std::vector<vec2i> p{
          {fb.bounds.x - 8, y_start},     {fb.bounds.x - 7, y_start},
          {fb.bounds.x - 6, y_start},     {fb.bounds.x - 5, y_start},
          {fb.bounds.x - 4, y_start},     {fb.bounds.x - 3, y_start},
          {fb.bounds.x - 2, y_start},     {fb.bounds.x - 1, y_start},
          {fb.bounds.x, y_start},         {fb.bounds.x - 8, y_start + 1},
          {fb.bounds.x - 7, y_start + 1}, {fb.bounds.x - 6, y_start + 1},
          {fb.bounds.x - 5, y_start + 1}, {fb.bounds.x - 4, y_start + 1},
          {fb.bounds.x - 3, y_start + 1}, {fb.bounds.x - 2, y_start + 1},
          {fb.bounds.x - 1, y_start + 1}, {fb.bounds.x, y_start + 1},
          {fb.bounds.x - 8, y_start + 2}, {fb.bounds.x - 7, y_start + 2},
          {fb.bounds.x - 6, y_start + 2}, {fb.bounds.x - 5, y_start + 2},
          {fb.bounds.x - 4, y_start + 2}, {fb.bounds.x - 3, y_start + 2},
          {fb.bounds.x - 2, y_start + 2}, {fb.bounds.x - 1, y_start + 2},
          {fb.bounds.x, y_start + 2},     {fb.bounds.x - 8, y_start + 3},
          {fb.bounds.x - 7, y_start + 3}, {fb.bounds.x - 6, y_start + 3},
          {fb.bounds.x - 5, y_start + 3}, {fb.bounds.x - 4, y_start + 3},
          {fb.bounds.x - 3, y_start + 3}, {fb.bounds.x - 2, y_start + 3},
          {fb.bounds.x - 1, y_start + 3}, {fb.bounds.x, y_start + 3},
          {fb.bounds.x - 8, y_start + 4}, {fb.bounds.x - 7, y_start + 4},
          {fb.bounds.x - 6, y_start + 4}, {fb.bounds.x - 5, y_start + 4},
          {fb.bounds.x - 4, y_start + 4}, {fb.bounds.x - 3, y_start + 4},
          {fb.bounds.x - 2, y_start + 4}, {fb.bounds.x - 1, y_start + 4},
          {fb.bounds.x, y_start + 4},     {fb.bounds.x - 8, y_start + 5},
          {fb.bounds.x - 7, y_start + 5}, {fb.bounds.x - 6, y_start + 5},
          {fb.bounds.x - 5, y_start + 5}, {fb.bounds.x - 4, y_start + 5},
          {fb.bounds.x - 3, y_start + 5}, {fb.bounds.x - 2, y_start + 5},
          {fb.bounds.x - 1, y_start + 5}, {fb.bounds.x, y_start + 5},
          {fb.bounds.x - 8, y_start + 6}, {fb.bounds.x - 7, y_start + 6},
          {fb.bounds.x - 6, y_start + 6}, {fb.bounds.x - 5, y_start + 6},
          {fb.bounds.x - 4, y_start + 6}, {fb.bounds.x - 3, y_start + 6},
          {fb.bounds.x - 2, y_start + 6}, {fb.bounds.x - 1, y_start + 6},
          {fb.bounds.x, y_start + 6},
      };

      dc = "         "
           "         "
           "  _____  "
           ">|_   _\\="
           "   | /   "
           "   |/    "
           "___'_____";

      Obstacle obj6{p, dc, 6};
      m_object_set.push_back(obj6);
      break;
    }
    }
  }

  return false;
}
