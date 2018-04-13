#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <bonsai_types.h>
#include <net/server.h>

socket_t
WaitForClientConnection(int socket_desc, sockaddr_in Client)
{
  int c = sizeof(sockaddr_in);
  socket_t Socket = accept(socket_desc, (sockaddr *)&Client, (socklen_t*)&c);

  if (Socket < 0)
    { Error("Accept Failed"); return 1; }
  else
    { Debug("Connection accepted"); }

  return Socket;
}

int
main(int ArgCount, char **Arguments)
{
  sockaddr_in client;
  sockaddr_in server;

  int socket_desc = socket(AF_INET , SOCK_STREAM , 0);
  if (socket_desc == -1) { Debug("Could not create socket"); return 1; }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( REMOTE_PORT );

  if( bind(socket_desc,(sockaddr *)&server , sizeof(server)) < 0)
    { Error("Bind Failed"); return 1; }
  else
    { Debug("Bind Successful"); }

  listen(socket_desc , 3);



  {
    server_message Message = {};
    int BytesRead = 0;

    b32 ConnectedToClient = False;
    socket_t ClientSocket = {};

    while(true)
    {
      if (ConnectedToClient)
      {
        BytesRead = recv(ClientSocket , (void*)&Message , sizeof(server_message), 0);

        if(BytesRead == -1)
        { Error("Reading socket failed"); ConnectedToClient = False; }


        if (!Send(ClientSocket, &Message))
        {
          ConnectedToClient = False;
        }

      }
      else
      {
        ClientSocket = WaitForClientConnection(socket_desc, client);
        ConnectedToClient = True;
      }

    }


  }

  return 0;
}
