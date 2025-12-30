/**
 * Octree.cpp - 八叉树空间分区实现
 */

#include<hgl/math/spatial/Octree.h>
#include<algorithm>

namespace hgl::math
{
    /**
     * 八叉树节点内部实现
     */
    class Octree::OctreeNode
    {
    public:
        AABB bounds;
        std::vector<IOctreeData*> objects;
        OctreeNode* children[8];  // 八个子节点
        int depth;
        bool is_leaf;

        OctreeNode(const AABB &bounds, int depth)
            : bounds(bounds), depth(depth), is_leaf(true)
        {
            for (int i = 0; i < 8; i++)
                children[i] = nullptr;
        }

        ~OctreeNode()
        {
            Clear();
        }

        void Clear()
        {
            objects.clear();
            
            for (int i = 0; i < 8; i++)
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
         * 分裂节点创建八个子节点
         */
        void Split()
        {
            if (!is_leaf)
                return;

            Vector3f center = bounds.GetCenter();
            Vector3f extent = bounds.GetExtent();

            // 创建八个子节点
            for (int i = 0; i < 8; i++)
            {
                Vector3f child_min, child_max;
                
                // 根据索引决定子节点位置
                // i的二进制位：bit0=x, bit1=y, bit2=z
                child_min.x = (i & 1) ? center.x : bounds.GetMinPoint().x;
                child_min.y = (i & 2) ? center.y : bounds.GetMinPoint().y;
                child_min.z = (i & 4) ? center.z : bounds.GetMinPoint().z;
                
                child_max.x = (i & 1) ? bounds.GetMaxPoint().x : center.x;
                child_max.y = (i & 2) ? bounds.GetMaxPoint().y : center.y;
                child_max.z = (i & 4) ? bounds.GetMaxPoint().z : center.z;
                
                AABB child_bounds;
                child_bounds.SetMinMax(child_min, child_max);
                
                children[i] = new OctreeNode(child_bounds, depth + 1);
            }

            is_leaf = false;

            // 将现有对象重新插入到子节点
            std::vector<IOctreeData*> temp_objects = objects;
            objects.clear();
            
            for (auto obj : temp_objects)
            {
                InsertIntoChildren(obj);
            }
        }

        /**
         * 将对象插入到合适的子节点
         */
        bool InsertIntoChildren(IOctreeData *data)
        {
            AABB obj_bounds = data->GetBounds();
            bool inserted = false;

            for (int i = 0; i < 8; i++)
            {
                if (children[i] && children[i]->bounds.Cross(obj_bounds))
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
        bool Insert(IOctreeData *data, int max_objects, int max_depth)
        {
            if (!bounds.Cross(data->GetBounds()))
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
        bool Remove(IOctreeData *data)
        {
            if (!bounds.Cross(data->GetBounds()))
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
                for (int i = 0; i < 8; i++)
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
        void Query(const AABB &area, std::vector<IOctreeData*> &results) const
        {
            if (!bounds.Cross(area))
                return;

            if (is_leaf)
            {
                for (auto obj : objects)
                {
                    if (area.Cross(obj->GetBounds()))
                    {
                        results.push_back(obj);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    if (children[i])
                        children[i]->Query(area, results);
                }
            }
        }

        /**
         * 查询点所在的对象
         */
        void QueryPoint(const Vector3f &point, std::vector<IOctreeData*> &results) const
        {
            if (!bounds.PointIn(point))
                return;

            if (is_leaf)
            {
                for (auto obj : objects)
                {
                    if (obj->GetBounds().PointIn(point))
                    {
                        results.push_back(obj);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    if (children[i])
                        children[i]->QueryPoint(point, results);
                }
            }
        }

        /**
         * 检查 AABB 与视锥体的相交性
         * 返回: -1=完全外部, 0=部分相交, 1=完全内部
         */
        int TestFrustum(const FrustumPlanes &frustum, const AABB &aabb) const
        {
            bool intersect = false;
            
            // 测试每个平面
            for (int i = 0; i < 6; i++)
            {
                const Vector4f &plane = frustum[i];
                Vector3f plane_normal(plane.x, plane.y, plane.z);
                float plane_d = plane.w;
                
                // 获取 AABB 的正顶点和负顶点
                Vector3f p_vertex = aabb.GetVertexP(plane_normal);
                Vector3f n_vertex = aabb.GetVertexN(plane_normal);
                
                // 检查正顶点是否在平面外侧
                if (dot(plane_normal, p_vertex) + plane_d < 0)
                    return -1;  // 完全外部
                
                // 检查负顶点是否在平面外侧
                if (dot(plane_normal, n_vertex) + plane_d < 0)
                    intersect = true;
            }
            
            return intersect ? 0 : 1;  // 0=相交, 1=完全内部
        }

        /**
         * 查询与视锥体相交的对象
         */
        void QueryFrustum(const FrustumPlanes &frustum, std::vector<IOctreeData*> &results) const
        {
            int test = TestFrustum(frustum, bounds);
            
            if (test == -1)
                return;  // 完全外部，跳过

            if (is_leaf)
            {
                for (auto obj : objects)
                {
                    if (TestFrustum(frustum, obj->GetBounds()) >= 0)
                    {
                        results.push_back(obj);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 8; i++)
                {
                    if (children[i])
                        children[i]->QueryFrustum(frustum, results);
                }
            }
        }

        /**
         * 计算节点数
         */
        int CountNodes() const
        {
            int count = 1;
            if (!is_leaf)
            {
                for (int i = 0; i < 8; i++)
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
                for (int i = 0; i < 8; i++)
                {
                    if (children[i])
                        count += children[i]->CountObjects();
                }
            }
            return count;
        }
    };

    // ==================== Octree 公开接口实现 ====================

    Octree::Octree(const AABB &bounds, int max_objects, int max_depth)
        : max_objects(max_objects), max_depth(max_depth), total_objects(0)
    {
        root = new OctreeNode(bounds, 0);
    }

    Octree::~Octree()
    {
        delete root;
    }

    bool Octree::Insert(IOctreeData *data)
    {
        if (data == nullptr)
            return false;

        bool result = root->Insert(data, max_objects, max_depth);
        if (result)
            total_objects++;
        return result;
    }

    bool Octree::Remove(IOctreeData *data)
    {
        if (data == nullptr)
            return false;

        bool result = root->Remove(data);
        if (result)
            total_objects--;
        return result;
    }

    void Octree::Query(const AABB &area, std::vector<IOctreeData*> &out_results) const
    {
        root->Query(area, out_results);
    }

    void Octree::QueryPoint(const Vector3f &point, std::vector<IOctreeData*> &out_results) const
    {
        root->QueryPoint(point, out_results);
    }

    void Octree::QueryFrustum(const FrustumPlanes &frustum, std::vector<IOctreeData*> &out_results) const
    {
        root->QueryFrustum(frustum, out_results);
    }

    void Octree::Clear()
    {
        root->Clear();
        total_objects = 0;
    }

    int Octree::GetTotalObjects() const
    {
        return total_objects;
    }

    int Octree::GetTotalNodes() const
    {
        return root->CountNodes();
    }

}//namespace hgl::math
