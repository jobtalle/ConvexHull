#include <stdlib.h>
#include <math.h>

#include <convexHull/convexHull.h>
#include <ccTrigonometry/ccTrigonometry.h>

static void convexHullIntSwap(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}

convexHull convexHullCreate(unsigned char *source, unsigned int width, unsigned int height, unsigned int offsetx, unsigned int offsety, unsigned int precision)
{
	unsigned int i;
	unsigned int *colors = (unsigned int*)source;
	float r;
	float rStep = CC_TRI_PI_DOUBLE_F / precision;
	ccVec2 halfDim = (ccVec2){ (float)(width >> 1), (float)(height >> 1) };
	convexHull convexHull;

	// Create nodes
	convexHull.nodeCount = precision;
	convexHull.nodes = malloc(sizeof(ccVec2)* precision);

	r = 0;
	for(i = 0; i < precision; ++i) {
		ccVec2 *node = convexHull.nodes + i;
		ccVec2 direction = (ccVec2){ -cosf(r), -sinf(r) };
		float abscos;
		float abssin;
		float radius;

		// Create edge points
		node->x = -direction.x;
		node->y = -direction.y;
		abscos = (float)fabs(node->x);
		abssin = (float)fabs(node->y);

		if(halfDim.x * abssin <= halfDim.y * abscos) {
			radius = halfDim.x / abscos;
		}
		else {
			radius = halfDim.y / abssin;
		}

		--radius;
		node->x *= radius;
		node->y *= radius;
		
		// Crop until opaque pixel is found
		for(;;) {
			int x = (int)(node->x + halfDim.x);
			int y = (int)(node->y + halfDim.y);

			// Check alpha
			if(colors[x + y * width] & 0xFF000000) {
				break;
			}

			// Next
			*node = ccVec2Add(*node, direction);
		}

		r += rStep;
	}

	// Sweep concave nodes
	for(i = 0; i < precision; ++i) {

	}

	return convexHull;
}