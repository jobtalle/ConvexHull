#include <convexHull/convexHull.h>

#include <math.h>

static float convexHullDist(convexHullVector p1, convexHullVector p2)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	return sqrtf(dx * dx + dy * dy);
}

void convexHullTrimInsignificantNodes(convexHull *convexHull, float trimDistance)
{
	unsigned int i, j, k;
	convexHullVector root = convexHull->nodes[0];

	j = 0;
	k = 1;
	for(i = 1; i <= convexHull->nodeCount; ++i) {
		convexHullVector current = convexHull->nodes[i == convexHull->nodeCount?0:i];

		if(convexHullDist(convexHull->nodes[j], current) > trimDistance) {
			// Add node to final graph
			float factor = 1.0f / k;
			convexHull->nodes[j++] = (convexHullVector){ root.x * factor, root.y * factor };

			// Reset root
			if(i != convexHull->nodeCount) {
				convexHull->nodes[j] = current;
				k = 1;
				root = current;
			}
		}
		else {
			// Add node to root
			root = (convexHullVector){ root.x + current.x, root.y + current.y };
			++k;
		}
	}

	convexHull->nodeCount = j;
}