
struct string_builder
{
  memory_arena* Memory = AllocateArena();
  counted_string_stream Chunks;
};

