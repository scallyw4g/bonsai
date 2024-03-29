#include <stdint.h>
#include <bonsai_types.h>

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
CheckForConnectingClient(socket_t *ListeningSocket, network_connection *ClientConnection, s32 ClientId)
{
  Assert(ListeningSocket->Type == Socket_NonBlocking);

  u32 AddressSize = sizeof(ClientConnection->Address);
  errno = 0;
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
    Info("Connection Accepted");
    ClientConnection->Socket.Id = SocketId;
    ClientConnection->Socket.Type = Socket_NonBlocking;
    ClientConnection->State = ConnectionState_Connected;
    ClientConnection->Client->Id = ClientId;


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
  const char *IpAddress = "127.0.0.1";
  if (ArgCount == 2)
  {
    IpAddress = Arguments[1];
  }

  network_connection IncomingConnections = CreateNetworkConnection(Socket_NonBlocking, IpAddress);

  errno = 0;
  s32 BindResult =
    bind(IncomingConnections.Socket.Id,
        (sockaddr *)&IncomingConnections.Address,
        sizeof(IncomingConnections.Address));

  if( BindResult < 0)
    { Error("Bind Failed"); Log("Error Code: %d", errno); return 1; }


  Debug("Bind Successful");

  listen(IncomingConnections.Socket.Id, 0);

  Debug("Listening on %s", IpAddress);

  network_connection ClientConnections[MAX_CLIENTS] = { CreateNetworkConnection(Socket_NonBlocking, "Ignored"),
                                                        CreateNetworkConnection(Socket_NonBlocking, "Ignored") };

  client_to_server_message InputMessage = {};
  server_to_client_message ServerState = {};

  for (s32 ClientIndex = 0;
      ClientIndex < MAX_CLIENTS;
      ++ClientIndex)
  {
    network_connection *Connection = &ClientConnections[ClientIndex];
    Connection->Client = &ServerState.Clients[ClientIndex];
    Connection->Client->Id = -1;
  }

  for(;;)
  {
    for (s32 ClientIndex = 0;
        ClientIndex < MAX_CLIENTS;
        ++ClientIndex)
    {
      network_connection *Connection = &ClientConnections[ClientIndex];
      client_state *Client = &ServerState.Clients[ClientIndex];

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
        CheckForConnectingClient(&IncomingConnections.Socket, Connection, ClientIndex);
      }
    }

    RejectIncomingConnnections(&IncomingConnections.Socket);
  }
}
