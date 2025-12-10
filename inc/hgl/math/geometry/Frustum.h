#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/MatrixTypes.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/Plane.h>

namespace hgl::math
{
    /**
    * 视锥体平面数组类型
    * 存储视锥体的6个平面（左、右、前、后、上、下）
    */
    using FrustumPlanes=Vector4f[6];

    /**
    * 从MVP矩阵提取视锥体的6个平面
    * 
    * 该函数从模型-视图-投影(MVP)矩阵中提取视锥体的6个裁剪平面。
    * 这些平面定义了相机可见的空间区域。
    * 
    * 算法基于：将MVP矩阵的行向量进行组合来获得每个平面的方程
    * 参考：Gribb & Hartmann, "Fast Extraction of Viewing Frustum Planes"
    * 
    * @param fp 输出参数，存储提取的6个平面
    * @param mvp 模型-视图-投影矩阵
    */
    void GetFrustumPlanes(FrustumPlanes &fp,const Matrix4f &mvp);

    /**
    * 平截头体/视锥体(View Frustum)
    * 
    * 视锥体是3D图形学中用于视图裁剪的核心概念，定义了相机可见的空间区域。
    * 它是一个被6个平面包围的截头锥体（金字塔顶部被切掉的部分）。
    * 
    * 六个平面：
    * - 左平面(Left)：视锥体左侧边界
    * - 右平面(Right)：视锥体右侧边界
    * - 近平面(Front/Near)：离相机最近的平面
    * - 远平面(Back/Far)：离相机最远的平面
    * - 上平面(Top)：视锥体上方边界
    * - 下平面(Bottom)：视锥体下方边界
    * 
    * 用途：
    * - 视图裁剪(View Culling)：只渲染视锥体内的物体
    * - 可见性判断：判断物体是否在相机视野内
    * - 优化渲染性能：提前剔除不可见物体
    * 
    * 工作原理：
    * 通过测试物体与6个平面的位置关系来判断可见性：
    * - 如果物体在任一平面的外侧：完全不可见
    * - 如果物体与平面相交：部分可见
    * - 如果物体在所有平面的内侧：完全可见
    */
    class Frustum
    {
        Plane pl[6];    // 视锥体的6个裁剪平面

    public:
            
        /**
        * 视锥体的六个面的枚举
        * 
        * 对应关系：
        * - Left/Right: 对应屏幕左右边界
        * - Front/Back: 对应近裁剪面和远裁剪面
        * - Top/Bottom: 对应屏幕上下边界
        */
        enum class Side 
        { 
            Left=0,     // 左平面（屏幕左边界）
            Right,      // 右平面（屏幕右边界）
            Front,      // 前平面（近裁剪面，离相机最近）
            Back,       // 后平面（远裁剪面，离相机最远）
            Top,        // 上平面（屏幕上边界）
            Bottom      // 下平面（屏幕下边界）
        };

        /**
        * 空间位置关系枚举
        * 
        * 描述物体相对于视锥体的位置关系
        */
        enum class Scope
        {
            OUTSIDE,    // 外部：物体完全在视锥体外，不可见，应被裁剪
            INTERSECT,  // 相交：物体部分在视锥体内，部分在外，需要进一步处理
            INSIDE      // 内部：物体完全在视锥体内，完全可见
        };

    public:

        /**
        * 从MVP矩阵设置视锥体
        * 
        * 从模型-视图-投影矩阵中提取6个裁剪平面，更新视锥体定义。
        * 通常在相机参数改变时调用（如移动、旋转、改变FOV等）。
        * 
        * @param mvp 模型-视图-投影组合矩阵
        */
        void SetMatrix(const Matrix4f &mvp);

        /**
        * 判断点是否在视锥体内
        * 
        * 测试一个3D空间点相对于视锥体的位置关系。
        * 
        * 算法：检查点与6个平面的位置关系
        * - 如果点在任一平面的外侧（负半空间），返回OUTSIDE
        * - 如果点在所有平面的内侧（正半空间），返回INSIDE
        * 
        * @param point 待测试的3D空间点
        * @return INSIDE表示点在视锥体内，OUTSIDE表示在外
        */
        Scope PointIn(const Vector3f &point)const;
        
        /**
        * 判断球体是否在视锥体内
        * 
        * 测试一个球体（由中心点和半径定义）相对于视锥体的位置关系。
        * 这是最常用的可见性测试方法，用于包围球裁剪。
        * 
        * 算法：检查球心到6个平面的距离
        * - 如果球心到某平面的距离 < -radius：球体完全在外，返回OUTSIDE
        * - 如果球心到某平面的距离 < radius：球体与平面相交，返回INTERSECT
        * - 如果球体完全在所有平面内侧：返回INSIDE
        * 
        * @param center 球体中心点
        * @param radius 球体半径
        * @return OUTSIDE/INTERSECT/INSIDE 表示球体的可见性状态
        */
        Scope SphereIn(const Vector3f &center,float radius)const;
        
        /**
        * 判断AABB包围盒是否在视锥体内
        * 
        * 测试一个轴对齐包围盒(AABB)相对于视锥体的位置关系。
        * 用于快速裁剪场景中的物体或物体组。
        * 
        * 算法：使用P-顶点和N-顶点方法
        * - P-顶点：AABB在平面法线方向最远的顶点
        * - N-顶点：AABB在平面法线方向最近的顶点
        * - 如果P-顶点在平面外侧：AABB完全在外，返回OUTSIDE
        * - 如果N-顶点在平面内侧：AABB完全在内，继续检查下一平面
        * - 否则：AABB与平面相交，返回INTERSECT
        * 
        * @param box 轴对齐包围盒
        * @return OUTSIDE/INTERSECT/INSIDE 表示包围盒的可见性状态
        */
        Scope BoxIn(const AABB &box)const;
    };//class Frustum
}//namespace hgl::math
