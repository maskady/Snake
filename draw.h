#include "point.h"
#ifndef DRAW_H
#define DRAW_H

void initialize(int fruitx, int fruity);
void draw_border();
void erase_old_snake(const Point* snake, int length);
void draw_snake(const Point* snake, int length);
void erase_old_fruit(int fruitx, int fruity);
void draw_fruit(int fruitx, int fruity);
void draw_score(int score);
void draw_footer();
void draw(int length, const Point* snake);

#endif