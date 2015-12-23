#include <convexHull/convexHull.h>
#include <math.h>

#define CONVEX_HULL_ALPHA_MASK 0xFF000000

void convexHullCrop(convexHull *convexHull, const uint8_t *source, const uint32_t width, const uint32_t height, const convexHullVector pivot)
{
	uint32_t i;
	float r;
	float rStep = 6.283185f / convexHull->nodeCount;
	const convexHullVector halfDim = (convexHullVector){ (float)(width >> 1), (float)(height >> 1) };

	r = 0;
	for(i = 0; i < convexHull->nodeCount;  ++i) {
		convexHullVector *node = convexHull->nodes + i;
		convexHullVector direction = (convexHullVector){ -cosf(r), -sinf(r) };
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
		node->x += halfDim.x;
		node->y += halfDim.y;

		// Crop until opaque pixel is found
		for(;;) {
			const uint32_t x = (uint32_t)node->x;
			const uint32_t y = (uint32_t)node->y;

			// Check alpha
			if(((uint32_t*)source)[x + y * width] & CONVEX_HULL_ALPHA_MASK) {
				break;
			}

			// Next
			node->x += direction.x;
			node->y += direction.y;
			radius -= 1.0f;

			// Prevent overflows
			if(radius < 0) break;
		}

		// Move to pivot
		node->x -= pivot.x;
		node->y -= pivot.y;

		r += rStep;
	}
}