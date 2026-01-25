/**
 * DistanceQuery.h - Distance calculations between geometries
 *
 * Provides distance measurement and closest point finding between
 * various geometry primitives. Essential for:
 * - Proximity queries
 * - Minimum distance calculations
 * - Collision prediction
 * - Separation vector computation
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<hgl/math/geometry/primitives/Capsule.h>
#include<hgl/math/geometry/primitives/Cylinder.h>

namespace hgl::math
{
    /**
     * Closest points result
     *
     * Contains the pair of closest points between two geometries
     * and their distance.
     */
    struct ClosestPointsResult
    {
        Vector3f pointOnA;       // Closest point on first geometry
        Vector3f pointOnB;       // Closest point on second geometry
        float distance;          // Distance between the points

        ClosestPointsResult()
            : pointOnA(0, 0, 0), pointOnB(0, 0, 0), distance(0.0f)
        {
        }
    };

    /**
     * DistanceQuery - Static distance calculation methods
     *
     * Provides distance measurements and closest point queries
     * between geometry primitives.
     *
     * Two types of queries:
     * 1. Distance(point, geometry): Point-to-geometry distance
     * 2. Distance(geom1, geom2): Geometry-to-geometry distance
     * 3. ClosestPoints(geom1, geom2): Find closest point pairs
     *
     * Usage:
     *     Sphere s(...);
     *     Capsule c(...);
     *     float dist = DistanceQuery::Distance(s, c);
     *     ClosestPointsResult result = DistanceQuery::ClosestPoints(s, c);
     */
    class DistanceQuery
    {
    public:

        //=============================================================================
        // Point-to-geometry distance
        //=============================================================================

        /**
         * Distance from point to sphere surface
         * @return Distance (0 if point is inside)
         */
        static float Distance(const Vector3f& point, const Sphere& sphere);

        /**
         * Distance from point to capsule surface
         */
        static float Distance(const Vector3f& point, const Capsule& capsule);

        /**
         * Distance from point to AABB surface
         */
        static float Distance(const Vector3f& point, const AABB& box);

        /**
         * Distance from point to cylinder surface
         */
        static float Distance(const Vector3f& point, const Cylinder& cylinder);

        //=============================================================================
        // Geometry-to-geometry distance
        //=============================================================================

        /**
         * Distance between two spheres (surface to surface)
         * @return Distance (0 if intersecting)
         */
        static float Distance(const Sphere& a, const Sphere& b);

        /**
         * Distance between sphere and capsule
         */
        static float Distance(const Sphere& sphere, const Capsule& capsule);
        static float Distance(const Capsule& capsule, const Sphere& sphere)
        {
            return Distance(sphere, capsule);
        }

        /**
         * Distance between two capsules
         */
        static float Distance(const Capsule& a, const Capsule& b);

        /**
         * Distance between two cylinders
         */
        static float Distance(const Cylinder& a, const Cylinder& b);

        //=============================================================================
        // Closest point pairs
        //=============================================================================

        /**
         * Find closest points between two capsules
         * @return Result with points on both capsules and distance
         */
        static ClosestPointsResult ClosestPoints(const Capsule& a, const Capsule& b);

        /**
         * Find closest points between sphere and capsule
         */
        static ClosestPointsResult ClosestPoints(const Sphere& sphere, const Capsule& capsule);
        static ClosestPointsResult ClosestPoints(const Capsule& capsule, const Sphere& sphere)
        {
            auto result = ClosestPoints(sphere, capsule);
            std::swap(result.pointOnA, result.pointOnB);
            return result;
        }

        //=============================================================================
        // Helper functions
        //=============================================================================

        /**
         * Find closest point on a line segment to a given point
         *
         * @param point Query point
         * @param segmentStart Line segment start
         * @param segmentEnd Line segment end
         * @return Closest point on segment [segmentStart, segmentEnd]
         */
        static Vector3f ClosestPointOnLineSegment(const Vector3f& point,
                                                  const Vector3f& segmentStart,
                                                  const Vector3f& segmentEnd);

        /**
         * Find closest points between two line segments
         *
         * This is a fundamental geometric primitive used by many
         * geometry-to-geometry distance calculations.
         *
         * @param seg1Start First segment start
         * @param seg1End First segment end
         * @param seg2Start Second segment start
         * @param seg2End Second segment end
         * @return Result with closest points on both segments
         */
        static ClosestPointsResult ClosestPointsOnLineSegments(
            const Vector3f& seg1Start, const Vector3f& seg1End,
            const Vector3f& seg2Start, const Vector3f& seg2End);
    };

}//namespace hgl::math
