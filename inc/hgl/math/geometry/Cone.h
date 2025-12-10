#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
    * 圆锥体(Cone)
    * 
    * 圆锥体是由一个直角三角形绕其一条直角边旋转一周形成的立体几何体。
    * 形状类似于圆锥、漏斗或交通锥筒。
    * 
    * 几何定义:
    * - 顶点(Apex): 圆锥的尖顶点
    * - 轴向(Axis): 从顶点指向底面中心的方向(必须归一化)
    * - 高度(Height): 从顶点到底面的垂直距离
    * - 底面半径(Base Radius): 底面圆的半径
    * 
    * 数学特性:
    * - 母线(Slant Height) = sqrt(h² + r²)
    * - 任意高度 h' 处的半径 = r * (1 - h'/h)
    * - 圆锥角 = arctan(r/h)
    * 
    * 应用场景: 圆锥状物体、聚光灯光锥、射线相交测试等。
    */
    class Cone
    {
    protected:

        Vector3f apex;        // 圆锥顶点(尖顶位置)
        Vector3f axis;        // 圆锥轴方向(从顶点指向底面中心，必须归一化)
        float height;               // 圆锥高度(从顶点到底面的距离)
        float base_radius;          // 底面半径(底面圆的半径)

    public:

        Cone()
        {
            apex = Vector3f(0, 1, 0);
            axis = Vector3f(0, -1, 0);
            height = 1.0f;
            base_radius = 0.5f;
        }

        Cone(const Vector3f &a, const Vector3f &ax, float h, float r)
        {
            Set(a, ax, h, r);
        }

        void Set(const Vector3f &a, const Vector3f &ax, float h, float r)
        {
            apex = a;
            axis = normalize(ax);
            height = h;
            base_radius = r;
        }

        const Vector3f &GetApex() const { return apex; }
        const Vector3f &GetAxis() const { return axis; }
        float GetHeight() const { return height; }
        float GetBaseRadius() const { return base_radius; }

        /**
        * 获取底面中心点
        */
        Vector3f GetBaseCenter() const
        {
            return apex + axis * height;
        }

        /**
        * 计算圆锥体积
        */
        float GetVolume() const
        {
            return (1.0f / 3.0f) * math::pi * base_radius * base_radius * height;
        }

        /**
        * 计算圆锥表面积(包含底面)
        */
        float GetSurfaceArea() const
        {
            float slant_height = sqrt(height * height + base_radius * base_radius);
            return math::pi * base_radius * (base_radius + slant_height);
        }

        /**
        * 计算侧面积
        */
        float GetLateralArea() const
        {
            float slant_height = sqrt(height * height + base_radius * base_radius);
            return math::pi * base_radius * slant_height;
        }

        /**
        * 获取指定高度处的半径
        */
        float GetRadiusAtHeight(float h) const
        {
            if (h < 0.0f || h > height)
                return 0.0f;

            // 线性插值
            return base_radius * (1.0f - h / height);
        }

        /**
        * 判断点是否在圆锥体内
        */
        bool ContainsPoint(const Vector3f &point) const;

        /**
        * 计算点到圆锥体表面的最近点
        */
        Vector3f ClosestPoint(const Vector3f &point) const;

        /**
        * 计算点到圆锥体的距离
        */
        float DistanceToPoint(const Vector3f &point) const;
    };//class Cone
}//namespace hgl::math
