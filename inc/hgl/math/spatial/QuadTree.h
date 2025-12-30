/**
 * QuadTree.h - 四叉树空间分区
 * 
 * 实现 2D 场景的四叉树空间分区结构
 * 用于快速空间查询和碰撞检测优化
 * 
 * 参考：
 * - "Real-Time Collision Detection" by Christer Ericson
 * - Spatial partitioning algorithms for game development
 */

#pragma once

#include<hgl/math/Vector.h>
#include<vector>

namespace hgl::math
{
    /**
     * 2D 轴对齐包围盒 (用于 QuadTree)
     */
    struct AABB2D
    {
        Vector2f min;
        Vector2f max;

        AABB2D() : min(0.0f, 0.0f), max(0.0f, 0.0f) {}
        AABB2D(const Vector2f &min_point, const Vector2f &max_point) 
            : min(min_point), max(max_point) {}
        AABB2D(float min_x, float min_y, float max_x, float max_y)
            : min(min_x, min_y), max(max_x, max_y) {}

        /**
         * 获取中心点
         */
        Vector2f GetCenter() const { return (min + max) * 0.5f; }
        
        /**
         * 获取大小
         */
        Vector2f GetSize() const { return max - min; }
        
        /**
         * 获取半径
         */
        Vector2f GetExtent() const { return (max - min) * 0.5f; }
        
        /**
         * 判断是否包含点
         */
        bool Contains(const Vector2f &point) const
        {
            return point.x >= min.x && point.x <= max.x &&
                   point.y >= min.y && point.y <= max.y;
        }
        
        /**
         * 判断是否与另一个 AABB 相交
         */
        bool Intersects(const AABB2D &other) const
        {
            return !(max.x < other.min.x || min.x > other.max.x ||
                     max.y < other.min.y || min.y > other.max.y);
        }
        
        /**
         * 合并另一个 AABB
         */
        void Merge(const AABB2D &other)
        {
            min.x = glm::min(min.x, other.min.x);
            min.y = glm::min(min.y, other.min.y);
            max.x = glm::max(max.x, other.max.x);
            max.y = glm::max(max.y, other.max.y);
        }
    };

    /**
     * 四叉树节点数据接口
     * 用户数据需实现此接口以存储在四叉树中
     */
    struct IQuadTreeData
    {
        virtual AABB2D GetBounds() const = 0;
        virtual ~IQuadTreeData() = default;
    };

    /**
     * 四叉树实现
     * 用于 2D 场景的空间分区和快速查询
     */
    class QuadTree
    {
    public:
        /**
         * 构造四叉树
         * @param bounds 根节点包围盒
         * @param max_objects 单节点最大对象数（触发分裂）
         * @param max_depth 最大深度
         */
        QuadTree(const AABB2D &bounds, int max_objects = 10, int max_depth = 8);
        ~QuadTree();

        // 禁止拷贝
        QuadTree(const QuadTree&) = delete;
        QuadTree& operator=(const QuadTree&) = delete;
        
        /**
         * 插入对象
         * @param data 对象指针（四叉树不拥有所有权）
         * @return 是否插入成功
         */
        bool Insert(IQuadTreeData *data);
        
        /**
         * 移除对象
         * @param data 对象指针
         * @return 是否移除成功
         */
        bool Remove(IQuadTreeData *data);
        
        /**
         * 查询与区域相交的所有对象
         * @param area 查询区域
         * @param out_results 输出结果列表
         */
        void Query(const AABB2D &area, std::vector<IQuadTreeData*> &out_results) const;
        
        /**
         * 查询包含点的所有对象
         * @param point 查询点
         * @param out_results 输出结果列表
         */
        void QueryPoint(const Vector2f &point, std::vector<IQuadTreeData*> &out_results) const;
        
        /**
         * 清空四叉树
         */
        void Clear();
        
        /**
         * 获取统计信息
         */
        int GetTotalObjects() const;
        int GetTotalNodes() const;
        int GetMaxDepth() const { return max_depth; }
        
    private:
        class QuadTreeNode;
        QuadTreeNode *root;
        int max_objects;
        int max_depth;
        int total_objects;
    };

}//namespace hgl::math
