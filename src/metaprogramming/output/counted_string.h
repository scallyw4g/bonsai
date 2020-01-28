struct counted_string_stream_chunk
{
  counted_string Element;
  counted_string_stream_chunk* Next;
  counted_string_stream_chunk* Prev;
};

struct counted_string_stream
{
  counted_string_stream_chunk Sentinel;
};

struct counted_string_iterator
{
  counted_string_stream* Stream;
  counted_string_stream_chunk* At;
};


