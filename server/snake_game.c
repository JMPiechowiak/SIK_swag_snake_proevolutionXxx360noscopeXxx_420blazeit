#include "snake_game.h"
#define DEBUG TRUE
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

void send_first_packet(int socket, const char *size1, const char *size2, Snake s)
{
  char *packet = malloc(512);//6 = 4 x empty spaces + state + \x00
  strcpy(packet, to_str(0));
  strcat(packet, " ");
  strcat(packet, size1);
  strcat(packet, " ");
  strcat(packet, size2);
  strcat(packet, " ");
  strcat(packet, to_str(s.head->x));
  strcat(packet, " ");
  strcat(packet, to_str(s.head->y));
  strcat(packet, " ");
  strcat(packet, to_str(s.direction));
  #ifndef DEBUG
  send(socket, packet, strlen(packet), 0);
  #else
  printf("@send_first_packet: %s\n", packet);
  #endif
  free(packet);
}
/*status:
0 - WAIT
1 - INIT
2 - READY
3 - RUN
4 - END
*/
void send_packet(int socket, int status, Snake player_rec, Snake player_2, Apple a)
{
  char bufor[16];
  short need_response = 0;
  char *packet = malloc(1024);
  switch(status){
    case 0:
      strcpy(packet, to_str(0));
      break;
    case 1:
      strcpy(packet, to_str(1));
      strcat(packet, " ");
      strcat(packet, to_str(player_2.head->x));
      strcat(packet, " ");
      strcat(packet, to_str(player_2.head->y));
      strcat(packet, " ");
      strcat(packet, to_str(a.x));
      strcat(packet, " ");
      strcat(packet, to_str(a.y));
      need_response = 1;
      break;
    case 2:
      strcpy(packet, to_str(2));
      need_response = 1;
      break;
    case 3:
      strcpy(packet, to_str(3));
      strcat(packet, " ");
      strcat(packet, to_str(0));
      strcat(packet, " ");
      strcat(packet, to_str(player_rec.head->x));
      strcat(packet, " ");
      strcat(packet, to_str(player_rec.head->y));
      strcat(packet, " ");
      strcat(packet, to_str(0));
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
      strcpy(packet, to_str(4));
      break;
    default:
      strcpy(packet, to_str(101));
      printf("error: unknown status!");
      exit(-1);
  }

  #ifndef DEBUG
  send(socket, packet, strlen(packet), 0);
  if(need_response == 1) recv(socket, bufor, 16, 0);
  #else
  printf("@send_packet: %s\n", packet);
  #endif
  free(packet);
}

void Game(int socket1, int socket2)
{
  Snake *player_1 = snake_init(0, GRID_HEIGHT/2);
  Snake *player_2 = snake_init(GRID_WIDTH - 1, GRID_HEIGHT/2);
  Apple apple = apple_init(GRID_WIDTH/2, GRID_HEIGHT/2);
  send_first_packet(socket1,to_str(GRID_WIDTH), to_str(GRID_HEIGHT), *player_1);
  send_packet(socket1, 1, *player_1, *player_2, apple);
  send_packet(socket1, 2, *player_1, *player_2, apple);
  send_packet(socket1, 3, *player_1, *player_2, apple);
  send_packet(socket1, 4, *player_1, *player_2, apple);
  //snake_print(*player_1);
  //snake_print(*player_2);
  //sleep(5);
  // send_status(socket1, 1);
  // //update
  // send_packet(socket1, 2, *player_1, *player_2);
  // snakes_update(player_1, player_2);



  //sleep(5);
  //snake_init(player_2, GRID_WIDTH-1, GRID_HEIGHT/2);
  //send options
  //first_packet(socket1, '1', to_str(GRID_WIDTH), to_str(GRID_HEIGHT), to_str(0) ,to_str(GRID_HEIGHT/2));
  //get response
  //recv(socket1, bufor, sizeof(bufor),0);
}
