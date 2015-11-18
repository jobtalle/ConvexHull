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
convexHull convexHullCreate(const unsigned char *source, const unsigned int width, const unsigned int height, const ccVec2 pivot, const unsigned int spacing, int phase);