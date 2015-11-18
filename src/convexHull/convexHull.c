#include <stdlib.h>

#include "passes.h"

static unsigned int convexHullRayCount(const unsigned int width, const unsigned int height, const unsigned int spacing)
{
	return (width + height) / (spacing >> 1);
}

convexHull convexHullCreate(const unsigned char *source, const unsigned int width, const unsigned int height, const ccVec2 pivot, const unsigned int spacing, int phase)
{
	unsigned int i;
	const unsigned int rays = convexHullRayCount(width, height, spacing);
	convexHull convexHull;

	convexHull.nodeCount = rays;
	convexHull.nodes = malloc(sizeof(ccVec2)* rays);

	// Crop image
	convexHullCrop(&convexHull, source, width, height, pivot);

	if(phase == 0) return convexHull;

	// Graham scan
	convexHullGrahamScan(&convexHull);

	if(phase == 1) return convexHull;

	// Trim insignificant nodes
	convexHullTrimInsignificantNodes(&convexHull, 64.0f);

	// Realloc trimmed graph
	convexHull.nodes = realloc(convexHull.nodes, sizeof(ccVec2)* convexHull.nodeCount);

	return convexHull;
}