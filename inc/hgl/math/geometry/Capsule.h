#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
    * 胶囊体(Capsule)
    * 
    * 胶囊体是一种由两个半球和一个圆柱体组合而成的几何体。
    * 形状类似于药丸或胶囊，常用于角色控制器的碰撞体。
    * 
    * 几何定义:
    * 胶囊体由一条线段（中心轴）和一个半径定义：
    * - 起点(Start): 中心轴线段的起始端点
    * - 终点(End): 中心轴线段的终止端点
    * - 半径(Radius): 围绕中心轴的圆柱半径，同时也是两端半球的半径
    * 
    * 数学表示:
    * 胶囊体表面上的任意点满足：
    * - 到线段的距离 = radius
    * - 线段两端各有一个半径为radius的半球
    * - 中间是半径为radius、长度为||end-start||的圆柱
    * 
    * 特性:
    * - 总长度 = ||end-start|| + 2*radius
    * - 体积 = π*r²*h + (4/3)*π*r³  (h为圆柱高度)
    * - 对旋转不敏感，适合作为角色碰撞体
    * - 碰撞检测相对简单高效
    * 
    * 应用场景:
    * - 角色控制器碰撞体（人形角色的身体）
    * - 物理模拟中的胶囊刚体
    * - 射线扫描体积检测
    * - 近似圆柱形物体（管道、柱子等）
    */
    class Capsule
    {
    protected:

        Vector3f start;   // 胶囊中心轴线段的起始点
        Vector3f end;     // 胶囊中心轴线段的终止点

        float radius;           // 胶囊半径（圆柱半径和球半径）

    public:

        /** 获取胶囊中心轴的起始点 */
        const Vector3f &GetStart()const{return start;}
        
        /** 获取胶囊中心轴的终止点 */
        const Vector3f &GetEnd()const{return end;}
        
        /** 获取胶囊半径 */
        const float GetRadius()const{return radius;}
        
        /**
        * 获取胶囊的中心点
        * @return 线段中点位置
        */
        Vector3f GetCenter()const{return (start + end) * 0.5f;}
        
        /**
        * 获取胶囊中心轴的方向（归一化）
        * @return 从起点指向终点的单位向量
        */
        Vector3f GetDirection()const{return normalize(end - start);}
        
        /**
        * 获取胶囊中心轴的长度（不包括两端半球）
        * @return 圆柱部分的高度
        */
        float GetCylinderHeight()const{return length(end - start);}
        
        /**
        * 获取胶囊的总长度（包括两端半球）
        * @return 总长度 = 圆柱高度 + 2*半径
        */
        float GetTotalLength()const{return GetCylinderHeight() + 2.0f * radius;}
        
        /**
        * 计算胶囊体的体积
        * 体积 = 圆柱体积 + 球体体积
        * @return V = π*r²*h + (4/3)*π*r³
        */
        float GetVolume()const
        {
            float h = GetCylinderHeight();
            return math::pi * radius * radius * h + 
                   (4.0f / 3.0f) * math::pi * radius * radius * radius;
        }
        
        /**
        * 计算胶囊体的表面积
        * 表面积 = 圆柱侧面积 + 球表面积
        * @return S = 2πrh + 4πr²
        */
        float GetSurfaceArea()const
        {
            float h = GetCylinderHeight();
            return 2.0f * math::pi * radius * h + 
                   4.0f * math::pi * radius * radius;
        }
    };//class Capsule
}//namespace hgl::math
