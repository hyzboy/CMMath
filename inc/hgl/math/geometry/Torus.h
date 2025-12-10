#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
    * 圆环(Torus) - 圆形截面版本
    * 
    * 圆环是一种特殊的旋转曲面，由一个圆沿着另一个圆旋转360°形成。
    * 形状类似于甲板、泳圈或甘甘圈。
    * 
    * 几何定义:
    * - 中心点: 圆环的几何中心
    * - 轴向: 垂直于主圆所在平面的方向(必须归一化)
    * - 主半径(Major Radius/R): 从圆环中心到管道中心线的距离
    * - 次半径(Minor Radius/r): 管道的圆形截面半径
    * 
    * 数学表达:
    * 在圆环坐标系中，点(x,y,z)在圆环表面当且仅当:
    * (sqrt(x²+y²) - R)² + z² = r²
    * 
    * 应用场景: 甘甸圈、管道弯头、装饰性物体等。
    */
    class Torus
    {
    protected:

        Vector3f center;      // 圆环中心点
        Vector3f axis;        // 圆环轴方向(单位向量)，垂直于主圆所在平面
        float major_radius;         // 主半径R(从中心到管道中心线的距离)
        float minor_radius;         // 次半径r(管道的圆形截面半径)

    public:

        /**
        * 默认构造函数
        * 创建一个位于原点、轴向为Y轴正方向、主半径为1.0、次半径为0.3的圆环
        */
        Torus()
        {
            center = Vector3f(0, 0, 0);
            axis = Vector3f(0, 1, 0);
            major_radius = 1.0f;
            minor_radius = 0.3f;
        }

        /**
        * 参数化构造函数
        * @param c 圆环中心点位置
        * @param a 圆环轴向(会被自动归一化)
        * @param major_r 主半径(到管道中心线的距离)
        * @param minor_r 次半径(管道截面半径)
        */
        Torus(const Vector3f &c, const Vector3f &a, float major_r, float minor_r)
        {
            Set(c, a, major_r, minor_r);
        }

        /**
        * 设置圆环参数
        * @param c 圆环中心点位置
        * @param a 圆环轴向(会被自动归一化)
        * @param major_r 主半径，必须 > minor_r
        * @param minor_r 次半径，必须 > 0
        */
        void Set(const Vector3f &c, const Vector3f &a, float major_r, float minor_r)
        {
            center = c;
            axis = normalize(a);  // 确保轴向为单位向量
            major_radius = major_r;
            minor_radius = minor_r;
        }

        /** 获取圆环中心点 */
        const Vector3f &GetCenter() const { return center; }
        
        /** 获取圆环轴向(单位向量) */
        const Vector3f &GetAxis() const { return axis; }
        
        /** 获取主半径R(到管道中心线的距离) */
        float GetMajorRadius() const { return major_radius; }
        
        /** 获取次半径r(管道截面半径) */
        float GetMinorRadius() const { return minor_radius; }

        /**
        * 计算圆环体积
        * 公式: V = 2π² * R * r²
        * 其中 R 为主半径，r 为次半径
        * @return 圆环的体积
        */
        float GetVolume() const
        {
            return 2.0f * math::pi * math::pi * major_radius * minor_radius * minor_radius;
        }

        /**
        * 计算圆环表面积
        * 公式: S = 4π² * R * r
        * 其中 R 为主半径，r 为次半径
        * @return 圆环的表面积
        */
        float GetSurfaceArea() const
        {
            return 4.0f * math::pi * math::pi * major_radius * minor_radius;
        }

        /**
        * 判断点是否在圆环内(实体部分)
        * 
        * 算法思路:
        * 1. 将点投影到主平面，得到到中心的平面距离
        * 2. 计算点在轴向的分量(高度)
        * 3. 找到管道中心线上最近的点(距离为 major_radius)
        * 4. 计算目标点到管道中心点的距离
        * 5. 如果该距离 ≤ minor_radius，则点在圆环内
        * 
        * @param point 待检测的点
        * @return true表示点在圆环内，false表示在外部
        */
        bool ContainsPoint(const Vector3f &point) const
        {
            Vector3f to_point = point - center;
            
            // 计算点在轴方向的分量(高度z)
            float axis_component = dot(to_point, axis);
            
            // 计算点在主平面上的投影
            // plane_projection 是移除了轴向分量后的向量
            Vector3f plane_projection = to_point - axis * axis_component;
            float plane_distance = length(plane_projection);  // 到中心轴的水平距离
            
            // 计算到管道中心线的距离
            // 管道中心线在距离中心轴 major_radius 的圆周上
            // distance_to_tube_center = sqrt((plane_distance - R)² + z²)
            float distance_to_tube_center = sqrt(
                (plane_distance - major_radius) * (plane_distance - major_radius) + 
                axis_component * axis_component
            );
            
            // 如果到管道中心的距离小于等于管道半径，则在圆环内
            return distance_to_tube_center <= minor_radius;
        }

        /**
        * 计算点到圆环表面的距离
        * 如果点在圆环内，返回负值或到表面的最短距离
        * @param point 空间中的任意点
        * @return 点到圆环表面的距离
        */
        float DistanceToPoint(const Vector3f &point) const;

        /**
        * 计算点到圆环表面的最近点
        * 通过投影到管道中心线，然后沿管道半径方向投影
        * @param point 空间中的任意点
        * @return 圆环表面上距离该点最近的点
        */
        Vector3f ClosestPoint(const Vector3f &point) const;
    };//class Torus
}//namespace hgl::math
