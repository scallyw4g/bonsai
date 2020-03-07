
#include <bonsai_types.h>

#include <test_utils.cpp>

inline void
TestPacking()
{
  v4 Red   = V4(1,0,0,0);
  v4 Green = V4(0,1,0,0);
  v4 Blue  = V4(0,0,1,0);
  v4 Pink  = V4(1,0,1,0);

  u32 PackedRed   = PackRGBALinearTo255(Red);
  u32 PackedGreen = PackRGBALinearTo255(Green);
  u32 PackedBlue  = PackRGBALinearTo255(Blue);
  u32 PackedPink  = PackRGBALinearTo255(Pink);

  TestThat(PackedRed   == 0x000000FF);
  TestThat(PackedGreen == 0x0000FF00);
  TestThat(PackedBlue  == 0x00FF0000);
  TestThat(PackedPink  == 0x00FF00FF);


  v4 UnpackedRed = Red;
  v4 UnpackedGreen = Green;
  v4 UnpackedBlue = Blue;
  v4 UnpackedPink = Pink;

  TestThat(UnpackedRed == Red);
  TestThat(UnpackedGreen == Green);
  TestThat(UnpackedBlue == Blue);
  TestThat(UnpackedPink == Pink);

  return;
}

s32
main()
{
  TestSuiteBegin("Bitmap");

  memory_arena Arena = {};

  v4 Red   = V4(1,0,0,0);
  v4 Green = V4(0,1,0,0);
  v4 Blue  = V4(0,0,1,0);
  v4 Pink  = V4(1,0,1,0);

  TestPacking();

  u32 FirstBitmapPixelCount = 0;
  u32 SecondBitmapPixelCount = 0;
  {
    bitmap Bitmap = AllocateBitmap(V2i(8), &Arena);
    FillBitmap(0xFFFFFFFF, &Bitmap);

    {
      b32 Wrote = WriteBitmapToDisk(&Bitmap, "white.bmp");
      TestThat(Wrote);
    }

    FirstBitmapPixelCount = (u32)(Bitmap.Dim.x*Bitmap.Dim.y);

    u32 *Pixels = Bitmap.Pixels.Start;
    u32 *PixelsEnd = Bitmap.Pixels.End;
    u32 PackedRed   = PackRGBALinearTo255(Red);
    u32 PackedGreen = PackRGBALinearTo255(Green);
    u32 PackedPink  = PackRGBALinearTo255(Pink);
    u32 PackedBlue  = PackRGBALinearTo255(Blue);

    Pixels[0]     = PackedRed;
    Pixels[7]     = PackedGreen;
    PixelsEnd[-8] = PackedPink;
    PixelsEnd[-1] = PackedBlue;

    {
      b32 Wrote = WriteBitmapToDisk(&Bitmap, "test.bmp");
      TestThat(Wrote);
    }
  }

  {
    bitmap Bitmap = ReadBitmapFromDisk("test.bmp", &Arena);
    SecondBitmapPixelCount = (u32)(Bitmap.Dim.x*Bitmap.Dim.y);

    Assert(SecondBitmapPixelCount == FirstBitmapPixelCount);

    u32 *Pixels = Bitmap.Pixels.Start;
    u32 *PixelsEnd = Bitmap.Pixels.End;

    TestThat( Unpack255RGBAToLinear(Pixels[0])  == Red);
    TestThat( Unpack255RGBAToLinear(Pixels[7])  == Green);
    TestThat( Unpack255RGBAToLinear(PixelsEnd[-8]) == Pink);
    TestThat( Unpack255RGBAToLinear(Pixels[SecondBitmapPixelCount-1]) == Blue);
    TestThat( Unpack255RGBAToLinear(PixelsEnd[-1]) == Blue);
  }

  TestSuiteEnd();
  exit(TestsFailed);
}
