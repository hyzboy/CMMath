/**
 * Cone.h - 简化的圆锥体几何图元
 *
 * 圆锥体由一个圆形底面和一个顶点连接而成。
 * 该轻量级类专注于几何属性和简单查询。
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<numbers>
#include<cmath>

namespace hgl::math
{
    /**
     * Cone - 带有顶点、轴线、高度和底面半径的圆锥体
     *
     * 几何定义：
     * - Apex：圆锥顶点
     * - Axis：从顶点指向底面中心的方向（归一化）
     * - Height：顶点到底面的距离
     * - Base radius：底面圆的半径
     *
     * 属性：
     * - 斜高 = sqrt(h² + r²)
     * - 距顶点h'处的半径 = r * (1 - h'/h)
     * - 圆锥角 = arctan(r/h)
     *
     * 常见用途：
     * - 圆锥形物体
     * - 聚光体积
     * - 视野锥体
     */
    class Cone
    {
        Vector3f apex;           // Cone apex (tip point)
        Vector3f axis;           // Axis direction (normalized)
        float height;            // Height from apex to base
        float base_radius;       // Base circle radius

    public:

        /**
         * 默认构造函数
         * 在(0,1,0)创建一个朝下的圆锥体，高度1.0，半径0.5
         */
        Cone()
            : apex(0, 1, 0), axis(0, -1, 0), height(1.0f), base_radius(0.5f)
        {
        }

        /**
         * 带参数构造函数
         * @param a 顶点位置
         * @param ax 轴线方向（会归一化）
         * @param h 高度
         * @param r 底面半径
         */
        Cone(const Vector3f& a, const Vector3f& ax, float h, float r)
        {
            Set(a, ax, h, r);
        }

        /**
         * 设置圆锥体参数
         * @param a 顶点位置
         * @param ax 轴线方向（会归一化）
         * @param h 高度
         * @param r 底面半径
         */
        void Set(const Vector3f& a, const Vector3f& ax, float h, float r)
        {
            apex = a;
            axis = Normalized(ax);
            height = h;
            base_radius = r;
        }

        /** 获取顶点位置 */
        const Vector3f& GetApex() const { return apex; }

        /** 获取轴线方向 */
        const Vector3f& GetAxis() const { return axis; }

        /** 获取高度 */
        float GetHeight() const { return height; }

        /** 获取底面半径 */
        float GetBaseRadius() const { return base_radius; }

        /**
         * 获取几何中心（距底面1/4高度处）
         * @return 质心位置
         */
        Vector3f GetCenter() const
        {
            return apex + axis * (height * 0.75f);
        }

        /**
         * 获取底面中心
         * @return 圆形底面的中心
         */
        Vector3f GetBaseCenter() const
        {
            return apex + axis * height;
        }

        /**
         * 计算圆锥体体积
         * 公式：V = (1/3)πr²h
         * @return 体积
         */
        float GetVolume() const
        {
            return (1.0f / 3.0f) * std::numbers::pi_v<float> * base_radius * base_radius * height;
        }

        /**
         * 计算圆锥体表面积（含底面）
         * 公式：S = πr(r + s)，s为斜高
         * @return 总表面积
         */
        float GetSurfaceArea() const
        {
            float slant_height = std::sqrt(height * height + base_radius * base_radius);
            return std::numbers::pi_v<float> * base_radius * (base_radius + slant_height);
        }

        /**
         * 计算侧面积（不含底面）
         * @return 侧面积
         */
        float GetLateralArea() const
        {
            float slant_height = std::sqrt(height * height + base_radius * base_radius);
            return std::numbers::pi_v<float> * base_radius * slant_height;
        }

        /**
         * 获取距顶点某高度处的半径
         * @param h 距顶点高度（0到height）
         * @return 该高度处的半径
         */
        float GetRadiusAtHeight(float h) const
        {
            if (h < 0.0f || h > height)
                return 0.0f;

            // Linear interpolation
            return base_radius * (h / height);
        }

        /**
         * 判断点是否在圆锥体内
         * @param point 待测试点
         * @return 若在内部或表面返回true
         */
        bool ContainsPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - apex;
            float axis_projection = Dot(to_point, axis);

            // Check height bounds
            if (axis_projection < 0.0f || axis_projection > height)
                return false;

            // Get radius at this height
            float radius_at_height = GetRadiusAtHeight(axis_projection);

            // Check radial distance
            Vector3f axis_point = apex + axis * axis_projection;
            float radial_distance = Length(point - axis_point);

            return radial_distance <= radius_at_height;
        }

        /**
         * 求空间中某点到圆锥体表面最近的点
         * @param point 空间中的点
         * @return 表面上最近的点
         */
        Vector3f ClosestPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - apex;
            float axis_projection = Dot(to_point, axis);

            if (axis_projection <= 0.0f)
            {
                // Point above apex
                return apex;
            }

            if (axis_projection >= height)
            {
                // Point below base
                Vector3f base_center = GetBaseCenter();
                Vector3f to_base = point - base_center;
                Vector3f radial = to_base - axis * Dot(to_base, axis);
                float radial_length = Length(radial);

                if (radial_length <= base_radius)
                {
                    // Projects inside base circle
                    return point - axis * Dot(to_base, axis);
                }
                else
                {
                    // Projects outside base circle
                    return base_center + glm::normalize(radial) * base_radius;
                }
            }

            // Point within height range
            Vector3f axis_point = apex + axis * axis_projection;
            float radius_at_height = GetRadiusAtHeight(axis_projection);

            Vector3f radial = point - axis_point;
            float radial_length = Length(radial);

            if (radial_length <= radius_at_height)
            {
                // Point inside - return point on lateral surface
                if (radial_length < 0.0001f)
                {
                    // Point on axis - choose arbitrary direction
                    Vector3f perpendicular;
                    if (std::abs(axis.x) < 0.9f)
                        perpendicular = Cross(axis, Vector3f(1, 0, 0));
                    else
                        perpendicular = Cross(axis, Vector3f(0, 1, 0));

                    return axis_point + glm::normalize(perpendicular) * radius_at_height;
                }
                else
                {
                    return axis_point + glm::normalize(radial) * radius_at_height;
                }
            }
            else
            {
                // Point outside
                return axis_point + glm::normalize(radial) * radius_at_height;
            }
        }

        /**
         * 计算空间中某点到圆锥体表面的距离
         * @param point 空间中的点
         * @return 到表面的距离（若在内部则为0）
         */
        float DistanceToPoint(const Vector3f& point) const
        {
            Vector3f closest = ClosestPoint(point);
            return Length(point - closest);
        }

        /**
         * 获取轴对齐包围盒
         * @return 包含该圆锥体的AABB
         */
        AABB GetBoundingBox() const
        {
            Vector3f base_center = GetBaseCenter();

            // Conservative bounding box considering apex and base
            Vector3f r(base_radius, base_radius, base_radius);
            Vector3f min_pt = glm::min(apex, base_center - r);
            Vector3f max_pt = glm::max(apex, base_center + r);

            AABB box;
            box.SetMinMax(min_pt, max_pt);
            return box;
        }

    };//class Cone

}//namespace hgl::math
