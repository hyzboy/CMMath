/**
 * RaycastQuery.cpp - Implementation of ray intersection tests
 * 
 * Implements ray-geometry intersection algorithms.
 */
#include<hgl/math/geometry/queries/RaycastQuery.h>
#include<hgl/math/MathUtils.h>

namespace hgl::math
{
    //=============================================================================
    // Ray-Sphere intersection
    //=============================================================================
    
    bool RaycastQuery::Intersects(const Ray& ray, const Sphere& sphere, float& t)
    {
        Vector3f oc = ray.origin - sphere.GetCenter();
        float a = Dot(ray.direction, ray.direction);
        float b = 2.0f * Dot(oc, ray.direction);
        float c = Dot(oc, oc) - sphere.GetRadius() * sphere.GetRadius();
        float discriminant = b * b - 4 * a * c;
        
        if (discriminant < 0)
            return false;
        
        float sqrtDisc = sqrt(discriminant);
        t = (-b - sqrtDisc) / (2.0f * a);
        
        if (t < 0)
        {
            t = (-b + sqrtDisc) / (2.0f * a);
            if (t < 0)
                return false;
        }
        
        return true;
    }
    
    bool RaycastQuery::Intersects(const Ray& ray, const Sphere& sphere)
    {
        float t;
        return Intersects(ray, sphere, t);
    }
    
    RaycastHit RaycastQuery::Test(const Ray& ray, const Sphere& sphere)
    {
        RaycastHit hit;
        
        if (Intersects(ray, sphere, hit.distance))
        {
            hit.hit = true;
            hit.point = ray.origin + ray.direction * hit.distance;
            hit.normal = Normalized(hit.point - sphere.GetCenter());
        }
        
        return hit;
    }
    
    //=============================================================================
    // Ray-Capsule intersection
    //=============================================================================
    
    bool RaycastQuery::Intersects(const Ray& ray, const Capsule& capsule, float& t)
    {
        // Simplified implementation: treat as swept sphere
        // Find closest point on capsule axis to ray
        // Then do sphere intersection test
        
        // For now, use a conservative approximation
        // Full implementation would solve the quadratic equation for capsule
        
        // Test against bounding sphere as approximation
        Vector3f center = capsule.GetCenter();
        float boundingRadius = capsule.GetRadius() + capsule.GetCylinderHeight() * 0.5f;
        Sphere boundingSphere(center, boundingRadius);
        
        return Intersects(ray, boundingSphere, t);
    }
    
    bool RaycastQuery::Intersects(const Ray& ray, const Capsule& capsule)
    {
        float t;
        return Intersects(ray, capsule, t);
    }
    
    RaycastHit RaycastQuery::Test(const Ray& ray, const Capsule& capsule)
    {
        RaycastHit hit;
        
        if (Intersects(ray, capsule, hit.distance))
        {
            hit.hit = true;
            hit.point = ray.origin + ray.direction * hit.distance;
            hit.normal = Normalized(capsule.ClosestPoint(hit.point) - hit.point);
            if (Length(hit.normal) < 0.0001f)
                hit.normal = Vector3f(0, 1, 0);
            else
                hit.normal = -hit.normal;
        }
        
        return hit;
    }
    
    //=============================================================================
    // Ray-Cylinder intersection (stub implementations)
    //=============================================================================
    
    bool RaycastQuery::Intersects(const Ray& ray, const Cylinder& cylinder, float& t)
    {
        // Stub: Use bounding sphere approximation
        Vector3f center = cylinder.GetCenter();
        float boundingRadius = sqrt(cylinder.GetRadius() * cylinder.GetRadius() + 
                                   cylinder.GetHeight() * cylinder.GetHeight() * 0.25f);
        Sphere boundingSphere(center, boundingRadius);
        return Intersects(ray, boundingSphere, t);
    }
    
    bool RaycastQuery::Intersects(const Ray& ray, const Cylinder& cylinder)
    {
        float t;
        return Intersects(ray, cylinder, t);
    }
    
    RaycastHit RaycastQuery::Test(const Ray& ray, const Cylinder& cylinder)
    {
        RaycastHit hit;
        if (Intersects(ray, cylinder, hit.distance))
        {
            hit.hit = true;
            hit.point = ray.origin + ray.direction * hit.distance;
            hit.normal = Vector3f(0, 1, 0);
        }
        return hit;
    }
    
    //=============================================================================
    // Ray-Cone intersection (stub implementations)
    //=============================================================================
    
    bool RaycastQuery::Intersects(const Ray& ray, const Cone& cone, float& t)
    {
        // Stub implementation
        Vector3f center = cone.GetCenter();
        float boundingRadius = std::max(cone.GetBaseRadius(), cone.GetHeight());
        Sphere boundingSphere(center, boundingRadius);
        return Intersects(ray, boundingSphere, t);
    }
    
    bool RaycastQuery::Intersects(const Ray& ray, const Cone& cone)
    {
        float t;
        return Intersects(ray, cone, t);
    }
    
    RaycastHit RaycastQuery::Test(const Ray& ray, const Cone& cone)
    {
        RaycastHit hit;
        if (Intersects(ray, cone, hit.distance))
        {
            hit.hit = true;
            hit.point = ray.origin + ray.direction * hit.distance;
            hit.normal = Vector3f(0, 1, 0);
        }
        return hit;
    }
    
    //=============================================================================
    // Ray-Torus intersection (stub implementations)
    //=============================================================================
    
    bool RaycastQuery::Intersects(const Ray& ray, const Torus& torus, float& t)
    {
        // Stub implementation
        Vector3f center = torus.GetCenter();
        float boundingRadius = torus.GetMajorRadius() + torus.GetMinorRadius();
        Sphere boundingSphere(center, boundingRadius);
        return Intersects(ray, boundingSphere, t);
    }
    
    bool RaycastQuery::Intersects(const Ray& ray, const Torus& torus)
    {
        float t;
        return Intersects(ray, torus, t);
    }
    
    RaycastHit RaycastQuery::Test(const Ray& ray, const Torus& torus)
    {
        RaycastHit hit;
        if (Intersects(ray, torus, hit.distance))
        {
            hit.hit = true;
            hit.point = ray.origin + ray.direction * hit.distance;
            hit.normal = Vector3f(0, 1, 0);
        }
        return hit;
    }
    
    //=============================================================================
    // Ray-AABB intersection
    //=============================================================================
    
    bool RaycastQuery::Intersects(const Ray& ray, const AABB& box, float& t)
    {
        // Use slab method
        Vector3f invDir = 1.0f / ray.direction;
        Vector3f t0 = (box.GetMin() - ray.origin) * invDir;
        Vector3f t1 = (box.GetMax() - ray.origin) * invDir;
        
        Vector3f tmin = glm::min(t0, t1);
        Vector3f tmax = glm::max(t0, t1);
        
        float tNear = std::max(std::max(tmin.x, tmin.y), tmin.z);
        float tFar = std::min(std::min(tmax.x, tmax.y), tmax.z);
        
        if (tNear > tFar || tFar < 0)
            return false;
        
        t = tNear >= 0 ? tNear : tFar;
        return t >= 0;
    }
    
    bool RaycastQuery::Intersects(const Ray& ray, const AABB& box)
    {
        float t;
        return Intersects(ray, box, t);
    }
    
    RaycastHit RaycastQuery::Test(const Ray& ray, const AABB& box)
    {
        RaycastHit hit;
        if (Intersects(ray, box, hit.distance))
        {
            hit.hit = true;
            hit.point = ray.origin + ray.direction * hit.distance;
            
            // Determine which face was hit
            Vector3f localPoint = hit.point - box.GetCenter();
            Vector3f absLocal = glm::abs(localPoint);
            Vector3f halfSize = box.GetLength() * 0.5f;
            
            if (absLocal.x > absLocal.y && absLocal.x > absLocal.z)
                hit.normal = Vector3f(localPoint.x > 0 ? 1 : -1, 0, 0);
            else if (absLocal.y > absLocal.z)
                hit.normal = Vector3f(0, localPoint.y > 0 ? 1 : -1, 0);
            else
                hit.normal = Vector3f(0, 0, localPoint.z > 0 ? 1 : -1);
        }
        return hit;
    }
    
    //=============================================================================
    // Ray-OBB intersection (stub)
    //=============================================================================
    
    bool RaycastQuery::Intersects(const Ray& ray, const OBB& box, float& t)
    {
        // Stub: use AABB of the OBB
        AABB aabb;
        aabb.SetMinMax(box.GetMin(), box.GetMax());
        return Intersects(ray, aabb, t);
    }
    
    bool RaycastQuery::Intersects(const Ray& ray, const OBB& box)
    {
        float t;
        return Intersects(ray, box, t);
    }
    
    RaycastHit RaycastQuery::Test(const Ray& ray, const OBB& box)
    {
        RaycastHit hit;
        if (Intersects(ray, box, hit.distance))
        {
            hit.hit = true;
            hit.point = ray.origin + ray.direction * hit.distance;
            hit.normal = Vector3f(0, 1, 0);
        }
        return hit;
    }
    
    //=============================================================================
    // Ray-Plane intersection
    //=============================================================================
    
    bool RaycastQuery::Intersects(const Ray& ray, const Plane& plane, float& t)
    {
        float denom = Dot(plane.normal, ray.direction);
        
        if (abs(denom) < 0.0001f)
            return false;  // Ray parallel to plane
        
        t = -(Dot(plane.normal, ray.origin) + plane.d) / denom;
        return t >= 0;
    }
    
    bool RaycastQuery::Intersects(const Ray& ray, const Plane& plane)
    {
        float t;
        return Intersects(ray, plane, t);
    }
    
    RaycastHit RaycastQuery::Test(const Ray& ray, const Plane& plane)
    {
        RaycastHit hit;
        if (Intersects(ray, plane, hit.distance))
        {
            hit.hit = true;
            hit.point = ray.origin + ray.direction * hit.distance;
            hit.normal = plane.normal;
        }
        return hit;
    }
    
    //=============================================================================
    // Ray-Triangle intersection (Möller-Trumbore)
    //=============================================================================
    
    bool RaycastQuery::IntersectsTriangle(const Ray& ray,
                                         const Vector3f& v0,
                                         const Vector3f& v1,
                                         const Vector3f& v2,
                                         float& t, float& u, float& v)
    {
        const float EPSILON = 0.0000001f;
        
        Vector3f edge1 = v1 - v0;
        Vector3f edge2 = v2 - v0;
        Vector3f h = Cross(ray.direction, edge2);
        float a = Dot(edge1, h);
        
        if (a > -EPSILON && a < EPSILON)
            return false;  // Ray parallel to triangle
        
        float f = 1.0f / a;
        Vector3f s = ray.origin - v0;
        u = f * Dot(s, h);
        
        if (u < 0.0f || u > 1.0f)
            return false;
        
        Vector3f q = Cross(s, edge1);
        v = f * Dot(ray.direction, q);
        
        if (v < 0.0f || u + v > 1.0f)
            return false;
        
        t = f * Dot(edge2, q);
        return t > EPSILON;
    }
    
    bool RaycastQuery::IntersectsTriangle(const Ray& ray,
                                         const Vector3f& v0,
                                         const Vector3f& v1,
                                         const Vector3f& v2,
                                         float& t)
    {
        float u, v;
        return IntersectsTriangle(ray, v0, v1, v2, t, u, v);
    }

}//namespace hgl::math
