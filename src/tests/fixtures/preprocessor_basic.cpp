// single line comment's "work"
/* multi line comment's "work" */

function void
FunctionName()
{
  counted_string S = CS("thing\n");
  int foo = 3/3;
  return;
}

function void
WriteStructTo(d_union_decl* dUnion, native_file* OutFile, memory_arena* Memory)
{
  counted_string UnionName = dUnion->Name;
  counted_string Decl = FormatCountedString(Memory, "\n");
}
