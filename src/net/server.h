#define REMOTE_PORT 1337

struct server_message
{
  u32 ClientId;
};

b32
Send(socket_t Socket, server_message *Message)
{
  b32 Result = True;
  if (Socket)
  {
    u32 Flags = MSG_NOSIGNAL;
    s64 BytesWritten = send(Socket , (void*)Message, sizeof(server_message) , Flags);

    if( BytesWritten < 0)
    {
      Error("Disconnected");
      Result = False;
    }

  }

  return Result;
}

