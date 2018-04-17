#define REMOTE_PORT 1337

inline socket_t
CreateSocket(socket_type Type)
{
  u32 SocketType = SOCK_STREAM | (Type == Socket_Blocking ? 0:SOCK_NONBLOCK);

  socket_t Socket = {Type};
  Socket.Id = socket(AF_INET , SocketType, 0);

  if (Socket.Id == -1)
  {
    Error("Could not create socket");
    Socket.Id = 0;
  }
  return Socket;
}

inline sockaddr_in
CreateAddress()
{
  sockaddr_in Address = {};
  Address.sin_family = AF_INET;
  Address.sin_port = htons( REMOTE_PORT );
  return Address;
}

struct network_connection
{
  sockaddr_in Address;
  socket_t Socket;
  b32 Connected;

  network_connection(socket_type Type)
  {
    Clear(this);
    this->Socket = CreateSocket(Type);
    this->Address = CreateAddress();
  }

  network_connection() = default;
};

enum socket_op
{
  SocketOp_Null,
  SocketOp_Read,
  SocketOp_Write,
  SocketOp_Count
};

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
  Assert(Connection->Socket.Id);

  close(Connection->Socket.Id);
  Connection->Socket = NullSocket;
  Connection->Connected = False;

  Assert(!IsConnected(Connection));

  return;
}

enum socket_op_result
{
  SocketOpResult_Noop,
  SocketOpResult_CompletedRW,
  SocketOpResult_Fail
};

socket_op_result
NetworkOp(network_connection *Connection, void *Message, u32 MessageSize, socket_op SocketOp)
{
  Assert(Connection);
  Assert(Message);

  socket_op_result OpResult = SocketOpResult_Fail;
  s64 SocketReturnValue = 0;

  // We may have disconnected on a previous attempt to read/write this frame
  if (IsConnected(Connection))
  {
    switch(SocketOp)
    {
      case SocketOp_Read:
      {
        u32 Flags = 0;
        SocketReturnValue = recv(Connection->Socket.Id, (void*)Message, MessageSize, Flags);
      } break;

      case SocketOp_Write:
      {
        u32 Flags = MSG_NOSIGNAL;
        SocketReturnValue = send(Connection->Socket.Id, (void*)Message, MessageSize , Flags);
      } break;

      InvalidDefaultCase;
    }

    const s32 SocketError = -1;
    const s32 SocketClosed = 0;
    switch(SocketReturnValue)
    {
      case SocketError:
      {
        switch (errno)
        {
          case EAGAIN:
          {
            // Non-blocking socket would block
            Assert(Connection->Socket.Type == Socket_NonBlocking);
            OpResult = SocketOpResult_Noop;
          } break;

          default:
          {
            Error("SocketOp failed : %s", strerror(errno));
          } break;
        }
      } break;

      case SocketClosed:
      {
        Info("Network peer closed connection gracefully");
      } break;

      default:
      {
        // s64 BytesRead = SocketReturnValue;
        OpResult = SocketOpResult_CompletedRW;
      } break;
    }

    if (OpResult == SocketOpResult_Fail) { Disconnect(Connection); }
  }

  return OpResult;
}

template <typename T> void
Send(network_connection *Connection, T *Message)
{
  NetworkOp(Connection, Message, sizeof(T), SocketOp_Write);
  return;
}

template <typename T> socket_op_result
Read(network_connection *Connection, T *Message)
{
  socket_op_result Result = NetworkOp(Connection, Message, sizeof(T), SocketOp_Read);
  return Result;
}
