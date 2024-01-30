// src/engine/editor.cpp:137:0

link_internal counted_string
ToString(file_traversal_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case FileTraversalType_None: { Result = CSz("None"); } break;
    case FileTraversalType_Dir: { Result = CSz("Dir"); } break;
    case FileTraversalType_File: { Result = CSz("File"); } break;

    
  }
  return Result;
}

link_internal file_traversal_type
FileTraversalType(counted_string S)
{
  file_traversal_type Result = {};

  if (StringsMatch(S, CSz("FileTraversalType_None"))) { return FileTraversalType_None; }
  if (StringsMatch(S, CSz("FileTraversalType_Dir"))) { return FileTraversalType_Dir; }
  if (StringsMatch(S, CSz("FileTraversalType_File"))) { return FileTraversalType_File; }

  return Result;
}


