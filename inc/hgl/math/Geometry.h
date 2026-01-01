/**
 * Geometry.h - Unified geometry library entry point
 * 
 * This is the main convenience header that includes all geometry primitives 
 * and query tools in the CMMath library. Use this for quick prototyping or 
 * when you need access to the full geometry system.
 * 
 * Usage:
 *     #include<hgl/math/Geometry.h>  // Everything
 * 
 * For faster compilation in production, use selective includes:
 *     #include<hgl/math/GeometryCore.h>      // Common primitives only
 *     #include<hgl/math/GeometryExtended.h>  // Extended shapes
 *     #include<hgl/math/GeometryQueries.h>   // Query tools only
 */
#pragma once

#include<hgl/math/GeometryCore.h>
#include<hgl/math/GeometryExtended.h>
#include<hgl/math/GeometryQueries.h>
