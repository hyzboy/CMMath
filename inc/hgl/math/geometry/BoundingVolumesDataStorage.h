#pragma once

#include<hgl/math/geometry/BoundingVolumes.h>
#include<hgl/type/List.h>
#include<vector>

namespace hgl::math
{
    /**
     * BoundingVolumesDataStorage - SOA (Structure of Arrays) 存储
     * 
     * 用于 ECS 体系中高效存储和访问多个 BoundingVolumes 数据
     * 采用 SOA 模式将数据分解为多个连续数组，便于：
     * 1. SIMD 优化（数据对齐和连续访问）
     * 2. 缓存友好（相同类型数据连续存储）
     * 3. 批量操作（可以对同类数据进行向量化处理）
     * 
     * 数据布局：
     * - AABB: minPoint[n], maxPoint[n]
     * - OBB: center[n], axis[n*3], halfLength[n]
     * - Sphere: center[n], radius[n]
     */
    class BoundingVolumesDataStorage
    {
    public:
        // AABB 数据
        std::vector<Vector3f> aabbMinPoints;    ///< AABB 最小点数组
        std::vector<Vector3f> aabbMaxPoints;    ///< AABB 最大点数组

        // OBB 数据
        std::vector<Vector3f> obbCenters;       ///< OBB 中心点数组
        std::vector<Vector3f> obbAxis0;         ///< OBB X 轴数组
        std::vector<Vector3f> obbAxis1;         ///< OBB Y 轴数组
        std::vector<Vector3f> obbAxis2;         ///< OBB Z 轴数组
        std::vector<Vector3f> obbHalfLengths;   ///< OBB 半长度数组

        // BoundingSphere 数据
        std::vector<Vector3f> sphereCenters;    ///< 球体中心点数组
        std::vector<float>    sphereRadii;      ///< 球体半径数组

    private:
        size_t capacity;                        ///< 当前容量
        size_t count;                           ///< 当前元素数量

    public:

        BoundingVolumesDataStorage()
            : capacity(0), count(0)
        {
        }

        /**
         * 构造函数 - 预分配指定容量
         * @param initial_capacity 初始容量
         */
        explicit BoundingVolumesDataStorage(size_t initial_capacity)
            : capacity(0), count(0)
        {
            Reserve(initial_capacity);
        }

        /**
         * 获取当前元素数量
         */
        size_t GetCount() const { return count; }

        /**
         * 获取当前容量
         */
        size_t GetCapacity() const { return capacity; }

        /**
         * 检查是否为空
         */
        bool IsEmpty() const { return count == 0; }

        /**
         * 预分配容量
         * @param new_capacity 新容量
         */
        void Reserve(size_t new_capacity)
        {
            if (new_capacity <= capacity)
                return;

            // AABB
            aabbMinPoints.reserve(new_capacity);
            aabbMaxPoints.reserve(new_capacity);

            // OBB
            obbCenters.reserve(new_capacity);
            obbAxis0.reserve(new_capacity);
            obbAxis1.reserve(new_capacity);
            obbAxis2.reserve(new_capacity);
            obbHalfLengths.reserve(new_capacity);

            // Sphere
            sphereCenters.reserve(new_capacity);
            sphereRadii.reserve(new_capacity);

            capacity = new_capacity;
        }

        /**
         * 清空所有数据
         */
        void Clear()
        {
            // AABB
            aabbMinPoints.clear();
            aabbMaxPoints.clear();

            // OBB
            obbCenters.clear();
            obbAxis0.clear();
            obbAxis1.clear();
            obbAxis2.clear();
            obbHalfLengths.clear();

            // Sphere
            sphereCenters.clear();
            sphereRadii.clear();

            count = 0;
        }

        /**
         * 收缩容量以匹配实际元素数量
         */
        void ShrinkToFit()
        {
            // AABB
            aabbMinPoints.shrink_to_fit();
            aabbMaxPoints.shrink_to_fit();

            // OBB
            obbCenters.shrink_to_fit();
            obbAxis0.shrink_to_fit();
            obbAxis1.shrink_to_fit();
            obbAxis2.shrink_to_fit();
            obbHalfLengths.shrink_to_fit();

            // Sphere
            sphereCenters.shrink_to_fit();
            sphereRadii.shrink_to_fit();

            capacity = count;
        }

        /**
         * 添加一个 BoundingVolumes
         * @param bv 包围体
         * @return 添加的索引
         */
        size_t Add(const BoundingVolumes &bv)
        {
            size_t index = count;

            // AABB
            aabbMinPoints.push_back(bv.aabb.GetMin());
            aabbMaxPoints.push_back(bv.aabb.GetMax());

            // OBB
            obbCenters.push_back(bv.obb.GetCenter());
            obbAxis0.push_back(bv.obb.GetAxis(0));
            obbAxis1.push_back(bv.obb.GetAxis(1));
            obbAxis2.push_back(bv.obb.GetAxis(2));
            obbHalfLengths.push_back(bv.obb.GetHalfExtend());

            // Sphere
            sphereCenters.push_back(bv.bsphere.GetCenter());
            sphereRadii.push_back(bv.bsphere.GetRadius());

            count++;
            return index;
        }

        /**
         * 批量添加多个 BoundingVolumes
         * @param bvs 包围体数组
         * @param num 数量
         */
        void AddBatch(const BoundingVolumes *bvs, size_t num)
        {
            if (!bvs || num == 0)
                return;

            Reserve(count + num);

            for (size_t i = 0; i < num; i++)
            {
                Add(bvs[i]);
            }
        }

        /**
         * 设置指定索引的 BoundingVolumes
         * @param index 索引
         * @param bv 包围体
         */
        void Set(size_t index, const BoundingVolumes &bv)
        {
            if (index >= count)
                return;

            // AABB
            aabbMinPoints[index] = bv.aabb.GetMin();
            aabbMaxPoints[index] = bv.aabb.GetMax();

            // OBB
            obbCenters[index] = bv.obb.GetCenter();
            obbAxis0[index] = bv.obb.GetAxis(0);
            obbAxis1[index] = bv.obb.GetAxis(1);
            obbAxis2[index] = bv.obb.GetAxis(2);
            obbHalfLengths[index] = bv.obb.GetHalfExtend();

            // Sphere
            sphereCenters[index] = bv.bsphere.GetCenter();
            sphereRadii[index] = bv.bsphere.GetRadius();
        }

        /**
         * 获取指定索引的 BoundingVolumes
         * @param index 索引
         * @param out_bv 输出的包围体
         * @return 是否成功
         */
        bool Get(size_t index, BoundingVolumes &out_bv) const
        {
            if (index >= count)
                return false;

            // AABB
            out_bv.aabb.SetMinMax(aabbMinPoints[index], aabbMaxPoints[index]);

            // OBB
            out_bv.obb.Set(obbCenters[index],
                          obbAxis0[index],
                          obbAxis1[index],
                          obbAxis2[index],
                          obbHalfLengths[index]);

            // Sphere
            out_bv.bsphere.Set(sphereCenters[index], sphereRadii[index]);

            return true;
        }

        /**
         * 移除最后一个元素
         */
        void PopBack()
        {
            if (count == 0)
                return;

            // AABB
            aabbMinPoints.pop_back();
            aabbMaxPoints.pop_back();

            // OBB
            obbCenters.pop_back();
            obbAxis0.pop_back();
            obbAxis1.pop_back();
            obbAxis2.pop_back();
            obbHalfLengths.pop_back();

            // Sphere
            sphereCenters.pop_back();
            sphereRadii.pop_back();

            count--;
        }

        /**
         * 交换两个元素
         * @param index1 第一个索引
         * @param index2 第二个索引
         */
        void Swap(size_t index1, size_t index2)
        {
            if (index1 >= count || index2 >= count || index1 == index2)
                return;

            // AABB
            std::swap(aabbMinPoints[index1], aabbMinPoints[index2]);
            std::swap(aabbMaxPoints[index1], aabbMaxPoints[index2]);

            // OBB
            std::swap(obbCenters[index1], obbCenters[index2]);
            std::swap(obbAxis0[index1], obbAxis0[index2]);
            std::swap(obbAxis1[index1], obbAxis1[index2]);
            std::swap(obbAxis2[index1], obbAxis2[index2]);
            std::swap(obbHalfLengths[index1], obbHalfLengths[index2]);

            // Sphere
            std::swap(sphereCenters[index1], sphereCenters[index2]);
            std::swap(sphereRadii[index1], sphereRadii[index2]);
        }

        /**
         * 移除指定索引的元素（使用交换到末尾再删除的方式）
         * @param index 索引
         * @note 这会破坏元素顺序，但速度快
         */
        void RemoveSwap(size_t index)
        {
            if (index >= count)
                return;

            if (index != count - 1)
            {
                Swap(index, count - 1);
            }

            PopBack();
        }

    public: // 批量查询接口（便于 SIMD 优化）

        /**
         * 获取 AABB 最小点数组（用于批量操作）
         */
        const Vector3f* GetAABBMinPoints() const { return aabbMinPoints.data(); }
        Vector3f* GetAABBMinPoints() { return aabbMinPoints.data(); }

        /**
         * 获取 AABB 最大点数组
         */
        const Vector3f* GetAABBMaxPoints() const { return aabbMaxPoints.data(); }
        Vector3f* GetAABBMaxPoints() { return aabbMaxPoints.data(); }

        /**
         * 获取 OBB 中心点数组
         */
        const Vector3f* GetOBBCenters() const { return obbCenters.data(); }
        Vector3f* GetOBBCenters() { return obbCenters.data(); }

        /**
         * 获取球体中心点数组
         */
        const Vector3f* GetSphereCenters() const { return sphereCenters.data(); }
        Vector3f* GetSphereCenters() { return sphereCenters.data(); }

        /**
         * 获取球体半径数组
         */
        const float* GetSphereRadii() const { return sphereRadii.data(); }
        float* GetSphereRadii() { return sphereRadii.data(); }

    public: // 批量操作

        /**
         * 批量变换所有包围体
         * @param transform 变换矩阵
         */
        void TransformAll(const Matrix4f &transform)
        {
            // 提取变换的缩放因子
            const float s0 = glm::length(Vector3f(transform[0]));
            const float s1 = glm::length(Vector3f(transform[1]));
            const float s2 = glm::length(Vector3f(transform[2]));
            const float max_scale = glm::max(glm::max(s0, s1), s2);

            // 提取归一化的轴
            Vector3f a0 = (s0 > 0.0f) ? (Vector3f(transform[0]) / s0) : Vector3f(1, 0, 0);
            Vector3f a1 = (s1 > 0.0f) ? (Vector3f(transform[1]) / s1) : Vector3f(0, 1, 0);
            Vector3f a2 = (s2 > 0.0f) ? (Vector3f(transform[2]) / s2) : Vector3f(0, 0, 1);

            for (size_t i = 0; i < count; i++)
            {
                // Transform AABB (重建)
                Vector3f aabb_center = (aabbMinPoints[i] + aabbMaxPoints[i]) * 0.5f;
                Vector3f aabb_extent = (aabbMaxPoints[i] - aabbMinPoints[i]) * 0.5f;
                
                Vector3f new_center = Vector3f(transform * Vector4f(aabb_center, 1.0f));
                
                // 计算变换后的 extent
                const Vector3f ax = glm::abs(a0);
                const Vector3f ay = glm::abs(a1);
                const Vector3f az = glm::abs(a2);
                Vector3f scaled_extent = aabb_extent * Vector3f(s0, s1, s2);
                Vector3f new_extent = ax * scaled_extent.x + ay * scaled_extent.y + az * scaled_extent.z;
                
                aabbMinPoints[i] = new_center - new_extent;
                aabbMaxPoints[i] = new_center + new_extent;

                // Transform OBB
                obbCenters[i] = Vector3f(transform * Vector4f(obbCenters[i], 1.0f));
                
                // 旋转 OBB 的轴（注意：这里假设 OBB 的轴已经是正交的）
                Matrix3f rotation(a0, a1, a2);
                obbAxis0[i] = rotation * obbAxis0[i];
                obbAxis1[i] = rotation * obbAxis1[i];
                obbAxis2[i] = rotation * obbAxis2[i];
                
                // 缩放半长度
                obbHalfLengths[i] = obbHalfLengths[i] * Vector3f(s0, s1, s2);

                // Transform Sphere
                sphereCenters[i] = Vector3f(transform * Vector4f(sphereCenters[i], 1.0f));
                sphereRadii[i] = sphereRadii[i] * max_scale;
            }
        }

        /**
         * 批量平移所有包围体
         * @param offset 偏移向量
         */
        void TranslateAll(const Vector3f &offset)
        {
            for (size_t i = 0; i < count; i++)
            {
                // AABB
                aabbMinPoints[i] += offset;
                aabbMaxPoints[i] += offset;

                // OBB
                obbCenters[i] += offset;

                // Sphere
                sphereCenters[i] += offset;
            }
        }

        /**
         * 批量缩放所有包围体（从各自中心点缩放）
         * @param scale 缩放因子
         */
        void ScaleAll(float scale)
        {
            for (size_t i = 0; i < count; i++)
            {
                // AABB（从中心缩放）
                Vector3f aabb_center = (aabbMinPoints[i] + aabbMaxPoints[i]) * 0.5f;
                Vector3f aabb_extent = (aabbMaxPoints[i] - aabbMinPoints[i]) * 0.5f * scale;
                aabbMinPoints[i] = aabb_center - aabb_extent;
                aabbMaxPoints[i] = aabb_center + aabb_extent;

                // OBB（半长度缩放）
                obbHalfLengths[i] *= scale;

                // Sphere
                sphereRadii[i] *= scale;
            }
        }

    public: // 碰撞检测批量操作

        /**
         * 批量检查哪些包围体与指定点相交（使用球体快速测试）
         * @param point 测试点
         * @param out_indices 输出相交的索引列表
         */
        void FindIntersectingPoint(const Vector3f &point, std::vector<size_t> &out_indices) const
        {
            out_indices.clear();

            for (size_t i = 0; i < count; i++)
            {
                // 使用球体快速测试
                Vector3f diff = point - sphereCenters[i];
                float dist_sq = glm::dot(diff, diff);
                float radius_sq = sphereRadii[i] * sphereRadii[i];

                if (dist_sq <= radius_sq)
                {
                    out_indices.push_back(i);
                }
            }
        }

        /**
         * 批量检查哪些包围体与指定球体相交
         * @param sphere_center 球体中心
         * @param sphere_radius 球体半径
         * @param out_indices 输出相交的索引列表
         */
        void FindIntersectingSphere(const Vector3f &sphere_center, float sphere_radius,
                                    std::vector<size_t> &out_indices) const
        {
            out_indices.clear();

            for (size_t i = 0; i < count; i++)
            {
                Vector3f diff = sphere_center - sphereCenters[i];
                float dist_sq = glm::dot(diff, diff);
                float sum_radius = sphere_radius + sphereRadii[i];
                float sum_radius_sq = sum_radius * sum_radius;

                if (dist_sq <= sum_radius_sq)
                {
                    out_indices.push_back(i);
                }
            }
        }

        /**
         * 批量检查哪些包围体与指定 AABB 相交
         * @param aabb 测试 AABB
         * @param out_indices 输出相交的索引列表
         */
        void FindIntersectingAABB(const AABB &aabb, std::vector<size_t> &out_indices) const
        {
            out_indices.clear();

            Vector3f test_min = aabb.GetMin();
            Vector3f test_max = aabb.GetMax();

            for (size_t i = 0; i < count; i++)
            {
                // AABB vs AABB 测试
                if (!(aabbMaxPoints[i].x <= test_min.x || aabbMinPoints[i].x >= test_max.x ||
                      aabbMaxPoints[i].y <= test_min.y || aabbMinPoints[i].y >= test_max.y ||
                      aabbMaxPoints[i].z <= test_min.z || aabbMinPoints[i].z >= test_max.z))
                {
                    out_indices.push_back(i);
                }
            }
        }

    public: // 调试和诊断

        /**
         * 验证数据一致性
         * @return 是否一致
         */
        bool ValidateConsistency() const
        {
            // 检查所有数组大小是否一致
            return aabbMinPoints.size() == count &&
                   aabbMaxPoints.size() == count &&
                   obbCenters.size() == count &&
                   obbAxis0.size() == count &&
                   obbAxis1.size() == count &&
                   obbAxis2.size() == count &&
                   obbHalfLengths.size() == count &&
                   sphereCenters.size() == count &&
                   sphereRadii.size() == count;
        }

        /**
         * 获取内存使用量（字节）
         */
        size_t GetMemoryUsage() const
        {
            size_t total = 0;
            
            // AABB: 2 * Vector3f * count
            total += aabbMinPoints.capacity() * sizeof(Vector3f);
            total += aabbMaxPoints.capacity() * sizeof(Vector3f);

            // OBB: 5 * Vector3f * count
            total += obbCenters.capacity() * sizeof(Vector3f);
            total += obbAxis0.capacity() * sizeof(Vector3f);
            total += obbAxis1.capacity() * sizeof(Vector3f);
            total += obbAxis2.capacity() * sizeof(Vector3f);
            total += obbHalfLengths.capacity() * sizeof(Vector3f);

            // Sphere: Vector3f + float * count
            total += sphereCenters.capacity() * sizeof(Vector3f);
            total += sphereRadii.capacity() * sizeof(float);

            return total;
        }

        /**
         * 打印统计信息
         */
        void PrintStats() const
        {
            printf("BoundingVolumesDataStorage Statistics:\n");
            printf("  Count: %zu\n", count);
            printf("  Capacity: %zu\n", capacity);
            printf("  Memory Usage: %.2f KB\n", GetMemoryUsage() / 1024.0f);
            printf("  Consistency: %s\n", ValidateConsistency() ? "OK" : "FAILED");
        }
    };//class BoundingVolumesDataStorage
}//namespace hgl::math
