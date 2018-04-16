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

#define MAX_CLIENTS 2

int
main(int ArgCount, char **Arguments)
{
  network_connection IncomingConnections = {};

  s32 BindResult =
    bind(IncomingConnections.Socket,
        (sockaddr *)&IncomingConnections.Address,
        sizeof(IncomingConnections.Address));

  if( BindResult < 0)
    { Error("Bind Failed"); return 1; }

  Debug("Bind Successful");

  listen(IncomingConnections.Socket , 3);

  Debug("Listening");

  network_connection ClientList[2] = {};
  server_message Message = {};


  for (u32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    ClientList[ClientIndex] = WaitForClientConnection(IncomingConnections.Socket);
  }


  for(;;)
  {
    for (u32 ClientIndex = 0;
        ClientIndex < MAX_CLIENTS;
        ++ClientIndex)
    {
      Read(&ClientList[ClientIndex], &Message);
      Send(&ClientList[ClientIndex], &Message);
    }
  }

  return 0;
}
