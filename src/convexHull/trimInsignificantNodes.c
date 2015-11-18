#include <convexHull/convexHull.h>

#include <math.h>

static float convexHullDist(ccVec2 p1, ccVec2 p2)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	return sqrtf(dx * dx + dy * dy);
}

void convexHullTrimInsignificantNodes(convexHull *convexHull, float trimDistance)
{
	unsigned int i, j, k;
	ccVec2 root = convexHull->nodes[0];

	j = 0;
	k = 1;
	for(i = 1; i <= convexHull->nodeCount; ++i) {
		ccVec2 current = convexHull->nodes[i == convexHull->nodeCount?0:i];

		if(convexHullDist(convexHull->nodes[j], current) > trimDistance) {
			// Add node to final graph
			convexHull->nodes[j] = ccVec2Multiply(root, 1.0f / k);

			// Reset root
			convexHull->nodes[++j] = current;
			k = 1;
			root = current;
		}
		else {
			// Add node to root
			root = ccVec2Add(root, current);
			++k;
		}
	}

	convexHull->nodeCount = j;
}