#include <stdlib.h>

#include "passes.h"

static uint32_t convexHullRayCount(const uint32_t width, const uint32_t height, const uint32_t spacing)
{
	return (width + height) / (spacing >> 1);
}

convexHull convexHullCreate(const uint8_t *source, const uint32_t width, const uint32_t height, const convexHullVector pivot, const uint32_t spacing, const float precision)
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

void convexHullFree(convexHull convexHull)
{
	free(convexHull.nodes);
}