#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <bonsai_types.h>
#include <net/server.h>

network_connection
WaitForClientConnection(socket_t ListeningSocket, sockaddr_in Client)
{
  network_connection ClientConnection = {};

  int c = sizeof(sockaddr_in);
  socket_t Socket = accept(ListeningSocket, (sockaddr *)&Client, (socklen_t*)&c);

  if (Socket < 0)
  {
    Error("Accept Failed");
  }
  else
  {
    Debug("Connection accepted");
    ClientConnection.Socket = Socket;
    ClientConnection.Connected = True;
  }

  return ClientConnection;
}

int
main(int ArgCount, char **Arguments)
{
  sockaddr_in client;

  server Server = CreateServer();
  Server.Address.sin_addr.s_addr = INADDR_ANY;

  network_connection IncomingConnections = {};
  IncomingConnections.Socket = CreateSocket();

  if( bind(IncomingConnections.Socket, (sockaddr *)&Server , sizeof(Server)) < 0)
  {
    Error("Bind Failed");
    return 1;
  }

  Debug("Bind Successful");

  listen(IncomingConnections.Socket , 3);

  Debug("Listening");

  network_connection ClientConnection = {};
  server_message Message = {};

  for(;;)
  {
    if (IsConnected(&ClientConnection))
    {
      Read(&ClientConnection, &Message);
      Send(&ClientConnection, &Message);
    }
    else
    {
      ClientConnection = WaitForClientConnection(IncomingConnections.Socket, client);
    }
  }

  return 0;
}
