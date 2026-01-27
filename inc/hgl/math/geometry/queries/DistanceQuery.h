/**
 * DistanceQuery.h - 各类几何体间距离计算
 *
 * 提供各种几何体间的距离测量和最近点查找。主要用于：
 * - 邻近性查询
 * - 最小距离计算
 * - 碰撞预测
 * - 分离向量计算
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<hgl/math/geometry/primitives/Capsule.h>
#include<hgl/math/geometry/primitives/Cylinder.h>

namespace hgl::math
{
    /**
     * 最近点结果结构体
     *
     * 包含两个几何体间最近点对及其距离。
     */
    struct ClosestPointsResult
    {
        Vector3f pointOnA;       // 第一个几何体上的最近点
        Vector3f pointOnB;       // 第二个几何体上的最近点
        float distance;          // 两点间距离

        ClosestPointsResult()
            : pointOnA(0, 0, 0), pointOnB(0, 0, 0), distance(0.0f)
        {
        }
    };

    /**
     * DistanceQuery - 静态距离计算方法
     *
     * 提供几何体间距离测量和最近点查询。
     *
     * 查询类型：
     * 1. Distance(point, geometry)：点到几何体距离
     * 2. Distance(geom1, geom2)：几何体间距离
     * 3. ClosestPoints(geom1, geom2)：查找最近点对
     *
     * 用法示例：
     *     Sphere s(...);
     *     Capsule c(...);
     *     float dist = DistanceQuery::Distance(s, c);
     *     ClosestPointsResult result = DistanceQuery::ClosestPoints(s, c);
     */
    class DistanceQuery
    {
    public:

        //=============================================================================
        // 点到几何体距离
        //=============================================================================

        /**
         * 点到球体表面的距离
         * @return 距离（若点在内部为0）
         */
        static float Distance(const Vector3f& point, const Sphere& sphere);

        /**
         * 点到胶囊体表面的距离
         */
        static float Distance(const Vector3f& point, const Capsule& capsule);

        /**
         * 点到AABB表面的距离
         */
        static float Distance(const Vector3f& point, const AABB& box);

        /**
         * 点到圆柱体表面的距离
         */
        static float Distance(const Vector3f& point, const Cylinder& cylinder);

        //=============================================================================
        // 几何体间距离
        //=============================================================================

        /**
         * 两球体表面间距离
         * @return 距离（若相交为0）
         */
        static float Distance(const Sphere& a, const Sphere& b);

        /**
         * 球体与胶囊体间距离
         */
        static float Distance(const Sphere& sphere, const Capsule& capsule);
        static float Distance(const Capsule& capsule, const Sphere& sphere)
        {
            return Distance(sphere, capsule);
        }

        /**
         * 两胶囊体间距离
         */
        static float Distance(const Capsule& a, const Capsule& b);

        /**
         * 两圆柱体间距离
         */
        static float Distance(const Cylinder& a, const Cylinder& b);

        //=============================================================================
        // 最近点对
        //=============================================================================

        /**
         * 查找两胶囊体间最近点对
         * @return 包含两胶囊体上最近点及距离的结果
         */
        static ClosestPointsResult ClosestPoints(const Capsule& a, const Capsule& b);

        /**
         * 查找球体与胶囊体间最近点对
         */
        static ClosestPointsResult ClosestPoints(const Sphere& sphere, const Capsule& capsule);
        static ClosestPointsResult ClosestPoints(const Capsule& capsule, const Sphere& sphere)
        {
            auto result = ClosestPoints(sphere, capsule);
            std::swap(result.pointOnA, result.pointOnB);
            return result;
        }

        //=============================================================================
        // 辅助函数
        //=============================================================================

        /**
         * 查找点到线段的最近点
         *
         * @param point 查询点
         * @param segmentStart 线段起点
         * @param segmentEnd 线段终点
         * @return 线段上的最近点
         */
        static Vector3f ClosestPointOnLineSegment(const Vector3f& point,
                                                  const Vector3f& segmentStart,
                                                  const Vector3f& segmentEnd);

        /**
         * 查找两线段间最近点对
         *
         * 这是许多几何体间距离计算的基础。
         *
         * @param seg1Start 第一条线段起点
         * @param seg1End 第一条线段终点
         * @param seg2Start 第二条线段起点
         * @param seg2End 第二条线段终点
         * @return 包含两线段上最近点的结果
         */
        static ClosestPointsResult ClosestPointsOnLineSegments(
            const Vector3f& seg1Start, const Vector3f& seg1End,
            const Vector3f& seg2Start, const Vector3f& seg2End);
    };

}//namespace hgl::math
