#include "snake_game.h"
#define DEBUG
#define MULTITHREADING 0

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

void send_last_packet(int socket, int event)
{
  // [ packet_id | event id ]
  // events:
  // 0 - this snake is a winner
  // 1 - this snake is a loser
  // 2 - it is a draw
  char *packet = malloc(10);
  strcpy(packet, to_str(4));
  strcat(packet, " ");
  strcat(packet, to_str(event));
  #ifndef DEBUG
  send(socket, packet, strlen(packet), 0);
  #else
  printf("@send_last_packet: %s\n", packet);
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
/*
  TO DO
  - ok rozszerzenie pakietu END o dodatkowy "bit"
  - x zamkniecie operacji w osobnych watkach
  - x odpowiedznie zastotowanie fork() do obslugi wielu klientow w jendym czasie
  - x opakowanie wysylania pakietow w jedna funckcje
  ------------ reszta nie jest konieczna -------------
  - x obsluga naglego rozlaczenia klienta
  - x dodanie trybu palzy i oczekiwania po rozlaczeniu
  - x wczytywanie ustawien z pliku lub wpisywanie recznie

*/
void Game(int socket1, int socket2)
{
  char bufor;
  int run = 1;
  Snake *player_1 = snake_init(0, GRID_HEIGHT/2, 2);
  Snake *player_2 = snake_init(GRID_WIDTH-1, GRID_HEIGHT/2, 3);
  Apple apple = apple_init(GRID_WIDTH/2, GRID_HEIGHT/2);
  //if multithreading is off-------------------------------------------
  if(!MULTITHREADING)
  {
    send_first_packet(socket1,to_str(GRID_WIDTH), to_str(GRID_HEIGHT), *player_1, *player_2, apple);
    send_first_packet(socket2,to_str(GRID_WIDTH), to_str(GRID_HEIGHT), *player_2, *player_1, apple);
    recv(socket1, &bufor, sizeof(char), 0);
    recv(socket2, &bufor, sizeof(char), 0);

    send_packet(socket1, 2, *player_1, *player_2, apple);
    recv(socket1, &bufor, sizeof(char), 0);
    send_packet(socket2, 2, *player_2, *player_1, apple);
    recv(socket2, &bufor, sizeof(char), 0);

    while(run)
    {
      if(!player_1->alive || !player_2->alive)
      {
        if(!player_1->alive && !player_2->alive)//both snakes are losers
        {
          send_last_packet(socket1, 2);
          send_last_packet(socket2, 2);
        }
        else if(!player_1->alive)//snake #2 is a winner
        {
          send_last_packet(socket1, 0);
          send_last_packet(socket2, 1);
        }
        else if(!player_2->alive)// snake #1 is a winner
        {
          send_last_packet(socket1, 1);
          send_last_packet(socket2, 0);
        }
        run = 0;
      }
      else
      {
        send_packet(socket1, 3, *player_1, *player_2, apple);
        send_packet(socket2, 3, *player_2, *player_1, apple);
        get_response(socket1, player_1);
        get_response(socket2, player_2);

        snakes_update(player_1, player_2, GRID_WIDTH, GRID_HEIGHT, &apple);
      }
    }
  }
  //if multithreading is on---------------------------------------------
  else
  {

  }
}
