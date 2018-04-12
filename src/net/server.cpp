#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <net/server.h>

int
main(int ArgCount, char **Arguments)
{
  struct sockaddr_in server , client;
  char client_message[SERVER_MESSAGE_LENGTH];

  int socket_desc = socket(AF_INET , SOCK_STREAM , 0);
  if (socket_desc == -1) { printf("Could not create socket\n"); return 1; }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( 1337 );

  if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) { printf("bind failed. Error\n"); return 1; }
  printf("bind done\n");

  listen(socket_desc , 3);

  int c = sizeof(struct sockaddr_in);
  int client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

  if (client_sock < 0) { perror("accept failed"); return 1; }
  printf("Connection accepted\n");

  int read_size = 0;
  while( (read_size = recv(client_sock , client_message , SERVER_MESSAGE_LENGTH , 0)) > 0 )
  {
    write(client_sock , "Hello" , 5);
  }

  if(read_size == 0)
  {
    printf("Client disconnected\n");
    fflush(stdout);
  }
  else if(read_size == -1)
  {
    perror("recv failed");
  }

  return 0;
}
