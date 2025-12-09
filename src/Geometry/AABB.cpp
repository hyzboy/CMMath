#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/Ray.h>
#include<hgl/math/geometry/Triangle.h>
#include<algorithm>
#include<limits>

namespace hgl::math
{
    math::Vector3f AABB::GetVertexP(const math::Vector3f &normal) const
    {
        math::Vector3f res = minPoint;

        if (normal[0] > 0)res[0] += length[0];
        if (normal[1] > 0)res[1] += length[1];
        if (normal[2] > 0)res[2] += length[2];

        return(res);
    }

    math::Vector3f AABB::GetVertexN(const math::Vector3f &normal) const
    {
        math::Vector3f res = minPoint;

        if (normal[0] < 0)res[0] += length[0];
        if (normal[1] < 0)res[1] += length[1];
        if (normal[2] < 0)res[2] += length[2];

        return(res);
    }

    void AABB::Update()
    {
        face_center_point[0]=Vector3f(minPoint.x, center.y, center.z);
        face_center_point[1]=Vector3f(maxPoint.x, center.y, center.z);
        face_center_point[2]=Vector3f(center.x, minPoint.y, center.z);
        face_center_point[3]=Vector3f(center.x, maxPoint.y, center.z);
        face_center_point[4]=Vector3f(center.x, center.y, minPoint.z);
        face_center_point[5]=Vector3f(center.x, center.y, maxPoint.z);

        for(uint i=0;i<6;i++)
            planes[i].Set(face_center_point[i],AABBFaceNormal[i]);
    }

    void AABB::SetFromPoints(const float *pts,const uint32_t count,const uint32_t component_count)
    {
        Clear();

        if(pts==nullptr||count==0)
            return;

        const float *p=pts;
            
        math::Vector3f minp(p[0],p[1],p[2]);
        math::Vector3f maxp(p[0],p[1],p[2]);
        p+=component_count;

        for(uint32_t i=1;i<count;++i)
        {
            math::Vector3f v(p[0],p[1],p[2]);
            minp=glm::min(minp,v);
            maxp=glm::max(maxp,v);
            p+=component_count;
        }

        SetMinMax(minp,maxp);
    }

    AABB AABB::Transformed(const math::Matrix4f &m)const
    {
        if(IsEmpty())
            return *this;

        const math::Vector3f corners[8]=
        {
            minPoint,
            math::Vector3f(maxPoint.x,minPoint.y,minPoint.z),
            math::Vector3f(minPoint.x,maxPoint.y,minPoint.z),
            math::Vector3f(maxPoint.x,maxPoint.y,minPoint.z),
            math::Vector3f(minPoint.x,minPoint.y,maxPoint.z),
            math::Vector3f(maxPoint.x,minPoint.y,maxPoint.z),
            math::Vector3f(minPoint.x,maxPoint.y,maxPoint.z),
            maxPoint
        };

        math::Vector3f transformed[8];

        for(int i=0;i<8;++i)
            transformed[i] = Vector3f(m * math::Vector4f(corners[i], 1.0f));

        AABB result;
        result.SetFromPoints((const float *)&transformed,8,
                             sizeof(Vector3f)/sizeof(float));       //这里不能直接写3，因为Vector3f可能为了硬件SIMD指令加速而对齐保存。
        return result;
    }

    // ============================================================================
    // 碰撞检测实现
    // ============================================================================

    math::Vector3f AABB::ClosestPoint(const math::Vector3f &point) const
    {
        return glm::clamp(point, minPoint, maxPoint);
    }

    float AABB::DistanceToPoint(const math::Vector3f &point) const
    {
        return glm::sqrt(DistanceToPointSquared(point));
    }

    float AABB::DistanceToPointSquared(const math::Vector3f &point) const
    {
        math::Vector3f closest = ClosestPoint(point);
        return glm::length2(point - closest);
    }

    bool AABB::GetIntersection(const AABB &other, AABB &intersection) const
    {
        if (!Intersects(other))
            return false;

        math::Vector3f int_min = MaxVector(minPoint, other.minPoint);
        math::Vector3f int_max = MinVector(maxPoint, other.maxPoint);

        intersection.SetMinMax(int_min, int_max);
        return true;
    }

    float AABB::Distance(const AABB &other) const
    {
        if (Intersects(other))
            return 0.0f;

        math::Vector3f delta(0.0f);

        // X轴
        if (maxPoint.x < other.minPoint.x) delta.x = other.minPoint.x - maxPoint.x;
        else if (minPoint.x > other.maxPoint.x) delta.x = minPoint.x - other.maxPoint.x;

        // Y轴
        if (maxPoint.y < other.minPoint.y) delta.y = other.minPoint.y - maxPoint.y;
        else if (minPoint.y > other.maxPoint.y) delta.y = minPoint.y - other.maxPoint.y;

        // Z轴
        if (maxPoint.z < other.minPoint.z) delta.z = other.minPoint.z - maxPoint.z;
        else if (minPoint.z > other.maxPoint.z) delta.z = minPoint.z - other.maxPoint.z;

        return glm::length(delta);
    }

    bool AABB::IntersectsSphere(const math::Vector3f &sphere_center, float sphere_radius) const
    {
        float distSq = DistanceToPointSquared(sphere_center);
        return distSq <= sphere_radius * sphere_radius;
    }

    bool AABB::IntersectsOBB(const OBB &obb) const
    {
        // 简化实现：通过OBB的IntersectsAABB来实现
        return obb.IntersectsAABB(*this);
    }

    bool AABB::IntersectsRay(const Ray &ray, float &distance) const
    {
        float t_min, t_max;
        if (IntersectsRay(ray, t_min, t_max))
        {
            distance = t_min >= 0.0f ? t_min : t_max;
            return distance >= 0.0f;
        }
        return false;
    }

    bool AABB::IntersectsRay(const Ray &ray) const
    {
        float t_min, t_max;
        return IntersectsRay(ray, t_min, t_max);
    }

    bool AABB::IntersectsRay(const Ray &ray, float &t_min, float &t_max) const
    {
        t_min = 0.0f;
        t_max = std::numeric_limits<float>::infinity();

        for (int i = 0; i < 3; i++)
        {
            if (std::abs(ray.direction[i]) < 1e-8f)
            {
                // 射线平行于该轴的平板
                if (ray.origin[i] < minPoint[i] || ray.origin[i] > maxPoint[i])
                    return false;
            }
            else
            {
                float inv_d = 1.0f / ray.direction[i];
                float t1 = (minPoint[i] - ray.origin[i]) * inv_d;
                float t2 = (maxPoint[i] - ray.origin[i]) * inv_d;

                if (t1 > t2) std::swap(t1, t2);

                t_min = std::max(t_min, t1);
                t_max = std::min(t_max, t2);

                if (t_min > t_max)
                    return false;
            }
        }

        return t_max >= 0.0f;
    }

    int AABB::ClassifyPlane(const Plane &plane) const
    {
        // 计算盒子在平面法线方向的"半径"
        float r = length.x * 0.5f * std::abs(plane.normal.x) +
                  length.y * 0.5f * std::abs(plane.normal.y) +
                  length.z * 0.5f * std::abs(plane.normal.z);

        // 计算中心到平面的距离
        float d = plane.Distance(center);

        if (d > r)
            return 1;   // 完全在平面前面
        else if (d < -r)
            return -1;  // 完全在平面后面
        else
            return 0;   // 相交
    }

    bool AABB::IntersectsPlane(const Plane &plane) const
    {
        return ClassifyPlane(plane) == 0;
    }

    bool AABB::IntersectsTriangle(const Triangle3f &triangle) const
    {
        // 简化实现：检查三角形顶点是否在AABB内，或边是否与AABB相交
        // 完整实现需要使用分离轴定理，这里先提供基本版本

        // 检查三角形顶点是否在AABB内
        if (ContainsPoint(triangle[0]) || 
            ContainsPoint(triangle[1]) || 
            ContainsPoint(triangle[2]))
            return true;

        // 检查AABB顶点是否在三角形内（简化版）
        // TODO: 实现完整的SAT测试

        return false;
    }

    void AABB::ExpandToInclude(const math::Vector3f &point)
    {
        math::Vector3f new_min = MinVector(minPoint, point);
        math::Vector3f new_max = MaxVector(maxPoint, point);
        SetMinMax(new_min, new_max);
    }

}//namespace hgl::math
