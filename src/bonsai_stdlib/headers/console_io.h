global_variable native_file Stdout =
{
  .Handle = stdout,
  .Path = CSz("stdout")
};

bonsai_function void
SetupStdout(u32 ArgCount, const char** ArgStrings)
{
  setvbuf(stdout, 0, _IONBF, 0);
  setvbuf(stderr, 0, _IONBF, 0);

  for ( u32 ArgIndex = 1;
        ArgIndex < ArgCount;
        ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);

    if (StringsMatch(CS("-c0"), Arg) ||
        StringsMatch(CS("--colors-off"), Arg) )
    {
      SetTerminalColorsOff();
    }

  }

  return;
}
