//
//  Spectrum.h
//  nicoPBRT
//
//  Created by Lito Nicolai on 8/20/13.
//  Copyright (c) 2013 Lito Nicolai. All rights reserved.
//

#ifndef __nicoPBRT__Spectrum__
#define __nicoPBRT__Spectrum__

#include "pbrt.h"

template <int nSamples> class CoefficientSpectrum { //a list of sample-values across a spectrum
public:
    CoefficientSpectrum(float v = 0.f) {
        for (int i = 0; i < nSamples; ++i){
            c[i] = v;
        }
    }
    
    CoefficientSpectrum &operator+=(const CoefficientSpectrum &s2) {
        for(int i = 0; i < nSamples; ++i){
            c[i] += s2.c[i];
        }
        return *this;
    }
    
    
    CoefficientSpectrum &operator-=(const CoefficientSpectrum &s2) {
        for(int i = 0; i < nSamples; ++i){
            c[i] -= s2.c[i];
        }
        return *this;
    }
    
    CoefficientSpectrum &operator*=(const CoefficientSpectrum &s2) {
        for(int i = 0; i < nSamples; ++i){
            c[i] *= s2.c[i];
        }
        return *this;
    }
    
    CoefficientSpectrum operator+(const CoefficientSpectrum &s2) const {
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i){
            ret.c[i] += s2.c[i];
        }
        return ret;
    }
    
    
    CoefficientSpectrum operator-(const CoefficientSpectrum &s2) const {
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i){
            ret.c[i] -= s2.c[i];
        }
        return ret;
    }
    
    
    CoefficientSpectrum operator*(const CoefficientSpectrum &s2) const {
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i){
            ret.c[i] *= s2.c[i];
        }
        return ret;
    }
    
    
    CoefficientSpectrum operator/(const CoefficientSpectrum &s2) const { //double-check
        CoefficientSpectrum ret = *this;
        for (int i = 0; i < nSamples; ++i){
            ret.c[i] *= 1.f/s2.c[i];
        }
        return ret;
    }
    
    //more operator methods
    
    bool IsBlack() const {
        for (int i = 0; i < nSamples; ++i){
            if (c[i] != 0.) return false;
        }
        return true;
    }
    
    friend CoefficientSpectrum Sqrt(const CoefficientSpectrum &s) {
        CoefficientSpectrum ret;
        for (int i = 0; i < nSamples; ++i) {
            ret.c[i] = sqrtf(s.c[i]);
        }
        return ret;
    }
    
    CoefficientSpectrum Clamp(float low = 0, float high = INFINITY) const {
        CoefficientSpectrum ret;
        for (int i = 0; i < nSamples; ++i){
            ret.c[i] = ::Clamp(c[i], low, high);
        }
        return ret;
    }
    bool HasNaNs() const {
        for (int i; i<nSamples; ++i){
            if (isnan(c[i])) {
                return true;
            }
            return false;
        }
    }
    
protected:
    float c[nSamples];
    
};

static const int sampledLambdaStart = 400;
static const int sampledLambdaEnd = 700;
static const int nSpectralSamples = 30;







inline Spectrum Lerp(float t, const Spectrum &s1, const Spectrum &s2) {
    return (1.f - t) * s1 + t * s2;


#endif /* defined(__nicoPBRT__Spectrum__) */
