#include "snake.h"

Snake* snake_init(int x, int y, int dir)
{
  Segment *s = malloc(sizeof(Segment));
  s->x = x;
  s->y = y;
  s->next = NULL;
  s->prev = NULL;
  Snake *new_snake = malloc(sizeof(Snake));
  //new_snake->lenght = 1;
  new_snake->head = s;
  new_snake->tail = s;
  new_snake->direction = dir;
  new_snake->ate_apple = 0;
  new_snake->alive = 1;
  new_snake->length = 1;
  return new_snake;
}

Apple apple_init(int x, int y)
{
  Apple new_apple;
  new_apple.x = x;
  new_apple.y = y;
  new_apple.taken = 0;
  return new_apple;
}

void snake_print(Snake s)
{
  if(s.head == NULL)
  {
    printf("Snake is empty!\n");
    return;
  }
  else
  {
    printf("Snake direction: %d ;Snake segments:\n", s.direction);
    Segment *seg = s.head;
    while(seg != NULL)
    {
      printf("x: %d, y: %d\n", seg->x, seg->y);
      seg = seg->next;
    }
  }
}

int snake_move_x(Snake s)
{
  if(s.direction == 2) return 1;
  else if(s.direction == 3) return -1;
  else return 0;
}

int snake_move_y(Snake s)
{
  if(s.direction == 0) return -1;
  else if(s.direction == 1) return 1;
  else return 0;
}

void update_segments(Snake *s)
{
  if(s->ate_apple)//if snake ate already apple
  {
    Segment *new_segment = malloc(sizeof(Segment));
    new_segment->x = s->head->x + snake_move_x(*s);
    new_segment->y = s->head->y + snake_move_y(*s);
    new_segment->next = s->head;
    new_segment->prev = NULL;
    s->head->prev = new_segment;
    s->head = new_segment;
    s->ate_apple = 0;
  }
  else
  {
    if(s->head != s->tail)//s->length > 1
    {
      Segment *hold = s->tail;
      s->tail = s->tail->prev;
      s->tail->next = NULL;
      s->head->prev = hold;
      hold->next = s->head;
      hold->prev = NULL;
      hold->x = s->head->x;
      hold->y = s->head->y;
      s->head = hold;
    }
    //if snake have only one segment
    s->head->x += snake_move_x(*s);
    s->head->y += snake_move_y(*s);
  }
}

void snake_collisions(Snake *s1, Snake *s2, int width, int height, Apple *a)
{
  //Segment *i = s2->head;
  /*check golbal colliosns, only for head segment,
    we assume that other segments where checked previously*/
  if(s1->head->x < 0 || s1->head->x >= width || s1->head->y < 0 || s1->head->y >= height)
  {
    s1->alive = 0;
    return;
  }

  //
  //check colliosn snakes head to enemy snake body
  if(point_in_snake(s1->head->x, s1->head->y, *s2))
  {
    s1->alive = 0;
    return;
  }

  //check if snake ate apple
  if(s1->head->x == a->x && s1->head->y == a->y && !a->taken)
  {
    a->taken = 1;
    s1->ate_apple = 1;
    s1->length++;
  }
}

int point_in_snake(int x, int y, Snake s)
{
  Segment *i = s.head;
  while(i != NULL)
  {
    if(x == i->x && y == i->y) return 1;
    i = i->next;
  }
  return 0;
}

void pick_new_apple(Apple *a, int width, int height, Snake s1, Snake s2)
{
  int x, y, counter = 0;//to prevent invinite or very long loop
  a->x = 0;
  a->y = 0;
  while(counter < 1000)
  {
    x = rand() % width;
    y = rand() % height;

    //if(!point_in_snake(x,y,s1) && !point_in_snake(x,y,s2))
    //{
      a->x = x;
      a->y = y;
      a->taken = 0;
      break;
    //}
    counter++;
  }
}

void snakes_update(Snake *s1, Snake *s2, int width, int height, Apple *apple)
{
  /*update snakes segments,
    if snake ate already apple we need to create
    new segment, otherwise we will take segment from tail
    and add it, with new data, as a head*/
    //printf("Before update: %d %d\n",s1->head->x, s1->head->y);
    update_segments(s1);
    update_segments(s2);
  //check collisions
    snake_collisions(s1,s2, width, height, apple);
    snake_collisions(s2,s1, width, height, apple);
  //pick new apple if needed
    if(apple->taken) pick_new_apple(apple, width, height, *s1, *s2);
}
