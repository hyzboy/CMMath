#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
    * 圆柱体(Cylinder)
    * 
    * 圆柱体是由一个矩形绕其一边旋转一周形成的立体几何体。
    * 在3D图形学中，圆柱体由以下参数定义：
    * - 中心点：圆柱体的几何中心位置
    * - 轴向：圆柱体的对称轴方向(必须归一化)
    * - 高度：沿轴向的长度
    * - 半径：垂直于轴向的圆形截面半径
    * 
    * 常用于表示柱状物体，如管道、支柱、树干等。
    */
    class Cylinder
    {
    protected:

        Vector3f center;      // 圆柱中心点(整个圆柱体的几何中心)
        Vector3f axis;        // 圆柱轴方向(必须是归一化的单位向量)
        float height;               // 圆柱高度(沿轴向的长度)
        float radius;               // 圆柱半径(任意垂直于轴的圆形截面半径)

    public:

        /**
        * 默认构造函数
        * 创建一个位于原点、轴向为Y轴正方向、高度为1.0、半径为0.5的圆柱体
        */
        Cylinder()
        {
            center = Vector3f(0, 0, 0);
            axis = Vector3f(0, 1, 0);
            height = 1.0f;
            radius = 0.5f;
        }

        /**
        * 参数化构造函数
        * @param c 圆柱中心点位置
        * @param a 圆柱轴向(会被自动归一化)
        * @param h 圆柱高度
        * @param r 圆柱半径
        */
        Cylinder(const Vector3f &c, const Vector3f &a, float h, float r)
        {
            Set(c, a, h, r);
        }

        /**
        * 设置圆柱体参数
        * @param c 圆柱中心点位置
        * @param a 圆柱轴向(会被自动归一化)
        * @param h 圆柱高度
        * @param r 圆柱半径
        */
        void Set(const Vector3f &c, const Vector3f &a, float h, float r)
        {
            center = c;
            axis = normalize(a);  // 确保轴向为单位向量
            height = h;
            radius = r;
        }

        /** 获取圆柱中心点 */
        const Vector3f &GetCenter() const { return center; }
        
        /** 获取圆柱轴向(单位向量) */
        const Vector3f &GetAxis() const { return axis; }
        
        /** 获取圆柱高度 */
        float GetHeight() const { return height; }
        
        /** 获取圆柱半径 */
        float GetRadius() const { return radius; }

        /**
        * 获取圆柱体顶部中心点
        * 顶部是指沿轴向正方向的端面中心
        * @return 顶部端面的中心点坐标
        */
        Vector3f GetTopCenter() const
        {
            return center + axis * (height * 0.5f);
        }

        /**
        * 获取圆柱体底部中心点
        * 底部是指沿轴向负方向的端面中心
        * @return 底部端面的中心点坐标
        */
        Vector3f GetBottomCenter() const
        {
            return center - axis * (height * 0.5f);
        }

        /**
        * 计算圆柱体体积
        * 公式: V = π * r² * h
        * @return 圆柱体的体积
        */
        float GetVolume() const
        {
            return math::pi * radius * radius * height;
        }

        /**
        * 计算圆柱体表面积(包含顶部和底部圆面)
        * 公式: S = 2πr(r + h) = 侧面积 + 顶底面积
        * 其中侧面积 = 2πrh，顶底面积 = 2πr²
        * @return 圆柱体的总表面积
        */
        float GetSurfaceArea() const
        {
            return 2.0f * math::pi * radius * (radius + height);
        }

        /**
        * 判断点是否在圆柱体内
        * 检查条件:
        * 1. 点在高度范围内(沿轴向投影在[-h/2, h/2]之间)
        * 2. 点到轴的距离小于等于半径
        * @param point 待检测的点
        * @return true表示点在圆柱体内，false表示在外部
        */
        bool ContainsPoint(const Vector3f &point) const;

        /**
        * 计算点到圆柱体表面的最近点
        * 根据点的位置分为几种情况:
        * - 点在圆柱内部: 返回最近的表面点(可能在侧面或端面)
        * - 点在圆柱外部: 返回侧面或端面上的最近点
        * @param point 空间中的任意点
        * @return 圆柱体表面上距离该点最近的点
        */
        Vector3f ClosestPoint(const Vector3f &point) const;

        /**
        * 计算点到圆柱体的距离
        * 如果点在圆柱体内部，返回到表面的距离
        * 如果点在圆柱体外部，返回到最近表面点的距离
        * @param point 空间中的任意点
        * @return 点到圆柱体的最短距离
        */
        float DistanceToPoint(const Vector3f &point) const;
    };//class Cylinder
}//namespace hgl::math
