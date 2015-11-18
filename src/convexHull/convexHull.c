#include <stdlib.h>
#include <math.h>

#include <convexHull/convexHull.h>
#include <ccTrigonometry/ccTrigonometry.h>

#include "passes.h"

convexHull convexHullCreate(const unsigned char *source, const unsigned int width, const unsigned int height, const ccVec2 pivot, const unsigned int precision, int phase)
{
	convexHull convexHull;
	convexHull.nodeCount = precision;
	convexHull.nodes = malloc(sizeof(ccVec2)* precision);

	// Crop image
	convexHullCrop(&convexHull, source, width, height, pivot);

	if(phase == 0) return convexHull;

	// Graham scan
	convexHullGrahamScan(&convexHull);

	return convexHull;
}