#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <bonsai_types.h>

inline void
CheckForConnectingClient(socket_t *ListeningSocket, network_connection *ClientConnection)
{
  Assert(ListeningSocket->Type == Socket_NonBlocking);

  u32 AddressSize = sizeof(ClientConnection->Address);
  s32 SocketId =
    accept(ListeningSocket->Id,
        (sockaddr*)&ClientConnection->Address,
        &AddressSize);

  // The accept() call overwrites this value to let us know how many bytes it
  // wrote to ClientConnection.Address
  Assert(AddressSize == sizeof(ClientConnection->Address));

  if (SocketId < 0)
  {
    switch(errno)
    {
      case EAGAIN:
      {
        // No incoming connections
      } break;

      default:
      {
        Error("Accept Failed");
      } break;
    }
  }
  else
  {
    Debug("Connection accepted");
    ClientConnection->Socket.Id = SocketId;
    ClientConnection->Connected = True;
  }

  return;
}

#define MAX_CLIENTS 2

int
main(int ArgCount, char **Arguments)
{
  network_connection IncomingConnections = { Socket_NonBlocking };

  s32 BindResult =
    bind(IncomingConnections.Socket.Id,
        (sockaddr *)&IncomingConnections.Address,
        sizeof(IncomingConnections.Address));

  if( BindResult < 0)
    { Error("Bind Failed"); return 1; }

  Debug("Bind Successful");

  listen(IncomingConnections.Socket.Id, 3);

  Debug("Listening");

  network_connection ClientList[2] = {};
  server_message Message = {};

  for(;;)
  {
    for (u32 ClientIndex = 0;
        ClientIndex < MAX_CLIENTS;
        ++ClientIndex)
    {
      network_connection *ClientConn = &ClientList[ClientIndex];
      if (IsConnected(ClientConn))
      {
        while(Read(&ClientList[ClientIndex], &Message) == SocketOpResult_CompletedRW);
        Send(&ClientList[ClientIndex], &Message);
      }
      else
      {
        CheckForConnectingClient(&IncomingConnections.Socket, ClientConn);
      }
    }

  }

  return 0;
}
