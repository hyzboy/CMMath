/**
 * ContainmentQuery.h - Geometry containment and inclusion tests
 * 
 * Provides tests for checking if one geometry fully contains another.
 * Useful for:
 * - Hierarchical spatial queries
 * - Frustum culling
 * - Region testing
 * - Bounding volume optimization
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<hgl/math/geometry/primitives/Capsule.h>

namespace hgl::math
{
    /**
     * ContainmentQuery - Static containment test methods
     * 
     * Provides two types of queries:
     * 1. Point containment: Is point inside geometry?
     *    (Delegates to geometry.ContainsPoint() for consistency)
     * 2. Geometry containment: Does geometry A fully contain geometry B?
     * 
     * Usage:
     *     Sphere s(...);
     *     AABB box(...);
     *     
     *     // Point containment
     *     if (ContainmentQuery::Contains(s, point))
     *         // Point is inside sphere
     *     
     *     // Geometry containment
     *     if (ContainmentQuery::Contains(box, s))
     *         // Sphere is fully inside box
     */
    class ContainmentQuery
    {
    public:
    
        //=============================================================================
        // Point containment (delegates to geometry classes for consistency)
        //=============================================================================
        
        /**
         * Test if point is inside sphere
         * Delegates to Sphere::ContainsPoint() for consistency
         */
        static bool Contains(const Sphere& container, const Vector3f& point)
        {
            return container.ContainsPoint(point);
        }
        
        /**
         * Test if point is inside capsule
         */
        static bool Contains(const Capsule& container, const Vector3f& point)
        {
            return container.ContainsPoint(point);
        }
        
        /**
         * Test if point is inside AABB
         */
        static bool Contains(const AABB& container, const Vector3f& point)
        {
            return container.ContainsPoint(point);
        }
        
        /**
         * Test if point is inside OBB
         */
        static bool Contains(const OBB& container, const Vector3f& point)
        {
            return container.ContainsPoint(point);
        }
        
        //=============================================================================
        // Sphere containment
        //=============================================================================
        
        /**
         * Test if one sphere fully contains another
         * 
         * Sphere A contains sphere B if:
         * distance(centerA, centerB) + radiusB <= radiusA
         * 
         * @return true if container fully contains sphere
         */
        static bool Contains(const Sphere& container, const Sphere& sphere);
        
        /**
         * Test if AABB fully contains a sphere
         * 
         * AABB contains sphere if:
         * - Sphere center is inside AABB with margin >= radius
         * 
         * @return true if AABB fully contains sphere
         */
        static bool Contains(const AABB& container, const Sphere& sphere);
        
        //=============================================================================
        // AABB containment
        //=============================================================================
        
        /**
         * Test if one AABB fully contains another
         * 
         * AABB A contains AABB B if:
         * - B.min >= A.min (in all dimensions)
         * - B.max <= A.max (in all dimensions)
         * 
         * @return true if container fully contains box
         */
        static bool Contains(const AABB& container, const AABB& box);
        
        /**
         * Test if OBB fully contains an AABB
         */
        static bool Contains(const OBB& container, const AABB& box);
        
        //=============================================================================
        // OBB containment
        //=============================================================================
        
        /**
         * Test if one OBB fully contains another
         * 
         * More expensive test requiring projection and separation tests.
         * 
         * @return true if container fully contains box
         */
        static bool Contains(const OBB& container, const OBB& box);
    };

}//namespace hgl::math
