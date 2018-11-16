/*
 * C code from the article
 * "Testing the Convexity of a Polygon"
 * by Peter Schorn and Frederick Fisher,
 *	(schorn@inf.ethz.ch, fred@kpc.com)
 * in "Graphics Gems IV", Academic Press, 1994
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { NotConvex, NotConvexDegenerate, ConvexDegenerate, ConvexCCW, ConvexCW } PolygonClass;

PolygonClass classifyPolygon2(int nvert, float pVert[][2]);

#ifdef __cplusplus
}
#endif

