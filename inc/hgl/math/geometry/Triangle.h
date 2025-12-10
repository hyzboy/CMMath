/**
* 三角形几何类
* 
* 提供2D和3D三角形的表示和操作功能。
* 三角形是计算机图形学中最基本的图元，广泛用于：
* - 网格建模（模型的基本单元）
* - 光栅化渲染
* - 碰撞检测
* - 射线追踪
* - 物理模拟
*/
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/Area.h>

namespace hgl::math
{
    /**
    * 判断点是否在2D三角形内
    * 
    * 使用重心坐标法(Barycentric Coordinates)判断点是否在三角形内部。
    * 
    * 重心坐标原理：
    * 三角形内的任意点P可以表示为三个顶点的线性组合：
    * P = u*v0 + v*v1 + w*v2，其中 u+v+w=1
    * 
    * 点在三角形内的充要条件：
    * u >= 0 && v >= 0 && w >= 0 (由于u+v+w=1，只需检查两个即可)
    * 
    * 算法步骤：
    * 1. 计算三角形的两条边向量
    * 2. 计算点相对于顶点的向量
    * 3. 通过点积计算重心坐标
    * 4. 判断坐标是否在有效范围内
    * 
    * @param vertex 三角形的三个顶点数组
    * @param pt 待测试的点
    * @return true表示点在三角形内（包括边界），false表示在外
    */
    template<typename T>
    inline const bool PointInTriangle(const glm::vec<2,T> *vertex,const glm::vec<2,T> &pt)
    {
        // Compute vectors
        glm::dvec2 v2v1 = vertex[1] - vertex[0];
        glm::dvec2 v3v1 = vertex[2] - vertex[0];
        glm::dvec2 ptv1 = pt        - vertex[0];

        // Compute dot products
        double dot00 = glm::dot(v3v1, v3v1);
        double dot01 = glm::dot(v3v1, v2v1);
        double dot02 = glm::dot(v3v1, ptv1);
        double dot11 = glm::dot(v2v1, v2v1);
        double dot12 = glm::dot(v2v1, ptv1);

        // Compute barycentric coordinates
        double invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
        double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
        double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

        // Check if point is in triangle
        return (u >= 0) && (v >= 0) && (u + v < 1);
    }

    /**
    * 2D三角形类
    * 
    * 表示2D平面上的三角形，由三个顶点定义。
    * 
    * 常用功能：
    * - 顶点管理：设置和获取三个顶点
    * - 几何计算：面积、边长、重心等
    * - 点包含测试：判断点是否在三角形内
    * 
    * 应用场景：
    * - 2D图形渲染
    * - UI界面元素
    * - 2D碰撞检测
    * - 平面几何计算
    * - 纹理映射（UV坐标）
    * 
    * @tparam T 坐标类型（short, int, float, double等）
    */
    template<typename T> class Triangle2
    {
        glm::vec<2,T> vertex[3];    // 三角形的三个顶点

    public:

        Triangle2()
        {
            vertex[0]=glm::vec<2,T>(0,0);
            vertex[1]=glm::vec<2,T>(0,0);
            vertex[2]=glm::vec<2,T>(0,0);
        }

        Triangle2(const glm::vec<2,T> &v0,const glm::vec<2,T> &v1,const glm::vec<2,T> &v2)
        {
            vertex[0]=v0;
            vertex[1]=v1;
            vertex[2]=v2;
        }

        virtual ~Triangle2()=default;

        void SetVertex(const glm::vec<2,T> v[3])
        {
            vertex=v;
        }

        void SetVertex(const glm::vec<2,T> &v0,const glm::vec<2,T> &v1,const glm::vec<2,T> &v2)
        {
            vertex[0]=v0;
            vertex[1]=v1;
            vertex[2]=v2;
        }

        void SetVertex(const int index,const glm::vec<2,T> &v)
        {
            if(index>=0&&index<=2)
                vertex[index]=v;
        }

        const glm::vec<2,T> &GetVertex(const int index)const
        {
            if(index>=0&&index<=2)return vertex[index];

            return glm::vec<2,T>(0,0);
        }

        glm::vec<2,T> &operator[](const int index)
        {
            return vertex[index];
        }

        const glm::vec<2,T> &operator[](const int index)const
        {
            return vertex[index];
        }

        const T GetEdgeLength(const int index)const
        {
            if(index==0)return length_2d(vertex[0],vertex[1]);else
            if(index==1)return length_2d(vertex[1],vertex[2]);else
            if(index==2)return length_2d(vertex[2],vertex[0]);else
                return 0;
        }

        const T Area()const
        {
            return TriangleArea(vertex[0],vertex[1],vertex[2]);
        }

        const bool PointIn(const glm::vec<2,T> &pt)const
        {
            return PointInTriangle<T>(vertex,pt);
        }
    };//template<typename T> class Triangle2

    // 2D三角形的常用类型别名
    using Triangle2s=Triangle2<short>;      // 短整型顶点（节省内存，用于大量简单图形）
    using Triangle2i=Triangle2<int>;        // 整型顶点（精确像素坐标）
    using Triangle2f=Triangle2<float>;      // 单精度浮点顶点（最常用，性能和精度平衡）
    using Triangle2d=Triangle2<double>;     // 双精度浮点顶点（高精度计算）

    /**
    * 3D三角形类
    * 
    * 表示3D空间中的三角形，是3D图形学最基本的图元。
    * 
    * 核心特性：
    * - 三个顶点定义一个平面
    * - 可计算法线向量（用于光照）
    * - 支持各种空间变换
    * - 用于构建复杂3D模型
    * 
    * 常用功能：
    * - 顶点管理：设置和获取三个顶点
    * - 几何计算：面积、周长、法线、重心
    * - 空间关系：与射线相交、点到三角形距离
    * - 变换操作：平移、旋转、缩放
    * 
    * 应用场景：
    * - 3D模型网格（Mesh）的基本单元
    * - 光线追踪的相交测试
    * - 物理碰撞检测
    * - 阴影计算
    * - 视锥体裁剪
    * - 纹理映射
    * 
    * 重要概念：
    * - 法线(Normal)：垂直于三角形平面的向量，用于光照计算
    * - 环绕顺序(Winding Order)：顶点顺序决定正反面
    *   - 逆时针(CCW)：通常定义为正面
    *   - 顺时针(CW)：通常定义为背面
    * - 重心坐标：用于插值顶点属性（颜色、法线、UV等）
    * 
    * @tparam T 坐标类型（short, int, float, double等）
    */
    template<typename T> class Triangle3
    {
        glm::vec<3,T> vertex[3];    // 三角形的三个3D顶点

    public:

        Triangle3()
        {
            vertex[0]=glm::vec<3,T>(0,0,0);
            vertex[1]=glm::vec<3,T>(0,0,0);
            vertex[2]=glm::vec<3,T>(0,0,0);
        }

        Triangle3(const glm::vec<3,T> &v0,const glm::vec<3,T> &v1,const glm::vec<3,T> &v2)
        {
            vertex[0]=v0;
            vertex[1]=v1;
            vertex[2]=v2;
        }

        virtual ~Triangle3()=default;

        void SetVertex(const glm::vec<3,T> v[3])
        {
            vertex=v;
        }

        void SetVertex(const glm::vec<3,T> &v0,const glm::vec<3,T> &v1,const glm::vec<3,T> &v2)
        {
            vertex[0]=v0;
            vertex[1]=v1;
            vertex[2]=v2;
        }

        void SetVertex(const int index,const glm::vec<3,T> &v)
        {
            if(index>=0&&index<=2)
                vertex[index]=v;
        }

        const glm::vec<3,T> &GetVertex(const int index)const
        {
            if(index>=0&&index<=2)return vertex[index];

            return glm::vec<3,T>(0,0,0);
        }

        glm::vec<3,T> &operator[](const int index)
        {
            return vertex[index];
        }

        const glm::vec<3,T> &operator[](const int index)const
        {
            return vertex[index];
        }

        const T GetEdgeLength(const int index)const
        {
            if(index==0)return length_3d(vertex[0],vertex[1]);else
            if(index==1)return length_3d(vertex[1],vertex[2]);else
            if(index==2)return length_3d(vertex[2],vertex[0]);else
                return 0;
        }

        const T Area()const
        {
            return TriangleArea(vertex[0],vertex[1],vertex[2]);
        }
    };//template<typename T> class Triangle3

    // 3D三角形的常用类型别名
    using Triangle3s=Triangle3<short>;      // 短整型顶点（极少使用）
    using Triangle3i=Triangle3<int>;        // 整型顶点（体素或离散空间）
    using Triangle3f=Triangle3<float>;      // 单精度浮点顶点（最常用，渲染和物理模拟）
    using Triangle3d=Triangle3<double>;     // 双精度浮点顶点（科学计算、CAD等高精度场景）
}//namespace hgl::math
