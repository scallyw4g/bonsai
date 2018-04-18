#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <bonsai_types.h>

#define MAX_CLIENTS 2

inline void
RejectIncomingConnnections(socket_t *ListeningSocket)
{
  s32 SocketId = accept4(ListeningSocket->Id,
                         0, 0, // Address write-back information
                         SOCK_NONBLOCK);

  while (SocketId > 0)
  {
    close(SocketId);
    SocketId = accept4(ListeningSocket->Id,
                       0, 0, // Address write-back information
                       SOCK_NONBLOCK);
  }

  Assert(errno == EAGAIN || errno == EWOULDBLOCK);
}

inline void
CheckForConnectingClient(socket_t *ListeningSocket, network_connection *ClientConnection)
{
  Assert(ListeningSocket->Type == Socket_NonBlocking);

  u32 AddressSize = sizeof(ClientConnection->Address);
  s32 SocketId = accept4(ListeningSocket->Id,
                        (sockaddr*)&ClientConnection->Address,
                        &AddressSize,
                        SOCK_NONBLOCK);

  // The accept() call overwrites this value to let us know how many bytes it
  // wrote to ClientConnection.Address
  Assert(AddressSize == sizeof(ClientConnection->Address));

  if (SocketId == -1)
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
  else if (SocketId > 0)
  {
    Debug("Connection accepted");
    ClientConnection->Socket.Id = SocketId;
    ClientConnection->Socket.Type = Socket_NonBlocking;
    ClientConnection->Connected = True;
  }
  else
  {
    InvalidCodePath();
  }

  return;
}

#define MAX_CLIENTS 2

struct client_state
{
  u32 ClientId;
  canonical_position P;
};

void
BroadcastServerState(client_state *Clients, network_connection *Connected)
{
}

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

  listen(IncomingConnections.Socket.Id, 0);

  Debug("Listening");

  client_state Clients[MAX_CLIENTS] = {};
  network_connection ClientConnections[MAX_CLIENTS] = {};
  server_to_client_message OutputMessage = {};

  for(;;)
  {
    for (u32 ClientIndex = 0;
        ClientIndex < MAX_CLIENTS;
        ++ClientIndex)
    {
      network_connection *Connection = &ClientConnections[ClientIndex];
      if (IsConnected(Connection))
      {
        if (FlushIncomingMessages(Connection, &OutputMessage)
            == SocketOpResult_CompletedRW)
        {
          Send(Connection, &OutputMessage);
        }

        BroadcastServerState(Clients, Connection);
      }
      else
      {
        CheckForConnectingClient(&IncomingConnections.Socket, Connection);
      }
    }

    RejectIncomingConnnections(&IncomingConnections.Socket);
  }

  return 0;
}
