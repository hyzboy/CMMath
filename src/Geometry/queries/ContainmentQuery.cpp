/**
 * ContainmentQuery.cpp - Implementation of containment tests
 * 
 * Implements geometry containment checks.
 */
#include<hgl/math/geometry/queries/ContainmentQuery.h>
#include<hgl/math/MathUtils.h>

namespace hgl::math
{
    //=============================================================================
    // Sphere containment
    //=============================================================================
    
    bool ContainmentQuery::Contains(const Sphere& container, const Sphere& sphere)
    {
        // Container sphere must have center-to-center distance + sphere radius <= container radius
        float centerDist = Length(container.GetCenter() - sphere.GetCenter());
        return centerDist + sphere.GetRadius() <= container.GetRadius();
    }
    
    bool ContainmentQuery::Contains(const AABB& container, const Sphere& sphere)
    {
        // Sphere center must be inside AABB with margin >= radius
        Vector3f center = sphere.GetCenter();
        float radius = sphere.GetRadius();
        
        Vector3f minBound = container.GetMin() + Vector3f(radius, radius, radius);
        Vector3f maxBound = container.GetMax() - Vector3f(radius, radius, radius);
        
        return center.x >= minBound.x && center.x <= maxBound.x &&
               center.y >= minBound.y && center.y <= maxBound.y &&
               center.z >= minBound.z && center.z <= maxBound.z;
    }
    
    //=============================================================================
    // AABB containment
    //=============================================================================
    
    bool ContainmentQuery::Contains(const AABB& container, const AABB& box)
    {
        // All corners of box must be inside container
        return box.GetMin().x >= container.GetMin().x &&
               box.GetMin().y >= container.GetMin().y &&
               box.GetMin().z >= container.GetMin().z &&
               box.GetMax().x <= container.GetMax().x &&
               box.GetMax().y <= container.GetMax().y &&
               box.GetMax().z <= container.GetMax().z;
    }
    
    bool ContainmentQuery::Contains(const OBB& container, const AABB& box)
    {
        // Check if all 8 corners of AABB are inside OBB
        Vector3f corners[8];
        box.GetCorners(corners);
        
        for (int i = 0; i < 8; ++i)
        {
            if (!container.ContainsPoint(corners[i]))
                return false;
        }
        
        return true;
    }
    
    //=============================================================================
    // OBB containment
    //=============================================================================
    
    bool ContainmentQuery::Contains(const OBB& container, const OBB& box)
    {
        // Check if all 8 corners of box are inside container
        Vector3f corners[8];
        box.GetCorners(corners);
        
        for (int i = 0; i < 8; ++i)
        {
            if (!container.ContainsPoint(corners[i]))
                return false;
        }
        
        return true;
    }

}//namespace hgl::math
