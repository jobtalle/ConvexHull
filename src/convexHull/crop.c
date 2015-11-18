#include <convexHull/convexHull.h>

#include <ccTrigonometry/ccTrigonometry.h>

#define CONVEX_HULL_ALPHA_MASK 0xFF000000

void convexHullCrop(convexHull *convexHull, const unsigned char *source, const unsigned int width, const unsigned int height, const ccVec2 pivot)
{
	unsigned int i;
	float r;
	float rStep = CC_TRI_PI_DOUBLE_F / convexHull->nodeCount;
	const ccVec2 halfDim = (ccVec2){ (float)(width >> 1), (float)(height >> 1) };

	r = 0;
	for(i = 0; i < convexHull->nodeCount;  ++i) {
		ccVec2 *node = convexHull->nodes + i;
		ccVec2 direction = (ccVec2){ -cosf(r), -sinf(r) };
		float abscos;
		float abssin;
		float radius;

		// Create edge points
		node->x = -direction.x;
		node->y = -direction.y;
		abscos = fabsf(node->x);
		abssin = fabsf(node->y);

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
			if(((unsigned int*)source)[x + y * width] & CONVEX_HULL_ALPHA_MASK) {
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
}