//
//  BxDF.h
//  nicoPBRT
//
//  Created by Lito Nicolai on 8/14/13.
//  Copyright (c) 2013 Lito Nicolai. All rights reserved.
//

#ifndef __nicoPBRT__BxDF__
#define __nicoPBRT__BxDF__
#include "geometry.h"

enum BxDFType {
    BSDF_REFLECTION     = 1<<0,
    BSDF_TRANSMISSION   = 1<<1,
    BSDF_DIFFUSE        = 1<<2,
    BSDF_GLOSSY         = 1<<3,
    BSDF_SPECULAR       = 1<<4,
    BSDF_ALL_TYPES          = BSDF_GLOSSY | BSDF_DIFFUSE | BSDF_SPECULAR,
    BSDF_ALL_REFLECTIION    = BSDF_REFLECTION | BSDF_ALL_TYPES,
    BSDF_ALL_TRANSMISSION   = BSDF_TRANSMISSION | BSDF_ALL_TYPES,
    BSDF_ALL                = BSDF_ALL_REFLECTIION | BSDF_ALL_TRANSMISSION
};

class BxDF {
public:
    const BxDFType type;
    BxDF(BxDFType t) : type(t){}
    
    bool MatchesFlags(BxDFType flags) const {
        return (type & flags) == type;
    }
    
    virtual Spectrum f(const Vector &wo, const Vector &wi) const = 0;
    virtual Spectrum Sample_f(const Vector &wo, const Vector *wi, float u1, float u2, float *pdf) const = 0;
};





#endif /* defined(__nicoPBRT__BxDF__) */
