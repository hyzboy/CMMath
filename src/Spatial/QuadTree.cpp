/**
 * QuadTree.cpp - 四叉树空间分区实现
 */

#include<hgl/math/spatial/QuadTree.h>
#include<algorithm>

namespace hgl::math
{
    /**
     * 四叉树节点内部实现
     */
    class QuadTree::QuadTreeNode
    {
    public:
        AABB2D bounds;
        std::vector<IQuadTreeData*> objects;
        QuadTreeNode* children[4];  // 四个子节点：NW, NE, SW, SE
        int depth;
        bool is_leaf;

        QuadTreeNode(const AABB2D &bounds, int depth)
            : bounds(bounds), depth(depth), is_leaf(true)
        {
            children[0] = children[1] = children[2] = children[3] = nullptr;
        }

        ~QuadTreeNode()
        {
            Clear();
        }

        void Clear()
        {
            objects.clear();
            
            for (int i = 0; i < 4; i++)
            {
                if (children[i])
                {
                    delete children[i];
                    children[i] = nullptr;
                }
            }
            
            is_leaf = true;
        }

        /**
         * 分裂节点创建四个子节点
         */
        void Split()
        {
            if (!is_leaf)
                return;

            Vector2f center = bounds.GetCenter();
            Vector2f extent = bounds.GetExtent();

            // 创建四个子节点
            // 0: 西北 (NW)
            children[0] = new QuadTreeNode(
                AABB2D(bounds.min.x, center.y, center.x, bounds.max.y),
                depth + 1
            );
            
            // 1: 东北 (NE)
            children[1] = new QuadTreeNode(
                AABB2D(center.x, center.y, bounds.max.x, bounds.max.y),
                depth + 1
            );
            
            // 2: 西南 (SW)
            children[2] = new QuadTreeNode(
                AABB2D(bounds.min.x, bounds.min.y, center.x, center.y),
                depth + 1
            );
            
            // 3: 东南 (SE)
            children[3] = new QuadTreeNode(
                AABB2D(center.x, bounds.min.y, bounds.max.x, center.y),
                depth + 1
            );

            is_leaf = false;

            // 将现有对象重新插入到子节点
            std::vector<IQuadTreeData*> temp_objects = objects;
            objects.clear();
            
            for (auto obj : temp_objects)
            {
                InsertIntoChildren(obj);
            }
        }

        /**
         * 将对象插入到合适的子节点
         */
        bool InsertIntoChildren(IQuadTreeData *data)
        {
            AABB2D obj_bounds = data->GetBounds();
            bool inserted = false;

            for (int i = 0; i < 4; i++)
            {
                if (children[i] && children[i]->bounds.Intersects(obj_bounds))
                {
                    if (children[i]->Insert(data))
                        inserted = true;
                }
            }

            return inserted;
        }

        /**
         * 插入对象到节点
         */
        bool Insert(IQuadTreeData *data, int max_objects, int max_depth)
        {
            if (!bounds.Intersects(data->GetBounds()))
                return false;

            if (is_leaf)
            {
                objects.push_back(data);

                // 检查是否需要分裂
                if (objects.size() > static_cast<size_t>(max_objects) && depth < max_depth)
                {
                    Split();
                }

                return true;
            }
            else
            {
                return InsertIntoChildren(data);
            }
        }

        /**
         * 移除对象
         */
        bool Remove(IQuadTreeData *data)
        {
            if (!bounds.Intersects(data->GetBounds()))
                return false;

            if (is_leaf)
            {
                auto it = std::find(objects.begin(), objects.end(), data);
                if (it != objects.end())
                {
                    objects.erase(it);
                    return true;
                }
                return false;
            }
            else
            {
                bool removed = false;
                for (int i = 0; i < 4; i++)
                {
                    if (children[i] && children[i]->Remove(data))
                        removed = true;
                }
                return removed;
            }
        }

        /**
         * 查询区域内的对象
         */
        void Query(const AABB2D &area, std::vector<IQuadTreeData*> &results) const
        {
            if (!bounds.Intersects(area))
                return;

            if (is_leaf)
            {
                for (auto obj : objects)
                {
                    if (area.Intersects(obj->GetBounds()))
                    {
                        results.push_back(obj);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    if (children[i])
                        children[i]->Query(area, results);
                }
            }
        }

        /**
         * 查询点所在的对象
         */
        void QueryPoint(const Vector2f &point, std::vector<IQuadTreeData*> &results) const
        {
            if (!bounds.Contains(point))
                return;

            if (is_leaf)
            {
                for (auto obj : objects)
                {
                    if (obj->GetBounds().Contains(point))
                    {
                        results.push_back(obj);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    if (children[i])
                        children[i]->QueryPoint(point, results);
                }
            }
        }

        /**
         * 计算节点数
         */
        int CountNodes() const
        {
            int count = 1;  // 自己
            if (!is_leaf)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (children[i])
                        count += children[i]->CountNodes();
                }
            }
            return count;
        }

        /**
         * 计算对象总数
         */
        int CountObjects() const
        {
            int count = objects.size();
            if (!is_leaf)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (children[i])
                        count += children[i]->CountObjects();
                }
            }
            return count;
        }
    };

    // ==================== QuadTree 公开接口实现 ====================

    QuadTree::QuadTree(const AABB2D &bounds, int max_objects, int max_depth)
        : max_objects(max_objects), max_depth(max_depth), total_objects(0)
    {
        root = new QuadTreeNode(bounds, 0);
    }

    QuadTree::~QuadTree()
    {
        delete root;
    }

    bool QuadTree::Insert(IQuadTreeData *data)
    {
        if (data == nullptr)
            return false;

        bool result = root->Insert(data, max_objects, max_depth);
        if (result)
            total_objects++;
        return result;
    }

    bool QuadTree::Remove(IQuadTreeData *data)
    {
        if (data == nullptr)
            return false;

        bool result = root->Remove(data);
        if (result)
            total_objects--;
        return result;
    }

    void QuadTree::Query(const AABB2D &area, std::vector<IQuadTreeData*> &out_results) const
    {
        root->Query(area, out_results);
    }

    void QuadTree::QueryPoint(const Vector2f &point, std::vector<IQuadTreeData*> &out_results) const
    {
        root->QueryPoint(point, out_results);
    }

    void QuadTree::Clear()
    {
        root->Clear();
        total_objects = 0;
    }

    int QuadTree::GetTotalObjects() const
    {
        return total_objects;
    }

    int QuadTree::GetTotalNodes() const
    {
        return root->CountNodes();
    }

}//namespace hgl::math
