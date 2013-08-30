//
//  main.cpp
//  nicoPBRT
//
//  Created by Lito Nicolai on 7/22/13.
//  Copyright (c) 2013 Lito Nicolai. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <geometry.h>
#include <diffgeom.h>


int main(int argc, const char * argv[])
{
    Options options;
    vector<string> filenames;
    //process commandline
    pbrtInit(options);
    if (filenames.size() == 0){ // process scene description
        //parse scene from standard;
        else {
            //parse scene from input;
        }
    }
    pbrtCleanup();
    return 0;
}

