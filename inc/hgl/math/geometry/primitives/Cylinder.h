/**
 * Cylinder.h - Simplified cylinder geometry primitive
 * 
 * A cylinder is a 3D shape with circular cross-sections perpendicular
 * to its axis. This lightweight class focuses on geometric properties
 * and simple queries.
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>

namespace hgl::math
{
    /**
     * Cylinder - Circular cylinder with axis, height, and radius
     * 
     * Defined by:
     * - Center: Geometric center of the cylinder
     * - Axis: Direction of cylinder axis (normalized)
     * - Height: Length along axis
     * - Radius: Radius of circular cross-section
     * 
     * Common uses:
     * - Columnar structures (pillars, trees)
     * - Pipe modeling
     * - Collision volumes
     */
    class Cylinder
    {
        Vector3f center;     // Cylinder center point
        Vector3f axis;       // Cylinder axis direction (normalized)
        float height;        // Cylinder height (along axis)
        float radius;        // Cylinder radius

    public:

        /**
         * Default constructor
         * Creates a cylinder at origin, Y-axis aligned, height 1.0, radius 0.5
         */
        Cylinder()
            : center(0, 0, 0), axis(0, 1, 0), height(1.0f), radius(0.5f)
        {
        }

        /**
         * Parameterized constructor
         * @param c Center position
         * @param a Axis direction (will be normalized)
         * @param h Height
         * @param r Radius
         */
        Cylinder(const Vector3f& c, const Vector3f& a, float h, float r)
        {
            Set(c, a, h, r);
        }

        /**
         * Set cylinder parameters
         * @param c Center position
         * @param a Axis direction (will be normalized)
         * @param h Height
         * @param r Radius
         */
        void Set(const Vector3f& c, const Vector3f& a, float h, float r)
        {
            center = c;
            axis = Normalized(a);  // Ensure axis is unit vector
            height = h;
            radius = r;
        }

        /** Get cylinder center */
        const Vector3f& GetCenter() const { return center; }
        
        /** Get cylinder axis (unit vector) */
        const Vector3f& GetAxis() const { return axis; }
        
        /** Get cylinder height */
        float GetHeight() const { return height; }
        
        /** Get cylinder radius */
        float GetRadius() const { return radius; }

        /**
         * Get top cap center
         * Top is along positive axis direction
         * @return Center of top circular cap
         */
        Vector3f GetTopCenter() const
        {
            return center + axis * (height * 0.5f);
        }

        /**
         * Get bottom cap center
         * Bottom is along negative axis direction
         * @return Center of bottom circular cap
         */
        Vector3f GetBottomCenter() const
        {
            return center - axis * (height * 0.5f);
        }

        /**
         * Calculate cylinder volume
         * Formula: V = πr²h
         * @return Volume
         */
        float GetVolume() const
        {
            return pi * radius * radius * height;
        }

        /**
         * Calculate cylinder surface area (including caps)
         * Formula: S = 2πr(r + h) = lateral area + cap areas
         * @return Total surface area
         */
        float GetSurfaceArea() const
        {
            return 2.0f * pi * radius * (radius + height);
        }

        /**
         * Test if point is inside cylinder
         * Checks:
         * 1. Point is within height range along axis
         * 2. Radial distance from axis <= radius
         * @param point Point to test
         * @return true if inside or on surface
         */
        bool ContainsPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - center;
            
            // Project onto axis
            float axis_projection = Dot(to_point, axis);
            
            // Check height bounds
            float half_height = height * 0.5f;
            if (abs(axis_projection) > half_height)
                return false;
            
            // Check radial distance
            Vector3f axis_point = center + axis * axis_projection;
            float radial_distance = Length(point - axis_point);
            
            return radial_distance <= radius;
        }

        /**
         * Find closest point on cylinder surface
         * @param point Point in space
         * @return Closest point on surface
         */
        Vector3f ClosestPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - center;
            float axis_projection = Dot(to_point, axis);
            
            // Clamp to height range
            float half_height = height * 0.5f;
            axis_projection = clamp(axis_projection, -half_height, half_height);
            
            Vector3f axis_point = center + axis * axis_projection;
            Vector3f radial = point - axis_point;
            float radial_length = Length(radial);
            
            if (radial_length <= radius)
            {
                // Point inside - find nearest surface
                float top_dist = half_height - axis_projection;
                float bottom_dist = half_height + axis_projection;
                float radial_dist = radius - radial_length;
                
                if (radial_dist < top_dist && radial_dist < bottom_dist)
                {
                    // Nearest is lateral surface
                    return axis_point + glm::normalize(radial) * radius;
                }
                else if (top_dist < bottom_dist)
                {
                    // Nearest is top cap
                    return center + axis * half_height + radial;
                }
                else
                {
                    // Nearest is bottom cap
                    return center - axis * half_height + radial;
                }
            }
            else
            {
                // Point outside - project to lateral surface
                return axis_point + glm::normalize(radial) * radius;
            }
        }

        /**
         * Calculate distance from point to cylinder surface
         * @param point Point in space
         * @return Distance to surface (0 if inside)
         */
        float DistanceToPoint(const Vector3f& point) const
        {
            Vector3f closest = ClosestPoint(point);
            return Length(point - closest);
        }

        /**
         * Get axis-aligned bounding box
         * @return AABB containing this cylinder
         */
        AABB GetBoundingBox() const
        {
            // Calculate the 8 corner points of the bounding cylinder
            // by considering the caps and maximum radial extent
            Vector3f half_axis = axis * (height * 0.5f);
            Vector3f top_center = center + half_axis;
            Vector3f bottom_center = center - half_axis;
            
            // For axis-aligned bbox, we need maximum extent in all directions
            // This is conservative but correct for any axis orientation
            Vector3f extent(radius, radius, radius);
            
            // Account for axis direction by adding half-height extent
            extent += glm::abs(half_axis);
            
            AABB box;
            box.SetMinMax(center - extent, center + extent);
            return box;
        }

    };//class Cylinder

}//namespace hgl::math
