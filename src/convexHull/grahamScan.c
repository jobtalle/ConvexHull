#include <convexHull/convexHull.h>

#include <stdlib.h>

static convexHullVector comparePoint;

static float convexHullOrientation(convexHullVector p, convexHullVector q, convexHullVector r)
{
	return (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
}

static void convexHullSwap(convexHullVector *a, convexHullVector *b)
{
	convexHullVector buffer = *a;
	*a = *b;
	*b = buffer;
}

static float convexHullDist(convexHullVector p1, convexHullVector p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

static int convexHullCompare(const void *a, const void *b)
{
	const convexHullVector *p1 = a;
	const convexHullVector *p2 = b;

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
	qsort(convexHull->nodes, convexHull->nodeCount, sizeof(convexHullVector), convexHullCompare);

	// Create & initialize stack
	for(i = 3; i < convexHull->nodeCount; ++i) {
		while(convexHullOrientation(convexHull->nodes[stackIndex - 2], convexHull->nodes[stackIndex - 1], convexHull->nodes[i]) >= 0) {
			--stackIndex;
		}

		convexHull->nodes[stackIndex++] = convexHull->nodes[i];
	}

	// Store final list
	convexHull->nodeCount = stackIndex;
}