#pragma once

#include <convexHull/convexHull.h>

void convexHullCrop(
	convexHull *convexHull,
	const unsigned char *source,
	const unsigned int width,
	const unsigned int height,
	const convexHullVector pivot);

void convexHullGrahamScan(
	convexHull *convexHull);

void convexHullTrimInsignificantNodes(
	convexHull *convexHull,
	float trimDistance);