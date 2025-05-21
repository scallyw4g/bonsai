// src/engine/editor.h:623:0

link_internal counted_string
ToStringPrefixless(maybe_tag Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case Maybe_No: { Result = CSz("No"); } break;
    case Maybe_Yes: { Result = CSz("Yes"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(maybe_tag Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case Maybe_No: { Result = CSz("Maybe_No"); } break;
    case Maybe_Yes: { Result = CSz("Maybe_Yes"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal maybe_tag
MaybeTag(counted_string S)
{
  maybe_tag Result = {};

    if (StringsMatch(S, CSz("Maybe_No"))) { return Maybe_No; }
  if (StringsMatch(S, CSz("Maybe_Yes"))) { return Maybe_Yes; }

  return Result;
}


