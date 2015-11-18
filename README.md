# ConvexHull
Automatic convex hull generation for raster images

The test case requires [ccore](https://github.com/ccore/ccore) to build, and uses [lodepng](https://github.com/lvandeve/lodepng) to decode a PNG image.

![Convex hull algorithm stages](example.gif)

The animated example above shows the three stages of the algorithm at fairly low precision;
- Cropping an inital number of nodes around the image based on spacing.
- Mean nodes that are close to each other based on precision.
- Perform a Graham scan to remove concavity.
