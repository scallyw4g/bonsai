// src/engine/editor.h:595:0

link_internal counted_string
ToStringPrefixless(data_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case DataType_Undefinded: { Result = CSz("Undefinded"); } break;
    case DataType_v3: { Result = CSz("v3"); } break;
    case DataType_v3_u8: { Result = CSz("u8"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(data_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case DataType_Undefinded: { Result = CSz("DataType_Undefinded"); } break;
    case DataType_v3: { Result = CSz("DataType_v3"); } break;
    case DataType_v3_u8: { Result = CSz("DataType_v3_u8"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
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


