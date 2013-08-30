//
//  diffgeom.h
//  nicoPBRT
//
//  Created by Lito Nicolai on 8/12/13.
//  Copyright (c) 2013 Lito Nicolai. All rights reserved.
//

#ifndef __nicoPBRT__diffgeom__
#define __nicoPBRT__diffgeom__

#include "geometry.h"

struct DifferentialGeometry {
    DifferentialGeometry () {
        u = v = 0.f;
        shape = NULL;
    }
    Point p;
    Normal nn; // 'normalized normal'
    float u, v;
    const Shape *shape;
    Vector dpdu, dpdv;
    Normal dndu, dndv;
    
    DifferentialGeometry::DifferentialGeometry(const Point &P, const Vector &DPDU, const Vector &DPDV, const Normal &DNDU, const Normal &DNDV, float uu, float vv, const Shape *sh)
    : p(P), dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV){
        nn = Normal(Normalize(Cross(dpdu, dpdv)));
        u= uu;
        v = vv;
        shape = sh;
        
        if (shape && (shape -> ReverseOrientation ^ shape->TransformSwapsHandedness)){
            nn *= -1.f;
        }
    }
};





#endif /* defined(__nicoPBRT__diffgeom__) */
