#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
    * 空心圆柱体(Hollow Cylinder) - 圆环的空心圆柱形式
    * 
    * 空心圆柱体是具有内外两个同心圆柱面的几何体。
    * 形状类似于管道、圆筒或空心柱体。
    * 
    * 几何定义:
    * - 中心点: 圆柱的几何中心
    * - 轴向: 圆柱的对称轴方向(必须归一化)
    * - 高度: 沿轴向的长度
    * - 外半径: 外侧圆柱面的半径
    * - 内半径: 内侧圆柱面的半径 (必须 < 外半径)
    * 
    * 实体部分是外圆柱和内圆柱之间的环形区域。
    * 应用场景: 管道、筒体、空心支柱等。
    */
    class HollowCylinder
    {
    protected:

        Vector3f center;      // 圆柱中心点(整个空心圆柱的几何中心)
        Vector3f axis;        // 圆柱轴方向(必须是归一化的单位向量)
        float height;               // 圆柱高度(沿轴向的长度)
        float outer_radius;         // 外半径(外侧圆柱面半径)
        float inner_radius;         // 内半径(内侧圆柱面半径，必须 < outer_radius)

    public:

        HollowCylinder()
        {
            center = Vector3f(0, 0, 0);
            axis = Vector3f(0, 1, 0);
            height = 1.0f;
            outer_radius = 0.5f;
            inner_radius = 0.3f;
        }

        HollowCylinder(const Vector3f &c, const Vector3f &a, float h, float outer_r, float inner_r)
        {
            Set(c, a, h, outer_r, inner_r);
        }

        void Set(const Vector3f &c, const Vector3f &a, float h, float outer_r, float inner_r)
        {
            center = c;
            axis = normalize(a);
            height = h;
            outer_radius = outer_r;
            inner_radius = inner_r;
        }

        const Vector3f &GetCenter() const { return center; }
        const Vector3f &GetAxis() const { return axis; }
        float GetHeight() const { return height; }
        float GetOuterRadius() const { return outer_radius; }
        float GetInnerRadius() const { return inner_radius; }

        /**
        * 获取顶部中心点
        */
        Vector3f GetTopCenter() const
        {
            return center + axis * (height * 0.5f);
        }

        /**
        * 获取底部中心点
        */
        Vector3f GetBottomCenter() const
        {
            return center - axis * (height * 0.5f);
        }

        /**
        * 计算空心圆柱体体积
        */
        float GetVolume() const
        {
            return math::pi * (outer_radius * outer_radius - inner_radius * inner_radius) * height;
        }

        /**
        * 计算空心圆柱体表面积
        */
        float GetSurfaceArea() const
        {
            // 外侧面 + 内侧面 + 顶部圆环 + 底部圆环
            float lateral_outer = 2.0f * math::pi * outer_radius * height;
            float lateral_inner = 2.0f * math::pi * inner_radius * height;
            float ring_area = math::pi * (outer_radius * outer_radius - inner_radius * inner_radius);
            
            return lateral_outer + lateral_inner + 2.0f * ring_area;
        }

        /**
        * 判断点是否在空心圆柱体内(实心部分)
        */
        bool ContainsPoint(const Vector3f &point) const
        {
            Vector3f to_point = point - center;
            
            // 计算点在圆柱轴方向的投影长度
            float axis_projection = dot(to_point, axis);
            
            // 检查是否在高度范围内
            float half_height = height * 0.5f;
            if (abs(axis_projection) > half_height)
                return false;
            
            // 计算点到圆柱轴的距离
            Vector3f axis_point = center + axis * axis_projection;
            float radial_distance = length(point - axis_point);
            
            // 在外半径内且在内半径外
            return radial_distance <= outer_radius && radial_distance >= inner_radius;
        }

        /**
        * 计算点到空心圆柱体表面的最近点
        */
        Vector3f ClosestPoint(const Vector3f &point) const;

        /**
        * 计算点到空心圆柱体的距离
        */
        float DistanceToPoint(const Vector3f &point) const;
    };//class HollowCylinder
}//namespace hgl::math
