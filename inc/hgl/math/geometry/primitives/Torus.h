/**
 * Torus.h - 简化的环面几何图元
 *
 * 环面是通过将一个圆绕与其共面的轴旋转生成的曲面，也称为甜甜圈形状。
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<numbers>
#include<cmath>

namespace hgl::math
{
    /**
     * Torus - 圆截面的环面
     *
     * 几何定义：
     * - Center：环面中心
     * - Axis：主圆平面法线方向（归一化）
     * - Major radius (R)：从环面中心到管道中心的距离
     * - Minor radius (r)：管道截面的半径
     *
     * 数学方程：
     * 对于环面上的点(x,y,z)：
     * (sqrt(x² + y²) - R)² + z² = r²
     *
     * 属性：
     * - 体积 = 2π²Rr²
     * - 表面积 = 4π²Rr
     * - 有效环面需满足 R > r
     *
     * 常见用途：
     * - 甜甜圈形状
     * - 管道弯头
     * - 装饰物体
     */
    class Torus
    {
        Vector3f center;         // Torus center
        Vector3f axis;           // Axis direction (normalized)
        float major_radius;      // Major radius R (to tube center)
        float minor_radius;      // Minor radius r (tube thickness)

    public:

        /**
         * 默认构造函数
         * 在原点创建一个Y轴对齐的环面，R=1.0，r=0.3
         */
        Torus()
            : center(0, 0, 0), axis(0, 1, 0), major_radius(1.0f), minor_radius(0.3f)
        {
        }

        /**
         * 带参数构造函数
         * @param c 中心位置
         * @param a 轴线方向（会归一化）
         * @param major_r 主半径（必须大于次半径）
         * @param minor_r 次半径（必须大于0）
         */
        Torus(const Vector3f& c, const Vector3f& a, float major_r, float minor_r)
        {
            Set(c, a, major_r, minor_r);
        }

        /**
         * 设置环面参数
         * @param c 中心位置
         * @param a 轴线方向（会归一化）
         * @param major_r 主半径（必须大于次半径）
         * @param minor_r 次半径（必须大于0）
         */
        void Set(const Vector3f& c, const Vector3f& a, float major_r, float minor_r)
        {
            center = c;
            axis = Normalized(a);
            major_radius = major_r;
            minor_radius = minor_r;
        }

        /** 获取环面中心 */
        const Vector3f& GetCenter() const { return center; }

        /** 获取轴线方向 */
        const Vector3f& GetAxis() const { return axis; }

        /** 获取主半径R */
        float GetMajorRadius() const { return major_radius; }

        /** 获取次半径r */
        float GetMinorRadius() const { return minor_radius; }

        /**
         * 计算环面体积
         * 公式：V = 2π²Rr²
         * @return 体积
         */
        float GetVolume() const
        {
            constexpr float pi_squared = std::numbers::pi_v<float> * std::numbers::pi_v<float>;
            return 2.0f * pi_squared * major_radius * minor_radius * minor_radius;
        }

        /**
         * 计算环面表面积
         * 公式：S = 4π²Rr
         * @return 表面积
         */
        float GetSurfaceArea() const
        {
            constexpr float pi_squared = std::numbers::pi_v<float> * std::numbers::pi_v<float>;
            return 4.0f * pi_squared * major_radius * minor_radius;
        }

        /**
         * 判断点是否在环面内部（实体）
         *
         * 算法：
         * 1. 将点投影到主平面
         * 2. 计算平面距离（水平）
         * 3. 计算轴向分量（高度）
         * 4. 求到管道中心线的距离
         * 5. 判断是否 <= 次半径
         *
         * @param point 待测试点
         * @return 若在内部或表面返回 true
         */
        bool ContainsPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - center;

            // Height component along axis
            float axis_component = Dot(to_point, axis);

            // Projection onto major plane
            Vector3f plane_projection = to_point - axis * axis_component;
            float plane_distance = Length(plane_projection);

            // Distance to tube centerline
            float distance_to_tube_center = std::sqrt(
                (plane_distance - major_radius) * (plane_distance - major_radius) +
                axis_component * axis_component
            );

            return distance_to_tube_center <= minor_radius;
        }

        /**
         * 求空间中某点到环面表面最近的点
         * @param point 空间中的点
         * @return 表面上最近的点
         */
        Vector3f ClosestPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - center;

            // Project onto major plane
            float axis_component = Dot(to_point, axis);
            Vector3f plane_projection = to_point - axis * axis_component;
            float plane_distance = Length(plane_projection);

            // Find point on major circle (tube centerline)
            Vector3f tube_center_direction;
            if (plane_distance > 0.0001f)
            {
                tube_center_direction = plane_projection / plane_distance;
            }
            else
            {
                // Point on axis - choose arbitrary direction
                if (std::abs(axis.x) < 0.9f)
                    tube_center_direction = Cross(axis, Vector3f(1, 0, 0));
                else
                    tube_center_direction = Cross(axis, Vector3f(0, 1, 0));
                tube_center_direction = glm::normalize(tube_center_direction);
            }

            Vector3f tube_center = center + tube_center_direction * major_radius;

            // Project from tube center to surface
            Vector3f to_surface = point - tube_center;
            float dist_to_center = Length(to_surface);

            if (dist_to_center > 0.0001f)
            {
                return tube_center + to_surface * (minor_radius / dist_to_center);
            }
            else
            {
                // Point exactly at tube center - return point on surface
                return tube_center + tube_center_direction * minor_radius;
            }
        }

        /**
         * 计算空间中某点到环面表面的距离
         * @param point 空间中的点
         * @return 到表面的距离（若在内部则为0）
         */
        float DistanceToPoint(const Vector3f& point) const
        {
            Vector3f to_point = point - center;

            float axis_component = Dot(to_point, axis);
            Vector3f plane_projection = to_point - axis * axis_component;
            float plane_distance = Length(plane_projection);

            float distance_to_tube_center = std::sqrt(
                (plane_distance - major_radius) * (plane_distance - major_radius) +
                axis_component * axis_component
            );

            float dist = distance_to_tube_center - minor_radius;
            return dist > 0.0f ? dist : 0.0f;
        }

        /**
         * 获取轴对齐包围盒
         * @return 包含该环面的AABB
         */
        AABB GetBoundingBox() const
        {
            // Torus extends major_radius + minor_radius in plane directions
            // and minor_radius along axis
            float max_planar = major_radius + minor_radius;

            // Conservative bbox accounting for arbitrary axis orientation
            Vector3f extent_planar(max_planar, max_planar, max_planar);
            Vector3f extent_axial = glm::abs(axis) * minor_radius;
            Vector3f total_extent = extent_planar + extent_axial;

            AABB box;
            box.SetMinMax(center - total_extent, center + total_extent);
            return box;
        }

    };//class Torus

}//namespace hgl::math
