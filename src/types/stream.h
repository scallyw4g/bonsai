struct u32_stream
{
  u32* Start;
  u32* At;
  u32* End;
};

struct u8_stream
{
  u8* Start;
  u8* At;
  u8* End;
};

struct ansi_stream
{
  const char* Start;
  const char* At;
  const char* End;
};

struct r32_stream
{
  r32* Start;
  r32* At;
  r32* End;
};

struct v3_stream
{
  v3* Start;
  v3* At;
  v3* End;
};
