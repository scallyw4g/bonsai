#! /bin/python

# Generates arrays of pre-calculated cosine & arccosine values as literals for C.

from math import acos, cos, pi

def gen_cos_table(f, PRECISION, NAME, math_func):
    f.write("global_variable f32 %s[] = {\n" % NAME)
    j = 0
    p = 0.0
    while True:
        f.write("{:.10f}f, ".format(math_func(p)))
        j += 1
        if j % 16 == 0:
            f.write("\n")
        p += PRECISION
        if p > 2*pi:
            break
    f.write("1.f, 1.f };// NOTE(Jesse): Add a duplicate for the max value so we can blindly add 1 to the lookup index and not overflow (to lerp)\n")

def gen_acos_table(f, PRECISION, NAME, math_func):
    f.write("global_variable f32 %s[] = {\n" % NAME)
    j = 0
    p = -1.0
    while True:
        f.write("{:.10f}f, ".format(math_func(p)))
        j += 1
        if j % 16 == 0:
            f.write("\n")
        p += PRECISION
        if p > 1.0:
            break
    f.write("0.f, 0.f }; // NOTE(Jesse): Add a duplicate for the max value so we can blindly add 1 to the lookup index and not overflow (to lerp)\n")


if __name__ == '__main__':
    gen_cos_table(open("external/bonsai_stdlib/src/costable.h", "w"), 0.0001, "CosineTable", cos)
    gen_acos_table(open("external/bonsai_stdlib/src/arccostable.h", "w"), 0.0001, "ArcCosineTable", acos)
