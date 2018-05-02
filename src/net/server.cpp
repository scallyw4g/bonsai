#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#include <bonsai_types.h>
#include <unix_platform.cpp>

inline void
RejectIncomingConnnections(socket_t *ListeningSocket)
{
  errno = 0;

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
        Info("No Connections");
      } break;

      default:
      {
        Error("Accept Failed");
      } break;
    }
  }
  else if (SocketId > 0)
  {
    Info("Connection Accepted");
    ClientConnection->Socket.Id = SocketId;
    ClientConnection->Socket.Type = Socket_NonBlocking;
    ClientConnection->State = ConnectionState_Connected;


    handshake_message Handshake = {ClientConnection->Client->Id};
    Send(ClientConnection, &Handshake);
  }
  else
  {
    InvalidCodePath();
  }

  return;
}

int
main(int ArgCount, char **Arguments)
{
  network_connection IncomingConnections = { Socket_NonBlocking, "127.0.0.1" };

  s32 BindResult =
    bind(IncomingConnections.Socket.Id,
        (sockaddr *)&IncomingConnections.Address,
        sizeof(IncomingConnections.Address));

  if( BindResult < 0)
    { Error("Bind Failed"); perror(strerror(errno)); return 1; }


  Debug("Bind Successful");

  listen(IncomingConnections.Socket.Id, 0);

  Debug("Listening");

  network_connection ClientConnections[MAX_CLIENTS] = { {Socket_NonBlocking, "127.0.0.1"},
                                                        {Socket_NonBlocking, "127.0.0.1"} };

  client_to_server_message InputMessage = {};
  server_to_client_message ServerState = {};

  for (u32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    network_connection *Connection = &ClientConnections[ClientIndex];
    Connection->Client = &ServerState.Clients[ClientIndex];
    Connection->Client->Id = ClientIndex;
  }

  for(;;)
  {
    for (u32 ClientIndex = 0;
        ClientIndex < MAX_CLIENTS;
        ++ClientIndex)
    {
      network_connection *Connection = &ClientConnections[ClientIndex];
      client_state *Client = &ServerState.Clients[ClientIndex];

      // FIXME(Jesse): Write on connection instead of here?
      //
      // These get overwritten when disconnecting and therefore must be written
      // each time through this loop.
      Connection->Client->Id = ClientIndex;

      if (IsConnected(Connection))
      {
        if (FlushIncomingMessages(Connection, &InputMessage)
            == SocketOpResult_CompletedRW)
        {
          *Client = InputMessage.Client;
          Send(Connection, &ServerState);
        }
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
