function void
Append(string_builder* Builder, counted_string String)
{
  Push(&Builder->Chunks, String, &Builder->Memory);
}

function counted_string
Finalize(string_builder* Builder, memory_arena* PermMemory)
{
  u32 TotalMemRequired = 0;

  ITERATE_OVER(counted_string, &Builder->Chunks)
  {
    counted_string* At = GET_ELEMENT(Iter);
    TotalMemRequired += At->Count;
  }

  counted_string Result = CountedString(TotalMemRequired, PermMemory);

  u32 AtIndex = 0;

  ITERATE_OVER(counted_string, &Builder->Chunks)
  {
    counted_string* At = GET_ELEMENT(Iter);
    MemCopy((u8*)At->Start, (u8*)(Result.Start+AtIndex), At->Count);
    AtIndex += At->Count;
    Assert(AtIndex <= Result.Count);
  }

  // @temp-string-builder-memory
  // TODO(Jesse): Jettison the temp memory used by this thing!

  Assert(AtIndex == Result.Count);
  return Result;
}

function counted_string
FormatCountedString_(memory_arena* Memory, const char* fmt...)
{
  string_builder Builder = {};

  va_list args;
  va_start(args, fmt);

#define TMP_BUFFER_SIZE (1024)
  char Temp[TMP_BUFFER_SIZE];
  while (*fmt != '\0')
  {
    if ( *fmt == '%' )
    {
      ++fmt;

      if (*fmt == 'd')
      {
        s32 Value = va_arg(args, s32);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%d", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CS(Temp);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'l')
      {
        ++fmt;
        if (*fmt == 'u')
        {
          u64 Value = va_arg(args, u64);
          s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%lu", Value);
          Assert(BytesWritten < TMP_BUFFER_SIZE);
          counted_string CSValue = CS(Temp);
          Append(&Builder, CSValue);
        }
        else if (*fmt == 'd')
        {
          s64 Value = va_arg(args, s64);
          s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%ld", Value);
          Assert(BytesWritten < TMP_BUFFER_SIZE);
          counted_string CSValue = CS(Temp);
          Append(&Builder, CSValue);
        }
      }
      else if (*fmt == 'x')
      {
        u64 Value = va_arg(args, u64);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%lu", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CS(Temp);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'u')
      {
        u32 Value = va_arg(args, u32);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%u", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CS(Temp);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'c')
      {
        s32 Value = va_arg(args, s32);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%d", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CS(Temp);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 's')
      {
        char* Value = va_arg(args, char*);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%s", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CS(Temp);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'f')
      {
        r64 Value = va_arg(args, r64);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%f", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CS(Temp);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'b')
      {
        b32 BoolVal = (b32)va_arg(args, int);
        counted_string Output = {};

        if (BoolVal)
          Output = CS("True");
        else
          Output = CS("False");

        Append(&Builder, Output);
      }
      else if (*fmt == 'S')
      {
        u32 Count = va_arg(args, u32);
        char* Start = va_arg(args, char*);

        counted_string Output = {
          .Start = Start,
          .Count = Count
        };

        Append(&Builder, Output);
      }
      else if (*fmt == '.')
      {
        Assert(*(++fmt) == '*')
        Assert(*(++fmt) == 's')
        u32 Count = va_arg(args, u32);
        char* Start = va_arg(args, char*);

        counted_string Output = {
          .Start = Start,
          .Count = Count
        };

        Append(&Builder, Output);
      }
      else
      {
        va_arg(args, void*);
        std::cout << "Invalid Format String";
      }

    }
    else
    {
      Append(&Builder, CS(fmt, 1));
    }

    ++fmt;
  }

  va_end(args);

  counted_string Result = Finalize(&Builder, Memory);
  return Result;
}
