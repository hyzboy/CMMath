/**
 * BVH.h - 层次包围盒 (Bounding Volume Hierarchy)
 *
 * 实现 BVH 用于射线追踪和碰撞检测优化
 * 支持 SAH (Surface Area Heuristic) 构建策略
 *
 * 参考：
 * - "Real-Time Collision Detection" by Christer Ericson
 * - "On fast Construction of SAH-based Bounding Volume Hierarchies"
 * - PBRT: Physically Based Rendering
 */

#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/Ray.h>
#include<vector>

namespace hgl::math
{
    /**
     * BVH 节点数据接口
     */
    struct IBVHData
    {
        virtual AABB GetBounds() const = 0;
        virtual Vector3f GetCenter() const = 0;
        virtual ~IBVHData() = default;
    };

    /**
     * BVH 构建策略
     */
    enum class BVHBuildStrategy
    {
        SAH,            // Surface Area Heuristic（质量最高，构建较慢）
        Middle,         // 中点分割（构建快，质量中等）
        EqualCounts     // 均分对象数（构建最快）
    };

    /**
     * 层次包围盒（BVH）
     * 用于高效的射线相交和范围查询
     */
    class BVH
    {
    public:
        /**
         * 构造 BVH
         * @param objects 对象列表
         * @param strategy 构建策略
         */
        BVH(const std::vector<IBVHData*> &objects, BVHBuildStrategy strategy = BVHBuildStrategy::SAH);
        ~BVH();

        // 禁止拷贝
        BVH(const BVH&) = delete;
        BVH& operator=(const BVH&) = delete;

        /**
         * 重建 BVH（当对象位置发生变化时）
         * @param objects 更新后的对象列表
         */
        void Rebuild(const std::vector<IBVHData*> &objects);

        /**
         * 射线相交测试
         * @param ray 射线
         * @param out_hit 最近命中对象
         * @param out_t 命中参数
         * @return 是否命中
         */
        bool IntersectRay(const Ray &ray, IBVHData* &out_hit, float &out_t) const;

        /**
         * 查询与区域相交的所有对象
         */
        void Query(const AABB &area, std::vector<IBVHData*> &out_results) const;

        /**
         * 查询与球体相交的所有对象
         */
        void QuerySphere(const Vector3f &center, float radius, std::vector<IBVHData*> &out_results) const;

        /**
         * 获取统计信息
         */
        int GetTotalNodes() const;
        int GetTreeDepth() const;
        float GetSAHCost() const;  // SAH 策略的成本评估

    private:
        class BVHNode;
        BVHNode *root;
        BVHBuildStrategy build_strategy;

        // 构建辅助函数
        BVHNode* BuildRecursive(std::vector<IBVHData*> &objects, int start, int end, int depth);
        BVHNode* BuildSAH(std::vector<IBVHData*> &objects, int start, int end, int depth);
        BVHNode* BuildMiddle(std::vector<IBVHData*> &objects, int start, int end, int depth);
        BVHNode* BuildEqualCounts(std::vector<IBVHData*> &objects, int start, int end, int depth);
    };

}//namespace hgl::math
