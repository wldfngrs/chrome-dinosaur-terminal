#ifndef GAME_H
#define GAME_H

#include <cstdint>

typedef struct {
	int_fast16_t x;
	int_fast16_t y;
} vec2i;

typedef struct {
	vec2i offset;
	vec2i bounds;

	int_fast16_t top() { return offset.y; }
	int_fast16_t bot() { return offset.y + bounds.y; }
	int_fast16_t left() { return offset.x; }
	int_fast16_t right() { return offset.x + bounds.x; }

	int_fast16_t width() { return bounds.x; }
	int_fast16_t height() { return bounds.y; }

	/*bool contains(vec2i a) { return (a.x >= offset.x && a.x < right()) && 
                                    (a.y >= offset.y && a.y < bot()); }*/
} rect;


bool isCollided(rect a, rect b); /* function to check if rect B is contained in rect B */
int init();
int run(int* max_score);
void close();

#endif