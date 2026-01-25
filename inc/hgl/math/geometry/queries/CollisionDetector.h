/**
 * CollisionDetector.h - 统一的碰撞检测系统
 *
 * 集中式碰撞检测工具，处理各种几何体之间的相交测试和碰撞信息。
 *
 * 设计原则：
 * - 静态方法（无状态，线程安全）
 * - 基于模板的多态（无虚函数开销）
 * - 两阶段检测：粗略阶段（AABB）+ 精确阶段
 * - 对称重载，使用直观
 *
 * 该类将复杂的碰撞逻辑从几何体类中剥离，使其保持轻量、专注于数据表达。
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<hgl/math/geometry/primitives/Capsule.h>
#include<hgl/math/geometry/primitives/Cylinder.h>
#include<hgl/math/geometry/primitives/Cone.h>
#include<hgl/math/geometry/primitives/Torus.h>

namespace hgl::math
{
    /**
     * 碰撞信息结构体
     *
     * 包含碰撞/相交的详细信息：
     * - intersects：几何体是否相交
     * - point：接触/碰撞点（世界空间）
     * - normal：碰撞法线（分离方向）
     * - penetration：穿透深度（相交时）
     * - distance：几何体间距离（未相交时）
     */
    struct CollisionInfo
    {
        bool intersects;         // 是否相交
        Vector3f point;          // 碰撞/接触点
        Vector3f normal;         // 碰撞法线（已归一化）
        float penetration;       // 穿透深度（相交时>0）
        float distance;          // 间距（未相交时）

        CollisionInfo()
            : intersects(false), point(0, 0, 0), normal(0, 1, 0)
            , penetration(0.0f), distance(0.0f)
        {
        }
    };

    /**
     * CollisionDetector - 静态碰撞检测方法
     *
     * 提供所有几何体对的相交测试和详细碰撞信息。
     *
     * 用法示例：
     *     Sphere s(...);
     *     Capsule c(...);
     *     bool hit = CollisionDetector::Intersects(s, c);
     *     CollisionInfo info = CollisionDetector::TestCollision(s, c);
     */
    class CollisionDetector
    {
    public:

        //=============================================================================
        // 球体碰撞方法
        //=============================================================================

        /**
         * 测试球体-球体是否相交
         * @return 若重叠返回true
         */
        static bool Intersects(const Sphere& a, const Sphere& b);

        /**
         * 获取球体-球体详细碰撞信息
         * @return 包含接触点、法线和穿透深度的碰撞信息
         */
        static CollisionInfo TestCollision(const Sphere& a, const Sphere& b);

        /**
         * 测试球体-AABB是否相交
         */
        static bool Intersects(const Sphere& sphere, const AABB& box);
        static bool Intersects(const AABB& box, const Sphere& sphere)
        {
            return Intersects(sphere, box);
        }

        /**
         * 测试球体-OBB是否相交
         */
        static bool Intersects(const Sphere& sphere, const OBB& box);
        static bool Intersects(const OBB& box, const Sphere& sphere)
        {
            return Intersects(sphere, box);
        }

        /**
         * 测试球体-胶囊体是否相交
         */
        static bool Intersects(const Sphere& sphere, const Capsule& capsule);
        static bool Intersects(const Capsule& capsule, const Sphere& sphere)
        {
            return Intersects(sphere, capsule);
        }

        //=============================================================================
        // 胶囊体碰撞方法
        //=============================================================================

        /**
         * 测试胶囊体-胶囊体是否相交
         */
        static bool Intersects(const Capsule& a, const Capsule& b);

        /**
         * 获取胶囊体-胶囊体详细碰撞信息
         */
        static CollisionInfo TestCollision(const Capsule& a, const Capsule& b);

        /**
         * 测试胶囊体-AABB是否相交
         */
        static bool Intersects(const Capsule& capsule, const AABB& box);
        static bool Intersects(const AABB& box, const Capsule& capsule)
        {
            return Intersects(capsule, box);
        }

        /**
         * 测试胶囊体-OBB是否相交
         */
        static bool Intersects(const Capsule& capsule, const OBB& box);
        static bool Intersects(const OBB& box, const Capsule& capsule)
        {
            return Intersects(capsule, box);
        }

        /**
         * 测试胶囊体-圆柱体是否相交
         */
        static bool Intersects(const Capsule& capsule, const Cylinder& cylinder);
        static bool Intersects(const Cylinder& cylinder, const Capsule& capsule)
        {
            return Intersects(capsule, cylinder);
        }

        //=============================================================================
        // 圆柱体碰撞方法
        //=============================================================================

        /**
         * 测试圆柱体-圆柱体是否相交
         */
        static bool Intersects(const Cylinder& a, const Cylinder& b);

        /**
         * 测试圆柱体-球体是否相交
         */
        static bool Intersects(const Cylinder& cylinder, const Sphere& sphere);
        static bool Intersects(const Sphere& sphere, const Cylinder& cylinder)
        {
            return Intersects(cylinder, sphere);
        }

        /**
         * 测试圆柱体-AABB是否相交
         */
        static bool Intersects(const Cylinder& cylinder, const AABB& box);
        static bool Intersects(const AABB& box, const Cylinder& cylinder)
        {
            return Intersects(cylinder, box);
        }

        //=============================================================================
        // 圆锥体碰撞方法
        //=============================================================================

        /**
         * 测试圆锥体-球体是否相交
         */
        static bool Intersects(const Cone& cone, const Sphere& sphere);
        static bool Intersects(const Sphere& sphere, const Cone& cone)
        {
            return Intersects(cone, sphere);
        }

        /**
         * 测试圆锥体-AABB是否相交
         */
        static bool Intersects(const Cone& cone, const AABB& box);
        static bool Intersects(const AABB& box, const Cone& cone)
        {
            return Intersects(cone, box);
        }

        //=============================================================================
        // 环面碰撞方法
        //=============================================================================

        /**
         * 测试环面-球体是否相交
         */
        static bool Intersects(const Torus& torus, const Sphere& sphere);
        static bool Intersects(const Sphere& sphere, const Torus& torus)
        {
            return Intersects(torus, sphere);
        }

        //=============================================================================
        // AABB/OBB统一接口（委托给已有方法）
        //=============================================================================

        /**
         * 测试AABB-AABB是否相交（委托AABB类）
         */
        static bool Intersects(const AABB& a, const AABB& b)
        {
            return a.Intersects(b);
        }

        /**
         * 测试AABB-OBB是否相交
         */
        static bool Intersects(const AABB& aabb, const OBB& obb);
        static bool Intersects(const OBB& obb, const AABB& aabb)
        {
            return Intersects(aabb, obb);
        }

        /**
         * 测试OBB-OBB是否相交（委托OBB类）
         */
        static bool Intersects(const OBB& a, const OBB& b)
        {
            return a.Intersects(b);
        }

        //=============================================================================
        // 泛型模板，自动粗略阶段
        //=============================================================================

        /**
         * 泛型相交测试，自动进行粗略阶段优化
         *
         * 先测试包围盒重叠（高效），如有必要再进行精确碰撞检测（耗时）。
         *
         * @param a 第一个几何体（需有GetBoundingBox方法）
         * @param b 第二个几何体（需有GetBoundingBox方法）
         * @return 若相交返回true
         */
        template<typename T1, typename T2>
        static bool IntersectsWithBroadPhase(const T1& a, const T2& b)
        {
            // Broad phase: quick AABB rejection
            if (!a.GetBoundingBox().Intersects(b.GetBoundingBox()))
                return false;

            // Narrow phase: precise collision detection
            return Intersects(a, b);
        }
    };

}//namespace hgl::math
