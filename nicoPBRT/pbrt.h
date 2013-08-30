//
//  pbrt.h
//  nicoPBRT
//
//  Created by Lito Nicolai on 7/25/13.
//  Copyright (c) 2013 Lito Nicolai. All rights reserved.
//

#ifndef nicoPBRT_pbrt_h
#define nicoPBRT_pbrt_h
#include <string>
#include <math.h>

typedef RGBSpectrum Spectrum; //choose spectrum type
//typedef SampledSpectrum Spectrum;

// global inline functions
inline float Lerp(float t, float v1, float v2) { // Linear Interpolation between pts v1 and v2
    return (1.f - t)*v1 + t*v2; 
}

inline float Clamp(float val, float low, float high) {
    if (val < low) return low;
    else if (val > high ) return high;
    else return val;
}

inline int Mod(int a, int divisor) { //works with negatives
    int n = int(a/divisor);
    a -= n*divisor;
    if (a < 0) a += divisor;
    return a;
}
inline float Radians (float deg) {
    return (M_PI/180.f) * deg;
}
inline float Degrees (float rad) {
    return (180.f/M_PI) * rad;
}
inline float Log2(float x) {
    static float invLog2 = 1.f/logf(2.f);
    return logf(x) *invLog2;
}
inline bool IsPowerOf2 (int v) { // Magic
    return (v & (v-1)) == 0;
}
#ifdef NDEBUG
#define Assert(expr) ((void)0) 
#else
#define Assert(expr) \
    ((expr) ? (void)0 : \
        Severe("Assertion \"%s\" failed in %s, line %d", \
            #expr, __FILE__, __LINE__))
#endif


//Constants
#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.14159265358979323846f
#define INV_PI 0.31830988618379067154f
#define INV_TWOPI  0.15915494309189533577f
#define INV_FOURPI 0.07957747154594766788f

#ifndef INFINITY
#define INFINITY FLT_MAX // is this a good idea?
#endif

#endif
