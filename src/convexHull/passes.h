#pragma once

#include <convexHull/convexHull.h>

void convexHullCrop(
	convexHull *convexHull,
	const uint8_t *source,
	const uint32_t width,
	const uint32_t height,
	const convexHullVector pivot);

void convexHullGrahamScan(
	convexHull *convexHull);

void convexHullTrimInsignificantNodes(
	convexHull *convexHull,
	const float trimDistance);