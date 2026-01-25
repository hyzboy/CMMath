/**
 * CollisionDetector.h - Unified collision detection system
 *
 * Centralized collision detection tool that handles intersection tests
 * and collision information between various geometry primitives.
 *
 * Design principles:
 * - Static methods (stateless, thread-safe)
 * - Template-based polymorphism (zero virtual function overhead)
 * - Two-phase detection: broad phase (AABB) + narrow phase (precise)
 * - Symmetric overloads for intuitive usage
 *
 * This class removes complex collision logic from geometry classes,
 * keeping them lightweight and focused on data representation.
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<hgl/math/geometry/primitives/Capsule.h>
#include<hgl/math/geometry/primitives/Cylinder.h>
#include<hgl/math/geometry/primitives/Cone.h>
#include<hgl/math/geometry/primitives/Torus.h>

namespace hgl::math
{
    /**
     * Collision information structure
     *
     * Contains detailed information about collision/intersection:
     * - intersects: Whether geometries are intersecting
     * - point: Contact/collision point (world space)
     * - normal: Collision normal (direction to separate)
     * - penetration: Penetration depth (if intersecting)
     * - distance: Distance between geometries (if not intersecting)
     */
    struct CollisionInfo
    {
        bool intersects;         // True if geometries intersect
        Vector3f point;          // Collision/contact point
        Vector3f normal;         // Collision normal (normalized)
        float penetration;       // Penetration depth (>0 if intersecting)
        float distance;          // Distance (if not intersecting)

        CollisionInfo()
            : intersects(false), point(0, 0, 0), normal(0, 1, 0)
            , penetration(0.0f), distance(0.0f)
        {
        }
    };

    /**
     * CollisionDetector - Static collision detection methods
     *
     * Provides intersection tests and detailed collision information
     * for all geometry primitive pairs.
     *
     * Usage:
     *     Sphere s(...);
     *     Capsule c(...);
     *     bool hit = CollisionDetector::Intersects(s, c);
     *     CollisionInfo info = CollisionDetector::TestCollision(s, c);
     */
    class CollisionDetector
    {
    public:

        //=============================================================================
        // Sphere collision methods
        //=============================================================================

        /**
         * Test sphere-sphere intersection
         * @return true if spheres overlap
         */
        static bool Intersects(const Sphere& a, const Sphere& b);

        /**
         * Get detailed sphere-sphere collision information
         * @return Collision info with contact point, normal, and penetration
         */
        static CollisionInfo TestCollision(const Sphere& a, const Sphere& b);

        /**
         * Test sphere-AABB intersection
         */
        static bool Intersects(const Sphere& sphere, const AABB& box);
        static bool Intersects(const AABB& box, const Sphere& sphere)
        {
            return Intersects(sphere, box);
        }

        /**
         * Test sphere-OBB intersection
         */
        static bool Intersects(const Sphere& sphere, const OBB& box);
        static bool Intersects(const OBB& box, const Sphere& sphere)
        {
            return Intersects(sphere, box);
        }

        /**
         * Test sphere-capsule intersection
         */
        static bool Intersects(const Sphere& sphere, const Capsule& capsule);
        static bool Intersects(const Capsule& capsule, const Sphere& sphere)
        {
            return Intersects(sphere, capsule);
        }

        //=============================================================================
        // Capsule collision methods
        //=============================================================================

        /**
         * Test capsule-capsule intersection
         */
        static bool Intersects(const Capsule& a, const Capsule& b);

        /**
         * Get detailed capsule-capsule collision information
         */
        static CollisionInfo TestCollision(const Capsule& a, const Capsule& b);

        /**
         * Test capsule-AABB intersection
         */
        static bool Intersects(const Capsule& capsule, const AABB& box);
        static bool Intersects(const AABB& box, const Capsule& capsule)
        {
            return Intersects(capsule, box);
        }

        /**
         * Test capsule-OBB intersection
         */
        static bool Intersects(const Capsule& capsule, const OBB& box);
        static bool Intersects(const OBB& box, const Capsule& capsule)
        {
            return Intersects(capsule, box);
        }

        /**
         * Test capsule-cylinder intersection
         */
        static bool Intersects(const Capsule& capsule, const Cylinder& cylinder);
        static bool Intersects(const Cylinder& cylinder, const Capsule& capsule)
        {
            return Intersects(capsule, cylinder);
        }

        //=============================================================================
        // Cylinder collision methods
        //=============================================================================

        /**
         * Test cylinder-cylinder intersection
         */
        static bool Intersects(const Cylinder& a, const Cylinder& b);

        /**
         * Test cylinder-sphere intersection
         */
        static bool Intersects(const Cylinder& cylinder, const Sphere& sphere);
        static bool Intersects(const Sphere& sphere, const Cylinder& cylinder)
        {
            return Intersects(cylinder, sphere);
        }

        /**
         * Test cylinder-AABB intersection
         */
        static bool Intersects(const Cylinder& cylinder, const AABB& box);
        static bool Intersects(const AABB& box, const Cylinder& cylinder)
        {
            return Intersects(cylinder, box);
        }

        //=============================================================================
        // Cone collision methods
        //=============================================================================

        /**
         * Test cone-sphere intersection
         */
        static bool Intersects(const Cone& cone, const Sphere& sphere);
        static bool Intersects(const Sphere& sphere, const Cone& cone)
        {
            return Intersects(cone, sphere);
        }

        /**
         * Test cone-AABB intersection
         */
        static bool Intersects(const Cone& cone, const AABB& box);
        static bool Intersects(const AABB& box, const Cone& cone)
        {
            return Intersects(cone, box);
        }

        //=============================================================================
        // Torus collision methods
        //=============================================================================

        /**
         * Test torus-sphere intersection
         */
        static bool Intersects(const Torus& torus, const Sphere& sphere);
        static bool Intersects(const Sphere& sphere, const Torus& torus)
        {
            return Intersects(torus, sphere);
        }

        //=============================================================================
        // AABB/OBB unified interface (delegates to existing methods)
        //=============================================================================

        /**
         * Test AABB-AABB intersection (delegates to AABB class)
         */
        static bool Intersects(const AABB& a, const AABB& b)
        {
            return a.Intersects(b);
        }

        /**
         * Test AABB-OBB intersection
         */
        static bool Intersects(const AABB& aabb, const OBB& obb);
        static bool Intersects(const OBB& obb, const AABB& aabb)
        {
            return Intersects(aabb, obb);
        }

        /**
         * Test OBB-OBB intersection (delegates to OBB class)
         */
        static bool Intersects(const OBB& a, const OBB& b)
        {
            return a.Intersects(b);
        }

        //=============================================================================
        // Generic template with automatic broad phase
        //=============================================================================

        /**
         * Generic intersection test with automatic broad-phase optimization
         *
         * First tests bounding box overlap (cheap), then performs
         * precise collision detection (expensive) only if needed.
         *
         * @param a First geometry (must have GetBoundingBox() method)
         * @param b Second geometry (must have GetBoundingBox() method)
         * @return true if geometries intersect
         */
        template<typename T1, typename T2>
        static bool IntersectsWithBroadPhase(const T1& a, const T2& b)
        {
            // Broad phase: quick AABB rejection
            if (!a.GetBoundingBox().Intersects(b.GetBoundingBox()))
                return false;

            // Narrow phase: precise collision detection
            return Intersects(a, b);
        }
    };

}//namespace hgl::math
