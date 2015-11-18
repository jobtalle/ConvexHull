#include <convexHull/convexHull.h>

#include <stdlib.h>

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

void convexHullGrahamScan(convexHull *convexHull)
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