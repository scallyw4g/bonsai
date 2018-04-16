#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <bonsai_types.h>
#include <net/server.h>

network_connection
WaitForClientConnection(socket_t ListeningSocket)
{
  network_connection ClientConnection = {};

  u32 AddressSize = sizeof(ClientConnection.Address);
  socket_t Socket = accept(ListeningSocket, (sockaddr*)&ClientConnection.Address, &AddressSize);

  // Accept overwrites this value to let us know how many bytes it wrote to ClientConnection.Address
  Assert(AddressSize == sizeof(ClientConnection.Address));

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
      ClientConnection = WaitForClientConnection(IncomingConnections.Socket);
    }
  }

  return 0;
}
