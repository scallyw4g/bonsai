struct string_builder
{
  memory_arena* Memory = AllocateArena();
  counted_string_stream Chunks;
};

bonsai_function void
Append(string_builder* Builder, counted_string String)
{
  Push(&Builder->Chunks, String, Builder->Memory);
}


