//
//  Scene.h
//  nicoPBRT
//
//  Created by Lito Nicolai on 7/24/13.
//  Copyright (c) 2013 Lito Nicolai. All rights reserved.
//

#ifndef __nicoPBRT__Scene__
#define __nicoPBRT__Scene__

#include <iostream>

#endif /* defined(__nicoPBRT__Scene__) */

class Scene {
    
public:
    //methods
    
    //data
    Primitive *aggregate;
    vector<Light *> lights;
    VolumeRegion *volumeRegion;
    
};