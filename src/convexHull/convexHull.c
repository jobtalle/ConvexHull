#include <stdlib.h>
#include <math.h>

#include <convexHull/convexHull.h>
#include <ccTrigonometry/ccTrigonometry.h>

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
		}

		r += rStep;
	}

	if(phase == 0) return convexHull;

	// Sweep concave nodes
	unsigned int newNodeCount = 0;
	ccVec2 *newNodes = malloc(sizeof(ccVec2)* precision);

	for(i = 0; i < precision; ++i) {
		unsigned int previous = i == 0?precision - 1:i - 1;
		unsigned int next = i == precision - 1?0:i + 1;
		ccVec2 direction = ccVec2Subtract(*(convexHull.nodes + i), *(convexHull.nodes + previous));
		ccVec2 orthogonal = (ccVec2){ -direction.y, direction.x };
		ccVec2 newDirection = ccVec2Subtract(*(convexHull.nodes + next), *(convexHull.nodes + i));
		float dot = ccVec2DotProduct(orthogonal, newDirection);

		if(dot > 0) {
			*(newNodes + newNodeCount) = *(convexHull.nodes + i);
			++newNodeCount;
		}

		printf("Turn %d\n", dot > 0);
	}

	convexHull.nodes = newNodes;
	convexHull.nodeCount = newNodeCount;

	return convexHull;
}