#include "snake.h"

Snake* snake_init(int x, int y)
{
  Segment *s = malloc(sizeof(Segment));
  s->x = x;
  s->y = y;
  s->next = NULL;
  Snake *new_snake = malloc(sizeof(Snake));
  new_snake->lenght = 1;
  new_snake->head = s;
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
    printf("Snake lenght: %d\nSnake segments:\n", s.lenght);
    Segment *seg = s.head;
    while(seg != NULL)
    {
      printf("x: %d, y: %d\n", seg->x, seg->y);
      seg = seg->next;
    }
  }
}

void snakes_update(Snake *s1, Snake *s2)
{
  //add segment
  s1->lenght++;
  Segment *new = malloc(sizeof(Segment));

    new->x = s1->head->x + 1;
    new->y = s1->head->y;
    new->next = s1->head;
    s1->head = new;

}
