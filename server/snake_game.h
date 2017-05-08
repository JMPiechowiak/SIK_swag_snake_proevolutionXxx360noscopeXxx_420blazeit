#ifndef _SNAKE_GAME_
#define _SNAKE_GAME_///////////////////////////
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "snake.h"
#include "packets.h"
#include "multithread_module.h"

#define GRID_WIDTH 80
#define GRID_HEIGHT 80

//init and main loop for each child process
void Game(int socket1, int socket2);

#endif///////////////////////////
