// external/bonsai_stdlib/src/simd_avx2.h:191:0

link_inline f32_8x
operator-(f32_8x A, f32_8x B)
{
  f32_8x Result = {{ _mm256_sub_ps(A.Sse, B.Sse) }};
  return Result;
}

link_inline f32_8x
operator-(f32_8x A, f32 B)
{
  f32_8x Result = {{ _mm256_sub_ps(A.Sse, F32_8X(B).Sse) }};
  return Result;
}

link_inline f32_8x
operator-(f32 A, f32_8x B)
{
  f32_8x Result = {{ _mm256_sub_ps(F32_8X(A).Sse, B.Sse) }};
  return Result;
}

link_inline f32_8x
operator-(f32_8x A, __m256 B)
{
  f32_8x Result = {{ _mm256_sub_ps(A.Sse, B) }};
  return Result;
}

link_inline f32_8x
operator-(__m256 A, f32_8x B)
{
  f32_8x Result = {{ _mm256_sub_ps(A, B.Sse) }};
  return Result;
}


