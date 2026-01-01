/**
 * Sphere.h - Simplified sphere geometry primitive
 * 
 * This is the new lightweight Sphere class that focuses only on:
 * - Property storage and access
 * - Simple, high-frequency geometric queries
 * - Bounding box calculation for broad-phase collision
 * 
 * Complex collision detection has been moved to CollisionDetector.
 * This keeps the class clean and focused on its core responsibility.
 * 
 * For backward compatibility, the original Sphere class remains at
 * inc/hgl/math/geometry/Sphere.h and is still available.
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>

namespace hgl::math
{
    /**
     * Sphere - Standard sphere with uniform radius
     * 
     * Mathematical definition:
     * Surface equation: ||P - center|| = radius
     * i.e., (x-cx)² + (y-cy)² + (z-cz)² = r²
     * 
     * Properties:
     * - Perfect symmetry (isotropic)
     * - Minimal surface area to volume ratio
     * - Rotation invariant
     * - Simplest collision detection
     * 
     * Common uses:
     * - Bounding spheres for broad-phase collision
     * - Particle systems
     * - Light attenuation ranges
     * - Physics simulation
     */
    class Sphere
    {
        Vector3f center;     // Sphere center position
        float radius;        // Sphere radius

    public:

        /** Get sphere center */
        const Vector3f& GetCenter() const { return center; }
        
        /** Get sphere radius */
        float GetRadius() const { return radius; }

    public:

        /**
         * Default constructor
         * Creates a unit sphere at origin with radius 1
         */
        Sphere()
            : center(0, 0, 0), radius(1.0f)
        {
        }

        /**
         * Parameterized constructor
         * @param c Center position
         * @param r Sphere radius
         */
        Sphere(const Vector3f& c, float r)
            : center(c), radius(r)
        {
        }

        /**
         * Set sphere parameters
         * @param c New center position
         * @param r New radius
         */
        void Set(const Vector3f& c, float r)
        {
            center = c;
            radius = r;
        }

        /**
         * Calculate sphere volume
         * Formula: V = (4/3)πr³
         * @return Volume of the sphere
         */
        float GetVolume() const
        {
            return (4.0f / 3.0f) * pi * radius * radius * radius;
        }

        /**
         * Calculate sphere surface area
         * Formula: S = 4πr²
         * @return Surface area of the sphere
         */
        float GetSurfaceArea() const
        {
            return 4.0f * pi * radius * radius;
        }

        /**
         * Get point on sphere surface in a given direction
         * 
         * Returns the point on the sphere surface along the given direction
         * from the center. Direction does not need to be normalized.
         * 
         * @param direction Direction vector from center
         * @return Point on sphere surface
         */
        Vector3f GetPoint(const Vector3f& direction) const
        {
            return center + Normalized(direction) * radius;
        }

        /**
         * Test if a point is inside or on the sphere
         * @param point Point to test
         * @return true if point is inside or on surface, false if outside
         */
        bool ContainsPoint(const Vector3f& point) const
        {
            return LengthSquared(point - center) <= radius * radius;
        }

        /**
         * Find the closest point on sphere surface to a given point
         * @param point Point in space
         * @return Closest point on sphere surface
         */
        Vector3f ClosestPoint(const Vector3f& point) const
        {
            Vector3f dir = point - center;
            float dist = Length(dir);
            
            if (dist <= radius)
                return point;  // Point is inside, it is its own closest point
            
            return center + dir * (radius / dist);
        }

        /**
         * Calculate distance from a point to sphere surface
         * @param point Point in space
         * @return Distance to surface (0 if point is inside)
         */
        float DistanceToPoint(const Vector3f& point) const
        {
            float dist = Length(point - center) - radius;
            return dist > 0.0f ? dist : 0.0f;
        }

        /**
         * Get axis-aligned bounding box for broad-phase collision
         * @return AABB that tightly contains this sphere
         */
        AABB GetBoundingBox() const
        {
            Vector3f r(radius, radius, radius);
            AABB box;
            box.SetMinMax(center - r, center + r);
            return box;
        }

    };//class Sphere

}//namespace hgl::math
