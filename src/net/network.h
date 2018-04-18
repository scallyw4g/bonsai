#define REMOTE_PORT 1337

struct server_to_client_message
{
  u64 Id;
  canonical_position P;
};

struct client_to_server_message
{
  u64 Id;
  canonical_position P;
};

enum socket_type
{
  Socket_Blocking,
  Socket_NonBlocking
};

struct socket_t
{
  s32 Id;
  socket_type Type;

  socket_t(socket_type Type)
  {
    Clear(this);
    this->Type = Type;
  }

  socket_t() {
    Clear(this);
  }
};

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

global_variable socket_t NullSocket = {};

enum socket_op_result
{
  SocketOpResult_Noop,
  SocketOpResult_CompletedRW,
  SocketOpResult_Fail
};

enum socket_op
{
  SocketOp_Null,
  SocketOp_Read,
  SocketOp_Write,
  SocketOp_Count
};

