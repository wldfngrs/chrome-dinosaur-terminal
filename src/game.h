#ifndef GAME_H
#define GAME_H

struct vec2i{
	int x;
	int y;
};

struct rect{
	vec2i offset;
	vec2i bounds;

	int top() { return offset.y; }
	int bot() { return offset.y + bounds.y; }
	int left() { return offset.x; }
	int right() { return offset.x + bounds.x; }

	int width() { return bounds.x; }
	int height() { return bounds.y; }

	/*bool contains(vec2i a) { return (a.x >= offset.x && a.x < right()) && 
                                    (a.y >= offset.y && a.y < bot()); }*/
};


bool isCollided(rect a, rect b); /* function to check if rect B is contained in rect B */
int init();
bool run(int &max_score);
void close();

#endif
