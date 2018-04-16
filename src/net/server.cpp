#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <bonsai_types.h>
#include <net/server.h>

network_connection
WaitForClientConnection(socket_t *ListeningSocket)
{
  network_connection ClientConnection = { Socket_Blocking };

  u32 AddressSize = sizeof(ClientConnection.Address);
  s32 SocketId =
    accept(ListeningSocket->Id,
        (sockaddr*)&ClientConnection.Address,
        &AddressSize);

  // The accept() call overwrites this value to let us know how many bytes it
  // wrote to ClientConnection.Address
  Assert(AddressSize == sizeof(ClientConnection.Address));

  if (SocketId < 0)
  {
    Error("Accept Failed");
  }
  else
  {
    Debug("Connection accepted");
    ClientConnection.Socket.Id = SocketId;
    ClientConnection.Connected = True;
  }

  return ClientConnection;
}

#define MAX_CLIENTS 2

int
main(int ArgCount, char **Arguments)
{
  network_connection IncomingConnections = { Socket_Blocking };

  s32 BindResult =
    bind(IncomingConnections.Socket.Id,
        (sockaddr *)&IncomingConnections.Address,
        sizeof(IncomingConnections.Address));

  if( BindResult < 0)
    { Error("Bind Failed"); return 1; }

  Debug("Bind Successful");

  listen(IncomingConnections.Socket.Id, 3);

  Debug("Listening");

  network_connection ClientList[2] = { Socket_Blocking, Socket_Blocking };
  server_message Message = {};


  for (u32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    ClientList[ClientIndex] = WaitForClientConnection(&IncomingConnections.Socket);
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
