#define REMOTE_PORT 1337

struct server_message
{
  u32 ClientId;
};

struct network_connection
{
  sockaddr_in Address;
  socket_t Socket;
  b32 Connected;
};

struct server
{
  sockaddr_in Address;
};

enum socket_op
{
  SocketOp_Null,
  SocketOp_Read,
  SocketOp_Write,
  SocketOp_Count
};

inline socket_t
CreateSocket()
{
  socket_t Socket = socket(AF_INET , SOCK_STREAM , 0);
  if (Socket == -1)
  {
    Error("Could not create socket");
    Socket = 0;
  }
  return Socket;
}

inline server
CreateServer()
{
  server Server = {};
  Server.Address.sin_family = AF_INET;
  Server.Address.sin_port = htons( REMOTE_PORT );
  return Server;
}

inline b32
IsConnected(network_connection *Conn)
{
  b32 Result = Conn->Connected;
  return Result;
}

inline void
Disconnect(network_connection *Connection)
{
  Info("Disconnecting");
  Assert(Connection->Socket);

  close(Connection->Socket);

  Connection->Socket = 0;
  Connection->Connected = False;

  Assert(!IsConnected(Connection));

  return;
}

void
NetworkOp(network_connection *Connection, server_message *Message, socket_op SocketOp)
{
  Assert(Connection);
  Assert(Message);

  b32 OpSuccessful = False;
  s64 NumBytes = 0;

  // We may have disconnected on a previous attempt to read/write this frame
  if (IsConnected(Connection))
  {
    switch(SocketOp)
    {
      case SocketOp_Read:
      {
        u32 Flags = 0;
        NumBytes = recv(Connection->Socket, (void*)Message, sizeof(server_message), Flags);
      } break;

      case SocketOp_Write:
      {
        u32 Flags = MSG_NOSIGNAL;
        NumBytes = send(Connection->Socket, (void*)Message, sizeof(server_message) , Flags);
      } break;

      InvalidDefaultCase;
    }

    switch(NumBytes)
    {
      case -1:
      {
        Error("SocketOp failed : %s", strerror(errno));
      } break;

      case 0:
      {
        Info("Network peer closed connection gracefully");
      } break;

      default:
      {
        OpSuccessful = True;
      } break;
    }

    if (!OpSuccessful) { Disconnect(Connection); }
  }

  return;
}

void
Send(network_connection *Connection, server_message *Message)
{
  NetworkOp(Connection, Message, SocketOp_Write);
  return;
}

void
Read(network_connection *Connection, server_message *Message)
{
  NetworkOp(Connection, Message, SocketOp_Read);
  return;
}
