#ifndef OBJECTS_H
#define OBJECTS_H

#include <random>
#include <vector>
#include <string>
#include <ncurses.h>

#include "game.h"

class Obstacle {
public:
	Obstacle(std::vector<vec2i>, std::string, int);
	void render(WINDOW*);
	void update(WINDOW*);
	vec2i getLastPos() const;
	std::vector<vec2i> getPos() const;
	std::string getDispChar() const;
	bool getFielded();
	void fieldedToTrue(); /* Turn fielded to true */
private:
	std::vector<vec2i> m_positions;
	std::string m_disp_char;
	int m_type;
	bool m_fielded;
};

class ObstacleField {
public:
    ObstacleField();
	bool update(WINDOW*, rect, int);
	std::vector<Obstacle> getObjects() const;
	void setBounds(rect);
	rect getBounds();
private:
    std::random_device m_rd;
    std::mt19937 m_gen;
	rect m_field_bounds;
	std::vector<Obstacle> m_object_set;
};

class Dirt {
public:
	Dirt(int, int, char);
	void update(WINDOW*);
	vec2i getPos() const;
private:
	char m_disp_char;
	vec2i m_position;
};

class DirtField {
public:
	void update(WINDOW*);
	void setBounds(rect);
	void seed();
	rect getBounds();
private:
	rect m_dirt_bounds;
	std::vector<Dirt> m_dirt_field;	
};

#endif
