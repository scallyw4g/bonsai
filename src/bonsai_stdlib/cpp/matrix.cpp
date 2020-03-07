

function m_nxn*
Allocate_NxN_Matrix(u32 Width, u32 Height, memory_arena* Memory)
{
  u32 N = Width*Height;

  m_nxn* Result = Allocate(m_nxn, Memory, 1);
  Result->Elements = Allocate(r32, Memory, N);
  return Result;
}

function m_nxn*
Allocate_Nx3_Matrix(u32 N, memory_arena* Memory)
{
  m_nxn* Result = Allocate_NxN_Matrix(N, 3, Memory);
  return Result;
}

function m_nxn*
Allocate_3xN_Matrix(u32 N, memory_arena* Memory)
{
  m_nxn* Result = Allocate_NxN_Matrix(3, N, Memory);
  return Result;
}

function m_nxn*
Transpose(m_nxn* Source, memory_arena* Memory)
{
  m_nxn* Result = Allocate_Nx3_Matrix(Source->End, Memory);

  for (u32 ResultRow = 0;
       ResultRow < Result->Rows;
       ++ResultRow)
  {
    for (u32 ResultColumn = 0;
         ResultColumn < Result->Columns;
         ++ResultColumn)
    {
      (*Result)[ResultColumn][ResultRow] = (*Source)[ResultRow][ResultColumn];
    }
  }

  return Result;
}

function m_nxn*
Multiply(m_nxn A, m_nxn B, memory_arena* Memory)
{
  u32 Width = Max(A.Columns, B.Columns);
  u32 Height = Max(A.Rows, B.Rows);

  m_nxn* Result = Allocate_NxN_Matrix(Width, Height, Memory);

  for(u32 Row = 0; Row < 4; ++Row)
  {
    for(u32 Column = 0; Column < 4; ++Column)
    {
      for(u32 ResultIndex = 0; ResultIndex < 4; ++ResultIndex)
      {
        (*Result)[Row][Column] += A[ResultIndex][Column] * B[Row][ResultIndex];
      }
    }
  }

  return Result;
}
