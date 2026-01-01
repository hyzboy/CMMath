/**
 * CollisionDetector.cpp - Implementation of collision detection methods
 * 
 * Implements intersection tests between various geometry primitives.
 * Focus on efficient, numerically stable algorithms.
 */
#include<hgl/math/geometry/queries/CollisionDetector.h>
#include<hgl/math/MathUtils.h>

namespace hgl::math
{
    //=============================================================================
    // Sphere-Sphere collision
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Sphere& a, const Sphere& b)
    {
        float sumRadii = a.GetRadius() + b.GetRadius();
        float distSq = LengthSquared(a.GetCenter() - b.GetCenter());
        return distSq <= sumRadii * sumRadii;
    }
    
    CollisionInfo CollisionDetector::TestCollision(const Sphere& a, const Sphere& b)
    {
        CollisionInfo info;
        
        Vector3f centerDiff = b.GetCenter() - a.GetCenter();
        float dist = Length(centerDiff);
        float sumRadii = a.GetRadius() + b.GetRadius();
        
        info.intersects = dist <= sumRadii;
        info.distance = dist - sumRadii;
        
        if (dist > 0.0001f)
        {
            info.normal = centerDiff / dist;
        }
        else
        {
            // Spheres at same position - choose arbitrary normal
            info.normal = Vector3f(1, 0, 0);
        }
        
        if (info.intersects)
        {
            info.penetration = sumRadii - dist;
            // Contact point is between centers, weighted by radii
            float t = a.GetRadius() / sumRadii;
            info.point = a.GetCenter() + info.normal * a.GetRadius();
        }
        else
        {
            // Closest points on both spheres
            info.point = a.GetCenter() + info.normal * a.GetRadius();
        }
        
        return info;
    }
    
    //=============================================================================
    // Sphere-AABB collision
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Sphere& sphere, const AABB& box)
    {
        // Find closest point on AABB to sphere center
        Vector3f closest = box.ClampPoint(sphere.GetCenter());
        
        // Check if distance to closest point is less than radius
        float distSq = LengthSquared(sphere.GetCenter() - closest);
        return distSq <= sphere.GetRadius() * sphere.GetRadius();
    }
    
    //=============================================================================
    // Sphere-OBB collision
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Sphere& sphere, const OBB& box)
    {
        // Find closest point on OBB to sphere center
        Vector3f closest = box.ClosestPoint(sphere.GetCenter());
        
        // Check if distance to closest point is less than radius
        float distSq = LengthSquared(sphere.GetCenter() - closest);
        return distSq <= sphere.GetRadius() * sphere.GetRadius();
    }
    
    //=============================================================================
    // Sphere-Capsule collision
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Sphere& sphere, const Capsule& capsule)
    {
        // Find closest point on capsule's line segment to sphere center
        Vector3f segment = capsule.GetEnd() - capsule.GetStart();
        Vector3f toSphere = sphere.GetCenter() - capsule.GetStart();
        
        float t = Dot(toSphere, segment) / Dot(segment, segment);
        t = clamp(t, 0.0f, 1.0f);
        
        Vector3f closestOnSegment = capsule.GetStart() + segment * t;
        
        // Check if distance between sphere center and closest point is less than sum of radii
        float sumRadii = sphere.GetRadius() + capsule.GetRadius();
        float distSq = LengthSquared(sphere.GetCenter() - closestOnSegment);
        return distSq <= sumRadii * sumRadii;
    }
    
    //=============================================================================
    // Capsule-Capsule collision
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Capsule& a, const Capsule& b)
    {
        // Find closest points between the two line segments
        Vector3f seg1 = a.GetEnd() - a.GetStart();
        Vector3f seg2 = b.GetEnd() - b.GetStart();
        Vector3f diff = a.GetStart() - b.GetStart();
        
        float a01 = -Dot(seg1, seg2);
        float b0 = Dot(seg1, diff);
        float c = Dot(diff, diff);
        float det = abs(1.0f - a01 * a01);
        
        float s, t;
        if (det >= 0.0001f)
        {
            float b1 = -Dot(seg2, diff);
            s = (a01 * b1 - b0) / det;
            t = (a01 * b0 - b1) / det;
            
            s = clamp(s, 0.0f, 1.0f);
            t = clamp(t, 0.0f, 1.0f);
        }
        else
        {
            // Segments are parallel
            s = 0.0f;
            t = clamp(-b0, 0.0f, 1.0f);
        }
        
        Vector3f closest1 = a.GetStart() + seg1 * s;
        Vector3f closest2 = b.GetStart() + seg2 * t;
        
        float sumRadii = a.GetRadius() + b.GetRadius();
        float distSq = LengthSquared(closest1 - closest2);
        return distSq <= sumRadii * sumRadii;
    }
    
    CollisionInfo CollisionDetector::TestCollision(const Capsule& a, const Capsule& b)
    {
        CollisionInfo info;
        
        // Find closest points between the two line segments (same as above)
        Vector3f seg1 = a.GetEnd() - a.GetStart();
        Vector3f seg2 = b.GetEnd() - b.GetStart();
        Vector3f diff = a.GetStart() - b.GetStart();
        
        float a01 = -Dot(seg1, seg2);
        float b0 = Dot(seg1, diff);
        float c = Dot(diff, diff);
        float det = abs(1.0f - a01 * a01);
        
        float s, t;
        if (det >= 0.0001f)
        {
            float b1 = -Dot(seg2, diff);
            s = (a01 * b1 - b0) / det;
            t = (a01 * b0 - b1) / det;
            
            s = clamp(s, 0.0f, 1.0f);
            t = clamp(t, 0.0f, 1.0f);
        }
        else
        {
            s = 0.0f;
            t = clamp(-b0, 0.0f, 1.0f);
        }
        
        Vector3f closest1 = a.GetStart() + seg1 * s;
        Vector3f closest2 = b.GetStart() + seg2 * t;
        
        Vector3f centerDiff = closest2 - closest1;
        float dist = Length(centerDiff);
        float sumRadii = a.GetRadius() + b.GetRadius();
        
        info.intersects = dist <= sumRadii;
        info.distance = dist - sumRadii;
        
        if (dist > 0.0001f)
        {
            info.normal = centerDiff / dist;
        }
        else
        {
            info.normal = Vector3f(1, 0, 0);
        }
        
        if (info.intersects)
        {
            info.penetration = sumRadii - dist;
            info.point = closest1 + info.normal * a.GetRadius();
        }
        else
        {
            info.point = closest1 + info.normal * a.GetRadius();
        }
        
        return info;
    }
    
    //=============================================================================
    // Capsule-AABB collision
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Capsule& capsule, const AABB& box)
    {
        // Check if capsule's line segment intersects expanded AABB
        // or if any point on line segment is within capsule radius of AABB
        
        Vector3f seg = capsule.GetEnd() - capsule.GetStart();
        float segLength = Length(seg);
        
        if (segLength < 0.0001f)
        {
            // Degenerate capsule - just test as sphere
            Sphere sphere(capsule.GetStart(), capsule.GetRadius());
            return Intersects(sphere, box);
        }
        
        // Sample points along capsule segment and check distance to AABB
        const int samples = 8;
        for (int i = 0; i <= samples; ++i)
        {
            float t = static_cast<float>(i) / samples;
            Vector3f point = capsule.GetStart() + seg * t;
            Vector3f closest = box.ClampPoint(point);
            
            if (LengthSquared(point - closest) <= capsule.GetRadius() * capsule.GetRadius())
                return true;
        }
        
        return false;
    }
    
    //=============================================================================
    // Capsule-OBB collision
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Capsule& capsule, const OBB& box)
    {
        // Similar to AABB, but use OBB's closest point method
        Vector3f seg = capsule.GetEnd() - capsule.GetStart();
        float segLength = Length(seg);
        
        if (segLength < 0.0001f)
        {
            Sphere sphere(capsule.GetStart(), capsule.GetRadius());
            return Intersects(sphere, box);
        }
        
        const int samples = 8;
        for (int i = 0; i <= samples; ++i)
        {
            float t = static_cast<float>(i) / samples;
            Vector3f point = capsule.GetStart() + seg * t;
            Vector3f closest = box.ClosestPoint(point);
            
            if (LengthSquared(point - closest) <= capsule.GetRadius() * capsule.GetRadius())
                return true;
        }
        
        return false;
    }
    
    //=============================================================================
    // Capsule-Cylinder collision
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Capsule& capsule, const Cylinder& cylinder)
    {
        // Simplified test: sample points along capsule and check cylinder containment
        Vector3f seg = capsule.GetEnd() - capsule.GetStart();
        float segLength = Length(seg);
        
        if (segLength < 0.0001f)
        {
            Sphere sphere(capsule.GetStart(), capsule.GetRadius());
            return Intersects(sphere, cylinder);
        }
        
        const int samples = 8;
        for (int i = 0; i <= samples; ++i)
        {
            float t = static_cast<float>(i) / samples;
            Vector3f point = capsule.GetStart() + seg * t;
            
            // Check if expanded point (with radius) is close to cylinder
            float dist = cylinder.DistanceToPoint(point);
            if (dist <= capsule.GetRadius())
                return true;
        }
        
        return false;
    }
    
    //=============================================================================
    // Cylinder collision methods
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Cylinder& a, const Cylinder& b)
    {
        // Simplified cylinder-cylinder test
        // Check if bounding boxes intersect first (broad phase)
        if (!a.GetBoundingBox().Intersects(b.GetBoundingBox()))
            return false;
        
        // For now, use conservative test by sampling points
        // A full implementation would use separating axis theorem
        Vector3f centerA = a.GetCenter();
        Vector3f centerB = b.GetCenter();
        
        // Check center distances as a quick conservative test
        float dist = Length(centerA - centerB);
        float maxDist = a.GetRadius() + b.GetRadius() + 
                        (a.GetHeight() + b.GetHeight()) * 0.5f;
        
        return dist <= maxDist;
    }
    
    bool CollisionDetector::Intersects(const Cylinder& cylinder, const Sphere& sphere)
    {
        float dist = cylinder.DistanceToPoint(sphere.GetCenter());
        return dist <= sphere.GetRadius();
    }
    
    bool CollisionDetector::Intersects(const Cylinder& cylinder, const AABB& box)
    {
        // Conservative test using bounding boxes
        if (!cylinder.GetBoundingBox().Intersects(box))
            return false;
        
        // Check if cylinder center is in box
        if (box.ContainsPoint(cylinder.GetCenter()))
            return true;
        
        // Check if any box corner is in cylinder
        Vector3f corners[8];
        box.GetCorners(corners);
        for (int i = 0; i < 8; ++i)
        {
            if (cylinder.ContainsPoint(corners[i]))
                return true;
        }
        
        return false;
    }
    
    //=============================================================================
    // Cone collision methods
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Cone& cone, const Sphere& sphere)
    {
        float dist = cone.DistanceToPoint(sphere.GetCenter());
        return dist <= sphere.GetRadius();
    }
    
    bool CollisionDetector::Intersects(const Cone& cone, const AABB& box)
    {
        // Conservative test
        if (!cone.GetBoundingBox().Intersects(box))
            return false;
        
        // Check if cone apex is in box
        if (box.ContainsPoint(cone.GetApex()))
            return true;
        
        // Check if any box corner is in cone
        Vector3f corners[8];
        box.GetCorners(corners);
        for (int i = 0; i < 8; ++i)
        {
            if (cone.ContainsPoint(corners[i]))
                return true;
        }
        
        return false;
    }
    
    //=============================================================================
    // Torus collision methods
    //=============================================================================
    
    bool CollisionDetector::Intersects(const Torus& torus, const Sphere& sphere)
    {
        float dist = torus.DistanceToPoint(sphere.GetCenter());
        return dist <= sphere.GetRadius();
    }
    
    //=============================================================================
    // AABB-OBB collision
    //=============================================================================
    
    bool CollisionDetector::Intersects(const AABB& aabb, const OBB& obb)
    {
        // Use SAT (Separating Axis Theorem)
        // For now, delegate to OBB's existing intersection method if available
        return obb.Intersects(aabb);
    }

}//namespace hgl::math
