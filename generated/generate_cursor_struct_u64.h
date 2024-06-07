// external/bonsai_stdlib/src/binary_parser.h:1:0

struct u64_cursor
{
  u64 *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  u64 *At;
  u64 *End;
  /* OWNED_BY_THREAD_MEMBER(); */
};


