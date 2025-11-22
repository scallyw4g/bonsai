// callsite
// external/bonsai_stdlib/src/counted_string.cpp:40:0

// def (tuple)
// external/bonsai_stdlib/src/poof_functions.h:695:0
struct tuple_cs_cs
{
    cs E0;
  cs E1;

};

/* poof_bind( TupleType <- { tuple_(Types.map.sep(_) (T) {(T.name)} } )*/
/* gen_constructor(TupleType) */

link_internal tuple_cs_cs
Tuple(  cs E0, cs E1 )
{
  /* TupleType Result = */
  tuple_cs_cs Result =
  {
        .E0 = E0,
    .E1 = E1,

  };
  return Result;
}


