#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/MatrixTypes.h>

namespace hgl::math
{
    class AABB;
    class OBB;
    class Frustum;
    class Sphere;
    class EllipseSphere;
    struct Ray;
    struct Plane;
    template<typename T> class Triangle3;
    using Triangle3f = Triangle3<float>;

    /**
     * Box - 通用包围盒类
     * 使用 center + extent (半长度) 表示
     * 可以从 AABB/OBB/Frustum 转换而来
     */
    class Box
    {
        math::Vector3f center;      ///< 盒子中心点
        math::Vector3f extent;      ///< 盒子半长度 (从中心到边界的距离)

    public:

        Box()
        {
            center = Vector3f(0, 0, 0);
            extent = Vector3f(1, 1, 1);
        }

        Box(const math::Vector3f &c, const math::Vector3f &e)
        {
            center = c;
            extent = e;
        }

        /**
         * 从 AABB 构造
         */
        Box(const AABB &aabb);

        /**
         * 从 OBB 构造 (注意: OBB的旋转信息会丢失)
         */
        Box(const OBB &obb);

        /**
         * 从 Frustum 构造 (计算包围该视锥体的轴对齐盒)
         */
        Box(const Frustum &frustum);

    public: // Getters & Setters

        const math::Vector3f &GetCenter() const { return center; }
        const math::Vector3f &GetExtent() const { return extent; }

        void SetCenter(const math::Vector3f &c) { center = c; }
        void SetExtent(const math::Vector3f &e) { extent = e; }

        void Set(const math::Vector3f &c, const math::Vector3f &e)
        {
            center = c;
            extent = e;
        }

        /**
         * 获取最小点
         */
        math::Vector3f GetMin() const { return center - extent; }

        /**
         * 获取最大点
         */
        math::Vector3f GetMax() const { return center + extent; }

        /**
         * 获取完整尺寸 (长宽高)
         */
        math::Vector3f GetSize() const { return extent * 2.0f; }

        /**
         * 设置最小和最大点
         */
        void SetMinMax(const math::Vector3f &min_point, const math::Vector3f &max_point)
        {
            center = (min_point + max_point) * 0.5f;
            extent = (max_point - min_point) * 0.5f;
        }

        /**
         * 获取表面积
         */
        float GetSurfaceArea() const
        {
            math::Vector3f size = GetSize();
            return 2.0f * (size.x * size.y + size.y * size.z + size.z * size.x);
        }

        /**
         * 获取体积
         */
        float GetVolume() const
        {
            return extent.x * extent.y * extent.z * 8.0f;
        }

        /**
         * 检查是否为空
         */
        bool IsEmpty() const
        {
            return IsNearlyZero(extent);
        }

        /**
         * 清空盒子
         */
        void Clear()
        {
            center = Vector3f(0, 0, 0);
            extent = Vector3f(0, 0, 0);
        }

        /**
         * 获取8个顶点
         */
        void GetCorners(math::Vector3f corners[8]) const;

    public: // 碰撞检测 - 点相关

        /**
         * 检查点是否在盒子内
         * @param point 待检查的点
         * @return true 如果点在盒子内部
         */
        bool ContainsPoint(const math::Vector3f &point) const;

        /**
         * 计算点到盒子的最近点
         * @param point 输入点
         * @return 盒子表面或内部最接近该点的点
         */
        math::Vector3f ClosestPoint(const math::Vector3f &point) const;

        /**
         * 计算点到盒子的距离
         * @param point 输入点
         * @return 如果点在盒子内返回0,否则返回到盒子表面的距离
         */
        float DistanceToPoint(const math::Vector3f &point) const;

        /**
         * 计算点到盒子的距离平方
         */
        float DistanceToPointSquared(const math::Vector3f &point) const;

    public: // 碰撞检测 - Box

        /**
         * 检查与另一个 Box 是否相交
         */
        bool Intersects(const Box &other) const;

        /**
         * 检查是否完全包含另一个 Box
         */
        bool Contains(const Box &other) const;

        /**
         * 计算与另一个 Box 的交集
         * @param other 另一个盒子
         * @param intersection 输出交集盒子
         * @return true 如果有交集
         */
        bool GetIntersection(const Box &other, Box &intersection) const;

        /**
         * 合并另一个 Box
         */
        void Merge(const Box &other);

        /**
         * 计算到另一个 Box 的距离
         */
        float Distance(const Box &other) const;

    public: // 碰撞检测 - Sphere

        /**
         * 检查与球体是否相交
         * @param sphere_center 球心
         * @param sphere_radius 球半径
         * @return true 如果相交
         */
        bool IntersectsSphere(const math::Vector3f &sphere_center, float sphere_radius) const;

        /**
         * 检查与球体是否相交
         */
        bool IntersectsSphere(const Sphere &sphere) const;

        /**
         * 检查与椭球是否相交
         */
        bool IntersectsEllipseSphere(const EllipseSphere &ellipse) const;

    public: // 碰撞检测 - Ray

        /**
         * 射线与盒子相交测试
         * @param ray 射线
         * @param distance 输出相交距离
         * @return true 如果相交
         */
        bool IntersectsRay(const Ray &ray, float &distance) const;

        /**
         * 射线与盒子相交测试(只返回是否相交)
         */
        bool IntersectsRay(const Ray &ray) const;

        /**
         * 射线与盒子相交测试(返回进入和退出距离)
         * @param ray 射线
         * @param t_min 输出进入距离
         * @param t_max 输出退出距离
         * @return true 如果相交
         */
        bool IntersectsRay(const Ray &ray, float &t_min, float &t_max) const;

    public: // 碰撞检测 - Plane

        /**
         * 检查与平面的关系
         * @param plane 平面
         * @return <0: 完全在平面后面, 0: 相交, >0: 完全在平面前面
         */
        int ClassifyPlane(const Plane &plane) const;

        /**
         * 检查是否与平面相交
         */
        bool IntersectsPlane(const Plane &plane) const;

    public: // 碰撞检测 - Triangle

        /**
         * 检查与三角形是否相交
         */
        bool IntersectsTriangle(const Triangle3f &triangle) const;

        /**
         * 检查与三角形是否相交(使用分离轴定理)
         */
        bool IntersectsTriangleSAT(const math::Vector3f &v0, 
                                   const math::Vector3f &v1, 
                                   const math::Vector3f &v2) const;

    public: // 碰撞检测 - AABB/OBB

        /**
         * 检查与 AABB 是否相交
         */
        bool IntersectsAABB(const AABB &aabb) const;

        /**
         * 检查与 OBB 是否相交
         */
        bool IntersectsOBB(const OBB &obb) const;

    public: // 工具函数

        /**
         * 扩展盒子以包含指定点
         */
        void ExpandToInclude(const math::Vector3f &point);

        /**
         * 扩展盒子以包含多个点
         */
        void ExpandToInclude(const float *points, uint32_t count, uint32_t component_count);

        /**
         * 按指定值扩展盒子
         */
        void Expand(float amount)
        {
            extent += Vector3f(amount, amount, amount);
        }

        /**
         * 按指定向量扩展盒子
         */
        void Expand(const math::Vector3f &amount)
        {
            extent += amount;
        }

        /**
         * 缩放盒子
         */
        void Scale(float scale)
        {
            extent *= scale;
        }

        /**
         * 平移盒子
         */
        void Translate(const math::Vector3f &offset)
        {
            center += offset;
        }

        /**
         * 变换盒子
         */
        Box Transformed(const math::Matrix4f &transform) const;

    public: // 转换函数

        /**
         * 转换为 AABB
         */
        AABB ToAABB() const;

        /**
         * 转换为 OBB (轴对齐的)
         */
        OBB ToOBB() const;

    public: // 运算符

        bool operator==(const Box &other) const
        {
            return center == other.center && extent == other.extent;
        }

        bool operator!=(const Box &other) const
        {
            return !(*this == other);
        }

        Box operator+(const math::Vector3f &offset) const
        {
            return Box(center + offset, extent);
        }

        Box& operator+=(const math::Vector3f &offset)
        {
            center += offset;
            return *this;
        }

        Box operator*(float scale) const
        {
            return Box(center, extent * scale);
        }

        Box& operator*=(float scale)
        {
            extent *= scale;
            return *this;
        }

    };//class Box

    /**
     * 从 AABB 创建 Box
     */
    Box ToBox(const AABB &aabb);

    /**
     * 从 OBB 创建 Box
     */
    Box ToBox(const OBB &obb);

    /**
     * 从 Frustum 创建 Box
     */
    Box ToBox(const Frustum &frustum);

}//namespace hgl::math
