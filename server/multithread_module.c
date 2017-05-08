#include "multithread_module.h"

init_data* new_init_data(int socket, int grid_w, int grid_h, Snake *p1, Snake *p2, Apple *a) {
  init_data* d = malloc(sizeof(init_data));
  d->socket = socket;
  d->grid_width = grid_w;
  d->grid_height = grid_h;
  d->s1 = p1;
  d->s2 = p2;
  d->a = a;
  return d;
}

run_data* new_run_data(int socket, Snake *s1, Snake *s2, Apple *a)
{
  run_data* d = malloc(sizeof(run_data));
  d->socket = socket;
  d->s1 = s1;
  d->s2 = s2;
  d->a = a;
  return d;
}

final_data* new_end_data(int socket, int status)
{
  final_data* d = malloc(sizeof(final_data));
  d->socket = socket;
  d->status = status;
  return d;
}

void *Send_packet_and_ready(void *threadarg)
{
   init_data *data;
   char bufor;
   data = (init_data*) threadarg;
   send_first_packet(data->socket, to_str(data->grid_width), to_str(data->grid_height), *data->s1, *data->s2, *data->a);
   recv(data->socket, &bufor, sizeof(char), 0);
   send_packet(data->socket, 2, *data->s1, *data->s2, *data->a);
   recv(data->socket, &bufor, sizeof(char), 0);
   pthread_exit(NULL);
}

void *Send_run_packet(void *threadarg)
{
  run_data *data;
  data = (run_data*) threadarg;
  send_packet(data->socket, 3, *data->s1, *data->s2, *data->a);
  get_response(data->socket, data->s1);
  pthread_exit(NULL);
}

void *Send_last_packet(void *threadarg)
{
  final_data *my_data;
  my_data = (final_data*) threadarg;

  pthread_exit(NULL);
}