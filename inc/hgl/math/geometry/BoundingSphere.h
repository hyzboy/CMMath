#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    struct Plane;
    struct Ray;

    struct BoundingSphere
    {
        Vector3f center { 0.0f };

        float radius=-1.0f;

    public:

        void Clear()
        {
            center=Vector3f(0.0f);
            radius=-1.0f;
        }

        bool IsEmpty() const
        {
            return radius<=0.0f;
        }

    public: // 访问器

        const Vector3f &GetCenter() const { return center; }
        float GetRadius() const { return radius; }

        void Set(const Vector3f &c, float r)
        {
            center = c;
            radius = r;
        }

    public: // 碰撞检测 - 点相关

        /**
         * 检查点是否在球体内
         */
        bool ContainsPoint(const Vector3f &point) const
        {
            const Vector3f diff = point - center;
            return glm::dot(diff, diff) <= radius * radius;
        }

        /**
         * 计算点到球体的最近点
         */
        Vector3f ClosestPoint(const Vector3f &point) const
        {
            Vector3f dir = point - center;
            float dist = glm::length(dir);
            if (dist <= radius)
                return point;
            return center + dir * (radius / dist);
        }

        /**
         * 计算点到球体的距离(点在球内返回0)
         */
        float DistanceToPoint(const Vector3f &point) const
        {
            float dist = glm::length(point - center) - radius;
            return dist > 0.0f ? dist : 0.0f;
        }

    public: // 碰撞检测 - 球体

        /**
         * 检查与另一个球体是否相交
         */
        bool Intersects(const BoundingSphere &other) const
        {
            const Vector3f diff = center - other.center;
            const float sum_radius = radius + other.radius;
            return glm::dot(diff, diff) <= sum_radius * sum_radius;
        }

        /**
         * 检查是否完全包含另一个球体
         */
        bool Contains(const BoundingSphere &other) const
        {
            float dist = glm::length(center - other.center);
            return dist + other.radius <= radius;
        }

        /**
         * 计算到另一个球体的距离
         */
        float Distance(const BoundingSphere &other) const
        {
            float dist = glm::length(center - other.center) - radius - other.radius;
            return dist > 0.0f ? dist : 0.0f;
        }

        /**
         * 合并另一个球体
         */
        void Merge(const BoundingSphere &other);

    public: // 碰撞检测 - 其他几何体

        /**
         * 检查与射线是否相交
         */
        bool IntersectsRay(const Ray &ray, float &distance) const;
        bool IntersectsRay(const Ray &ray) const;

        /**
         * 检查与平面的关系
         * @return <0: 完全在平面后面, 0: 相交, >0: 完全在平面前面
         */
        int ClassifyPlane(const Plane &plane) const;

        /**
         * 检查是否与平面相交
         */
        bool IntersectsPlane(const Plane &plane) const;

    public: // 工具函数

        /**
         * 扩展球体以包含指定点
         */
        void ExpandToInclude(const Vector3f &point);

        /**
         * 获取表面积
         */
        float GetSurfaceArea() const
        {
            return 4.0f * math::pi * radius * radius;
        }

        /**
         * 获取体积
         */
        float GetVolume() const
        {
            return (4.0f / 3.0f) * math::pi * radius * radius * radius;
        }

    public:

        void SetFromPoints(const float *pts,const uint32 count,const uint32 component_count);
    };//struct BoundingSphere
}//namespace hgl::math
