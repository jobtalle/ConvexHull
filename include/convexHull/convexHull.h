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

#include <stdint.h>

typedef struct {
	float x, y;
} convexHullVector;

typedef struct convexHull{
	uint32_t nodeCount;
	convexHullVector *nodes;
} convexHull;

// Create a convex hull from a RGBA raster source
convexHull convexHullCreate(
	const uint8_t *source,
	const uint32_t width,
	const uint32_t height,
	const convexHullVector pivot,
	const uint32_t spacing,
	const float precision);

// Free convex hull contents
void convexHullFree(convexHull convexHull);