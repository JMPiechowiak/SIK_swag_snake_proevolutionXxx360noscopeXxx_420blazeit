#include "snake_game.h"
#define MAX_GAMES 10
//#define DEBUG TRUE
int main(void) {

 unsigned int port;
 int main_socket, player1_soc, player2_soc, games;
 struct sockaddr_in adr, nadawca_1, nadawca_2;
 socklen_t dl = sizeof(struct sockaddr_in);

 srand(time(NULL));
 #ifndef DEBUG
 printf("Na ktorym porcie mam sluchac? : ");
 scanf("%u", &port);
 main_socket = socket(PF_INET, SOCK_STREAM, 0);
 adr.sin_family = AF_INET;
 adr.sin_port = htons(port);
 adr.sin_addr.s_addr = INADDR_ANY;
 if (bind(main_socket, (struct sockaddr*) &adr, sizeof(adr)) < 0) {
   printf("Bind nie powiodl sie.\n");
   return 1;
 }
 if (listen(main_socket, 10) < 0) {
   printf("Listen nie powiodl sie.\n");
   return 1;
 }

 printf("Czekam na polaczenie ...\n");
 games = 0;
 int pid;
 while((player1_soc = accept(main_socket,
   (struct sockaddr*) &nadawca_1,
   &dl)) > 0)
 {
   games++;
   if(1)
   {
     if(games < MAX_GAMES)
     {
       printf("New game created. Connected Player #1: %s\n Waiting for Player #2...\n", inet_ntoa(nadawca_1.sin_addr));
       player2_soc = accept(main_socket,(struct sockaddr*) &nadawca_2,&dl);
       printf("Connected Player #2: %s\n", inet_ntoa(nadawca_2.sin_addr));
       Game(player1_soc, player2_soc);
       printf("Close game\n");
       close(player1_soc);
       close(player2_soc);
     }
     else
     {
       printf("to many connections, disconnecting %s\n",inet_ntoa(nadawca_1.sin_addr));
       close(player1_soc);
     }
   }
   games--;
 }
 close(main_socket);
#else
  Game(-1, -1);
#endif
 return 0;
}
