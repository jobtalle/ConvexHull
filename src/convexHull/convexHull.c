#include <stdlib.h>

#include <convexHull/convexHull.h>
#include <ccTrigonometry/ccTrigonometry.h>

static void convexHullIntSwap(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

convexHull convexHullCreate(unsigned char *source, unsigned int width, unsigned int height, unsigned int precision)
{
	unsigned int i;
	float stepSize = CC_TRI_PI_DOUBLE_F / precision;
	float r;
	convexHull convexHull;

	// Create initial nodes
	convexHull.nodeCount = precision;
	convexHull.nodes = malloc(sizeof(ccVec2)* precision);

	r = 0;
	for(i = 0; i < precision; i++) {
		r += stepSize;
	}

	return convexHull;
}