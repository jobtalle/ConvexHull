# ConvexHull
Automatic convex hull generation for raster images

This project requires [ccTrigonometry](https://github.com/jobtalle/ccTrigonometry) and [ccVector](https://github.com/jobtalle/ccVector) to build. The test case also requires [ccore](https://github.com/ccore/ccore) to build.

![Convex hull algorithm stages](example.gif)

The animated example above shows the three stages of the algorithm at fairly low precision;
- Cropping an inital number of nodes around the image based on spacing.
- Mean nodes that are close to each other based on precision.
- Perform a Graham scan to remove concavity.
