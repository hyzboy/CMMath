/**
 * RaycastQuery.h - 射线与几何体相交测试
 *
 * 提供所有几何体的射线投射与相交测试。
 * 射线投射常用于：
 * - 鼠标拾取/选择
 * - 视线检测
 * - 抛射物碰撞
 * - AI视觉系统
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/Ray.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/Plane.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<hgl/math/geometry/primitives/Capsule.h>
#include<hgl/math/geometry/primitives/Cylinder.h>
#include<hgl/math/geometry/primitives/Cone.h>
#include<hgl/math/geometry/primitives/Torus.h>

namespace hgl::math
{
    /**
     * 射线命中信息
     *
     * 包含射线与几何体相交的详细信息：
     * - hit：是否命中几何体
     * - distance：射线到命中点的距离（t值）
     * - point：命中点（世界空间，ray.origin + ray.direction * distance）
     * - normal：命中点的表面法线
     */
    struct RaycastHit
    {
        bool hit;                // 是否命中几何体
        float distance;          // 沿射线的距离（t参数）
        Vector3f point;          // 命中点（世界空间）
        Vector3f normal;         // 命中点的表面法线

        RaycastHit()
            : hit(false), distance(FLT_MAX), point(0, 0, 0), normal(0, 1, 0)
        {
        }
    };

    /**
     * RaycastQuery - 静态射线相交方法
     *
     * 提供所有几何体类型的射线投射操作。
     * 方法分为两类：
     * - Intersects(..., float& t)：简单布尔测试并输出距离
     * - Test(...)：详细命中信息（点和法线）
     *
     * 用法示例：
     *     Ray ray(origin, direction);
     *     Sphere sphere(center, radius);
     *
     *     // 简单测试
     *     float t;
     *     if (RaycastQuery::Intersects(ray, sphere, t))
     *         Vector3f hitPoint = ray.origin + ray.direction * t;
     *
     *     // 详细测试
     *     RaycastHit hit = RaycastQuery::Test(ray, sphere);
     *     if (hit.hit)
     *         // 使用 hit.point, hit.normal, hit.distance
     */
    class RaycastQuery
    {
    public:

        //=============================================================================
        // 射线-球体相交
        //=============================================================================

        /**
         * 测试射线-球体相交
         * @param ray 待测射线
         * @param sphere 待测球体
         * @param t 输出：射线到命中点的距离
         * @return 若相交返回true
         */
        static bool Intersects(const Ray& ray, const Sphere& sphere, float& t);

        /**
         * 简单射线-球体相交测试（无距离输出）
         */
        static bool Intersects(const Ray& ray, const Sphere& sphere);

        /**
         * 详细射线-球体相交测试
         * @return 包含完整命中信息的RaycastHit
         */
        static RaycastHit Test(const Ray& ray, const Sphere& sphere);

        //=============================================================================
        // 射线-胶囊体相交
        //=============================================================================

        /**
         * 测试射线-胶囊体相交
         */
        static bool Intersects(const Ray& ray, const Capsule& capsule, float& t);
        static bool Intersects(const Ray& ray, const Capsule& capsule);
        static RaycastHit Test(const Ray& ray, const Capsule& capsule);

        //=============================================================================
        // 射线-圆柱体相交
        //=============================================================================

        /**
         * 测试射线-圆柱体相交
         */
        static bool Intersects(const Ray& ray, const Cylinder& cylinder, float& t);
        static bool Intersects(const Ray& ray, const Cylinder& cylinder);
        static RaycastHit Test(const Ray& ray, const Cylinder& cylinder);

        //=============================================================================
        // 射线-圆锥体相交
        //=============================================================================

        /**
         * 测试射线-圆锥体相交
         */
        static bool Intersects(const Ray& ray, const Cone& cone, float& t);
        static bool Intersects(const Ray& ray, const Cone& cone);
        static RaycastHit Test(const Ray& ray, const Cone& cone);

        //=============================================================================
        // 射线-环面相交
        //=============================================================================

        /**
         * 测试射线-环面相交
         */
        static bool Intersects(const Ray& ray, const Torus& torus, float& t);
        static bool Intersects(const Ray& ray, const Torus& torus);
        static RaycastHit Test(const Ray& ray, const Torus& torus);

        //=============================================================================
        // 射线-AABB/OBB/平面（统一接口）
        //=============================================================================

        /**
         * 测试射线-AABB相交
         */
        static bool Intersects(const Ray& ray, const AABB& box, float& t);
        static bool Intersects(const Ray& ray, const AABB& box);
        static RaycastHit Test(const Ray& ray, const AABB& box);

        /**
         * 测试射线-OBB相交
         */
        static bool Intersects(const Ray& ray, const OBB& box, float& t);
        static bool Intersects(const Ray& ray, const OBB& box);
        static RaycastHit Test(const Ray& ray, const OBB& box);

        /**
         * 测试射线-平面相交
         */
        static bool Intersects(const Ray& ray, const Plane& plane, float& t);
        static bool Intersects(const Ray& ray, const Plane& plane);
        static RaycastHit Test(const Ray& ray, const Plane& plane);

        //=============================================================================
        // 射线-三角形相交（Möller-Trumbore算法）
        //=============================================================================

        /**
         * 使用Möller-Trumbore算法测试射线-三角形相交
         *
         * @param ray 待测射线
         * @param v0, v1, v2 三角形顶点
         * @param t 输出：射线到交点距离
         * @param u, v 输出：三角形重心坐标
         * @return 若相交返回true
         */
        static bool IntersectsTriangle(const Ray& ray,
                                       const Vector3f& v0,
                                       const Vector3f& v1,
                                       const Vector3f& v2,
                                       float& t, float& u, float& v);

        /**
         * 简单射线-三角形相交测试（无重心坐标输出）
         */
        static bool IntersectsTriangle(const Ray& ray,
                                       const Vector3f& v0,
                                       const Vector3f& v1,
                                       const Vector3f& v2,
                                       float& t);
    };
}//namespace hgl::math
