/**
 * GeometryQueries.h - Geometry query tools
 * 
 * Centralized collision detection and geometric query tools.
 * These classes provide stateless, static methods for complex geometric
 * operations that were previously scattered across geometry classes.
 * 
 * Benefits:
 * - Separation of concerns: geometry data vs. query algorithms
 * - Easier to test and maintain
 * - Zero virtual function overhead (template-based static polymorphism)
 * - Consistent API across all geometry types
 * 
 * Included tools:
 * - CollisionDetector: Intersection tests and collision information
 * - RaycastQuery: Ray-geometry intersection tests
 * - DistanceQuery: Distance calculations between geometries
 * - ContainmentQuery: Containment and inclusion tests
 */
#pragma once

#include<hgl/math/geometry/queries/CollisionDetector.h>
#include<hgl/math/geometry/queries/RaycastQuery.h>
#include<hgl/math/geometry/queries/DistanceQuery.h>
#include<hgl/math/geometry/queries/ContainmentQuery.h>
