#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>
#include <random>

#include "game.h"
#include "objects.h"

int tick = 0;

///////////////////////////// OBJECT CLASS IMPLEMENTATION
////////////////////////////////

Object::Object(std::vector<vec2i> pos, std::string disp_char, int type)
    : pos(pos), disp_char(disp_char), type(type), fielded(false) {}

vec2i Object::getLastPos() const {
  return this->pos[this->disp_char.size() - 1];
}

std::vector<vec2i> Object::getPos() const { return this->pos; }

std::string Object::getDispChar() const { return this->disp_char; }

void Object::fieldedToTrue() { this->fielded = true; }

void Object::update(WINDOW *game_wnd) {
  for (size_t i = 0; i < this->disp_char.size(); i++) {
    if (this->pos[i].y == 19) {
      mvwaddch(game_wnd, this->pos[i].y, this->pos[i].x, '_');
    } else {
      mvwaddch(game_wnd, this->pos[i].y, this->pos[i].x, ' ');
    }
  }

  this->render(game_wnd);
  this->fieldedToTrue(); /* Object is now in the object field/game window */
}

void Object::render(WINDOW *game_wnd) {
  if (this->type == 6 && tick >= 20) {
    tick = 0;
    if (this->disp_char[5] == ',') {
      this->disp_char = "         "
                        "         "
                        "  _____  "
                        ">|_   _\\="
                        "   | /   "
                        "   |/    "
                        "___'_____";

    } else if (this->disp_char[5] == ' ') {

      this->disp_char = "     ,   "
                        "    /|   "
                        "  _/ |_  "
                        ">|_____\\="
                        "         "
                        "         "
                        "_________";
    }
  } else if (this->type == 5 && tick >= 20) {
    tick = 0;
    if (this->disp_char[5] == ',') {
      this->disp_char = "         "
                        "         "
                        "  _____  "
                        ">|_   _\\="
                        "   | /   "
                        "   |/    "
                        "   '     ";

    } else if (this->disp_char[5] == ' ') {

      this->disp_char = "     ,   "
                        "    /|   "
                        "  _/ |_  "
                        ">|_____\\="
                        "         "
                        "         "
                        "         ";
    }
  }

  for (size_t i = 0; i < this->disp_char.size(); i++) {
    this->pos[i].x -= 1;
    mvwaddch(game_wnd, this->pos[i].y, this->pos[i].x, this->disp_char[i]);
  }
  tick++;
}

bool Object::getFielded() { return this->fielded; }

///////////////////////////// DIRT CLASS IMPLEMENTATION
////////////////////////////////

Dirt::Dirt(int x, int y, char disp_char) {
  this->pos.x = x;
  this->pos.y = y;
  this->disp_char = disp_char;
}

void Dirt::update(WINDOW *game_wnd) {
  mvwaddch(game_wnd, this->pos.y, this->pos.x, ' ');
  this->pos.x -= 1;
  mvwaddch(game_wnd, this->pos.y, this->pos.x, this->disp_char);
}

vec2i Dirt::getPos() const { return this->pos; }

///////////////////////////// DIRT FIELD CLASS IMPLEMENTATION
////////////////////////////////

void DirtField::update(WINDOW *game_wnd) {
  for (auto counter{0}; auto& dirt: dirt_field) {
    if (dirt.getPos().x < dirt_bounds.left()) {
      dirt_field.erase(dirt_field.begin()+counter);
    }

    dirt.update(game_wnd);
  }

  size_t dirt_type = rand() % 4;

  std::random_device rd;  /* Obtain random number from hardware */
  std::mt19937 gen(rd()); /* Seed the generator */
  std::uniform_int_distribution<> distr(20, 22);

  size_t y_cord = distr(gen);

  if (dirt_type == 0) {
    Dirt dirt(this->dirt_bounds.width(), y_cord, '.');
    this->dirt_field.push_back(dirt);
  } else if (dirt_type == 1) {
    Dirt dirt(this->dirt_bounds.width(), y_cord, '-');
    this->dirt_field.push_back(dirt);
  } else if (dirt_type == 2) {
    Dirt dirt(this->dirt_bounds.width(), y_cord, '_');
    this->dirt_field.push_back(dirt);
  } else if (dirt_type == 3) {
    Dirt dirt(this->dirt_bounds.width(), y_cord, '\'');
    this->dirt_field.push_back(dirt);
  } else {
    Dirt dirt(this->dirt_bounds.width(), y_cord, '`');
    this->dirt_field.push_back(dirt);
  }
}

void DirtField::setBounds(rect a) { this->dirt_bounds = a; }

void DirtField::seed() {
  for (size_t i = 0; i < 100; i++) {
    size_t dirt_type = rand() % 4;

    std::random_device rd; /* Obtain random number from hardware */
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(20, 22);
    std::uniform_int_distribution<> distr2(0, 78);

    size_t y_cord = distr(gen);
    size_t x_cord = distr2(gen);

    if (dirt_type == 0) {
      Dirt dirt(x_cord, y_cord, '.');
      this->dirt_field.push_back(dirt);
    } else if (dirt_type == 1) {
      Dirt dirt(x_cord, y_cord, '-');
      this->dirt_field.push_back(dirt);
    } else if (dirt_type == 2) {
      Dirt dirt(x_cord, y_cord, '_');
      this->dirt_field.push_back(dirt);
    } else if (dirt_type == 3) {
      Dirt dirt(x_cord, y_cord, '\'');
      this->dirt_field.push_back(dirt);
    } else {
      Dirt dirt(x_cord, y_cord, '`');
      this->dirt_field.push_back(dirt);
    }
  }
}

rect DirtField::getBounds() { return this->dirt_bounds; }

///////////////////////////// OBJECT FIELD CLASS IMPLEMENTATION
////////////////////////////////

rect ObjectField::getBounds() { return this->field_bounds; }

void ObjectField::setBounds(rect a) { this->field_bounds = a; }

bool ObjectField::update(WINDOW *game_wnd, rect player_rect, int player_score) {
  int dist_index = rand() % 2;

  /* Update existing objects */
  for (size_t i = 0; i < this->object_set.size(); i++) {
    if (this->object_set.at(i).getLastPos().x < this->field_bounds.left()) {
      this->object_set.erase(object_set.begin() + i);
    }

    if (this->object_set.at(i).getFielded() || i == 0) {
      this->object_set.at(i).update(game_wnd);
      this->obj_dist_ref = &(this->object_set.at(i));

      std::vector<vec2i> obj_pos = this->object_set.at(i).getPos();
      size_t length = this->object_set.at(i).getDispChar().size();
      rect object_rect = {{obj_pos[0].x, obj_pos[0].y},
                          {obj_pos[length - 1].x, obj_pos[length - 1].y}};

      if (isCollided(player_rect, object_rect)) {
        return true;
      }
    } else {
      int diff = abs(this->obj_dist_ref->getLastPos().x -
                     this->object_set.at(i).getLastPos().x);
      if (dist_index == 0) {
        if (diff >= 60) {
          this->object_set.at(i).update(game_wnd);
          this->obj_dist_ref = &(this->object_set.at(i));
        }
      } else {
        if (diff >= 78) {
          this->object_set.at(i).update(game_wnd);
          this->obj_dist_ref = &(this->object_set.at(i));
        }
      }
    }
  }

  if (this->object_set.size() < 7) {
    int i = player_score > 500 ? 6 : 4;

    /* Spawn a new object */
    std::random_device rd;  /* Obtain random number from hardware */
    std::mt19937 gen(rd()); /* Seed the generator */
    std::uniform_int_distribution<> distr(0, i);

    size_t object_type =
        distr(gen); /* generate random number between 0 and 6 */
    rect fb = this->getBounds();

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

      Object obj0{p, dc, 0};
      this->object_set.push_back(obj0);
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

      Object obj1{p, dc, 1};
      this->object_set.push_back(obj1);
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

      Object obj2{p, dc, 2};
      this->object_set.push_back(obj2);
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

      Object obj3{p, dc, 3};
      this->object_set.push_back(obj3);
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

      Object obj4{p, dc, 4};
      this->object_set.push_back(obj4);
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

      Object obj5{p, dc, 5};
      this->object_set.push_back(obj5);
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

      Object obj6{p, dc, 6};
      this->object_set.push_back(obj6);
      break;
    }
    }
  }

  return false;
}
