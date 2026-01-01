/**
 * Cone.h - Simplified cone geometry primitive
 * 
 * A cone is formed by connecting a circular base to an apex point.
 * This lightweight class focuses on geometric properties and simple queries.
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>

namespace hgl::math
{
    /**
     * Cone - Circular cone with apex, axis, height, and base radius
     * 
     * Geometric definition:
     * - Apex: The tip of the cone
     * - Axis: Direction from apex to base center (normalized)
     * - Height: Distance from apex to base
     * - Base radius: Radius of circular base
     * 
     * Properties:
     * - Slant height = sqrt(h² + r²)
     * - Radius at height h' = r * (1 - h'/h)
     * - Cone angle = arctan(r/h)
     * 
     * Common uses:
     * - Cone-shaped objects
     * - Spotlight volumes
     * - Vision cones
     */
    class Cone
    {
        Vector3f apex;           // Cone apex (tip point)
        Vector3f axis;           // Axis direction (normalized)
        float height;            // Height from apex to base
        float base_radius;       // Base circle radius

    public:

        /**
         * Default constructor
         * Creates a cone at (0,1,0) pointing down, height 1.0, radius 0.5
         */
        Cone()
            : apex(0, 1, 0), axis(0, -1, 0), height(1.0f), base_radius(0.5f)
        {
        }

        /**
         * Parameterized constructor
         * @param a Apex position
         * @param ax Axis direction (will be normalized)
         * @param h Height
         * @param r Base radius
         */
        Cone(const Vector3f& a, const Vector3f& ax, float h, float r)
        {
            Set(a, ax, h, r);
        }

        /**
         * Set cone parameters
         * @param a Apex position
         * @param ax Axis direction (will be normalized)
         * @param h Height
         * @param r Base radius
         */
        void Set(const Vector3f& a, const Vector3f& ax, float h, float r)
        {
            apex = a;
            axis = Normalized(ax);
            height = h;
            base_radius = r;
        }

        /** Get apex position */
        const Vector3f& GetApex() const { return apex; }
        
        /** Get axis direction */
        const Vector3f& GetAxis() const { return axis; }
        
        /** Get height */
        float GetHeight() const { return height; }
        
        /** Get base radius */
        float GetBaseRadius() const { return base_radius; }

        /**
         * Get geometric center (1/4 height from base)
         * @return Center of mass position
         */
        Vector3f GetCenter() const
        {
            return apex + axis * (height * 0.75f);
        }

        /**
         * Get base center
         * @return Center of circular base
         */
        Vector3f GetBaseCenter() const
        {
            return apex + axis * height;
        }

        /**
         * Calculate cone volume
         * Formula: V = (1/3)πr²h
         * @return Volume
         */
        float GetVolume() const
        {
            return (1.0f / 3.0f) * pi * base_radius * base_radius * height;
        }

        /**
         * Calculate cone surface area (including base)
         * Formula: S = πr(r + s) where s is slant height
         * @return Total surface area
         */
        float GetSurfaceArea() const
        {
            float slant_height = sqrt(height * height + base_radius * base_radius);
            return pi * base_radius * (base_radius + slant_height);
        }

        /**
         * Calculate lateral surface area (excluding base)
         * @return Lateral surface area
         */
        float GetLateralArea() const
        {
            float slant_height = sqrt(height * height + base_radius * base_radius);
            return pi * base_radius * slant_height;
        }

        /**
         * Get radius at specific height from apex
         * @param h Height from apex (0 to height)
         * @return Radius at that height
         */
        float GetRadiusAtHeight(float h) const
        {
            if (h < 0.0f || h > height)
                return 0.0f;
            
            // Linear interpolation
            return base_radius * (h / height);
        }

        /**
         * Test if point is inside cone
         * @param point Point to test
         * @return true if inside or on surface
         */
        bool ContainsPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - apex;
            float axis_projection = Dot(to_point, axis);
            
            // Check height bounds
            if (axis_projection < 0.0f || axis_projection > height)
                return false;
            
            // Get radius at this height
            float radius_at_height = GetRadiusAtHeight(axis_projection);
            
            // Check radial distance
            Vector3f axis_point = apex + axis * axis_projection;
            float radial_distance = Length(point - axis_point);
            
            return radial_distance <= radius_at_height;
        }

        /**
         * Find closest point on cone surface
         * @param point Point in space
         * @return Closest point on surface
         */
        Vector3f ClosestPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - apex;
            float axis_projection = Dot(to_point, axis);
            
            if (axis_projection <= 0.0f)
            {
                // Point above apex
                return apex;
            }
            
            if (axis_projection >= height)
            {
                // Point below base
                Vector3f base_center = GetBaseCenter();
                Vector3f to_base = point - base_center;
                Vector3f radial = to_base - axis * Dot(to_base, axis);
                float radial_length = Length(radial);
                
                if (radial_length <= base_radius)
                {
                    // Projects inside base circle
                    return point - axis * Dot(to_base, axis);
                }
                else
                {
                    // Projects outside base circle
                    return base_center + glm::normalize(radial) * base_radius;
                }
            }
            
            // Point within height range
            Vector3f axis_point = apex + axis * axis_projection;
            float radius_at_height = GetRadiusAtHeight(axis_projection);
            
            Vector3f radial = point - axis_point;
            float radial_length = Length(radial);
            
            if (radial_length <= radius_at_height)
            {
                // Point inside - return point on lateral surface
                if (radial_length < 0.0001f)
                {
                    // Point on axis - choose arbitrary direction
                    Vector3f perpendicular;
                    if (abs(axis.x) < 0.9f)
                        perpendicular = Cross(axis, Vector3f(1, 0, 0));
                    else
                        perpendicular = Cross(axis, Vector3f(0, 1, 0));
                    
                    return axis_point + glm::normalize(perpendicular) * radius_at_height;
                }
                else
                {
                    return axis_point + glm::normalize(radial) * radius_at_height;
                }
            }
            else
            {
                // Point outside
                return axis_point + glm::normalize(radial) * radius_at_height;
            }
        }

        /**
         * Calculate distance from point to cone surface
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
         * @return AABB containing this cone
         */
        AABB GetBoundingBox() const
        {
            Vector3f base_center = GetBaseCenter();
            
            // Conservative bounding box considering apex and base
            Vector3f r(base_radius, base_radius, base_radius);
            Vector3f min_pt = glm::min(apex, base_center - r);
            Vector3f max_pt = glm::max(apex, base_center + r);
            
            AABB box;
            box.SetMinMax(min_pt, max_pt);
            return box;
        }

    };//class Cone

}//namespace hgl::math
