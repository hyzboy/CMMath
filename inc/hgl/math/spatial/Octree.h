/**
 * Octree.h - 八叉树空间分区
 *
 * 实现 3D 场景的八叉树空间分区结构
 * 用于快速空间查询、视锥体裁剪和碰撞检测优化
 *
 * 参考：
 * - "Real-Time Collision Detection" by Christer Ericson
 * - "Foundations of Game Engine Development, Volume 2: Rendering" by Eric Lengyel
 */

#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/Frustum.h>
#include<vector>

namespace hgl::math
{
    /**
     * 八叉树节点数据接口
     */
    struct IOctreeData
    {
        virtual AABB GetBounds() const = 0;
        virtual ~IOctreeData() = default;
    };

    /**
     * 八叉树实现
     * 用于 3D 场景的空间分区和快速查询
     */
    class Octree
    {
    public:
        /**
         * 构造八叉树
         * @param bounds 根节点包围盒
         * @param max_objects 单节点最大对象数
         * @param max_depth 最大深度
         */
        Octree(const AABB &bounds, int max_objects = 10, int max_depth = 8);
        ~Octree();

        // 禁止拷贝
        Octree(const Octree&) = delete;
        Octree& operator=(const Octree&) = delete;

        /**
         * 插入对象
         */
        bool Insert(IOctreeData *data);

        /**
         * 移除对象
         */
        bool Remove(IOctreeData *data);

        /**
         * 查询与区域相交的所有对象
         */
        void Query(const AABB &area, std::vector<IOctreeData*> &out_results) const;

        /**
         * 查询包含点的所有对象
         */
        void QueryPoint(const Vector3f &point, std::vector<IOctreeData*> &out_results) const;

        /**
         * 查询与视锥体相交的对象（视锥体裁剪）
         */
        void QueryFrustum(const FrustumPlanes &frustum, std::vector<IOctreeData*> &out_results) const;

        /**
         * 清空八叉树
         */
        void Clear();

        /**
         * 获取统计信息
         */
        int GetTotalObjects() const;
        int GetTotalNodes() const;
        int GetMaxDepth() const { return max_depth; }

    private:
        class OctreeNode;
        OctreeNode *root;
        int max_objects;
        int max_depth;
        int total_objects;
    };

}//namespace hgl::math
