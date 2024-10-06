#include <bonsai_types.h>
#include <bonsai_stdlib/test/utils.h>

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Perlin Performance", ArgCount, Args);

  memory_arena BootstrapArena = {};
  engine_resources Engine = {};
  Global_EngineResources = &Engine;
  Ensure( InitializeBonsaiStdlib( BonsaiInit_Default,
                                  {},
                                  &Engine.Stdlib,
                                  &BootstrapArena ) );

  timeBeginPeriod(1);

  s32 PhysicalProcessorIndex = 0;

  s32 ThreadIndex = 0;
  SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *RelationshipBuffer = Allocate(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX, GetTranArena(), 64);
  unsigned long AllocatedSize = sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)*64;
  if (GetLogicalProcessorInformationEx(RelationProcessorCore, RelationshipBuffer, &AllocatedSize))
  {
    // Count physical processors
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX *Info = RelationshipBuffer;
    for (u32 Offset = 0; Offset < AllocatedSize; Offset += Info->Size)
    {
      Info = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*)((u8*)RelationshipBuffer + Offset);
      Assert(Info->Processor.GroupCount == 1);
      if (PhysicalProcessorIndex == ThreadIndex)
      {
        SetThreadAffinityMask(GetCurrentThread(), Info->Processor.GroupMask->Mask);
      }

      PhysicalProcessorIndex++;
    }

    Info = RelationshipBuffer;
    for (u32 Offset = 0; Offset < AllocatedSize; Offset += Info->Size)
    {
      Info = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*)((u8*)RelationshipBuffer + Offset);
    }

  }
  else
  {
    SoftError("GetLogicalProcessorInformationEx Failed");
  }

  memory_arena *Memory = AllocateArena(Megabytes(32));


  v3i Dim = V3i(32);
  r32 *NoiseValues = Allocate(r32, Memory, Volume(Dim));

  auto PrimeX = U32_8X(501125321);
  auto PrimeY = U32_8X(1136930381);
  auto PrimeZ = U32_8X(1720413743);


  f32_8x RunningInc = F32_8X(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f);

  perlin_params zParams;
  perlin_params yParams;
  perlin_params xParams[2];

  NotImplemented;
#if 0
  u64 CyclesElapsed = 0;
  u64 CellsGenerated = 0;
  RangeIterator(Index, 4096*32)
  {
    u64 CycleCountStart = GetCycleCount();

    u32 DestIndex = 0;
    for ( s32 z = 0; z < Dim.z; ++ z)
    {
      zParams = ComputePerlinParameters(F32_8X(z), PrimeZ);

      for ( s32 y = 0; y < Dim.y; ++ y)
      {
        yParams = ComputePerlinParameters(F32_8X(y), PrimeY);

        for ( s32 x = 0; x < Dim.x; x += 16 )
        {
          u32 ParamsIndex = 0;
          auto _x0 = F32_8X(x) + RunningInc;
          auto _x1 = F32_8X(x) + RunningInc + RunningInc;
          xParams[0] = ComputePerlinParameters(_x0, PrimeX);
          xParams[1] = ComputePerlinParameters(_x1, PrimeX);

          PerlinNoise_16x_avx2_x(xParams, &yParams, &zParams, NoiseValues+DestIndex, 1.f);
          DestIndex += 16;
        }
      }
    }

    u64 CycleCountEnd = GetCycleCount();
    u64 Elapsed = CycleCountEnd-CycleCountStart;

    /* SleepMs(1); */

    CyclesElapsed += Elapsed;
    CellsGenerated += u64(Volume(Dim));

    if (Index % 512 == 0)
    {
      r64 CyclesPerCell = r64(CyclesElapsed)/r64(CellsGenerated);
      printf("CyclesPerCell (%.2f)\n", CyclesPerCell);
    }
  }
#endif


  /* RangeIterator(Index, 4096) */
  /* { */
  /*   PerlinNoise_16x_avx2(); */
  /*   PerlinNoise_16x_avx2(perlin_params *perlinX, perlin_params *perlinY, perlin_params *perlinZ, f32 *Dest, f32 Amplitude) */
  /* } */

  TestSuiteEnd();
}
