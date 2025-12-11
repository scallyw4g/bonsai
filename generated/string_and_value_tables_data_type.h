// callsite
// src/engine/editor.h:1003:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(data_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case DataType_Undefinded:
    case DataType_v3:
    case DataType_v3_u8:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(data_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case DataType_Undefinded: { Result = CSz("Undefinded"); } break;
      case DataType_v3: { Result = CSz("v3"); } break;
      case DataType_v3_u8: { Result = CSz("u8"); } break;


      
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
ToString(data_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case DataType_Undefinded: { Result = CSz("DataType_Undefinded"); } break;
    case DataType_v3: { Result = CSz("DataType_v3"); } break;
    case DataType_v3_u8: { Result = CSz("DataType_v3_u8"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal data_type
DataType(counted_string S)
{
  data_type Result = {};

    if (StringsMatch(S, CSz("DataType_Undefinded"))) { return DataType_Undefinded; }
  if (StringsMatch(S, CSz("DataType_v3"))) { return DataType_v3; }
  if (StringsMatch(S, CSz("DataType_v3_u8"))) { return DataType_v3_u8; }


  return Result;
}


