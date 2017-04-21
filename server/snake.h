#ifndef SNAKE_H
#define SNAKE_H////////////////////////////

#include <stdlib.h>
#include <stdio.h>

typedef struct Segment{
  int x;
  int y;
  struct Segment *next;
  struct Segment *prev;
}Segment;

typedef struct Snake{
  int direction;//0-up, 1-down, 2-right, 3-left
  int ate_apple;//1 - true, 0 - false
  int alive;//1- true, 0 false
  //int lenght;
  Segment *head;
  Segment *tail;
}Snake;

typedef struct Apple{
  int x;
  int y;
  int taken;//1 - true, 0 - false
}Apple;

Snake* snake_init(int x, int y, int dir);//init snake

Apple apple_init(int x, int y);//init apple

void snake_print(Snake s);//print snake to console, only for debug puropse

void snake_collisions(Snake *s1, Snake *s2, int width, int height, Apple *a);//move snakes and check collisions

void update_segments(Snake *s);//ubdate snake segments based on acutal movement direction

void pick_new_apple(Apple *a, int width, int height, Snake s1, Snake s2);//pick new apple position

int point_in_snake(int x, int y, Snake s);// 1 - true, 0 - false

void snakes_update(Snake *s1, Snake *s2, int width, int height, Apple *apple);

#endif////////////////////////
