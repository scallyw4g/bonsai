// single line comment's "work"
/* multi line comment's "work" */

function void
FunctionName()
{
  counted_string S = CS("thing\n");
  int foo = 3/3;
  return;
}

function void
WriteStructTo(d_union_decl* dUnion, native_file* OutFile, memory_arena* Memory)
{
  counted_string UnionName = dUnion->Name;
  counted_string Decl = FormatCountedString(Memory, "\n");
}

//
// Integral contant tests
//

42

42L
42LU
42LL
42LLU

42U
42UL
42ULL

42l
42lu
42ll
42llu

42u
42ul
42ull


//
// Float/Double tests
//

42.f
42.0f
42.42f
.42f

42.
42.0
42.42
.42


//
// Positive Exponent tests
//

42e1
42e2

42.e1
42.e2

42.0e1
42.0e2

.42e0
.42e1
.42e2

//
// Negative Exponent tests
//

42e-1
42e-2

42.e-1
42.e-2

42.0e-1
42.0e-2

.42e-1
.42e-2

//
// Positive Exponent tests
//

42E1
42E2

42.E1
42.E2

42.0E1
42.0E2

.42E0
.42E1
.42E2

//
// Negative Exponent tests
//

42E-1
42E-2

42.E-1
42.E-2

42.0E-1
42.0E-2

.42E-1
.42E-2
