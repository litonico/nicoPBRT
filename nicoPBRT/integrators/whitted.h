//
//  whitted.h
//  nicoPBRT
//
//  Created by Lito Nicolai on 8/13/13.
//  Copyright (c) 2013 Lito Nicolai. All rights reserved.
//


#ifndef __nicoPBRT__whitted__
#define __nicoPBRT__whitted__
#include "geometry.h"

class WhittedIntegrator : public SurfaceIntegrator { // this is super cool
public:
    Spectrum WhittedIntegrator::Li(const Scene *scene, const Renderer *renderer, const RayDifferential &ray, const Intersection &isect, const Sample *sample, RNG &rng, MemoryArena &arena) const {
        Spectrum L(0.); //L is a spectrum, initialized at 0
        //compute emitted light
        BSDF *bsdf = isect.GetBSDF(ray, arena); //evaluate BSDF at hit pt
        
        const Point &p = bsdf->dgShading.p;
        const Normal &n = bsdf->dgShading.nn;
        Vector wo = -ray.d;
        
        L += isect.Le(wo); //Compute emitted light if ray hit an area light source
        
            //add contribution of each source
        for (int i = 0; i < scene->lights.size(); i++){
            Vector wi; //incident direction
            float pdf; //probability density function (for monte carlo sim)
            VisibilityTester visibility;
            Spectrum Li = scene->lights[i]->Sample_L(p, isect.rayEpsilon, LightSample(rng), ray.time, &wi, &pdf, &visibility);
            
            if (Li.IsBlack() || pdf == 0.f){
                continue;
            }
            
            Spectrum f = bsdf->f(wo, wi)
            if (!f.IsBlack() && visibility.Unoccluded(scene)){
                L += f* Li * AbsDot(wi,n) * visibility.Transmittance(scene, renderer, sample, rng, arena)/pdf;
            }
        }
        
        if (ray.depth + 1 < maxDepth){
            L+= SpecularReflect(ray, bsdf, rng, iscect, renderer, scene, sample, arena);//trace more rays!
            L+= SpecularTransmit(ray, bsdf, rng, isect, renderer, scene, sample, arena);
        }
        return L;
    }
private:
    int maxDepth;
    
}





#endif /* defined(__nicoPBRT__whitted__) */
