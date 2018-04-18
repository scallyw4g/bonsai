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

global_variable socket_t NullSocket = {};
