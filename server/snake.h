#ifndef SNAKE_H
#define SNAKE_H////////////////////////////

#include <stdlib.h>
#include <stdio.h>

typedef struct Segment{
  int x;
  int y;
  struct Segment *next;
}Segment;

typedef struct Snake{
  int direction;//0-up, 1-down, 2-right, 3-left
  int lenght;
  Segment *head;
}Snake;

typedef struct Apple{
  int x;
  int y;
  int taken;
}Apple;

Snake* snake_init(int x, int y);

Apple apple_init(int x, int y);

void snake_print(Snake s);

void snakes_update(Snake *s1, Snake *s2);

//void snake_update(Snake s, int action);

#endif////////////////////////
