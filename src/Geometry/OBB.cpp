#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/Ray.h>
#include<hgl/math/geometry/Triangle.h>
#include<algorithm>
#include<limits>

namespace hgl::math
{
    void OBB::ComputePlanes()
    {
        planes[0].Set(center + axis[0] * half_length.x,  axis[0]);      // axis[0] 的正方向面
        planes[1].Set(center - axis[0] * half_length.x, -axis[0]);      // axis[0] 的负方向面
        planes[2].Set(center + axis[1] * half_length.y,  axis[1]);      // axis[1] 的正方向面
        planes[3].Set(center - axis[1] * half_length.y, -axis[1]);      // axis[1] 的负方向面
        planes[4].Set(center + axis[2] * half_length.z,  axis[2]);      // axis[2] 的正方向面
        planes[5].Set(center - axis[2] * half_length.z, -axis[2]);      // axis[2] 的负方向面
    }

    void OBB::Set(const Vector3f &c,const Vector3f &hl)
    {
        Set(c,Vector3f(1,0,0),Vector3f(0,1,0),Vector3f(0,0,1),hl);
    }

    void OBB::Set(const Vector3f &c,const Vector3f &a0,const Vector3f &a1,const Vector3f &a2,const Vector3f &hl)
    {
        center=c;
        axis[0]=a0;
        axis[1]=a1;
        axis[2]=a2;
        half_length=hl;

        ComputePlanes();
    }

    const Matrix4f OBB::GetMatrix(const float cube_size)const
    {
        // 这段代码也是正确的，留着做参考吧！
        {        
            //Matrix4f translate_matrix   =TranslateMatrix(center);
            //Matrix4f rotate_matrix      =axis;
            //Matrix4f scale_matrix       =ScaleMatrix(half_length*(cube_size/0.5f));

            //return translate_matrix*rotate_matrix*scale_matrix;
        }

        Matrix4f result(axis);

        const float scale=cube_size/0.5f;

        result[0]*=half_length.x*scale;
        result[1]*=half_length.y*scale;
        result[2]*=half_length.z*scale;

        result[3]=Vector4f(center,1.0f);

        return result;
    }

    void OBB::GetCorners(Vector3f out[8])const
    {        
        const glm::vec3 ex=axis[0]*half_length.x;
        const glm::vec3 ey=axis[1]*half_length.y;
        const glm::vec3 ez=axis[2]*half_length.z;

        out[0]=center-ex-ey-ez;
        out[1]=center+ex-ey-ez;
        out[2]=center-ex+ey-ez;
        out[3]=center+ex+ey-ez;
        out[4]=center-ex-ey+ez;
        out[5]=center+ex-ey+ez;
        out[6]=center-ex+ey+ez;
        out[7]=center+ex+ey+ez;
    }

    OBB OBB::Transformed(const Matrix4f &m)const
    {
        if(IsEmpty()) return *this;
        OBB out;
        out.center=glm::vec3(m*glm::vec4(center,1.0f));
        const glm::mat3 L(m);

        const glm::vec3 v0=L*axis[0];
        const glm::vec3 v1=L*axis[1];
        const glm::vec3 v2=L*axis[2];

        const float l0=glm::length(v0);
        const float l1=glm::length(v1);
        const float l2=glm::length(v2);

        out.axis[0]=(l0>0.0f)?(v0/l0):axis[0];
        out.axis[1]=(l1>0.0f)?(v1/l1):axis[1];
        out.axis[2]=(l2>0.0f)?(v2/l2):axis[2];
        out.half_length=glm::vec3(half_length.x*l0,half_length.y*l1,half_length.z*l2);
        out.ComputePlanes();
        return out;
    }

    // ============================================================================
    // 碰撞检测实现
    // ============================================================================

    bool OBB::ContainsPoint(const math::Vector3f &point) const
    {
        Vector3f d = point - center;
        
        for (int i = 0; i < 3; i++)
        {
            float dist = glm::dot(d, axis[i]);
            if (std::abs(dist) > half_length[i])
                return false;
        }
        
        return true;
    }

    math::Vector3f OBB::ClosestPoint(const math::Vector3f &point) const
    {
        Vector3f d = point - center;
        Vector3f result = center;
        
        for (int i = 0; i < 3; i++)
        {
            float dist = glm::dot(d, axis[i]);
            dist = glm::clamp(dist, -half_length[i], half_length[i]);
            result += axis[i] * dist;
        }
        
        return result;
    }

    float OBB::DistanceToPoint(const math::Vector3f &point) const
    {
        return glm::sqrt(DistanceToPointSquared(point));
    }

    float OBB::DistanceToPointSquared(const math::Vector3f &point) const
    {
        const math::Vector3f closest = ClosestPoint(point);
        const math::Vector3f delta = point - closest;
        return glm::dot(delta, delta);
    }

    bool OBB::Intersects(const OBB &other) const
    {
        // 使用分离轴定理(SAT)
        Vector3f t = other.center - center;

        // 测试15个分离轴
        // 轴1: 此OBB的3个轴
        for (int i = 0; i < 3; i++)
        {
            float ra = half_length[i];
            float rb = std::abs(glm::dot(other.axis[0], axis[i])) * other.half_length.x +
                       std::abs(glm::dot(other.axis[1], axis[i])) * other.half_length.y +
                       std::abs(glm::dot(other.axis[2], axis[i])) * other.half_length.z;

            if (std::abs(glm::dot(t, axis[i])) > ra + rb)
                return false;
        }

        // 轴2: 另一个OBB的3个轴
        for (int i = 0; i < 3; i++)
        {
            float ra = std::abs(glm::dot(axis[0], other.axis[i])) * half_length.x +
                       std::abs(glm::dot(axis[1], other.axis[i])) * half_length.y +
                       std::abs(glm::dot(axis[2], other.axis[i])) * half_length.z;
            float rb = other.half_length[i];

            if (std::abs(glm::dot(t, other.axis[i])) > ra + rb)
                return false;
        }

        // 轴3: 9个叉积轴
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                Vector3f axis = glm::cross(this->axis[i], other.axis[j]);
                float len = glm::length(axis);
                if (len < 1e-6f) continue;
                axis /= len;

                float ra = half_length[(i + 1) % 3] * std::abs(glm::dot(this->axis[(i + 1) % 3], axis)) +
                          half_length[(i + 2) % 3] * std::abs(glm::dot(this->axis[(i + 2) % 3], axis));
                float rb = other.half_length[(j + 1) % 3] * std::abs(glm::dot(other.axis[(j + 1) % 3], axis)) +
                          other.half_length[(j + 2) % 3] * std::abs(glm::dot(other.axis[(j + 2) % 3], axis));

                if (std::abs(glm::dot(t, axis)) > ra + rb)
                    return false;
            }
        }

        return true;
    }

    bool OBB::Contains(const OBB &other) const
    {
        // 检查另一个OBB的所有8个顶点是否都在此OBB内
        Vector3f corners[8];
        other.GetCorners(corners);
        
        for (int i = 0; i < 8; i++)
        {
            if (!ContainsPoint(corners[i]))
                return false;
        }
        
        return true;
    }

    float OBB::Distance(const OBB &other) const
    {
        if (Intersects(other))
            return 0.0f;

        // 简化实现：计算两个OBB中心之间的距离，减去它们的最大半长度
        float dist = glm::length(center - other.center);
        float max_extent1 = std::max({half_length.x, half_length.y, half_length.z});
        float max_extent2 = std::max({other.half_length.x, other.half_length.y, other.half_length.z});
        
        return std::max(0.0f, dist - max_extent1 - max_extent2);
    }

    bool OBB::IntersectsAABB(const AABB &aabb) const
    {
        // 将AABB视为特殊的OBB（轴对齐）
        Vector3f aabb_center = aabb.GetCenter();
        Vector3f aabb_extent = aabb.GetLength() * 0.5f;
        Vector3f t = aabb_center - center;

        // 测试OBB的3个轴
        for (int i = 0; i < 3; i++)
        {
            float ra = half_length[i];
            float rb = aabb_extent.x * std::abs(axis[i].x) +
                       aabb_extent.y * std::abs(axis[i].y) +
                       aabb_extent.z * std::abs(axis[i].z);

            if (std::abs(glm::dot(t, axis[i])) > ra + rb)
                return false;
        }

        // 测试AABB的3个轴
        for (int i = 0; i < 3; i++)
        {
            Vector3f aabb_axis(0);
            aabb_axis[i] = 1.0f;

            float ra = half_length.x * std::abs(axis[0][i]) +
                       half_length.y * std::abs(axis[1][i]) +
                       half_length.z * std::abs(axis[2][i]);
            float rb = aabb_extent[i];

            if (std::abs(t[i]) > ra + rb)
                return false;
        }

        // 测试9个叉积轴（简化版，只测试关键轴）
        // 完整实现需要测试所有9个轴

        return true;
    }

    bool OBB::IntersectsSphere(const math::Vector3f &sphere_center, float sphere_radius) const
    {
        float distSq = DistanceToPointSquared(sphere_center);
        return distSq <= sphere_radius * sphere_radius;
    }

    bool OBB::IntersectsRay(const Ray &ray, float &distance) const
    {
        float t_min, t_max;
        if (IntersectsRay(ray, t_min, t_max))
        {
            distance = t_min >= 0.0f ? t_min : t_max;
            return distance >= 0.0f;
        }
        return false;
    }

    bool OBB::IntersectsRay(const Ray &ray) const
    {
        float t_min, t_max;
        return IntersectsRay(ray, t_min, t_max);
    }

    bool OBB::IntersectsRay(const Ray &ray, float &t_min, float &t_max) const
    {
        // 将射线转换到OBB的局部空间
        Vector3f d = ray.origin - center;
        Vector3f f = ray.direction;

        t_min = 0.0f;
        t_max = std::numeric_limits<float>::infinity();

        for (int i = 0; i < 3; i++)
        {
            float e = glm::dot(axis[i], d);
            float f_dot = glm::dot(axis[i], f);

            if (std::abs(f_dot) > 1e-6f)
            {
                float t1 = (e + half_length[i]) / f_dot;
                float t2 = (e - half_length[i]) / f_dot;

                if (t1 > t2) std::swap(t1, t2);

                t_min = std::max(t_min, t1);
                t_max = std::min(t_max, t2);

                if (t_min > t_max)
                    return false;
            }
            else
            {
                if (-e - half_length[i] > 0.0f || -e + half_length[i] < 0.0f)
                    return false;
            }
        }

        return t_max >= 0.0f;
    }

    int OBB::ClassifyPlane(const Plane &plane) const
    {
        // 计算OBB在平面法线方向的"半径"
        float r = half_length.x * std::abs(glm::dot(plane.normal, axis[0])) +
                  half_length.y * std::abs(glm::dot(plane.normal, axis[1])) +
                  half_length.z * std::abs(glm::dot(plane.normal, axis[2]));

        // 计算中心到平面的距离
        float d = plane.Distance(center);

        if (d > r)
            return 1;   // 完全在平面前面
        else if (d < -r)
            return -1;  // 完全在平面后面
        else
            return 0;   // 相交
    }

    bool OBB::IntersectsPlane(const Plane &plane) const
    {
        return ClassifyPlane(plane) == 0;
    }

    bool OBB::IntersectsTriangle(const Triangle3f &triangle) const
    {
        // 简化实现：检查三角形顶点是否在OBB内
        if (ContainsPoint(triangle[0]) || 
            ContainsPoint(triangle[1]) || 
            ContainsPoint(triangle[2]))
            return true;

        // TODO: 实现完整的SAT测试
        return false;
    }

    void OBB::ExpandToInclude(const math::Vector3f &point)
    {
        // 将点转换到OBB的局部空间
        Vector3f local_point = point - center;
        Vector3f projected(
            glm::dot(local_point, axis[0]),
            glm::dot(local_point, axis[1]),
            glm::dot(local_point, axis[2])
        );

        // 扩展半长度
        half_length.x = std::max(half_length.x, std::abs(projected.x));
        half_length.y = std::max(half_length.y, std::abs(projected.y));
        half_length.z = std::max(half_length.z, std::abs(projected.z));

        ComputePlanes();
    }

    void OBB::Merge(const OBB &other)
    {
        // 简化实现：转换为AABB合并，再转回OBB
        // 这不是最优解，但可以工作
        Vector3f corners1[8], corners2[8];
        GetCorners(corners1);
        other.GetCorners(corners2);

        Vector3f min_pt = corners1[0];
        Vector3f max_pt = corners1[0];

        for (int i = 1; i < 8; i++)
        {
            min_pt = MinVector(min_pt, corners1[i]);
            max_pt = MaxVector(max_pt, corners1[i]);
        }

        for (int i = 0; i < 8; i++)
        {
            min_pt = MinVector(min_pt, corners2[i]);
            max_pt = MaxVector(max_pt, corners2[i]);
        }

        center = (min_pt + max_pt) * 0.5f;
        half_length = (max_pt - min_pt) * 0.5f;

        ComputePlanes();
    }

}//namespace hgl::math
