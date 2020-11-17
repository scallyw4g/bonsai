
#include <bonsai_types.h>

#include <tests/test_utils.cpp>

void
TestPopWord(ansi_stream *Stream, memory_arena *Memory)
{
  {
    char *Word = PopWord(Stream, Memory);
    TestThat(StringsMatch(Word, "word1"));
  }
  {
    char *Word = PopWord(Stream, Memory);
    TestThat(StringsMatch(Word, "word2"));
  }
  {
    char *Word = PopWord(Stream, Memory);
    TestThat(StringsMatch(Word, "word3"));
  }

  return;
}

void
TestPopU32(ansi_stream *Stream, memory_arena *Memory)
{
  {
    u32 N = PopU32(Stream, Memory);
    TestThat(N == 1);
  }
  {
    u32 N = PopU32(Stream, Memory);
    TestThat(N == 2);
  }
  {
    u32 N = PopU32(Stream, Memory);
    TestThat(N == 3);
  }

  return;
}

void
TestStreamCursor()
{
  memory_arena *Memory = AllocateArena(Megabytes(1));

  {
    const char *TestData = "word1 word2 word3";
    ansi_stream Stream = AnsiStream(TestData);
    TestPopWord(&Stream, Memory);
  }

  {
    const char *TestData = "word1\nword2\nword3";
    ansi_stream Stream = AnsiStream(TestData);
    TestPopWord(&Stream, Memory);
  }

  {
    const char *TestData = "word1    word2\nword3";
    ansi_stream Stream = AnsiStream(TestData);
    TestPopWord(&Stream, Memory);
  }

  {
    const char *TestData = "  word1    word2\nword3  ";
    ansi_stream Stream = AnsiStream(TestData);
    TestPopWord(&Stream, Memory);
  }


  {
    const char *TestData = "1 2 3";
    ansi_stream Stream = AnsiStream(TestData);
    TestPopU32(&Stream, Memory);
  }

  {
    const char *TestData = "1\n2\n3";
    ansi_stream Stream = AnsiStream(TestData);
    TestPopU32(&Stream, Memory);
  }

  {
    const char *TestData = "1    2\n3";
    ansi_stream Stream = AnsiStream(TestData);
    TestPopU32(&Stream, Memory);
  }

  {
    const char *TestData = "  1    2\n3  ";
    ansi_stream Stream = AnsiStream(TestData);
    TestPopU32(&Stream, Memory);
  }

  {
    const char *TestData = "1/2/3";
    ansi_stream Stream = AnsiStream(TestData);
    {
      u32 N = PopU32(&Stream, Memory, "/");
      TestThat(N == 1);
    }
    {
      u32 N = PopU32(&Stream, Memory, "/");
      TestThat(N == 2);
    }
    {
      u32 N = PopU32(&Stream, Memory);
      TestThat(N == 3);
    }
  }

  {
    const char *TestData = "1//3//";
    ansi_stream Stream = AnsiStream(TestData);
    {
      u32 N = PopU32(&Stream, Memory, "/");
      TestThat(N == 1);
    }
    {
      u32 N = PopU32(&Stream, Memory);
      TestThat(N == 3);
    }
  }

  {
    const char *TestData = "word1 \"word2\" word3";
    ansi_stream Stream = AnsiStream(TestData);
    char *W1 = PopWord(&Stream, Memory);
    TestThat(StringsMatch(W1, "word1"));

    counted_string W2 = PopQuotedString(&Stream);
    TestThat(StringsMatch(W2, CS("word2")));

    char *W3 = PopWord(&Stream, Memory);
    TestThat(StringsMatch(W3, "word3"));
  }

  {
    const char *TestData = "word1 \"word2\" word3";
    ansi_stream Stream = AnsiStream(TestData);
    char *W1 = PopWord(&Stream, Memory);
    TestThat(StringsMatch(W1, "word1"));

    counted_string W2 = PopQuotedString(&Stream, True);
    TestThat(StringsMatch(W2, CS("\"word2\"")));

    char *W3 = PopWord(&Stream, Memory);
    TestThat(StringsMatch(W3, "word3"));
  }



  {
    const char *TestData = "word1 'w' word3";
    ansi_stream Stream = AnsiStream(TestData);
    char *W1 = PopWord(&Stream, Memory);
    TestThat(StringsMatch(W1, "word1"));

    counted_string W2 = PopQuotedCharLiteral(&Stream);
    TestThat(StringsMatch(W2, CS("w")));

    char *W3 = PopWord(&Stream, Memory);
    TestThat(StringsMatch(W3, "word3"));
  }

  {
    const char *TestData = "word1 'w' word3";
    ansi_stream Stream = AnsiStream(TestData);
    char *W1 = PopWord(&Stream, Memory);
    TestThat(StringsMatch(W1, "word1"));

    counted_string W2 = PopQuotedCharLiteral(&Stream, True);
    TestThat(StringsMatch(W2, CS("'w'")));

    char *W3 = PopWord(&Stream, Memory);
    TestThat(StringsMatch(W3, "word3"));
  }


  return;
}

void
TestContainsForCountedString()
{
  TestThat( Contains(CSz("abc"), CSz("abc")) );
  TestThat( Contains(CSz("abcd"), CSz("abc")) );
  TestThat( Contains(CSz("dabc"), CSz("abc")) );
  TestThat( Contains(CSz("dabcd"), CSz("abc")) );

  TestThat(!Contains(CSz("bc"), CSz("abc")) );
  TestThat(!Contains(CSz("bacd"), CSz("abc")) );
  TestThat(!Contains(CSz("dacb"), CSz("abc")) );
  TestThat(!Contains(CSz("adbcd"), CSz("abc")) );

  return;
}

void
TestContainsForConstCharStar()
{
  TestThat( Contains("abc", "abc") );
  TestThat( Contains("abcd", "abc") );
  TestThat( Contains("dabc", "abc") );
  TestThat( Contains("dabcd", "abc") );

  TestThat(!Contains("bc", "abc") );
  TestThat(!Contains("bacd", "abc") );
  TestThat(!Contains("dacb", "abc") );
  TestThat(!Contains("adbcd", "abc") );

  return;
}

void
TestReadUntilTerminatorList()
{
  {
    const char *Text = "property='> some thing >'";

    {
      ansi_stream Stream = AnsiStream(Text);
      counted_string Result = ReadUntilTerminatorList(&Stream, "'");
      TestThat(Result.Start[Result.Count] == '\'');
      TestThat(Stream.At[0] == '>');
    }

    {
      ansi_stream Stream = AnsiStream(Text);
      counted_string Result = ReadUntilTerminatorList(&Stream, "=> \"'");
      TestThat(Result.Start[Result.Count] == '=');
      TestThat(Stream.At[0] == '\'');
    }

    {
      ansi_stream Stream = AnsiStream(Text);
      counted_string Result = ReadUntilTerminatorList(&Stream, "pr");
      TestThat(Result.Count == 0);
      TestThat(Result.Start[Result.Count] == 'p');
      TestThat(Stream.At[0] == 'r');
    }
  }

  {
    const char* Text = "\\'thing'";
    ansi_stream Stream = AnsiStream(Text);
    counted_string Result = ReadUntilTerminatorList(&Stream, "'", true);
    TestThat(StringsMatch(Result, CS("\\'thing")));
    TestThat(Stream.At[0] == 0);
  }

  {
    const char* Text = "\"thing\"x";
    ansi_stream Stream = AnsiStream(Text);
    counted_string Result = ReadUntilTerminatorList(&Stream, "x", true);
    TestThat(StringsMatch(Result, CS("\"thing\"")));
    TestThat(Stream.At[0] == 0);
  }

  {
    const char* Text = "struct %.*s {  \\x%.*s_type Type;  union  { x";
    ansi_stream Stream = AnsiStream(Text);
    counted_string Result = ReadUntilTerminatorList(&Stream, "x", true);
    TestThat(StringsMatch(Result, CS("struct %.*s {  \\x%.*s_type Type;  union  { ")));
    TestThat(Stream.At[0] == 0);
  }

  {
    const char* Text = "thing \n thang\nx";
    ansi_stream Stream = AnsiStream(Text);
    counted_string Result = ReadUntilTerminatorList(&Stream, "x", true);
    TestThat(StringsMatch(Result, CS("thing \n thang\n")));
    TestThat(Stream.At[0] == 0);
  }

  {
    const char* Text = "\\nx";
    ansi_stream Stream = AnsiStream(Text);
    counted_string Result = ReadUntilTerminatorList(&Stream, "x", true);
    TestThat(StringsMatch(Result, CS("\\n")));
    TestThat(Stream.At[0] == 0);
  }

  {
    const char* Text = "\\xx";
    ansi_stream Stream = AnsiStream(Text);
    counted_string Result = ReadUntilTerminatorList(&Stream, "x", true);
    TestThat(StringsMatch(Result, CS("\\x")));
    TestThat(Stream.At[0] == 0);
  }





  return;
}

void
TestSplit()
{
  counted_string Thing = CS("thing#stuff");
  counted_string Stuff = Split(&Thing, '#');

  counted_string ExpectedThing = CS("thing");
  counted_string ExpectedStuff = CS("stuff");

  TestThat(StringsMatch(&Thing, &ExpectedThing));
  TestThat(StringsMatch(&Stuff, &ExpectedStuff));

  return;
}

function void
TestTrim()
{

  {
    counted_string S1 = CS("  \n                  \n        \n  \ncode\n                    \n ");
    counted_string S2 = CS("\n    \n  \ncode\n   \n\n  \n ");
    S1 = Trim(S1);
    S2 = Trim(S2);
    TestThat(StringsMatch(S1, S2));
    TestThat(StringsMatch(S1, CS("code")));
    TestThat(StringsMatch(S2, CS("code")));
  }

  {
    counted_string S1 = CS("  \n                  \n        \n  \n \n                    \n ");
    counted_string S2 = CS("\n    \n  \n \n   \n\n  \n ");
    S1 = Trim(S1);
    S2 = Trim(S2);
    TestThat(StringsMatch(S1, S2));
    TestThat(StringsMatch(S1, CS("")));
    TestThat(StringsMatch(S2, CS("")));
  }
}

function void
TestToCapitalCase(memory_arena* Memory)
{
  {
    counted_string S = CS("this_is_a_string");
    counted_string Result = ToCapitalCase(S, Memory);
    TestThat(StringsMatch(CS("ThisIsAString"), Result));
  }
  {
    counted_string S = CS("this_is_");
    counted_string Result = ToCapitalCase(S, Memory);
    TestThat(StringsMatch(CS("ThisIs"), Result));
  }
  {
    counted_string S = CS("__this__is____");
    counted_string Result = ToCapitalCase(S, Memory);
    TestThat(StringsMatch(CS("ThisIs"), Result));
  }
  {
    counted_string S = CS("__This__is____");
    counted_string Result = ToCapitalCase(S, Memory);
    TestThat(StringsMatch(CS("ThisIs"), Result));
  }
  {
    counted_string S = CS("__THis__is____");
    counted_string Result = ToCapitalCase(S, Memory);
    TestThat(StringsMatch(CS("THisIs"), Result));
  }
  {
    counted_string S = CS("__THis__iS____");
    counted_string Result = ToCapitalCase(S, Memory);
    TestThat(StringsMatch(CS("THisIS"), Result));
  }
}

s32
main()
{
  TestSuiteBegin("String");

  {
    const char *Test1 = "v";
    const char *Test2 = "v";
    TestThat(StringsMatch(Test1, Test2));
  }

  {
    const char *Test1 = "vv";
    const char *Test2 = "vv";
    TestThat(StringsMatch(Test1, Test2));
  }

  {
    const char *Test1 = "v v";
    const char *Test2 = "v v";
    TestThat(StringsMatch(Test1, Test2));
  }

  {
    const char *Test1 = "vv";
    const char *Test2 = "v";
    TestThat(!StringsMatch(Test1, Test2));
  }

  {
    const char *Test1 = "v";
    const char *Test2 = "vv";
    TestThat(!StringsMatch(Test1, Test2));
  }

  TestStreamCursor();

  TestContainsForConstCharStar();
  TestContainsForCountedString();

  TestReadUntilTerminatorList();

  TestSplit();

  TestTrim();

  TestToCapitalCase(TranArena);

  {
    counted_string Path = CS("thing/ding/dong");
    counted_string Dong = Basename(Path);
    TestThat(StringsMatch(Dong, CS("/dong")));
  }

  counted_string Thing = CS("thing");
  counted_string Thang = CS("thang");

  counted_string ThingThang = Concat(Thing, Thang, TranArena);
  TestThat(StringsMatch(ThingThang, CS("thingthang")));







  memory_arena _Memory = {};
  memory_arena* Memory = &_Memory;

  {
    counted_string TestValue = FormatCountedString(Memory, CSz("%d"), 42);
    TestThat(StringsMatch(TestValue, CS("42")));
  }

  {
    counted_string TestValue = FormatCountedString(Memory, CSz("%u"), 42);
    TestThat(StringsMatch(TestValue, CS("42")));
  }

  {
    counted_string TestValue = FormatCountedString(Memory, CSz("%lu"), 42l);
    TestThat(StringsMatch(TestValue, CS("42")));
  }

  {
    counted_string TestValue = FormatCountedString(Memory, CSz("%ld"), 42lu);
    TestThat(StringsMatch(TestValue, CS("42")));
  }

  {
    counted_string TestValue = FormatCountedString(Memory, CSz("%.2f %.2f"), 42.42424242424242f, 42.42424242424242f);
    TestThat(StringsMatch(TestValue, CS("42.42 42.42")));
  }

  {
    counted_string TestValue = FormatCountedString(Memory, CSz("%.2f"), 42.42424242424242f);
    TestThat(StringsMatch(TestValue, CS("42.42")));
  }

  {
    counted_string TestValue = FormatCountedString(Memory, CSz("%.*s"), CS("thing"));
    TestThat(StringsMatch(TestValue, CS("thing")));
  }

  {
    counted_string TestValue = FormatCountedString(Memory, CSz("%.*s"), CS("thing"));
    TestThat(StringsMatch(TestValue, CS("thing")));
  }

  {
    counted_string TestValue = FormatCountedString(Memory, CSz("this %.*s this"), CS("thing"));
    TestThat(StringsMatch(TestValue, CS("this thing this")));
  }

  {
    counted_string TestValue = FormatCountedString(Memory, CSz("this %d this"), 42);
    TestThat(StringsMatch(TestValue, CS("this 42 this")));
  }

  {
    counted_string TestString = CS("thing");
    counted_string TestValue = FormatCountedString(Memory, CSz("this %.*s this"), TestString.Count, TestString.Start);
    TestThat(StringsMatch(TestValue, CS("this thing this")));
  }

  {
    counted_string TestString = CS("thing");
    counted_string TestValue = FormatCountedString(Memory, CSz("this %S this"), TestString);
    TestThat(StringsMatch(TestValue, CS("this thing this")));
  }

  {
    counted_string TestString = CS("thing");
    counted_string TestValue = FormatCountedString(Memory, CSz("this %.*s %.*s this"), TestString.Count, TestString.Start, TestString.Count, TestString.Start);
    TestThat(StringsMatch(TestValue, CS("this thing thing this")));
  }

  {
    counted_string TestString = CS("thing");
    counted_string TestValue = FormatCountedString(Memory, CSz("this %.*s this"), 3, TestString.Start);
    TestThat(StringsMatch(TestValue, CS("this thi this")));
  }

  {
    counted_string TestValue = FormatCountedString(TranArena, CSz("%4S"), CSz("HI"));
    TestThat(StringsMatch(TestValue, CS("  HI")));
  }
  {
    counted_string TestValue = FormatCountedString(TranArena, CSz("%*S"), 4, CSz("HI"));
    TestThat(StringsMatch(TestValue, CS("  HI")));
  }
  {
    counted_string TestValue = FormatCountedString(TranArena, CSz("%*s"), 4, "HI");
    TestThat(StringsMatch(TestValue, CS("  HI")));
  }
  {
    counted_string TestValue = FormatCountedString(TranArena, CSz("%4s"), "HI");
    TestThat(StringsMatch(TestValue, CS("  HI")));
  }
  {
    counted_string TestValue = FormatCountedString(TranArena, CSz("%14S"), CSz("HI"));
    TestThat(StringsMatch(TestValue, CS("            HI")));
  }
  {
    counted_string TestValue = FormatCountedString(TranArena, CSz("%*S"), 14, CSz("HI"));
    TestThat(StringsMatch(TestValue, CS("            HI")));
  }
  {
    counted_string TestValue = FormatCountedString(TranArena, CSz("%*s"), 14, "HI");
    TestThat(StringsMatch(TestValue, CS("            HI")));
  }
  {
    counted_string TestValue = FormatCountedString(TranArena, CSz("%14s"), "HI");
    TestThat(StringsMatch(TestValue, CS("            HI")));
  }
  {
    counted_string TestValue = FormatCountedString(TranArena, CSz("%14c"), '4');
    TestThat(StringsMatch(TestValue, CS("             4")));
  }
  {
    counted_string TestValue = FormatCountedString(TranArena, CSz("%*c"), 14, '4');
    TestThat(StringsMatch(TestValue, CS("             4")));
  }

  {
    counted_string TestString = CS("thing");
    counted_string TestValue = FormatCountedString(Memory, CSz("this %S %S this"), TestString, TestString);
    TestThat(StringsMatch(TestValue, CS("this thing thing this")));
  }



#define TEMP_BUFFER_SIZE (64)
  char TempBuffer[TEMP_BUFFER_SIZE];
  char_cursor TempCursor_ = {
    .Start = TempBuffer,
    .At = TempBuffer,
    .End = TempBuffer + TEMP_BUFFER_SIZE
  };

  char_cursor* TempCursor = &TempCursor_;

  {
    MemSet((u8*)TempBuffer, 0, TEMP_BUFFER_SIZE);
    Rewind(TempCursor);
    u64ToChar(TempCursor, 42);
    TestThat(StringsMatch(CS(TempBuffer), CS("42")));
  }
  {
    MemSet((u8*)TempBuffer, 0, TEMP_BUFFER_SIZE);
    Rewind(TempCursor);
    u64ToChar(TempCursor, 18446744073709551615ULL);
    TestThat(StringsMatch(CS(TempBuffer), CS("18446744073709551615")));
  }
  {
    MemSet((u8*)TempBuffer, 0, TEMP_BUFFER_SIZE);
    Rewind(TempCursor);
    u64ToChar(TempCursor, 18446744073709551615ULL);
    TestThat(StringsMatch(CS(TempBuffer), CS("18446744073709551615")));
  }



  {
    MemSet((u8*)TempBuffer, 0, TEMP_BUFFER_SIZE);
    Rewind(TempCursor);
    f64ToChar(TempCursor, 3.14, 2);
    TestThat(StringsMatch(CS(TempBuffer), CS("3.14")));
  }
  {
    MemSet((u8*)TempBuffer, 0, TEMP_BUFFER_SIZE);
    Rewind(TempCursor);
    f64ToChar(TempCursor, 3.14, 10);
    TestThat(StringsMatch(CS(TempBuffer), CS("3.1400000000")));
  }

  {
    TestThat(IsNumeric('0'));
    TestThat(IsNumeric('1'));
    TestThat(IsNumeric('2'));
    TestThat(IsNumeric('3'));
    TestThat(IsNumeric('4'));
    TestThat(IsNumeric('5'));
    TestThat(IsNumeric('6'));
    TestThat(IsNumeric('7'));
    TestThat(IsNumeric('8'));
    TestThat(IsNumeric('9'));

    TestThat(!IsNumeric('/'));
    TestThat(!IsNumeric(':'));
    TestThat(!IsNumeric((char)0));
  }

  {
    TestThat(ToU32('0') == 0);
    TestThat(ToU32('1') == 1);
    TestThat(ToU32('2') == 2);
    TestThat(ToU32('3') == 3);
    TestThat(ToU32('4') == 4);
    TestThat(ToU32('5') == 5);
    TestThat(ToU32('6') == 6);
    TestThat(ToU32('7') == 7);
    TestThat(ToU32('8') == 8);
    TestThat(ToU32('9') == 9);
  }

  {
    TestThat(Exp(10ULL, 0) == 1);
    TestThat(Exp(10ULL, 1) == 10);
    TestThat(Exp(10ULL, 2) == 100);

    TestThat(Exp(2ull, 2) == 4);
    TestThat(Exp(2ull, 3) == 8);
  }

  {
    TestThat(ToU32(CSz("42")) == 42);
    TestThat(ToU32(CSz("420")) == 420);
    TestThat(ToU32(CSz("4200")) == 4200);
    TestThat(ToU32(CSz("0200")) == 200);

    TestThat(ToU32(CSz("0")) == 0);
    TestThat(ToU32(CSz("0")) == 0);
  }





  TestSuiteEnd();
  exit(TestsFailed);
}
