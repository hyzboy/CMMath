#pragma once
#include<numbers>

#include<hgl/math/Vector.h>
#include<hgl/math/Matrix.h>

namespace hgl::math
{
    struct Plane;
    struct Ray;
    class AABB;
    class OBB;

    struct BoundingSphere
    {
        Vector3f center { 0.0f };

        float radius=-1.0f;

    public:

        /**
         * 默认构造函数 - 创建无效的球体
         */
        BoundingSphere() = default;

        /**
         * 从中心和半径构造
         */
        BoundingSphere(const Vector3f &c, float r) : center(c), radius(r) {}

        /**
         * 清空球体，设置为无效状态
         */
        void Clear()
        {
            center=Vector3f(0.0f);
            radius=-1.0f;
        }

        /**
         * 检查球体是否为空/无效
         * @note radius < 0 表示无效，radius = 0 表示一个点（有效）
         */
        bool IsEmpty() const
        {
            return radius < 0.0f;
        }

        /**
         * 检查球体是否有效（半径非负）
         */
        bool IsValid() const
        {
            return radius >= 0.0f;
        }

    public: // 静态工厂方法

        /**
         * 从中心和半径创建球体
         */
        static BoundingSphere FromCenterAndRadius(const Vector3f &center, float radius)
        {
            return BoundingSphere(center, radius);
        }

        /**
         * 创建包围单个点的球体（半径为0）
         */
        static BoundingSphere FromPoint(const Vector3f &point)
        {
            return BoundingSphere(point, 0.0f);
        }

        /**
         * 创建包围两个点的球体
         */
        static BoundingSphere FromTwoPoints(const Vector3f &p1, const Vector3f &p2)
        {
            Vector3f center = (p1 + p2) * 0.5f;
            float radius = glm::length(p2 - p1) * 0.5f;
            return BoundingSphere(center, radius);
        }

        /**
         * 从 AABB 创建包围球
         */
        static BoundingSphere FromAABB(const AABB &aabb);

        /**
         * 从 OBB 创建包围球
         */
        static BoundingSphere FromOBB(const OBB &obb);

        /**
         * 创建空的无效球体
         */
        static BoundingSphere Empty()
        {
            return BoundingSphere();
        }

    public: // 访问器

        const Vector3f &GetCenter() const { return center; }
        float GetRadius() const { return radius; }

        void Set(const Vector3f &c, float r)
        {
            center = c;
            radius = r;
        }

        void SetCenter(const Vector3f &c) { center = c; }
        void SetRadius(float r) { radius = r; }

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
         * @note 如果点在球内，返回该点本身；否则返回球面上的最近点
         */
        Vector3f ClosestPoint(const Vector3f &point) const
        {
            Vector3f dir = point - center;
            float dist_sq = glm::dot(dir, dir);

            if (dist_sq <= radius * radius)
                return point;

            float dist = glm::sqrt(dist_sq);
            if (dist < 1e-6f)  // 避免除零
                return center + Vector3f(radius, 0, 0);

            return center + dir * (radius / dist);
        }

        /**
         * 计算点到球体表面的距离(点在球内返回0)
         */
        float DistanceToPoint(const Vector3f &point) const
        {
            float dist = glm::length(point - center) - radius;
            return dist > 0.0f ? dist : 0.0f;
        }

        /**
         * 计算点到球体表面的距离平方
         */
        float DistanceToPointSquared(const Vector3f &point) const
        {
            float dist = DistanceToPoint(point);
            return dist * dist;
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
         * 合并另一个球体，生成包含两个球体的最小球体
         */
        void Merge(const BoundingSphere &other);

    public: // 碰撞检测 - 其他几何体

        /**
         * 检查与 AABB 是否相交
         */
        bool IntersectsAABB(const AABB &aabb) const;

        /**
         * 检查与 OBB 是否相交
         */
        bool IntersectsOBB(const OBB &obb) const;

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
         * @note 保持原中心不变，只扩展半径
         */
        void ExpandToInclude(const Vector3f &point);

        /**
         * 返回缩放后的球体
         */
        BoundingSphere Scaled(float factor) const
        {
            return BoundingSphere(center, radius * factor);
        }

        /**
         * 返回偏移后的球体
         */
        BoundingSphere Offset(const Vector3f &offset) const
        {
            return BoundingSphere(center + offset, radius);
        }

        /**
         * 返回变换后的球体
         * @param transform 变换矩阵
         * @note 使用矩阵的最大缩放分量来缩放半径
         */
        BoundingSphere Transformed(const Matrix4f &transform) const;

        /**
         * 原地缩放
         */
        void Scale(float factor)
        {
            radius *= factor;
        }

        /**
         * 原地偏移（平移）
         */
        void Translate(const Vector3f &offset)
        {
            center += offset;
        }

        /**
         * 获取表面积
         */
        float GetSurfaceArea() const
        {
            return 4.0f * std::numbers::pi_v<float> * radius * radius;
        }

        /**
         * 获取体积
         */
        float GetVolume() const
        {
            return (4.0f / 3.0f) * std::numbers::pi_v<float> * radius * radius * radius;
        }

        /**
         * 获取直径
         */
        float GetDiameter() const
        {
            return radius * 2.0f;
        }

    public:

        /**
         * 从点集创建包围球
         * @note 使用简化算法：中心为重心，半径为到最远点的距离
         * @warning 这不一定是最小包围球，但计算速度快
         */
        void SetFromPoints(const float *pts,const uint32 count,const uint32 component_count);
    };//struct BoundingSphere
}//namespace hgl::math
