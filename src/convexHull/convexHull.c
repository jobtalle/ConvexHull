#include <stdlib.h>
#include <math.h>

#include <convexHull/convexHull.h>
#include <ccTrigonometry/ccTrigonometry.h>

static ccVec2 comparePoint;

static int convexHullOrientation(ccVec2 p, ccVec2 q, ccVec2 r)
{
	int c = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if(c == 0) return 0;
	return c > 0?1:2;
}

static void convexHullSwap(ccVec2 *a, ccVec2 *b)
{
	ccVec2 buffer = *a;
	*a = *b;
	*b = buffer;
}

static int convexHullDist(ccVec2 p1, ccVec2 p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

static int convexHullCompare(const void *a, const void *b)
{
	ccVec2 *p1 = a;
	ccVec2 *p2 = b;

	int o = convexHullOrientation(comparePoint, *p1, *p2);
	if(o == 0) {
		return (convexHullDist(comparePoint, *p2) >= convexHullDist(comparePoint, *p1))?-1:1;
	}

	return (o == 2)?-1:1;
}

convexHull convexHullCreate(unsigned char *source, unsigned int width, unsigned int height, unsigned int offsetx, unsigned int offsety, unsigned int precision, int phase)
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
			radius -= 1.0f;
		}

		r += rStep;
	}

	if(phase == 0) return convexHull;

	// Graham scan

	// Find minimum y
	unsigned int minIndex = 0;
	float ymin = convexHull.nodes[0].y;

	for(i = 1; i < precision; ++i) {
		if(convexHull.nodes[i].y < ymin) {
			ymin = convexHull.nodes[i].y;
			minIndex = i;
		}
	}

	// Put minimum at zero
	convexHullSwap(convexHull.nodes, convexHull.nodes + minIndex);

	// Sort
	comparePoint = convexHull.nodes[0];
	qsort(convexHull.nodes, precision, sizeof(ccVec2), convexHullCompare);

	// Create & initialize stack
	unsigned int stackIndex = 3;
	ccVec2 *stack = malloc(sizeof(ccVec2)* precision);

	for(i = 0; i < 3; ++i) stack[i] = convexHull.nodes[i];

	for(i = 3; i < precision; ++i) {
		while(convexHullOrientation(stack[stackIndex - 2], stack[stackIndex - 1], convexHull.nodes[i]) != 2) {
			--stackIndex;
		}

		stack[stackIndex] = convexHull.nodes[i];
		++stackIndex;
	}

	// Store final list
	convexHull.nodeCount = stackIndex;
	convexHull.nodes = stack;

	return convexHull;
}