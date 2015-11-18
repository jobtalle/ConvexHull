#include <stdlib.h>
#include <math.h>

#include <convexHull/convexHull.h>
#include <ccTrigonometry/ccTrigonometry.h>

static ccVec2 comparePoint;

static float convexHullOrientation(ccVec2 p, ccVec2 q, ccVec2 r)
{
	return (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
}

static void convexHullSwap(ccVec2 *a, ccVec2 *b)
{
	ccVec2 buffer = *a;
	*a = *b;
	*b = buffer;
}

static float convexHullDist(ccVec2 p1, ccVec2 p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

static int convexHullCompare(const void *a, const void *b)
{
	const ccVec2 *p1 = a;
	const ccVec2 *p2 = b;

	float orientation = convexHullOrientation(comparePoint, *p1, *p2);

	if(orientation == 0) return (convexHullDist(comparePoint, *p2) >= convexHullDist(comparePoint, *p1))?-1:1;
	return (orientation < 0)?-1:1;
}

static void convexHullGrahamScan(convexHull *convexHull)
{
	unsigned int i;
	unsigned int minIndex = 0;
	unsigned int stackIndex = 3;
	float ymin = convexHull->nodes[0].y;

	// Find minimum Y
	for(i = 1; i < convexHull->nodeCount; ++i) {
		if(convexHull->nodes[i].y < ymin) {
			ymin = convexHull->nodes[i].y;
			minIndex = i;
		}
	}

	// Put minimum at zero
	convexHullSwap(convexHull->nodes, convexHull->nodes + minIndex);

	// Sort
	comparePoint = convexHull->nodes[0];
	qsort(convexHull->nodes, convexHull->nodeCount, sizeof(ccVec2), convexHullCompare);

	// Create & initialize stack
	for(i = 3; i < convexHull->nodeCount; ++i) {
		while(convexHullOrientation(convexHull->nodes[stackIndex - 2], convexHull->nodes[stackIndex - 1], convexHull->nodes[i]) >= 0) {
			--stackIndex;
		}

		convexHull->nodes[stackIndex] = convexHull->nodes[i];
		++stackIndex;
	}

	// Store final list
	convexHull->nodeCount = stackIndex;
}

convexHull convexHullCreate(const unsigned char *source, const unsigned int width, const unsigned int height, const ccVec2 pivot, const unsigned int precision, int phase)
{
	unsigned int i;
	float r;
	float rStep = CC_TRI_PI_DOUBLE_F / precision;
	const ccVec2 halfDim = (ccVec2){ (float)(width >> 1), (float)(height >> 1) };
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

		// Normalize
		*node = ccVec2Add(*node, halfDim);
		
		// Crop until opaque pixel is found
		for(;;) {
			const unsigned int x = (unsigned int)node->x;
			const unsigned int y = (unsigned int)node->y;

			// Check alpha
			if(((unsigned int*)source)[x + y * width] & 0xFF000000) {
				break;
			}

			// Next
			*node = ccVec2Add(*node, direction);
			radius -= 1.0f;

			// Prevent overflows
			if(radius < 0) break;
		}

		// Move to pivot
		*node = ccVec2Subtract(*node, pivot);

		r += rStep;
	}

	if(phase == 0) return convexHull;

	// Graham scan
	convexHullGrahamScan(&convexHull);

	return convexHull;
}