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

#define GRID_WIDTH 80
#define GRID_HEIGHT 80

char* to_str(int num);

char* concat(const char *s1, const char *s2);

void send_first_packet(int socket, const char *size1, const char *size2, Snake my_snake, Snake enemy_snake, Apple apple);

void send_packet(int socket, int status, Snake player_rec, Snake player_2, Apple a);

void Game(int socket1, int socket2);

#endif///////////////////////////
