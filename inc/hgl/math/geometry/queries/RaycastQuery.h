/**
 * RaycastQuery.h - Ray-geometry intersection testing
 *
 * Provides ray casting and intersection tests for all geometry primitives.
 * Ray casting is essential for:
 * - Mouse picking / selection
 * - Line-of-sight tests
 * - Projectile collision
 * - AI vision systems
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/Ray.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/Plane.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<hgl/math/geometry/primitives/Capsule.h>
#include<hgl/math/geometry/primitives/Cylinder.h>
#include<hgl/math/geometry/primitives/Cone.h>
#include<hgl/math/geometry/primitives/Torus.h>

namespace hgl::math
{
    /**
     * Raycast hit information
     *
     * Contains detailed information about a ray-geometry intersection:
     * - hit: Whether ray intersected the geometry
     * - distance: Distance along ray to hit point (t value)
     * - point: Hit point in world space (ray.origin + ray.direction * distance)
     * - normal: Surface normal at hit point
     */
    struct RaycastHit
    {
        bool hit;                // True if ray hit geometry
        float distance;          // Distance along ray (t parameter)
        Vector3f point;          // Hit point (world space)
        Vector3f normal;         // Surface normal at hit point

        RaycastHit()
            : hit(false), distance(FLT_MAX), point(0, 0, 0), normal(0, 1, 0)
        {
        }
    };

    /**
     * RaycastQuery - Static ray intersection methods
     *
     * Provides ray casting operations for all geometry types.
     * Methods come in two flavors:
     * - Intersects(..., float& t): Simple boolean test with distance
     * - Test(...): Detailed hit information with point and normal
     *
     * Usage:
     *     Ray ray(origin, direction);
     *     Sphere sphere(center, radius);
     *
     *     // Simple test
     *     float t;
     *     if (RaycastQuery::Intersects(ray, sphere, t))
     *         Vector3f hitPoint = ray.origin + ray.direction * t;
     *
     *     // Detailed test
     *     RaycastHit hit = RaycastQuery::Test(ray, sphere);
     *     if (hit.hit)
     *         // Use hit.point, hit.normal, hit.distance
     */
    class RaycastQuery
    {
    public:

        //=============================================================================
        // Ray-Sphere intersection
        //=============================================================================

        /**
         * Test ray-sphere intersection
         * @param ray Ray to test
         * @param sphere Sphere to test against
         * @param t Output: distance along ray to hit point
         * @return true if ray intersects sphere
         */
        static bool Intersects(const Ray& ray, const Sphere& sphere, float& t);

        /**
         * Simple ray-sphere intersection (no distance output)
         */
        static bool Intersects(const Ray& ray, const Sphere& sphere);

        /**
         * Detailed ray-sphere intersection test
         * @return RaycastHit with full intersection information
         */
        static RaycastHit Test(const Ray& ray, const Sphere& sphere);

        //=============================================================================
        // Ray-Capsule intersection
        //=============================================================================

        /**
         * Test ray-capsule intersection
         */
        static bool Intersects(const Ray& ray, const Capsule& capsule, float& t);
        static bool Intersects(const Ray& ray, const Capsule& capsule);
        static RaycastHit Test(const Ray& ray, const Capsule& capsule);

        //=============================================================================
        // Ray-Cylinder intersection
        //=============================================================================

        /**
         * Test ray-cylinder intersection
         */
        static bool Intersects(const Ray& ray, const Cylinder& cylinder, float& t);
        static bool Intersects(const Ray& ray, const Cylinder& cylinder);
        static RaycastHit Test(const Ray& ray, const Cylinder& cylinder);

        //=============================================================================
        // Ray-Cone intersection
        //=============================================================================

        /**
         * Test ray-cone intersection
         */
        static bool Intersects(const Ray& ray, const Cone& cone, float& t);
        static bool Intersects(const Ray& ray, const Cone& cone);
        static RaycastHit Test(const Ray& ray, const Cone& cone);

        //=============================================================================
        // Ray-Torus intersection
        //=============================================================================

        /**
         * Test ray-torus intersection
         */
        static bool Intersects(const Ray& ray, const Torus& torus, float& t);
        static bool Intersects(const Ray& ray, const Torus& torus);
        static RaycastHit Test(const Ray& ray, const Torus& torus);

        //=============================================================================
        // Ray-AABB/OBB/Plane (unified interface for existing functionality)
        //=============================================================================

        /**
         * Test ray-AABB intersection
         */
        static bool Intersects(const Ray& ray, const AABB& box, float& t);
        static bool Intersects(const Ray& ray, const AABB& box);
        static RaycastHit Test(const Ray& ray, const AABB& box);

        /**
         * Test ray-OBB intersection
         */
        static bool Intersects(const Ray& ray, const OBB& box, float& t);
        static bool Intersects(const Ray& ray, const OBB& box);
        static RaycastHit Test(const Ray& ray, const OBB& box);

        /**
         * Test ray-plane intersection
         */
        static bool Intersects(const Ray& ray, const Plane& plane, float& t);
        static bool Intersects(const Ray& ray, const Plane& plane);
        static RaycastHit Test(const Ray& ray, const Plane& plane);

        //=============================================================================
        // Ray-Triangle intersection (Möller-Trumbore algorithm)
        //=============================================================================

        /**
         * Test ray-triangle intersection using Möller-Trumbore algorithm
         *
         * @param ray Ray to test
         * @param v0, v1, v2 Triangle vertices
         * @param t Output: distance along ray
         * @param u, v Output: barycentric coordinates within triangle
         * @return true if ray intersects triangle
         */
        static bool IntersectsTriangle(const Ray& ray,
                                       const Vector3f& v0,
                                       const Vector3f& v1,
                                       const Vector3f& v2,
                                       float& t, float& u, float& v);

        /**
         * Simple ray-triangle intersection (no barycentric coordinates)
         */
        static bool IntersectsTriangle(const Ray& ray,
                                       const Vector3f& v0,
                                       const Vector3f& v1,
                                       const Vector3f& v2,
                                       float& t);
    };

}//namespace hgl::math
