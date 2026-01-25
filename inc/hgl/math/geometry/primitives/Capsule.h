/**
 * Capsule.h - Simplified capsule geometry primitive
 *
 * A capsule is formed by sweeping a sphere along a line segment.
 * It consists of a cylindrical body with hemispherical end caps.
 *
 * This lightweight class focuses on:
 * - Property storage and access
 * - Simple geometric queries
 * - Bounding box calculation
 *
 * Complex collision detection has been moved to CollisionDetector.
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<numbers>

namespace hgl::math
{
    /**
     * Capsule - A sphere swept along a line segment
     *
     * Geometric definition:
     * - Start point: One end of the center axis
     * - End point: Other end of the center axis
     * - Radius: Distance from axis to surface
     *
     * Mathematical representation:
     * Surface points satisfy: distance_to_segment(P, start, end) = radius
     *
     * Properties:
     * - Total length = ||end - start|| + 2 * radius
     * - Volume = πr²h + (4/3)πr³ (cylinder + sphere)
     * - Rotation invariant around axis
     * - Excellent for character controllers
     *
     * Common uses:
     * - Character collision bodies
     * - Physics capsule rigid bodies
     * - Swept volume tests
     * - Approximating cylindrical objects
     */
    class Capsule
    {
        Vector3f start;      // Start of center axis line segment
        Vector3f end;        // End of center axis line segment
        float radius;        // Capsule radius

    public:

        /** Get start point of center axis */
        const Vector3f& GetStart() const { return start; }

        /** Get end point of center axis */
        const Vector3f& GetEnd() const { return end; }

        /** Get capsule radius */
        float GetRadius() const { return radius; }

        /**
         * Get capsule geometric center
         * @return Midpoint of the line segment
         */
        Vector3f GetCenter() const
        {
            return (start + end) * 0.5f;
        }

        /**
         * Get capsule axis direction (normalized)
         * @return Unit vector from start to end
         */
        Vector3f GetDirection() const
        {
            return Normalized(end - start);
        }

        /**
         * Get cylinder height (excluding end caps)
         * @return Length of center axis
         */
        float GetCylinderHeight() const
        {
            return Length(end - start);
        }

        /**
         * Get total capsule length (including end caps)
         * @return Total length = cylinder height + 2 * radius
         */
        float GetTotalLength() const
        {
            return GetCylinderHeight() + 2.0f * radius;
        }

    public:

        /**
         * Default constructor
         * Creates a vertical capsule at origin
         */
        Capsule()
            : start(0, -0.5f, 0), end(0, 0.5f, 0), radius(0.5f)
        {
        }

        /**
         * Parameterized constructor
         * @param s Start point of axis
         * @param e End point of axis
         * @param r Capsule radius
         */
        Capsule(const Vector3f& s, const Vector3f& e, float r)
            : start(s), end(e), radius(r)
        {
        }

        /**
         * Set capsule parameters
         * @param s Start point of axis
         * @param e End point of axis
         * @param r Capsule radius
         */
        void Set(const Vector3f& s, const Vector3f& e, float r)
        {
            start = s;
            end = e;
            radius = r;
        }

        /**
         * Calculate capsule volume
         * Volume = cylinder volume + sphere volume
         * @return V = πr²h + (4/3)πr³
         */
        float GetVolume() const
        {
            float h = GetCylinderHeight();
            return std::numbers::pi_v<float> * radius * radius * h +
                   (4.0f / 3.0f) * std::numbers::pi_v<float> * radius * radius * radius;
        }

        /**
         * Calculate capsule surface area
         * Surface area = cylinder lateral area + sphere surface area
         * @return S = 2πrh + 4πr²
         */
        float GetSurfaceArea() const
        {
            float h = GetCylinderHeight();
            return 2.0f * std::numbers::pi_v<float> * radius * h +
                   4.0f * std::numbers::pi_v<float> * radius * radius;
        }

        /**
         * Test if a point is inside the capsule
         * @param point Point to test
         * @return true if point is inside or on surface
         */
        bool ContainsPoint(const Vector3f& point) const
        {
            // Find closest point on line segment
            Vector3f segment = end - start;
            Vector3f to_point = point - start;

            float t = Dot(to_point, segment) / Dot(segment, segment);
            t = clamp(t, 0.0f, 1.0f);

            Vector3f closest_on_segment = start + segment * t;

            // Check if distance to segment is within radius
            return LengthSquared(point - closest_on_segment) <= radius * radius;
        }

        /**
         * Find closest point on capsule surface to a given point
         * @param point Point in space
         * @return Closest point on capsule surface
         */
        Vector3f ClosestPoint(const Vector3f& point) const
        {
            // Find closest point on line segment
            Vector3f segment = end - start;
            Vector3f to_point = point - start;

            float t = Dot(to_point, segment) / Dot(segment, segment);
            t = clamp(t, 0.0f, 1.0f);

            Vector3f closest_on_segment = start + segment * t;

            // Project from segment to surface along radius
            Vector3f dir = point - closest_on_segment;
            float dist = Length(dir);

            if (dist <= radius)
                return point;  // Point is inside

            return closest_on_segment + dir * (radius / dist);
        }

        /**
         * Calculate distance from point to capsule surface
         * @param point Point in space
         * @return Distance to surface (0 if inside)
         */
        float DistanceToPoint(const Vector3f& point) const
        {
            // Find closest point on line segment
            Vector3f segment = end - start;
            Vector3f to_point = point - start;

            float t = Dot(to_point, segment) / Dot(segment, segment);
            t = clamp(t, 0.0f, 1.0f);

            Vector3f closest_on_segment = start + segment * t;

            float dist = Length(point - closest_on_segment) - radius;
            return dist > 0.0f ? dist : 0.0f;
        }

        /**
         * Get axis-aligned bounding box for broad-phase collision
         * @return AABB that contains this capsule
         */
        AABB GetBoundingBox() const
        {
            Vector3f r(radius, radius, radius);
            Vector3f min_pt = glm::min(start, end) - r;
            Vector3f max_pt = glm::max(start, end) + r;

            AABB box;
            box.SetMinMax(min_pt, max_pt);
            return box;
        }

    };//class Capsule

}//namespace hgl::math
