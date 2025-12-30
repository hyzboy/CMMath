/**
 * BVH.cpp - 层次包围盒实现
 */

#include<hgl/math/spatial/BVH.h>
#include<algorithm>
#include<limits>
#include<cmath>

namespace hgl::math
{
    /**
     * BVH 节点内部实现
     */
    class BVH::BVHNode
    {
    public:
        AABB bounds;
        BVHNode* left;
        BVHNode* right;
        std::vector<IBVHData*> objects;  // 叶节点存储对象
        bool is_leaf;

        BVHNode() : left(nullptr), right(nullptr), is_leaf(true) {}

        ~BVHNode()
        {
            delete left;
            delete right;
        }

        /**
         * 射线相交测试
         */
        bool IntersectRay(const Ray &ray, IBVHData* &out_hit, float &out_t) const
        {
            // 首先测试与节点包围盒的相交
            float t_near, t_far;
            if (!IntersectRayAABB(ray, bounds, t_near, t_far))
                return false;

            if (is_leaf)
            {
                // 叶节点：测试所有对象
                bool hit = false;
                float closest_t = std::numeric_limits<float>::max();
                
                for (auto obj : objects)
                {
                    float t;
                    if (IntersectRayAABB(ray, obj->GetBounds(), t, t_far))
                    {
                        if (t < closest_t)
                        {
                            closest_t = t;
                            out_hit = obj;
                            hit = true;
                        }
                    }
                }
                
                if (hit)
                {
                    out_t = closest_t;
                    return true;
                }
                
                return false;
            }
            else
            {
                // 内部节点：递归测试子节点
                bool hit_left = false, hit_right = false;
                float t_left = std::numeric_limits<float>::max();
                float t_right = std::numeric_limits<float>::max();
                IBVHData *hit_left_obj = nullptr, *hit_right_obj = nullptr;
                
                if (left)
                    hit_left = left->IntersectRay(ray, hit_left_obj, t_left);
                
                if (right)
                    hit_right = right->IntersectRay(ray, hit_right_obj, t_right);
                
                if (hit_left && hit_right)
                {
                    if (t_left < t_right)
                    {
                        out_hit = hit_left_obj;
                        out_t = t_left;
                    }
                    else
                    {
                        out_hit = hit_right_obj;
                        out_t = t_right;
                    }
                    return true;
                }
                else if (hit_left)
                {
                    out_hit = hit_left_obj;
                    out_t = t_left;
                    return true;
                }
                else if (hit_right)
                {
                    out_hit = hit_right_obj;
                    out_t = t_right;
                    return true;
                }
                
                return false;
            }
        }

        /**
         * 查询与 AABB 相交的对象
         */
        void Query(const AABB &area, std::vector<IBVHData*> &results) const
        {
            if (!bounds.Cross(area))
                return;

            if (is_leaf)
            {
                for (auto obj : objects)
                {
                    if (area.Cross(obj->GetBounds()))
                        results.push_back(obj);
                }
            }
            else
            {
                if (left)
                    left->Query(area, results);
                if (right)
                    right->Query(area, results);
            }
        }

        /**
         * 查询与球体相交的对象
         */
        void QuerySphere(const Vector3f &center, float radius, std::vector<IBVHData*> &results) const
        {
            // 简化实现：用包围盒近似球体
            Vector3f extent(radius, radius, radius);
            AABB sphere_bounds;
            sphere_bounds.SetMinMax(center - extent, center + extent);
            
            Query(sphere_bounds, results);
        }

        /**
         * 计算树深度
         */
        int GetDepth() const
        {
            if (is_leaf)
                return 1;
            
            int left_depth = left ? left->GetDepth() : 0;
            int right_depth = right ? right->GetDepth() : 0;
            
            return 1 + std::max(left_depth, right_depth);
        }

        /**
         * 计算节点数
         */
        int CountNodes() const
        {
            int count = 1;
            if (left)
                count += left->CountNodes();
            if (right)
                count += right->CountNodes();
            return count;
        }

        /**
         * 计算 SAH 成本
         */
        float ComputeSAHCost() const
        {
            if (is_leaf)
                return bounds.GetSurfaceArea() * objects.size();
            
            float cost = bounds.GetSurfaceArea();
            if (left)
                cost += left->ComputeSAHCost();
            if (right)
                cost += right->ComputeSAHCost();
            
            return cost;
        }

    private:
        /**
         * 射线与 AABB 相交测试
         */
        static bool IntersectRayAABB(const Ray &ray, const AABB &aabb, float &t_near, float &t_far)
        {
            Vector3f min_p = aabb.GetMinPoint();
            Vector3f max_p = aabb.GetMaxPoint();
            
            t_near = -std::numeric_limits<float>::max();
            t_far = std::numeric_limits<float>::max();
            
            for (int i = 0; i < 3; i++)
            {
                float ray_origin_i = ray.origin[i];
                float ray_dir_i = ray.direction[i];
                float min_i = min_p[i];
                float max_i = max_p[i];
                
                if (std::abs(ray_dir_i) < 1e-8f)
                {
                    // 射线平行于该轴
                    if (ray_origin_i < min_i || ray_origin_i > max_i)
                        return false;
                }
                else
                {
                    float t1 = (min_i - ray_origin_i) / ray_dir_i;
                    float t2 = (max_i - ray_origin_i) / ray_dir_i;
                    
                    if (t1 > t2)
                        std::swap(t1, t2);
                    
                    t_near = std::max(t_near, t1);
                    t_far = std::min(t_far, t2);
                    
                    if (t_near > t_far)
                        return false;
                }
            }
            
            return t_far >= 0;
        }
    };

    // ==================== BVH 公开接口实现 ====================

    BVH::BVH(const std::vector<IBVHData*> &objects, BVHBuildStrategy strategy)
        : root(nullptr), build_strategy(strategy)
    {
        if (!objects.empty())
        {
            std::vector<IBVHData*> build_objects = objects;
            root = BuildRecursive(build_objects, 0, build_objects.size(), 0);
        }
    }

    BVH::~BVH()
    {
        delete root;
    }

    void BVH::Rebuild(const std::vector<IBVHData*> &objects)
    {
        delete root;
        root = nullptr;
        
        if (!objects.empty())
        {
            std::vector<IBVHData*> build_objects = objects;
            root = BuildRecursive(build_objects, 0, build_objects.size(), 0);
        }
    }

    BVH::BVHNode* BVH::BuildRecursive(std::vector<IBVHData*> &objects, int start, int end, int depth)
    {
        switch (build_strategy)
        {
            case BVHBuildStrategy::SAH:
                return BuildSAH(objects, start, end, depth);
            case BVHBuildStrategy::Middle:
                return BuildMiddle(objects, start, end, depth);
            case BVHBuildStrategy::EqualCounts:
                return BuildEqualCounts(objects, start, end, depth);
            default:
                return BuildSAH(objects, start, end, depth);
        }
    }

    /**
     * SAH 策略构建
     */
    BVH::BVHNode* BVH::BuildSAH(std::vector<IBVHData*> &objects, int start, int end, int depth)
    {
        BVHNode* node = new BVHNode();
        
        // 计算所有对象的包围盒
        AABB bounds;
        bounds.Clear();
        for (int i = start; i < end; i++)
        {
            bounds.Merge(objects[i]->GetBounds());
        }
        node->bounds = bounds;
        
        int count = end - start;
        
        // 叶节点条件
        if (count <= 1)
        {
            node->is_leaf = true;
            for (int i = start; i < end; i++)
                node->objects.push_back(objects[i]);
            return node;
        }
        
        // 计算质心包围盒
        AABB centroid_bounds;
        centroid_bounds.Clear();
        for (int i = start; i < end; i++)
        {
            centroid_bounds.AddPoint(objects[i]->GetCenter());
        }
        
        int best_axis = 0;
        int best_split = start + count / 2;
        float best_cost = std::numeric_limits<float>::max();
        
        // 对每个轴测试分割
        for (int axis = 0; axis < 3; axis++)
        {
            // 按轴排序
            std::sort(objects.begin() + start, objects.begin() + end,
                [axis](IBVHData* a, IBVHData* b) {
                    return a->GetCenter()[axis] < b->GetCenter()[axis];
                });
            
            // 测试不同的分割位置
            const int num_buckets = 12;
            for (int i = 1; i < num_buckets; i++)
            {
                int split = start + (count * i) / num_buckets;
                
                // 计算左右子树的包围盒和成本
                AABB left_bounds, right_bounds;
                left_bounds.Clear();
                right_bounds.Clear();
                
                for (int j = start; j < split; j++)
                    left_bounds.Merge(objects[j]->GetBounds());
                for (int j = split; j < end; j++)
                    right_bounds.Merge(objects[j]->GetBounds());
                
                float left_cost = left_bounds.GetSurfaceArea() * (split - start);
                float right_cost = right_bounds.GetSurfaceArea() * (end - split);
                float cost = left_cost + right_cost;
                
                if (cost < best_cost)
                {
                    best_cost = cost;
                    best_axis = axis;
                    best_split = split;
                }
            }
        }
        
        // 按最佳轴重新排序
        std::sort(objects.begin() + start, objects.begin() + end,
            [best_axis](IBVHData* a, IBVHData* b) {
                return a->GetCenter()[best_axis] < b->GetCenter()[best_axis];
            });
        
        // 递归构建子树
        node->is_leaf = false;
        node->left = BuildSAH(objects, start, best_split, depth + 1);
        node->right = BuildSAH(objects, best_split, end, depth + 1);
        
        return node;
    }

    /**
     * 中点分割策略
     */
    BVH::BVHNode* BVH::BuildMiddle(std::vector<IBVHData*> &objects, int start, int end, int depth)
    {
        BVHNode* node = new BVHNode();
        
        // 计算包围盒
        AABB bounds;
        bounds.Clear();
        for (int i = start; i < end; i++)
        {
            bounds.Merge(objects[i]->GetBounds());
        }
        node->bounds = bounds;
        
        int count = end - start;
        
        if (count <= 1)
        {
            node->is_leaf = true;
            for (int i = start; i < end; i++)
                node->objects.push_back(objects[i]);
            return node;
        }
        
        // 选择最长轴
        Vector3f extent = bounds.GetLength();
        int axis = 0;
        if (extent.y > extent.x) axis = 1;
        if (extent.z > extent[axis]) axis = 2;
        
        // 按轴排序
        std::sort(objects.begin() + start, objects.begin() + end,
            [axis](IBVHData* a, IBVHData* b) {
                return a->GetCenter()[axis] < b->GetCenter()[axis];
            });
        
        int mid = start + count / 2;
        
        node->is_leaf = false;
        node->left = BuildMiddle(objects, start, mid, depth + 1);
        node->right = BuildMiddle(objects, mid, end, depth + 1);
        
        return node;
    }

    /**
     * 均分对象数策略
     */
    BVH::BVHNode* BVH::BuildEqualCounts(std::vector<IBVHData*> &objects, int start, int end, int depth)
    {
        return BuildMiddle(objects, start, end, depth);  // 与中点分割相同
    }

    bool BVH::IntersectRay(const Ray &ray, IBVHData* &out_hit, float &out_t) const
    {
        if (!root)
            return false;
        
        return root->IntersectRay(ray, out_hit, out_t);
    }

    void BVH::Query(const AABB &area, std::vector<IBVHData*> &out_results) const
    {
        if (root)
            root->Query(area, out_results);
    }

    void BVH::QuerySphere(const Vector3f &center, float radius, std::vector<IBVHData*> &out_results) const
    {
        if (root)
            root->QuerySphere(center, radius, out_results);
    }

    int BVH::GetTotalNodes() const
    {
        return root ? root->CountNodes() : 0;
    }

    int BVH::GetTreeDepth() const
    {
        return root ? root->GetDepth() : 0;
    }

    float BVH::GetSAHCost() const
    {
        return root ? root->ComputeSAHCost() : 0.0f;
    }

}//namespace hgl::math
