#define other_macro_2(a1, a2, a3) other_macro(a1, a2, a3)
#define other_macro(a1, a2, a3) a1 concat(a2, a3)
#define concat(x,y) x ## y

other_macro_2(foo, &, fda)
