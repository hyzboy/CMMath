/**
 * Capsule.h - 简化的胶囊体几何图元
 *
 * 胶囊体是通过将一个球体沿一条线段扫掠形成的。
 * 它由一个圆柱体和两个半球端盖组成。
 *
 * 该轻量级类专注于：
 * - 属性存储与访问
 * - 简单的几何查询
 * - 包围盒计算
 *
 * 复杂的碰撞检测已移至 CollisionDetector。
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<numbers>

namespace hgl::math
{
    /**
     * Capsule - 球体沿线段扫掠形成的胶囊体
     *
     * 几何定义：
     * - 起点：中心轴线的一端
     * - 终点：中心轴线的另一端
     * - 半径：从轴线到表面的距离
     *
     * 数学表达：
     * 满足 distance_to_segment(P, start, end) = radius 的点都在表面上
     *
     * 属性：
     * - 总长度 = ||end - start|| + 2 * radius
     * - 体积 = πr²h + (4/3)πr³（圆柱体+球体）
     * - 围绕轴线旋转不变
     * - 非常适合角色控制器
     *
     * 常见用途：
     * - 角色碰撞体
     * - 物理胶囊刚体
     * - 扫掠体积测试
     * - 圆柱体物体的近似
     */
    class Capsule
    {
        Vector3f start;      // Start of center axis line segment
        Vector3f end;        // End of center axis line segment
        float radius;        // Capsule radius

    public:

        /** 获取中心轴线的起点 */
        const Vector3f& GetStart() const { return start; }

        /** 获取中心轴线的终点 */
        const Vector3f& GetEnd() const { return end; }

        /** 获取胶囊体半径 */
        float GetRadius() const { return radius; }

        /**
         * 获取胶囊体的几何中心
         * @return 线段的中点
         */
        Vector3f GetCenter() const
        {
            return (start + end) * 0.5f;
        }

        /**
         * 获取胶囊体轴线方向（归一化）
         * @return 从起点到终点的单位向量
         */
        Vector3f GetDirection() const
        {
            return Normalized(end - start);
        }

        /**
         * 获取圆柱部分高度（不含端盖）
         * @return 中心轴线长度
         */
        float GetCylinderHeight() const
        {
            return Length(end - start);
        }

        /**
         * 获取胶囊体总长度（含端盖）
         * @return 总长度 = 圆柱高度 + 2 * 半径
         */
        float GetTotalLength() const
        {
            return GetCylinderHeight() + 2.0f * radius;
        }

    public:

        /**
         * 默认构造函数
         * 在原点创建一个竖直胶囊体
         */
        Capsule()
            : start(0, -0.5f, 0), end(0, 0.5f, 0), radius(0.5f)
        {
        }

        /**
         * 带参数构造函数
         * @param s 轴线起点
         * @param e 轴线终点
         * @param r 胶囊体半径
         */
        Capsule(const Vector3f& s, const Vector3f& e, float r)
            : start(s), end(e), radius(r)
        {
        }

        /**
         * 设置胶囊体参数
         * @param s 轴线起点
         * @param e 轴线终点
         * @param r 胶囊体半径
         */
        void Set(const Vector3f& s, const Vector3f& e, float r)
        {
            start = s;
            end = e;
            radius = r;
        }

        /**
         * 计算胶囊体体积
         * 体积 = 圆柱体体积 + 球体体积
         * @return V = πr²h + (4/3)πr³
         */
        float GetVolume() const
        {
            float h = GetCylinderHeight();
            return std::numbers::pi_v<float> * radius * radius * h +
                   (4.0f / 3.0f) * std::numbers::pi_v<float> * radius * radius * radius;
        }

        /**
         * 计算胶囊体表面积
         * 表面积 = 圆柱侧面积 + 球体表面积
         * @return S = 2πrh + 4πr²
         */
        float GetSurfaceArea() const
        {
            float h = GetCylinderHeight();
            return 2.0f * std::numbers::pi_v<float> * radius * h +
                   4.0f * std::numbers::pi_v<float> * radius * radius;
        }

        /**
         * 判断一个点是否在胶囊体内
         * @param point 待测试点
         * @return 若点在内部或表面返回 true
         */
        bool ContainsPoint(const Vector3f& point) const
        {
            // Find closest point on line segment
            Vector3f segment = end - start;
            Vector3f to_point = point - start;

            float t = Dot(to_point, segment) / Dot(segment, segment);
            t = clamp(t, 0.0f, 1.0f);

            Vector3f closest_on_segment = start + segment * t;

            // Check if distance to segment is within radius
            return LengthSquared(point - closest_on_segment) <= radius * radius;
        }

        /**
         * 求空间中某点到胶囊体表面最近的点
         * @param point 空间中的点
         * @return 胶囊体表面上最近的点
         */
        Vector3f ClosestPoint(const Vector3f& point) const
        {
            // Find closest point on line segment
            Vector3f segment = end - start;
            Vector3f to_point = point - start;

            float t = Dot(to_point, segment) / Dot(segment, segment);
            t = clamp(t, 0.0f, 1.0f);

            Vector3f closest_on_segment = start + segment * t;

            // Project from segment to surface along radius
            Vector3f dir = point - closest_on_segment;
            float dist = Length(dir);

            if (dist <= radius)
                return point;  // Point is inside

            return closest_on_segment + dir * (radius / dist);
        }

        /**
         * 计算空间中某点到胶囊体表面的距离
         * @param point 空间中的点
         * @return 到表面的距离（若在内部则为0）
         */
        float DistanceToPoint(const Vector3f& point) const
        {
            // Find closest point on line segment
            Vector3f segment = end - start;
            Vector3f to_point = point - start;

            float t = Dot(to_point, segment) / Dot(segment, segment);
            t = clamp(t, 0.0f, 1.0f);

            Vector3f closest_on_segment = start + segment * t;

            float dist = Length(point - closest_on_segment) - radius;
            return dist > 0.0f ? dist : 0.0f;
        }

        /**
         * 获取用于粗略碰撞检测的轴对齐包围盒
         * @return 包含该胶囊体的AABB
         */
        AABB GetBoundingBox() const
        {
            Vector3f r(radius, radius, radius);
            Vector3f min_pt = glm::min(start, end) - r;
            Vector3f max_pt = glm::max(start, end) + r;

            AABB box;
            box.SetMinMax(min_pt, max_pt);
            return box;
        }

    };//class Capsule

}//namespace hgl::math
