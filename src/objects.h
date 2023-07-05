#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <string>
#include <ncurses.h>

#include "game.h"

class Object {
public:
	Object(std::vector<vec2i>, std::string, int);
	void render(WINDOW*);
	void update(WINDOW*);
	vec2i getLastPos() const;
	std::vector<vec2i> getPos() const;
	std::string getDispChar() const;
	bool getFielded();
	void fieldedToTrue(); /* Turn fielded to true */
private:
	std::vector<vec2i> pos;
	std::string disp_char;
	int type;
	bool fielded;
};

class ObjectField {
public:
	bool update(WINDOW*, rect, int);
	std::vector<Object> getObjects() const;
	void setBounds(rect);
	rect getBounds();
private:
	Object* obj_dist_ref;
	rect field_bounds;
	std::vector<Object> object_set;
};

class Dirt {
public:
	Dirt(int, int, char);
	void update(WINDOW*);
	vec2i getPos() const;
private:
	char disp_char;
	vec2i pos;
};

class DirtField {
public:
	void update(WINDOW*);
	void setBounds(rect);
	void seed();
	rect getBounds();
private:
	rect dirt_bounds;
	std::vector<Dirt> dirt_field;	
};

#endif