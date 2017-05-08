#include "snake_game.h"
//#define DEBUG
#define MULTITHREADING 1

/*
  TO DO
  - ok rozszerzenie pakietu END o dodatkowy "bit"
  - ok zamkniecie operacji w osobnych watkach
  - ok odpowiedznie zastotowanie fork() do obslugi wielu klientow w jendym czasie
  - x opakowanie wysylania pakietow w jedna funckcje
  ------------ reszta nie jest konieczna -------------
  - x obsluga naglego rozlaczenia klienta
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
        int status_1, status_2;
        if(!player_1->alive && !player_2->alive) status_1 = status_2 = 2;//both snakes are losers
        else if(!player_1->alive){status_1 = 1; status_2 = 0;}//snake #2 is a winner
        else if(!player_2->alive){status_1 = 0; status_2 = 1;}// snake #1 is a winner
        send_last_packet(socket1, status_1);
        send_last_packet(socket2, status_2);
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
    pthread_t thread_p1, thread_p2;
    pthread_attr_t attr;
    int t1, t2;
    void *status;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    init_data* init_p1 = new_init_data(socket1, GRID_WIDTH, GRID_HEIGHT, player_1, player_2, &apple);
    init_data* init_p2 = new_init_data(socket2, GRID_WIDTH, GRID_HEIGHT, player_2, player_1, &apple);

    //send first config;
    printf("Send first config\n");
    t1 = pthread_create(&thread_p1, &attr, Send_packet_and_ready, (void *)  init_p1);
    if(t1)
    {
    printf("ERROR: can not create init-send threat\n");
    exit(-1);
    }
    t2 = pthread_create(&thread_p2, &attr, Send_packet_and_ready, (void *)  init_p2);
    if(t2)
    {
      printf("ERROR: can not create threat number %d, error code %d\n", 2, t2);
      exit(-1);
    }

    t1 = pthread_join(thread_p1, &status);
    if(t1)
    {
        printf("ERROR: can join threat number %d, error code %d\n", 1, t1);
        exit(-1);
    }
    t1 = pthread_join(thread_p2, &status);
    if(t2)
    {
      printf("ERROR: can join threat number %d, error code %d\n", 2, t2);
      exit(-1);
    }
    printf("Threads joined succesfully!\n");
    //run
    while(run)
    {
      if(!player_1->alive || !player_2->alive)
      {
        int status_1, status_2;
        if(!player_1->alive && !player_2->alive) status_1 = status_2 = 2;//both snakes are losers
        else if(!player_1->alive){status_1 = 1; status_2 = 0;}//snake #2 is a winner
        else if(!player_2->alive){status_1 = 0; status_2 = 1;}// snake #1 is a winner
        send_last_packet(socket1, status_1);
        send_last_packet(socket2, status_2);
        run = 0;
      }
      else
      {
        run_data* run_data_p1  = new_run_data(socket1, player_1, player_2, &apple);
        run_data* run_data_p2  = new_run_data(socket2, player_2, player_1, &apple);
        t1 = pthread_create(&thread_p1, &attr, Send_run_packet, (void *)  run_data_p1 );
        if(t1)
        {
        printf("ERROR: can not create init-send threat\n");
        exit(-1);
        }
        t2 = pthread_create(&thread_p2, &attr, Send_run_packet, (void *)  run_data_p2);
        if(t2)
        {
          printf("ERROR: can not create threat number %d, error code %d\n", 2, t2);
          exit(-1);
        }

        t1 = pthread_join(thread_p1, &status);
        if(t1)
        {
            printf("ERROR: can join threat number %d, error code %d\n", 1, t1);
            exit(-1);
        }
        t2 = pthread_join(thread_p2, &status);
        if(t2)
        {
          printf("ERROR: can join threat number %d, error code %d\n", 2, t2);
          exit(-1);
        }
        snakes_update(player_1, player_2, GRID_WIDTH, GRID_HEIGHT, &apple);
      }

    }
    pthread_attr_destroy(&attr);
  }
  //--------------------------------------------
}
