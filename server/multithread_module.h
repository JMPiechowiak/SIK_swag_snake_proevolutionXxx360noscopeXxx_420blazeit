#ifndef _MULTI_T_
#define _MULTI_T_/////////////////////////////////
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "snake.h"
#include "packets.h"

//structs for passing multiple argument to threads
typedef struct{
  int grid_width, grid_height, socket;
  Snake *s1, *s2;
  Apple *a;
}init_data;

typedef struct{
  int socket;
  Snake *s1, *s2;
  Apple *a;
}run_data;

typedef struct{
  int socket, status;
}final_data;

//structures "constructors"
init_data* new_init_data(int socket, int grid_w, int grid_h, Snake *p1, Snake *p2, Apple *a);
run_data* new_run_data(int socket, Snake *s1, Snake *s2, Apple *a);
final_data* new_end_data(int socket, int status);
///

//functions which will be passed to threads
void *Send_packet_and_ready(void *threadarg);

void *Send_run_packet(void *threadarg);

void *Send_last_packe(void *threadarg);

#endif/////////////////////////////
