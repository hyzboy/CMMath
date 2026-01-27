/**
 * Cylinder.h - 简化的圆柱体几何图元
 *
 * 圆柱体是具有垂直于轴线的圆形截面的三维形状。
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
     * Cylinder - 带有轴线、高度和半径的圆柱体
     *
     * 定义：
     * - Center：圆柱体的几何中心
     * - Axis：圆柱体轴线方向（归一化）
     * - Height：沿轴线的长度
     * - Radius：圆形截面的半径
     *
     * 常见用途：
     * - 柱状结构（柱子、树木）
     * - 管道建模
     * - 碰撞体积
     */
    class Cylinder
    {
        Vector3f center;     // Cylinder center point
        Vector3f axis;       // Cylinder axis direction (normalized)
        float height;        // Cylinder height (along axis)
        float radius;        // Cylinder radius

    public:

        /**
         * 默认构造函数
         * 在原点创建一个Y轴对齐的圆柱体，高度1.0，半径0.5
         */
        Cylinder()
            : center(0, 0, 0), axis(0, 1, 0), height(1.0f), radius(0.5f)
        {
        }

        /**
         * 带参数构造函数
         * @param c 中心位置
         * @param a 轴线方向（会归一化）
         * @param h 高度
         * @param r 半径
         */
        Cylinder(const Vector3f& c, const Vector3f& a, float h, float r)
        {
            Set(c, a, h, r);
        }

        /**
         * 设置圆柱体参数
         * @param c 中心位置
         * @param a 轴线方向（会归一化）
         * @param h 高度
         * @param r 半径
         */
        void Set(const Vector3f& c, const Vector3f& a, float h, float r)
        {
            center = c;
            axis = Normalized(a);  // Ensure axis is unit vector
            height = h;
            radius = r;
        }

        /** 获取圆柱体中心 */
        const Vector3f& GetCenter() const { return center; }

        /** 获取圆柱体轴线（单位向量） */
        const Vector3f& GetAxis() const { return axis; }

        /** 获取圆柱体高度 */
        float GetHeight() const { return height; }

        /** 获取圆柱体半径 */
        float GetRadius() const { return radius; }

        /**
         * 获取顶部圆盖中心
         * 顶部沿轴线正方向
         * @return 顶部圆盖的中心
         */
        Vector3f GetTopCenter() const
        {
            return center + axis * (height * 0.5f);
        }

        /**
         * 获取底部圆盖中心
         * 底部沿轴线负方向
         * @return 底部圆盖的中心
         */
        Vector3f GetBottomCenter() const
        {
            return center - axis * (height * 0.5f);
        }

        /**
         * 计算圆柱体体积
         * 公式：V = πr²h
         * @return 体积
         */
        float GetVolume() const
        {
            return std::numbers::pi_v<float> * radius * radius * height;
        }

        /**
         * 计算圆柱体表面积（含端盖）
         * 公式：S = 2πr(r + h) = 侧面积 + 端盖面积
         * @return 总表面积
         */
        float GetSurfaceArea() const
        {
            return 2.0f * std::numbers::pi_v<float> * radius * (radius + height);
        }

        /**
         * 判断点是否在圆柱体内
         * 检查：
         * 1. 点在轴线方向的高度范围内
         * 2. 到轴线的径向距离 <= 半径
         * @param point 待测试点
         * @return 若在内部或表面返回 true
         */
        bool ContainsPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - center;

            // Project onto axis
            float axis_projection = Dot(to_point, axis);

            // Check height bounds
            float half_height = height * 0.5f;
            if (std::abs(axis_projection) > half_height)
                return false;

            // Check radial distance
            Vector3f axis_point = center + axis * axis_projection;
            float radial_distance = Length(point - axis_point);

            return radial_distance <= radius;
        }

        /**
         * 求空间中某点到圆柱体表面最近的点
         * @param point 空间中的点
         * @return 表面上最近的点
         */
        Vector3f ClosestPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - center;
            float axis_projection = Dot(to_point, axis);

            // Clamp to height range
            float half_height = height * 0.5f;
            axis_projection = clamp(axis_projection, -half_height, half_height);

            Vector3f axis_point = center + axis * axis_projection;
            Vector3f radial = point - axis_point;
            float radial_length = Length(radial);

            if (radial_length <= radius)
            {
                // Point inside - find nearest surface
                float top_dist = half_height - axis_projection;
                float bottom_dist = half_height + axis_projection;
                float radial_dist = radius - radial_length;

                if (radial_dist < top_dist && radial_dist < bottom_dist)
                {
                    // Nearest is lateral surface
                    return axis_point + glm::normalize(radial) * radius;
                }
                else if (top_dist < bottom_dist)
                {
                    // Nearest is top cap
                    return center + axis * half_height + radial;
                }
                else
                {
                    // Nearest is bottom cap
                    return center - axis * half_height + radial;
                }
            }
            else
            {
                // Point outside - project to lateral surface
                return axis_point + glm::normalize(radial) * radius;
            }
        }

        /**
         * 计算空间中某点到圆柱体表面的距离
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
         * @return 包含该圆柱体的AABB
         */
        AABB GetBoundingBox() const
        {
            // Calculate the 8 corner points of the bounding cylinder
            // by considering the caps and maximum radial extent
            Vector3f half_axis = axis * (height * 0.5f);
            Vector3f top_center = center + half_axis;
            Vector3f bottom_center = center - half_axis;

            // For axis-aligned bbox, we need maximum extent in all directions
            // This is conservative but correct for any axis orientation
            Vector3f extent(radius, radius, radius);

            // Account for axis direction by adding half-height extent
            extent += glm::abs(half_axis);

            AABB box;
            box.SetMinMax(center - extent, center + extent);
            return box;
        }

    };//class Cylinder

}//namespace hgl::math
