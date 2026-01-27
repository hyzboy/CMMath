/**
 * BatchQueryStructures.h
 *
 * 批量几何查询的SOA（结构体数组）数据结构。
 * 设计用于SIMD（ISPC）和GPU（CUDA/OpenCL）加速。
 *
 * 主要设计原则：
 * 1. SOA布局：每个分量（x, y, z, radius等）单独数组
 * 2. 对齐内存：SIMD操作16/32字节对齐
 * 3. 批量操作：并行处理多个几何体
 * 4. 缓存友好：顺序内存访问
 * 5. GPU友好：结构可直接拷贝到设备内存
 */

#pragma once

#include <vector>
#include <hgl/math/Vector.h>
#include <cstdint>
#include <memory>

namespace hgl::math
{
    /**
     * SIMD/GPU操作的对齐辅助
     * AVX2用32字节对齐，AVX-512用64字节对齐
     */
    constexpr size_t SIMD_ALIGNMENT = 32;

    template<typename T>
    using AlignedVector = std::vector<T, std::aligned_allocator<T, SIMD_ALIGNMENT>>;

    //=========================================================================
    // 批量球体数据（SOA布局）
    //=========================================================================

    /**
     * 批量球体操作的SOA结构
     *
     * 内存布局（N个球体）：
     * [cx0, cx1, ..., cxN] - 球心X坐标
     * [cy0, cy1, ..., cyN] - 球心Y坐标
     * [cz0, cz1, ..., czN] - 球心Z坐标
     * [r0,  r1,  ..., rN]  - 半径
     *
     * 优点：
     * - 每个分量顺序访问
     * - 高效SIMD矢量化（AVX2一次处理8个球体）
     * - GPU友好内存布局
     * - 批量操作缓存高效
     */
    struct BatchSphereSOA
    {
        AlignedVector<float> centerX;     // 球心X坐标
        AlignedVector<float> centerY;     // 球心Y坐标
        AlignedVector<float> centerZ;     // 球心Z坐标
        AlignedVector<float> radius;      // 半径

        size_t count;                     // 球体数量

        BatchSphereSOA() : count(0) {}

        /**
         * 预留N个球体空间
         */
        void Reserve(size_t n) {
            centerX.reserve(n);
            centerY.reserve(n);
            centerZ.reserve(n);
            radius.reserve(n);
        }

        /**
         * 向批量中添加一个球体
         */
        void Add(const Vector3f& center, float r) {
            centerX.push_back(center.x);
            centerY.push_back(center.y);
            centerZ.push_back(center.z);
            radius.push_back(r);
            count++;
        }

        /**
         * 清空所有数据
         */
        void Clear() {
            centerX.clear();
            centerY.clear();
            centerZ.clear();
            radius.clear();
            count = 0;
        }

        /**
         * 获取ISPC/GPU核的原始数据指针
         */
        const float* GetCenterXData() const { return centerX.data(); }
        const float* GetCenterYData() const { return centerY.data(); }
        const float* GetCenterZData() const { return centerZ.data(); }
        const float* GetRadiusData() const { return radius.data(); }
    };

    //=========================================================================
    // 批量胶囊体数据（SOA布局）
    //=========================================================================

    /**
     * 批量胶囊体操作的SOA结构
     *
     * 内存布局：
     * [sx0, sx1, ...] - 起点X
     * [sy0, sy1, ...] - 起点Y
     * [sz0, sz1, ...] - 起点Z
     * [ex0, ex1, ...] - 终点X
     * [ey0, ey1, ...] - 终点Y
     * [ez0, ez1, ...] - 终点Z
     * [r0,  r1,  ...] - 半径
     */
    struct BatchCapsuleSOA
    {
        AlignedVector<float> startX, startY, startZ;
        AlignedVector<float> endX, endY, endZ;
        AlignedVector<float> radius;

        size_t count;

        BatchCapsuleSOA() : count(0) {}

        void Reserve(size_t n) {
            startX.reserve(n); startY.reserve(n); startZ.reserve(n);
            endX.reserve(n); endY.reserve(n); endZ.reserve(n);
            radius.reserve(n);
        }

        void Add(const Vector3f& start, const Vector3f& end, float r) {
            startX.push_back(start.x); startY.push_back(start.y); startZ.push_back(start.z);
            endX.push_back(end.x); endY.push_back(end.y); endZ.push_back(end.z);
            radius.push_back(r);
            count++;
        }

        void Clear() {
            startX.clear(); startY.clear(); startZ.clear();
            endX.clear(); endY.clear(); endZ.clear();
            radius.clear();
            count = 0;
        }
    };

    //=========================================================================
    // 批量AABB数据（SOA布局）
    //=========================================================================

    /**
     * 批量AABB操作的SOA结构
     */
    struct BatchAABBSOA
    {
        AlignedVector<float> minX, minY, minZ;
        AlignedVector<float> maxX, maxY, maxZ;

        size_t count;

        BatchAABBSOA() : count(0) {}

        void Reserve(size_t n) {
            minX.reserve(n); minY.reserve(n); minZ.reserve(n);
            maxX.reserve(n); maxY.reserve(n); maxZ.reserve(n);
        }

        void Add(const Vector3f& min, const Vector3f& max) {
            minX.push_back(min.x); minY.push_back(min.y); minZ.push_back(min.z);
            maxX.push_back(max.x); maxY.push_back(max.y); maxZ.push_back(max.z);
            count++;
        }

        void Clear() {
            minX.clear(); minY.clear(); minZ.clear();
            maxX.clear(); maxY.clear(); maxZ.clear();
            count = 0;
        }
    };

    //=========================================================================
    // 批量射线数据（SOA布局）
    //=========================================================================

    /**
     * 批量射线操作的SOA结构
     */
    struct BatchRaySOA
    {
        AlignedVector<float> originX, originY, originZ;
        AlignedVector<float> directionX, directionY, directionZ;

        size_t count;

        BatchRaySOA() : count(0) {}

        void Reserve(size_t n) {
            originX.reserve(n); originY.reserve(n); originZ.reserve(n);
            directionX.reserve(n); directionY.reserve(n); directionZ.reserve(n);
        }

        void Add(const Vector3f& origin, const Vector3f& direction) {
            originX.push_back(origin.x); originY.push_back(origin.y); originZ.push_back(origin.z);
            directionX.push_back(direction.x); directionY.push_back(direction.y); directionZ.push_back(direction.z);
            count++;
        }

        void Clear() {
            originX.clear(); originY.clear(); originZ.clear();
            directionX.clear(); directionY.clear(); directionZ.clear();
            count = 0;
        }
    };

    //=========================================================================
    // 批量查询结果（SOA布局）
    //=========================================================================

    /**
     * 批量碰撞检测结果
     * 以位标志存储结果，节省内存
     */
    struct BatchCollisionResults
    {
        AlignedVector<uint8_t> hits;      // 命中位标志（每次测试1位）
        AlignedVector<float> distances;    // 可选：命中距离

        void Reserve(size_t n) {
            hits.reserve((n + 7) / 8);  // 8 results per byte
            distances.reserve(n);
        }

        bool GetHit(size_t index) const {
            return (hits[index / 8] & (1 << (index % 8))) != 0;
        }

        void SetHit(size_t index, bool hit) {
            if (hit)
                hits[index / 8] |= (1 << (index % 8));
            else
                hits[index / 8] &= ~(1 << (index % 8));
        }
    };

    /**
     * 批量射线检测结果（含详细信息）
     */
    struct BatchRaycastResults
    {
        AlignedVector<uint8_t> hits;       // 命中标志（位打包）
        AlignedVector<float> distances;     // 命中距离（t值）
        AlignedVector<float> hitPointX;     // 命中点X坐标
        AlignedVector<float> hitPointY;     // 命中点Y坐标
        AlignedVector<float> hitPointZ;     // 命中点Z坐标
        AlignedVector<float> normalX;       // 命中法线X分量
        AlignedVector<float> normalY;       // 命中法线Y分量
        AlignedVector<float> normalZ;       // 命中法线Z分量

        size_t count;

        BatchRaycastResults() : count(0) {}

        void Reserve(size_t n) {
            hits.reserve((n + 7) / 8);
            distances.reserve(n);
            hitPointX.reserve(n); hitPointY.reserve(n); hitPointZ.reserve(n);
            normalX.reserve(n); normalY.reserve(n); normalZ.reserve(n);
        }
    };

    //=========================================================================
    // 用法示例（CPU实现，ISPC/GPU版本另行实现）
    //=========================================================================

    /**
     * 示例：批量球体-点距离计算
     * 本CPU版本展示算法结构。
     * ISPC/GPU版本每次可处理多个球体。
     *
     * ISPC实现：
     * - foreach (i = 0 ... count) 并行处理
     * - SIMD一次处理8个球体
     *
     * GPU实现：
     * - 每个球体一个线程
     * - SOA布局保证合并内存访问
     */
    inline void BatchSpherePointDistance_CPU(
        const BatchSphereSOA& spheres,
        const Vector3f& point,
        float* outDistances)
    {
        const float px = point.x;
        const float py = point.y;
        const float pz = point.z;

        // Process spheres sequentially (ISPC/GPU would parallelize this)
        for (size_t i = 0; i < spheres.count; ++i) {
            float dx = px - spheres.centerX[i];
            float dy = py - spheres.centerY[i];
            float dz = pz - spheres.centerZ[i];

            float distSquared = dx * dx + dy * dy + dz * dz;
            float dist = sqrtf(distSquared);

            outDistances[i] = fmaxf(0.0f, dist - spheres.radius[i]);
        }
    }

    /**
     * 示例：批量射线-球体相交检测
     * 返回位打包命中标志
     */
    inline void BatchRaySphereIntersection_CPU(
        const BatchRaySOA& rays,
        const BatchSphereSOA& spheres,
        BatchCollisionResults& results)
    {
        // Assumes rays.count == spheres.count (one-to-one testing)
        // For all-pairs, would need nested loops or different structure

        size_t count = rays.count;
        results.hits.resize((count + 7) / 8, 0);
        results.distances.resize(count);

        for (size_t i = 0; i < count; ++i) {
            // Ray parameters
            float ox = rays.originX[i];
            float oy = rays.originY[i];
            float oz = rays.originZ[i];
            float dx = rays.directionX[i];
            float dy = rays.directionY[i];
            float dz = rays.directionZ[i];

            // Sphere parameters
            float cx = spheres.centerX[i];
            float cy = spheres.centerY[i];
            float cz = spheres.centerZ[i];
            float r = spheres.radius[i];

            // Ray-sphere intersection
            float ocx = ox - cx;
            float ocy = oy - cy;
            float ocz = oz - cz;

            float a = dx * dx + dy * dy + dz * dz;
            float b = 2.0f * (ocx * dx + ocy * dy + ocz * dz);
            float c = ocx * ocx + ocy * ocy + ocz * ocz - r * r;

            float discriminant = b * b - 4 * a * c;

            bool hit = discriminant >= 0.0f;
            results.SetHit(i, hit);

            if (hit) {
                float t = (-b - sqrtf(discriminant)) / (2.0f * a);
                results.distances[i] = t;
            }
        }
    }

} // namespace hgl::math
