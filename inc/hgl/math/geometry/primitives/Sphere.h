/**
 * Sphere.h - 简化的球体几何图元
 *
 * 这是一个轻量级的球体类，仅专注于：
 * - 属性存储与访问
 * - 简单且高频的几何查询
 * - 用于粗略碰撞检测的包围盒计算
 *
 * 复杂的碰撞检测已移至 CollisionDetector，
 * 保持该类简洁专注于核心职责。
 *
 * 为了兼容，原有 Sphere 类仍保留在 inc/hgl/math/geometry/primitives/Sphere.h。
 */
#pragma once

#include<cmath>
#include<numbers>
#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>

namespace hgl::math
{
    /**
     * Sphere - 标准球体，半径均匀
     *
     * 数学定义：
     * 表面方程：||P - center|| = radius
     * 即 (x-cx)² + (y-cy)² + (z-cz)² = r²
     *
     * 属性：
     * - 完美对称（各向同性）
     * - 表面积/体积比最小
     * - 旋转不变性
     * - 最简单的碰撞检测
     *
     * 常见用途：
     * - 用于粗略碰撞检测的包围球
     * - 粒子系统
     * - 光照衰减范围
     * - 物理仿真
     */
    class Sphere
    {
        Vector3f center;     // Sphere center position
        float radius;        // Sphere radius

    public:

        /** 获取球心 */
        const Vector3f& GetCenter() const { return center; }

        /** 获取球半径 */
        float GetRadius() const { return radius; }

    public:

        /**
         * 默认构造函数
         * 在原点创建一个半径为1的单位球
         */
        Sphere()
            : center(0, 0, 0), radius(1.0f)
        {
        }

        /**
         * 带参数构造函数
         * @param c 球心位置
         * @param r 球半径
         */
        Sphere(const Vector3f& c, float r)
            : center(c), radius(r)
        {
        }

        /**
         * 设置球体参数
         * @param c 新球心位置
         * @param r 新半径
         */
        void Set(const Vector3f& c, float r)
        {
            center = c;
            radius = r;
        }

        /**
         * 计算球体体积
         * 公式：V = (4/3)πr³
         * @return 球体体积
         */
        float GetVolume() const
        {
            return (4.0f / 3.0f) * std::numbers::pi_v<float> * radius * radius * radius;
        }

        /**
         * 计算球体表面积
         * 公式：S = 4πr²
         * @return 球体表面积
         */
        float GetSurfaceArea() const
        {
            return 4.0f * std::numbers::pi_v<float> * radius * radius;
        }

        /**
         * 获取球面上某方向的点
         *
         * 返回球心沿给定方向的球面点，direction无需归一化。
         *
         * @param direction 从球心出发的方向向量
         * @return 球面上的点
         */
        Vector3f GetPoint(const Vector3f& direction) const
        {
            const float len = std::hypot(direction.x, std::hypot(direction.y, direction.z));
            const float inv_len = (len > 0.0f) ? (1.0f / len) : 0.0f;
            return center + direction * (inv_len * radius);
        }

        /**
         * 判断点是否在球体内或表面
         * @param point 待测试点
         * @return 若在内部或表面返回true，否则返回false
         */
        bool ContainsPoint(const Vector3f& point) const
        {
            const Vector3f diff = point - center;
            const float dist2 = std::fma(diff.z, diff.z, std::fma(diff.y, diff.y, diff.x * diff.x));
            return dist2 <= radius * radius;
        }

        /**
         * 求空间中某点到球面最近的点
         * @param point 空间中的点
         * @return 球面上最近的点
         */
        Vector3f ClosestPoint(const Vector3f& point) const
        {
            const Vector3f dir = point - center;
            const float dist = std::hypot(dir.x, std::hypot(dir.y, dir.z));

            if (dist <= radius)
                return point;  // Point is inside, it is its own closest point

            const float scale = (dist > 0.0f) ? (radius / dist) : 0.0f;
            return center + dir * scale;
        }

        /**
         * 计算空间中某点到球面表面的距离
         * @param point 空间中的点
         * @return 到表面的距离（若在内部则为0）
         */
        float DistanceToPoint(const Vector3f& point) const
        {
            const Vector3f diff = point - center;
            const float dist = std::hypot(diff.x, std::hypot(diff.y, diff.z)) - radius;
            return dist > 0.0f ? dist : 0.0f;
        }

        /**
         * 获取用于粗略碰撞检测的轴对齐包围盒
         * @return 紧密包含该球体的AABB
         */
        AABB GetBoundingBox() const
        {
            Vector3f r(radius, radius, radius);
            AABB box;
            box.SetMinMax(center - r, center + r);
            return box;
        }

    };//class Sphere

    /**
     * EllipseSphere（椭球体）- xyz方向半径不等
     *
     * 椭球体是球体在三个坐标轴方向半径不同的推广，类似拉伸或压扁的球体。
     *
     * 数学定义：
     * 椭球方程：(x-cx)²/a² + (y-cy)²/b² + (z-cz)²/c² = 1
     * 其中a、b、c分别为各轴半径
     *
     * 与标准球体的区别：
     * - 球体：radius.x = radius.y = radius.z（完全对称）
     * - 椭球体：radius.x ≠ radius.y ≠ radius.z（各向异性）
     *
     * 特殊情况：
     * - Spheroid：两个半径相等（如地球：赤道半径相等，极半径不同）
     * - 扁球体：极半径 < 赤道半径（地球形状）
     * - 长球体：极半径 > 赤道半径（橄榄球形状）
     *
     * 常见用途：
     * - 天体建模（地球、行星）
     * - 角色动画（头部、身体近似）
     * - 粒子特效（非均匀扩散）
     * - 声音在非均匀介质中的传播
     * - 物理碰撞（椭球刚体）
     */
    class EllipseSphere
    {
        Vector3f center{0, 0, 0};  // Ellipsoid center position
        Vector3f radius{1, 1, 1};  // Three axis radii (rx, ry, rz)

    public:

        /** 获取椭球体中心 */
        const Vector3f& GetCenter() const { return center; }

        /** 获取三个方向的半径向量 */
        const Vector3f& GetRadius() const { return radius; }

    public:

        /**
         * 默认构造函数
         * 在原点创建一个单位椭球体，所有半径为1（退化为球体）
         */
        EllipseSphere() = default;

        /**
         * 带参数构造函数
         * @param c 椭球体中心位置
         * @param r 三个方向的半径（rx, ry, rz）
         */
        EllipseSphere(const Vector3f& c, const Vector3f& r)
        {
            Set(c, r);
        }

        /**
         * 设置椭球体参数
         * @param c 新中心位置
         * @param r 新的三个方向半径
         */
        void Set(const Vector3f& c, const Vector3f& r)
        {
            center = c;
            radius = r;
        }

        /**
         * 计算椭球体体积
         * 公式：V = (4/3)π * rx * ry * rz
         * @return 椭球体体积
         */
        float GetVolume() const
        {
            return (4.0f / 3.0f) * std::numbers::pi_v<float> * radius.x * radius.y * radius.z;
        }

        /**
         * 计算椭球体表面积（近似）
         * 使用 Knud Thomsen 公式近似，精确表面积需椭圆积分（计算量大）
         * @return 近似表面积
         */
        float GetSurfaceArea() const
        {
            // Knud Thomsen's formula (approximation)
            float p = 1.6075f;
            float ap = pow(radius.x, p);
            float bp = pow(radius.y, p);
            float cp = pow(radius.z, p);
            return 4.0f * std::numbers::pi_v<float> * pow((ap * bp + ap * cp + bp * cp) / 3.0f, 1.0f / p);
        }

        /**
         * 获取椭球体表面某方向的点
         *
         * 注意：椭球体会按各轴半径缩放方向向量，返回点可能不在精确方向上。
         *
         * @param direction 方向向量
         * @return 该方向附近的椭球体表面点
         */
        Vector3f GetPoint(const Vector3f& direction) const
        {
            return center + direction * radius;
        }

        /**
         * 判断点是否在椭球体内
         * 使用标准椭球方程
         * @param point 待测试点
         * @return 若在内部或表面返回true，否则返回false
         */
        bool ContainsPoint(const Vector3f& point) const
        {
            Vector3f d = point - center;
            float x2 = (d.x * d.x) / (radius.x * radius.x);
            float y2 = (d.y * d.y) / (radius.y * radius.y);
            float z2 = (d.z * d.z) / (radius.z * radius.z);
            return (x2 + y2 + z2) <= 1.0f;
        }

        /**
         * 获取轴对齐包围盒
         * @return 包含该椭球体的AABB
         */
        AABB GetBoundingBox() const
        {
            AABB box;
            box.SetMinMax(center - radius, center + radius);
            return box;
        }

    };//class EllipseSphere

}//namespace hgl::math
