#pragma once

void convexHullCrop(
	convexHull *convexHull,
	const unsigned char *source,
	const unsigned int width,
	const unsigned int height,
	const ccVec2 pivot);

void convexHullGrahamScan(
	convexHull *convexHull);