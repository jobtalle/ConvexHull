//__________________________________________________________________________________//
//                                                                                  //
//                                Convex Hull V1.0                                  //
//                           Written by Job Talle (2015)                            //
//__________________________________________________________________________________//
//                                                                                  //
//      This program is free software: you can redistribute it and/or modify        //
//      it under the terms of the 3-clause BSD license.                             //
//                                                                                  //
//      You should have received a copy of the 3-clause BSD License along with      //
//      this program. If not, see <http://opensource.org/licenses/>.                //
//__________________________________________________________________________________//

#pragma once

#include <ccVector/ccVector.h>

typedef struct convexHull{
	unsigned int nodeCount;
	ccVec2 *nodes;
} convexHull;

// Create a convex hull from a RGBA raster source
convexHull convexHullCreate(unsigned char *source, unsigned int width, unsigned int height, unsigned int offsetx, unsigned int offsety, unsigned int precision);