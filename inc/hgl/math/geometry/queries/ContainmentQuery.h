/**
 * ContainmentQuery.h - 几何体包含与内含性测试
 *
 * 提供判断一个几何体是否完全包含另一个的测试。
 * 适用场景：
 * - 分层空间查询
 * - 视锥体裁剪
 * - 区域测试
 * - 包围体优化
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<hgl/math/geometry/primitives/Capsule.h>

namespace hgl::math
{
    /**
     * ContainmentQuery - 静态包含性测试方法
     *
     * 提供两类查询：
     * 1. 点包含：点是否在几何体内？（委托几何体的ContainsPoint方法）
     * 2. 几何体包含：A几何体是否完全包含B几何体？
     *
     * 用法示例：
     *     Sphere s(...);
     *     AABB box(...);
     *
     *     // 点包含
     *     if (ContainmentQuery::Contains(s, point))
     *         // 点在球体内
     *
     *     // 几何体包含
     *     if (ContainmentQuery::Contains(box, s))
     *         // 球体完全在盒子内
     */
    class ContainmentQuery
    {
    public:

        //=============================================================================
        // 点包含（委托几何体类，保持一致性）
        //=============================================================================

        /**
         * 测试点是否在球体内
         * 委托Sphere::ContainsPoint实现
         */
        static bool Contains(const Sphere& container, const Vector3f& point)
        {
            return container.ContainsPoint(point);
        }

        /**
         * 测试点是否在胶囊体内
         */
        static bool Contains(const Capsule& container, const Vector3f& point)
        {
            return container.ContainsPoint(point);
        }

        /**
         * 测试点是否在AABB内
         */
        static bool Contains(const AABB& container, const Vector3f& point)
        {
            return container.ContainsPoint(point);
        }

        /**
         * 测试点是否在OBB内
         */
        static bool Contains(const OBB& container, const Vector3f& point)
        {
            return container.ContainsPoint(point);
        }

        //=============================================================================
        // 球体包含
        //=============================================================================

        /**
         * 测试一个球体是否完全包含另一个球体
         *
         * 若A球体包含B球体：
         * distance(centerA, centerB) + radiusB <= radiusA
         *
         * @return 若完全包含返回true
         */
        static bool Contains(const Sphere& container, const Sphere& sphere);

        /**
         * 测试AABB是否完全包含球体
         *
         * 若AABB包含球体：
         * - 球心在AABB内，且边界距离>=半径
         *
         * @return 若完全包含返回true
         */
        static bool Contains(const AABB& container, const Sphere& sphere);

        //=============================================================================
        // AABB包含
        //=============================================================================

        /**
         * 测试一个AABB是否完全包含另一个AABB
         *
         * 若A包含B：
         * - B.min >= A.min（各维度）
         * - B.max <= A.max（各维度）
         *
         * @return 若完全包含返回true
         */
        static bool Contains(const AABB& container, const AABB& box);

        /**
         * 测试OBB是否完全包含AABB
         */
        static bool Contains(const OBB& container, const AABB& box);

        //=============================================================================
        // OBB包含
        //=============================================================================

        /**
         * 测试一个OBB是否完全包含另一个OBB
         *
         * 该测试较耗时，需要投影和分离轴测试。
         *
         * @return 若完全包含返回true
         */
        static bool Contains(const OBB& container, const OBB& box);
    };

}//namespace hgl::math
