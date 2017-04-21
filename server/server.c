#include "snake_game.h"
//#define DEBUG TRUE
int main(void) {

 unsigned int port;
 int gniazdo, gniazdo2;
 struct sockaddr_in adr, nadawca;
 socklen_t dl = sizeof(struct sockaddr_in);

 srand(time(NULL));
 #ifndef DEBUG
 printf("Na ktorym porcie mam sluchac? : ");
 scanf("%u", &port);
 gniazdo = socket(PF_INET, SOCK_STREAM, 0);
 adr.sin_family = AF_INET;
 adr.sin_port = htons(port);
 adr.sin_addr.s_addr = INADDR_ANY;
 if (bind(gniazdo, (struct sockaddr*) &adr, sizeof(adr)) < 0) {
   printf("Bind nie powiodl sie.\n");
   return 1;
 }
 if (listen(gniazdo, 10) < 0) {
   printf("Listen nie powiodl sie.\n");
   return 1;
 }

 printf("Czekam na polaczenie ...\n");
 if ((gniazdo2 = accept(gniazdo,
   (struct sockaddr*) &nadawca,
   &dl)) > 0)
 {
   printf("Connected with: %s\n", inet_ntoa(nadawca.sin_addr));
   Game(gniazdo2, gniazdo);
   printf("Disconnecting: %s\n", inet_ntoa(nadawca.sin_addr));
   close(gniazdo2);
 }
 close(gniazdo);
#else
  Game(-1, -1);
#endif
 return 0;
}
