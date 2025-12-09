#include <hgl/math/geometry/BoundingSphere.h>
#include <hgl/math/geometry/Ray.h>
#include <hgl/math/geometry/Plane.h>

namespace hgl::math
{
    void BoundingSphere::SetFromPoints(const float *pts,const uint32 count,const uint32 component_count)
    {
        Clear();

        if(pts==nullptr||count==0)
            return;
    
        glm::vec3 c(0.0f);

        const float *p=pts;
        for(uint32 i=0;i<count;++i)
        {
            c.x += p[0];
            c.y += p[1];
            c.z += p[2];

            p+=component_count;
        }
        c /= static_cast<float>(count);
    
        float r=0.0f;

        p=pts;

        for(uint32 i=0;i<count;++i)
        {
            r = hgl_max(r, glm::length(Vector3f(p[0] - c.x,
                                                 p[1] - c.y,
                                                 p[2] - c.z)));

            p+=component_count;
        }
    
        center = c;
        radius = r;
    }

    // ============================================================================
    // 碰撞检测实现
    // ============================================================================

    void BoundingSphere::Merge(const BoundingSphere &other)
    {
        if (other.IsEmpty())
            return;

        if (IsEmpty())
        {
            *this = other;
            return;
        }

        Vector3f d = other.center - center;
        float dist = glm::length(d);

        // 如果一个球包含另一个
        if (dist + other.radius <= radius)
            return;  // 当前球已包含另一个球

        if (dist + radius <= other.radius)
        {
            *this = other;  // 另一个球包含当前球
            return;
        }

        // 计算新的球
        float new_radius = (dist + radius + other.radius) * 0.5f;
        float t = (new_radius - radius) / dist;
        center = center + d * t;
        radius = new_radius;
    }

    bool BoundingSphere::IntersectsRay(const Ray &ray, float &distance) const
    {
        Vector3f m = ray.origin - center;
        float b = glm::dot(m, ray.direction);
        float c = glm::dot(m, m) - radius * radius;

        // 如果射线起点在球外且射线指向远离球的方向
        if (c > 0.0f && b > 0.0f)
            return false;

        float discr = b * b - c;

        // 负判别式意味着射线未击中球
        if (discr < 0.0f)
            return false;

        // 射线击中球，计算最小t值
        distance = -b - glm::sqrt(discr);

        // 如果t为负，射线起点在球内
        if (distance < 0.0f)
            distance = 0.0f;

        return true;
    }

    bool BoundingSphere::IntersectsRay(const Ray &ray) const
    {
        float distance;
        return IntersectsRay(ray, distance);
    }

    int BoundingSphere::ClassifyPlane(const Plane &plane) const
    {
        float d = plane.Distance(center);

        if (d > radius)
            return 1;   // 完全在平面前面
        else if (d < -radius)
            return -1;  // 完全在平面后面
        else
            return 0;   // 相交
    }

    bool BoundingSphere::IntersectsPlane(const Plane &plane) const
    {
        return ClassifyPlane(plane) == 0;
    }

    void BoundingSphere::ExpandToInclude(const math::Vector3f &point)
    {
        if (IsEmpty())
        {
            center = point;
            radius = 0.0f;
            return;
        }

        float dist = glm::length(point - center);
        if (dist > radius)
            radius = dist;
    }

}//namespace hgl::math
