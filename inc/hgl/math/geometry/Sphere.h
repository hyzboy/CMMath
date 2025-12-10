#pragma once
#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
    * 球体类(Sphere) - 标准球体(xyz半径相等)
    * 
    * 球体是3D空间中最简单和对称的几何体之一，由中心点和半径定义。
    * 球面上所有点到中心的距离都等于半径。
    * 
    * 数学定义：
    * 球面方程: ||P - center|| = radius
    * 即: (x-cx)² + (y-cy)² + (z-cz)² = r²
    * 
    * 特性：
    * - 完美的对称性（各向同性）
    * - 最小的表面积/体积比
    * - 旋转不变性
    * - 碰撞检测最简单高效
    * 
    * 应用场景：
    * - 包围球(Bounding Sphere)：快速粗略碰撞检测
    * - 粒子系统：粒子的影响范围
    * - 光照计算：点光源的照射范围
    * - 物理模拟：刚体球体
    * - LOD系统：根据距离选择细节级别
    * - 天体模拟：行星、恒星等
    */
    class Sphere
    {
        Vector3f center;  // 球心位置
        float radius;           // 球体半径

    public:

        /** 获取球心位置 */
        const Vector3f &GetCenter()const{return center;}
        
        /** 获取球体半径 */
        float GetRadius()const{return radius;}

    public:

        /**
        * 默认构造函数
        * 创建一个位于原点、半径为1的单位球
        */
        Sphere()
        {
            Set(Vector3f(0,0,0),1);
        }

        /**
        * 参数化构造函数
        * @param c 球心位置
        * @param r 球体半径
        */
        Sphere(const Vector3f &c,float r)
        {
            Set(c,r);
        }

        /**
        * 设置球体参数
        * @param c 新的球心位置
        * @param r 新的球体半径
        */
        void Set(const Vector3f &c,float r)
        {
            center=c;
            radius=r;
        }

        /**
        * 计算球体体积
        * 公式: V = (4/3)πr³
        * @return 球体的体积
        */
        float GetVolume()const
        {
            return (4.0f/3.0f)*math::pi*radius*radius*radius;
        }

        /**
        * 计算球体表面积
        * 公式: S = 4πr²
        * @return 球体的表面积
        */
        float GetSurfaceArea()const
        {
            return 4.0f*math::pi*radius*radius;
        }

        /**
        * 获取指定方向上的球面点
        * 
        * 沿给定方向从球心出发，到达球面的点。
        * direction不需要归一化，函数内部会自动处理。
        * 
        * @param direction 从球心指向目标点的方向向量
        * @return 球面上该方向的点坐标
        */
        Vector3f GetPoint(const Vector3f &direction)const
        {
            return center+normalize(direction)*radius;
        }

        /**
        * 判断点是否在球体内
        * @param point 待测试的点
        * @return true表示点在球内或球面上，false表示在球外
        */
        bool ContainsPoint(const Vector3f &point)const
        {
            return length_squared(point - center) <= radius * radius;
        }

        /**
        * 计算点到球面的最近点
        * @param point 空间中的任意点
        * @return 球面上距离该点最近的点
        */
        Vector3f ClosestPoint(const Vector3f &point)const
        {
            Vector3f dir = point - center;
            float dist = length(dir);
            
            if(dist <= radius)
                return point;  // 点在球内，最近点是自己
            
            return center + dir * (radius / dist);
        }

        /**
        * 计算点到球面的距离
        * @param point 空间中的任意点
        * @return 如果点在球外，返回到球面的距离；如果在球内，返回0
        */
        float DistanceToPoint(const Vector3f &point)const
        {
            float dist = length(point - center) - radius;
            return dist > 0.0f ? dist : 0.0f;
        }
    };//class Sphere

    /**
    * 椭球体类(Ellipsoid/Ellipse Sphere) - xyz半径不相等
    * 
    * 椭球体是球体的推广形式，在三个坐标轴方向上有不同的半径。
    * 形状类似于拉长或压扁的球体，如地球（极半径小于赤道半径）。
    * 
    * 数学定义：
    * 椭球面方程: (x-cx)²/a² + (y-cy)²/b² + (z-cz)²/c² = 1
    * 其中 a, b, c 分别是三个轴向的半径
    * 
    * 与标准球体的区别：
    * - 球体：radius.x = radius.y = radius.z（完美对称）
    * - 椭球：radius.x ≠ radius.y ≠ radius.z（各向异性）
    * 
    * 特殊情况：
    * - 旋转椭球：两个半径相等（如地球：赤道半径相等，极半径不同）
    * - 扁椭球(Oblate)：极半径 < 赤道半径（地球形状）
    * - 长椭球(Prolate)：极半径 > 赤道半径（橄榄球形状）
    * 
    * 应用场景：
    * - 天体建模：地球、行星等非完美球体
    * - 角色动画：头部、身体等部位的近似形状
    * - 粒子效果：非均匀扩散的粒子系统
    * - 声音传播：非均匀介质中的声波范围
    * - 物理碰撞：椭球形刚体
    */
    class EllipseSphere
    {
        Vector3f center;  // 椭球中心位置
        Vector3f radius;  // 三个轴向的半径 (rx, ry, rz)

    public:

        /** 获取椭球中心位置 */
        const Vector3f &GetCenter()const{return center;}
        
        /** 获取三个轴向的半径向量 */
        const Vector3f &GetRadius()const{return radius;}

    public:

        /**
        * 默认构造函数
        * 创建一个位于原点、各轴半径为1的单位椭球（退化为球体）
        */
        EllipseSphere()
        {
            Set(Vector3f(0,0,0),Vector3f(1,1,1));
        }

        /**
        * 参数化构造函数
        * @param c 椭球中心位置
        * @param r 三个轴向的半径 (rx, ry, rz)
        */
        EllipseSphere(const Vector3f&c,const Vector3f&r)
        {
            Set(c,r);
        }

        /**
        * 设置椭球参数
        * @param c 新的椭球中心位置
        * @param r 新的三个轴向半径
        */
        void Set(const Vector3f&c,const Vector3f&r)
        {
            center=c;
            radius=r;
        }

        /**
        * 计算椭球体积
        * 公式: V = (4/3)π * rx * ry * rz
        * @return 椭球的体积
        */
        float GetVolume()const
        {
            return (4.0f/3.0f)*math::pi*radius.x*radius.y*radius.z;
        }

        /**
        * 计算椭球表面积（近似公式）
        * 使用Knud Thomsen公式的近似值
        * 精确表面积需要椭圆积分，计算复杂
        * @return 椭球表面积的近似值
        */
        float GetSurfaceArea()const
        {
            // Knud Thomsen's formula (approximation)
            float p = 1.6075f;
            float ap = pow(radius.x, p);
            float bp = pow(radius.y, p);
            float cp = pow(radius.z, p);
            return 4.0f * math::pi * pow((ap*bp + ap*cp + bp*cp) / 3.0f, 1.0f/p);
        }

        /**
        * 获取指定方向上的椭球面点
        * 
        * 注意：对于椭球，方向向量会被各轴半径缩放。
        * 这意味着返回的点可能不在精确的direction方向上。
        * 
        * @param direction 方向向量
        * @return 椭球面上该方向附近的点
        */
        Vector3f GetPoint(const Vector3f &direction)const
        {
            return center+direction*radius;
        }

        /**
        * 判断点是否在椭球体内
        * 使用标准椭球方程判断
        * @param point 待测试的点
        * @return true表示点在椭球内或面上，false表示在外
        */
        bool ContainsPoint(const Vector3f &point)const
        {
            Vector3f d = point - center;
            float x2 = (d.x * d.x) / (radius.x * radius.x);
            float y2 = (d.y * d.y) / (radius.y * radius.y);
            float z2 = (d.z * d.z) / (radius.z * radius.z);
            return (x2 + y2 + z2) <= 1.0f;
        }
    };//class EllipseSphere
}//namespace hgl::math
