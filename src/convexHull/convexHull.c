#include <stdlib.h>

#include "passes.h"

static unsigned int convexHullRayCount(const unsigned int width, const unsigned int height, const unsigned int spacing)
{
	return (width + height) / (spacing >> 1);
}

convexHull convexHullCreate(const unsigned char *source, const unsigned int width, const unsigned int height, const convexHullVector pivot, const unsigned int spacing, float precision)
{
	convexHull convexHull;

	convexHull.nodeCount = convexHullRayCount(width, height, spacing);
	convexHull.nodes = malloc(sizeof(convexHullVector)* convexHull.nodeCount);

	// Crop image
	convexHullCrop(&convexHull, source, width, height, pivot);

	// Trim insignificant nodes
	convexHullTrimInsignificantNodes(&convexHull, precision);

	// Graham scan
	convexHullGrahamScan(&convexHull);

	// Realloc trimmed graph
	convexHull.nodes = realloc(convexHull.nodes, sizeof(convexHullVector)* convexHull.nodeCount);

	return convexHull;
}