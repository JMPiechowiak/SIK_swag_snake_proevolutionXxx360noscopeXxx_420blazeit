#include "snake_game.h"
//#define DEBUG TRUE
char* to_str(int num)
{
  char *s = malloc(10);
  sprintf(s,"%d",num);
  return s;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+2);//+1 for the zero-terminator, +1 for separator
    strcpy(result, s1);
    strcat(result, " ");
    strcat(result, s2);
    return result;
}

/*Packet by ID:
1 - INIT
2 - READY
3 - RUN
4 - END
*/

void send_first_packet(int socket, const char *size1, const char *size2, Snake my_snake, Snake enemy_snake, Apple apple)
{
  // [ packet_id | grid width | grid hegight | start x | start y | starct dir ..
  //   ...| enemy x | enemy y | apple x | apple y ] */
  char *packet = malloc(512);
  strcpy(packet, to_str(1));
  strcat(packet, " ");
  strcat(packet, size1);
  strcat(packet, " ");
  strcat(packet, size2);
  strcat(packet, " ");
  strcat(packet, to_str(my_snake.head->x));
  strcat(packet, " ");
  strcat(packet, to_str(my_snake.head->y));
  strcat(packet, " ");
  strcat(packet, to_str(my_snake.direction));
  strcat(packet, " ");
  strcat(packet, to_str(enemy_snake.head->x));
  strcat(packet, " ");
  strcat(packet, to_str(enemy_snake.head->y));
  strcat(packet, " ");
  strcat(packet, to_str(apple.x));
  strcat(packet, " ");
  strcat(packet, to_str(apple.y));
  #ifndef DEBUG
  send(socket, packet, strlen(packet), 0);
  #else
  printf("@send_first_packet: %s\n", packet);
  #endif
  free(packet);
}

void send_packet(int socket, int status, Snake player_rec, Snake player_2, Apple a)
{
  char bufor[16];
  short need_response = 0;
  char *packet = malloc(1024);
  switch(status){
    case 2:
    // [ packet id ]
      strcpy(packet, to_str(2));
      break;
    case 3:
    // [ packet id | ate apple? | player new x | player new y ...
    //  ... | enemy ate apple? | enemy new x | enemy new y ...
    //  ... | apple x | apple y ]
      strcpy(packet, to_str(3));
      strcat(packet, " ");
      strcat(packet, to_str(player_rec.ate_apple));
      strcat(packet, " ");
      strcat(packet, to_str(player_rec.head->x));
      strcat(packet, " ");
      strcat(packet, to_str(player_rec.head->y));
      strcat(packet, " ");
      strcat(packet, to_str(player_2.ate_apple));
      strcat(packet, " ");
      strcat(packet, to_str(player_2.head->x));
      strcat(packet, " ");
      strcat(packet, to_str(player_2.head->y));
      strcat(packet, " ");
      strcat(packet, to_str(a.x));
      strcat(packet, " ");
      strcat(packet, to_str(a.y));
      break;
    case 4:
    // [ packet id ]
      strcpy(packet, to_str(4));
      break;
    default:
      strcpy(packet, to_str(-1));
      printf("error: unknown status!");
      exit(-1);
  }

  #ifndef DEBUG
  send(socket, packet, strlen(packet), 0);
  #else
  printf("@send_packet: %s\n", packet);
  #endif
  free(packet);
}

void get_response(int socket, Snake *s)
{
  char bufor;
  #ifndef DEBUG
    recv(socket, &bufor, sizeof(char), 0);
    s->direction = bufor - '0';
  #endif
}

void Game(int socket1, int socket2)
{
  char bufor;
  Snake *player_1 = snake_init(0, GRID_HEIGHT/2, 2);
  Snake *player_2 = snake_init(GRID_WIDTH-1, GRID_HEIGHT/2, 3);
  Apple apple = apple_init(GRID_WIDTH/2, GRID_HEIGHT/2);

  send_first_packet(socket1,to_str(GRID_WIDTH), to_str(GRID_HEIGHT), *player_1, *player_2, apple);
  send_first_packet(socket2,to_str(GRID_WIDTH), to_str(GRID_HEIGHT), *player_2, *player_1, apple);
  recv(socket1, &bufor, sizeof(char), 0);
  recv(socket2, &bufor, sizeof(char), 0);

  send_packet(socket1, 2, *player_1, *player_2, apple);
  recv(socket1, &bufor, sizeof(char), 0);
  send_packet(socket2, 2, *player_2, *player_1, apple);
  recv(socket2, &bufor, sizeof(char), 0);

  send_packet(socket1, 4, *player_1, *player_2, apple);
  send_packet(socket2, 4, *player_2, *player_1, apple);
  // int run = 1;
  // while(run)
  // {
  //   if(!player_1->alive || !player_2->alive)
  //   {
  //     run = 0;
  //     send_packet(socket1, 4, *player_1, *player_2, apple);
  //   }
  //   else
  //   {
  //     send_packet(socket1, 3, *player_1, *player_2, apple);
  //     get_response(socket1, player_1);
  //     snakes_update(player_1, player_2, GRID_WIDTH, GRID_HEIGHT, &apple);
  //   }
  // }

}
