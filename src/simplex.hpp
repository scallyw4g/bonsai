/*
===============================================================================
A C++ port of a speed-improved simplex noise algorithm for 2D in Java.
Based on example code by Stefan Gustavson (stegu@itn.liu.se).
Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
Better rank ordering method by Stefan Gustavson in 2012.
C++ port and minor type and algorithm changes by Josh Koch (jdk1337@gmail.com).
This could be speeded up even further, but it's useful as it is.
Version 2012-04-12
The original Java code was placed in the public domain by its original author,
Stefan Gustavson. You may use it as you see fit,
but attribution is appreciated.
===============================================================================
*/

#ifndef __SIMPLEX_H__
#define __SIMPLEX_H__

#include <cstdint>

class Grad {
public:
    Grad( int8_t x, int8_t y, int8_t z ) : x(x), y(y), z(z) {}
    int8_t x, y, z;
};

class Simplex {
public:
    // Initialize permutation arrays
    static void init();
    // 2D simplex noise
    static double noise( double xin, double yin );

private:
    static int32_t fastFloor( double x );
    static double dot( const Grad& g, double x, double y );
    static const double F2;
    static const double G2;
    static const Grad grad3[12];
    static const uint8_t p[256];
    static uint8_t perm[512];
    static uint8_t permMod12[512];
};

#endif
