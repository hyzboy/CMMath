#include<hgl/math/geometry/Box.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/Frustum.h>
#include<hgl/math/geometry/Sphere.h>
#include<hgl/math/geometry/Ray.h>
#include<hgl/math/geometry/Plane.h>
#include<hgl/math/geometry/Triangle.h>
#include<glm/gtx/norm.inl>
#include<algorithm>

namespace hgl::math
{
    // ============================================================================
    // 构造函数实现
    // ============================================================================

    Box::Box(const AABB &aabb)
    {
        center = aabb.GetCenter();
        extent = aabb.GetLength() * 0.5f;
    }

    Box::Box(const OBB &obb)
    {
        center = obb.GetCenter();
        extent = obb.GetHalfExtend();
    }

    Box::Box(const Frustum &frustum)
    {
        // 从视锥体计算包围盒
        // 注意: 这需要获取视锥体的顶点,这里提供一个简化实现
        // 实际使用中可能需要根据具体需求调整
        
        // 暂时使用一个大的默认值
        // TODO: 实现真正的 Frustum 顶点提取
        center = Vector3f(0, 0, 0);
        extent = Vector3f(100, 100, 100);
    }

    void Box::GetCorners(math::Vector3f corners[8]) const
    {
        const math::Vector3f min_pt = center - extent;
        const math::Vector3f max_pt = center + extent;

        corners[0] = Vector3f(min_pt.x, min_pt.y, min_pt.z);
        corners[1] = Vector3f(max_pt.x, min_pt.y, min_pt.z);
        corners[2] = Vector3f(min_pt.x, max_pt.y, min_pt.z);
        corners[3] = Vector3f(max_pt.x, max_pt.y, min_pt.z);
        corners[4] = Vector3f(min_pt.x, min_pt.y, max_pt.z);
        corners[5] = Vector3f(max_pt.x, min_pt.y, max_pt.z);
        corners[6] = Vector3f(min_pt.x, max_pt.y, max_pt.z);
        corners[7] = Vector3f(max_pt.x, max_pt.y, max_pt.z);
    }

    // ============================================================================
    // 点相关碰撞检测
    // ============================================================================

    bool Box::ContainsPoint(const math::Vector3f &point) const
    {
        math::Vector3f d = glm::abs(point - center);
        return d.x <= extent.x && d.y <= extent.y && d.z <= extent.z;
    }

    math::Vector3f Box::ClosestPoint(const math::Vector3f &point) const
    {
        math::Vector3f min_pt = center - extent;
        math::Vector3f max_pt = center + extent;
        return glm::clamp(point, min_pt, max_pt);
    }

    float Box::DistanceToPoint(const math::Vector3f &point) const
    {
        return glm::sqrt(DistanceToPointSquared(point));
    }

    float Box::DistanceToPointSquared(const math::Vector3f &point) const
    {
        math::Vector3f closest = ClosestPoint(point);
        return glm::length2(point - closest);
    }

    // ============================================================================
    // Box vs Box 碰撞检测
    // ============================================================================

    bool Box::Intersects(const Box &other) const
    {
        math::Vector3f d = glm::abs(center - other.center);
        math::Vector3f sum_extent = extent + other.extent;
        
        return d.x <= sum_extent.x && 
               d.y <= sum_extent.y && 
               d.z <= sum_extent.z;
    }

    bool Box::Contains(const Box &other) const
    {
        math::Vector3f d = glm::abs(center - other.center);
        math::Vector3f diff_extent = extent - other.extent;
        
        return d.x <= diff_extent.x && 
               d.y <= diff_extent.y && 
               d.z <= diff_extent.z;
    }

    bool Box::GetIntersection(const Box &other, Box &intersection) const
    {
        if (!Intersects(other))
            return false;

        math::Vector3f min1 = GetMin();
        math::Vector3f max1 = GetMax();
        math::Vector3f min2 = other.GetMin();
        math::Vector3f max2 = other.GetMax();

        math::Vector3f int_min = MaxVector(min1, min2);
        math::Vector3f int_max = MinVector(max1, max2);

        intersection.SetMinMax(int_min, int_max);
        return true;
    }

    void Box::Merge(const Box &other)
    {
        math::Vector3f min1 = GetMin();
        math::Vector3f max1 = GetMax();
        math::Vector3f min2 = other.GetMin();
        math::Vector3f max2 = other.GetMax();

        SetMinMax(MinVector(min1, min2), MaxVector(max1, max2));
    }

    float Box::Distance(const Box &other) const
    {
        if (Intersects(other))
            return 0.0f;

        math::Vector3f delta(0.0f);
        math::Vector3f min1 = GetMin();
        math::Vector3f max1 = GetMax();
        math::Vector3f min2 = other.GetMin();
        math::Vector3f max2 = other.GetMax();

        // X轴
        if (max1.x < min2.x) delta.x = min2.x - max1.x;
        else if (min1.x > max2.x) delta.x = min1.x - max2.x;

        // Y轴
        if (max1.y < min2.y) delta.y = min2.y - max1.y;
        else if (min1.y > max2.y) delta.y = min1.y - max2.y;

        // Z轴
        if (max1.z < min2.z) delta.z = min2.z - max1.z;
        else if (min1.z > max2.z) delta.z = min1.z - max2.z;

        return glm::length(delta);
    }

    // ============================================================================
    // Sphere 碰撞检测
    // ============================================================================

    bool Box::IntersectsSphere(const math::Vector3f &sphere_center, float sphere_radius) const
    {
        float distSq = DistanceToPointSquared(sphere_center);
        return distSq <= sphere_radius * sphere_radius;
    }

    bool Box::IntersectsSphere(const Sphere &sphere) const
    {
        return IntersectsSphere(sphere.GetCenter(), sphere.GetRadius());
    }

    bool Box::IntersectsEllipseSphere(const EllipseSphere &ellipse) const
    {
        // 将椭球归一化为球体进行检测
        math::Vector3f scaled_center = ellipse.GetCenter() / ellipse.GetRadius();
        math::Vector3f scaled_box_center = center / ellipse.GetRadius();
        math::Vector3f scaled_extent = extent / ellipse.GetRadius();

        Box scaled_box(scaled_box_center, scaled_extent);
        return scaled_box.IntersectsSphere(scaled_center, 1.0f);
    }

    // ============================================================================
    // Ray 碰撞检测
    // ============================================================================

    bool Box::IntersectsRay(const Ray &ray, float &distance) const
    {
        float t_min, t_max;
        if (IntersectsRay(ray, t_min, t_max))
        {
            distance = t_min >= 0.0f ? t_min : t_max;
            return distance >= 0.0f;
        }
        return false;
    }

    bool Box::IntersectsRay(const Ray &ray) const
    {
        float t_min, t_max;
        return IntersectsRay(ray, t_min, t_max);
    }

    bool Box::IntersectsRay(const Ray &ray, float &t_min, float &t_max) const
    {
        math::Vector3f min_pt = center - extent;
        math::Vector3f max_pt = center + extent;

        t_min = 0.0f;
        t_max = std::numeric_limits<float>::infinity();

        for (int i = 0; i < 3; i++)
        {
            if (std::abs(ray.direction[i]) < 1e-8f)
            {
                // 射线平行于该轴的平板
                if (ray.origin[i] < min_pt[i] || ray.origin[i] > max_pt[i])
                    return false;
            }
            else
            {
                float inv_d = 1.0f / ray.direction[i];
                float t1 = (min_pt[i] - ray.origin[i]) * inv_d;
                float t2 = (max_pt[i] - ray.origin[i]) * inv_d;

                if (t1 > t2) std::swap(t1, t2);

                t_min = std::max(t_min, t1);
                t_max = std::min(t_max, t2);

                if (t_min > t_max)
                    return false;
            }
        }

        return t_max >= 0.0f;
    }

    // ============================================================================
    // Plane 碰撞检测
    // ============================================================================

    int Box::ClassifyPlane(const Plane &plane) const
    {
        // 计算盒子在平面法线方向的"半径"
        float r = extent.x * std::abs(plane.normal.x) +
                  extent.y * std::abs(plane.normal.y) +
                  extent.z * std::abs(plane.normal.z);

        // 计算中心到平面的距离
        float d = plane.Distance(center);

        if (d > r)
            return 1;   // 完全在平面前面
        else if (d < -r)
            return -1;  // 完全在平面后面
        else
            return 0;   // 相交
    }

    bool Box::IntersectsPlane(const Plane &plane) const
    {
        return ClassifyPlane(plane) == 0;
    }

    // ============================================================================
    // Triangle 碰撞检测 (使用分离轴定理 SAT)
    // ============================================================================

    bool Box::IntersectsTriangle(const Triangle3f &triangle) const
    {
        return IntersectsTriangleSAT(triangle[0], triangle[1], triangle[2]);
    }

    bool Box::IntersectsTriangleSAT(const math::Vector3f &v0,
                                    const math::Vector3f &v1,
                                    const math::Vector3f &v2) const
    {
        // 将三角形顶点转换到盒子空间 (盒子中心为原点)
        math::Vector3f v0_local = v0 - center;
        math::Vector3f v1_local = v1 - center;
        math::Vector3f v2_local = v2 - center;

        // 三角形边
        math::Vector3f f0 = v1_local - v0_local;
        math::Vector3f f1 = v2_local - v1_local;
        math::Vector3f f2 = v0_local - v2_local;

        // 盒子的轴
        math::Vector3f u0(1, 0, 0);
        math::Vector3f u1(0, 1, 0);
        math::Vector3f u2(0, 0, 1);

        // 测试9个叉积轴
        math::Vector3f axes[9] = {
            glm::cross(u0, f0), glm::cross(u0, f1), glm::cross(u0, f2),
            glm::cross(u1, f0), glm::cross(u1, f1), glm::cross(u1, f2),
            glm::cross(u2, f0), glm::cross(u2, f1), glm::cross(u2, f2)
        };

        for (int i = 0; i < 9; i++)
        {
            if (glm::length2(axes[i]) < 1e-8f)
                continue;

            float p0 = glm::dot(v0_local, axes[i]);
            float p1 = glm::dot(v1_local, axes[i]);
            float p2 = glm::dot(v2_local, axes[i]);

            float r = extent.x * std::abs(glm::dot(u0, axes[i])) +
                      extent.y * std::abs(glm::dot(u1, axes[i])) +
                      extent.z * std::abs(glm::dot(u2, axes[i]));

            float min_p = std::min({p0, p1, p2});
            float max_p = std::max({p0, p1, p2});

            if (max_p < -r || min_p > r)
                return false;
        }

        // 测试盒子的3个面法线
        {
            // X轴
            float min_v = std::min({v0_local.x, v1_local.x, v2_local.x});
            float max_v = std::max({v0_local.x, v1_local.x, v2_local.x});
            if (max_v < -extent.x || min_v > extent.x)
                return false;

            // Y轴
            min_v = std::min({v0_local.y, v1_local.y, v2_local.y});
            max_v = std::max({v0_local.y, v1_local.y, v2_local.y});
            if (max_v < -extent.y || min_v > extent.y)
                return false;

            // Z轴
            min_v = std::min({v0_local.z, v1_local.z, v2_local.z});
            max_v = std::max({v0_local.z, v1_local.z, v2_local.z});
            if (max_v < -extent.z || min_v > extent.z)
                return false;
        }

        // 测试三角形法线
        {
            math::Vector3f normal = glm::cross(f0, f1);
            float d = glm::dot(normal, v0_local);
            float r = extent.x * std::abs(normal.x) +
                      extent.y * std::abs(normal.y) +
                      extent.z * std::abs(normal.z);

            if (std::abs(d) > r)
                return false;
        }

        return true;
    }

    // ============================================================================
    // AABB/OBB 碰撞检测
    // ============================================================================

    bool Box::IntersectsAABB(const AABB &aabb) const
    {
        math::Vector3f aabb_center = aabb.GetCenter();
        math::Vector3f aabb_extent = aabb.GetLength() * 0.5f;

        math::Vector3f d = glm::abs(center - aabb_center);
        math::Vector3f sum_extent = extent + aabb_extent;

        return d.x <= sum_extent.x &&
               d.y <= sum_extent.y &&
               d.z <= sum_extent.z;
    }

    bool Box::IntersectsOBB(const OBB &obb) const
    {
        // Box 是轴对齐的，OBB 是有向的
        // 使用分离轴定理
        
        math::Vector3f t = obb.GetCenter() - center;

        // 测试盒子的3个轴
        for (int i = 0; i < 3; i++)
        {
            math::Vector3f axis;
            axis[i] = 1.0f;

            float ra = extent[i];
            float rb = std::abs(glm::dot(obb.GetAxis(0), axis)) * obb.GetHalfExtend().x +
                       std::abs(glm::dot(obb.GetAxis(1), axis)) * obb.GetHalfExtend().y +
                       std::abs(glm::dot(obb.GetAxis(2), axis)) * obb.GetHalfExtend().z;

            if (std::abs(glm::dot(t, axis)) > ra + rb)
                return false;
        }

        // 测试 OBB 的3个轴
        for (int i = 0; i < 3; i++)
        {
            const math::Vector3f &axis = obb.GetAxis(i);

            float ra = extent.x * std::abs(axis.x) +
                       extent.y * std::abs(axis.y) +
                       extent.z * std::abs(axis.z);
            float rb = obb.GetHalfExtend()[i];

            if (std::abs(glm::dot(t, axis)) > ra + rb)
                return false;
        }

        // 测试9个叉积轴
        math::Vector3f box_axes[3] = {
            Vector3f(1, 0, 0),
            Vector3f(0, 1, 0),
            Vector3f(0, 0, 1)
        };

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                math::Vector3f axis = glm::cross(box_axes[i], obb.GetAxis(j));
                float len = glm::length(axis);
                if (len < 1e-6f) continue;
                axis /= len;

                float ra = extent[(i + 1) % 3] * std::abs(glm::dot(box_axes[(i + 1) % 3], axis)) +
                          extent[(i + 2) % 3] * std::abs(glm::dot(box_axes[(i + 2) % 3], axis));
                float rb = obb.GetHalfExtend()[(j + 1) % 3] * std::abs(glm::dot(obb.GetAxis((j + 1) % 3), axis)) +
                          obb.GetHalfExtend()[(j + 2) % 3] * std::abs(glm::dot(obb.GetAxis((j + 2) % 3), axis));

                if (std::abs(glm::dot(t, axis)) > ra + rb)
                    return false;
            }
        }

        return true;
    }

    // ============================================================================
    // 工具函数
    // ============================================================================

    void Box::ExpandToInclude(const math::Vector3f &point)
    {
        math::Vector3f min_pt = MinVector(GetMin(), point);
        math::Vector3f max_pt = MaxVector(GetMax(), point);
        SetMinMax(min_pt, max_pt);
    }

    void Box::ExpandToInclude(const float *points, uint32_t count, uint32_t component_count)
    {
        if (!points || count == 0)
            return;

        math::Vector3f min_pt = GetMin();
        math::Vector3f max_pt = GetMax();

        const float *p = points;
        for (uint32_t i = 0; i < count; ++i)
        {
            math::Vector3f v(p[0], p[1], p[2]);
            min_pt = MinVector(min_pt, v);
            max_pt = MaxVector(max_pt, v);
            p += component_count;
        }

        SetMinMax(min_pt, max_pt);
    }

    Box Box::Transformed(const math::Matrix4f &transform) const
    {
        // 变换8个顶点,然后计算新的包围盒
        math::Vector3f corners[8];
        GetCorners(corners);

        math::Vector3f transformed[8];
        for (int i = 0; i < 8; ++i)
            transformed[i] = Vector3f(transform * math::Vector4f(corners[i], 1.0f));

        math::Vector3f min_pt = transformed[0];
        math::Vector3f max_pt = transformed[0];

        for (int i = 1; i < 8; ++i)
        {
            min_pt = MinVector(min_pt, transformed[i]);
            max_pt = MaxVector(max_pt, transformed[i]);
        }

        Box result;
        result.SetMinMax(min_pt, max_pt);
        return result;
    }

    // ============================================================================
    // 转换函数
    // ============================================================================

    AABB Box::ToAABB() const
    {
        AABB aabb;
        aabb.SetMinMax(GetMin(), GetMax());
        return aabb;
    }

    OBB Box::ToOBB() const
    {
        OBB obb;
        obb.Set(center, extent);
        return obb;
    }

    // ============================================================================
    // 全局转换函数
    // ============================================================================

    Box ToBox(const AABB &aabb)
    {
        return Box(aabb);
    }

    Box ToBox(const OBB &obb)
    {
        return Box(obb);
    }

    Box ToBox(const Frustum &frustum)
    {
        return Box(frustum);
    }

}//namespace hgl::math
