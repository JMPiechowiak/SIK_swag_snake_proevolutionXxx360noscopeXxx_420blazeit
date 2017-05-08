#ifndef _PACKETS_H_
#define _PACKETS_H_
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

char* to_str(int num);//convert string to int

//send init packet to socket
void send_first_packet(int socket, const char *size1, const char *size2, Snake my_snake, Snake enemy_snake, Apple apple);

//send "normal" packet, packet from depends on status
void send_packet(int socket, int status, Snake player_rec, Snake player_2, Apple a);

//send last packet
void send_last_packet(int socket, int event);

//get response - client's input, and update client's snake
void get_response(int socket, Snake *s);

#endif
