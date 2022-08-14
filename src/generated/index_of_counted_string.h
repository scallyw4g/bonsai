    bonsai_function umm
    IndexOf(counted_string_buffer *Buf, counted_string Element)
    {
      umm Result = Buf->Count;

      for (u32 ElementIndex = 0;
          ElementIndex < Buf->Count;
          ++ElementIndex)
      {
        if (AreEqual(Buf->Start[ElementIndex], Element))
        {
          Result = ElementIndex;
          break;
        }
      }

      return Result;
    }

