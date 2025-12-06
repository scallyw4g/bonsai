// callsite
// src/engine/editor.cpp:333:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(file_traversal_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case FileTraversalType_None:
    case FileTraversalType_Dir:
    case FileTraversalType_File:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(file_traversal_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case FileTraversalType_None: { Result = CSz("None"); } break;
      case FileTraversalType_Dir: { Result = CSz("Dir"); } break;
      case FileTraversalType_File: { Result = CSz("File"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(file_traversal_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case FileTraversalType_None: { Result = CSz("FileTraversalType_None"); } break;
    case FileTraversalType_Dir: { Result = CSz("FileTraversalType_Dir"); } break;
    case FileTraversalType_File: { Result = CSz("FileTraversalType_File"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
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


