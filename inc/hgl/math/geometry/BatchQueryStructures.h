/**
 * BatchQueryStructures.h
 * 
 * SOA (Structure of Arrays) data structures for batch geometry queries.
 * Designed for SIMD (ISPC) and GPU (CUDA/OpenCL) acceleration.
 * 
 * Key Design Principles:
 * 1. SOA Layout: Separate arrays for each component (x, y, z, radius, etc.)
 * 2. Aligned Memory: 16/32-byte alignment for SIMD operations
 * 3. Batch Operations: Process multiple geometries in parallel
 * 4. Cache-Friendly: Sequential memory access patterns
 * 5. GPU-Ready: Structures can be directly copied to device memory
 */

#pragma once

#include <vector>
#include <hgl/math/Vector.h>
#include <cstdint>
#include <memory>

namespace hgl::math
{
    /**
     * Alignment helper for SIMD/GPU operations
     * Use 32-byte alignment for AVX2, 64-byte for AVX-512
     */
    constexpr size_t SIMD_ALIGNMENT = 32;
    
    template<typename T>
    using AlignedVector = std::vector<T, std::aligned_allocator<T, SIMD_ALIGNMENT>>;

    //=========================================================================
    // Batch Sphere Data (SOA Layout)
    //=========================================================================
    
    /**
     * Structure of Arrays for batch sphere operations
     * 
     * Memory Layout (for N spheres):
     * [cx0, cx1, cx2, ..., cxN] - Center X coordinates
     * [cy0, cy1, cy2, ..., cyN] - Center Y coordinates
     * [cz0, cz1, cz2, ..., czN] - Center Z coordinates
     * [r0,  r1,  r2,  ..., rN]  - Radii
     * 
     * Benefits:
     * - Sequential access for each component
     * - Efficient SIMD vectorization (process 8 spheres at once with AVX2)
     * - GPU-friendly memory layout
     * - Cache-efficient for bulk operations
     */
    struct BatchSphereSOA
    {
        AlignedVector<float> centerX;     // X coordinates of centers
        AlignedVector<float> centerY;     // Y coordinates of centers
        AlignedVector<float> centerZ;     // Z coordinates of centers
        AlignedVector<float> radius;      // Radii
        
        size_t count;                     // Number of spheres
        
        BatchSphereSOA() : count(0) {}
        
        /**
         * Reserve space for N spheres
         */
        void Reserve(size_t n) {
            centerX.reserve(n);
            centerY.reserve(n);
            centerZ.reserve(n);
            radius.reserve(n);
        }
        
        /**
         * Add a sphere to the batch
         */
        void Add(const Vector3f& center, float r) {
            centerX.push_back(center.x);
            centerY.push_back(center.y);
            centerZ.push_back(center.z);
            radius.push_back(r);
            count++;
        }
        
        /**
         * Clear all data
         */
        void Clear() {
            centerX.clear();
            centerY.clear();
            centerZ.clear();
            radius.clear();
            count = 0;
        }
        
        /**
         * Get raw data pointers for ISPC/GPU kernels
         */
        const float* GetCenterXData() const { return centerX.data(); }
        const float* GetCenterYData() const { return centerY.data(); }
        const float* GetCenterZData() const { return centerZ.data(); }
        const float* GetRadiusData() const { return radius.data(); }
    };

    //=========================================================================
    // Batch Capsule Data (SOA Layout)
    //=========================================================================
    
    /**
     * Structure of Arrays for batch capsule operations
     * 
     * Memory Layout:
     * [sx0, sx1, ...] - Start point X
     * [sy0, sy1, ...] - Start point Y
     * [sz0, sz1, ...] - Start point Z
     * [ex0, ex1, ...] - End point X
     * [ey0, ey1, ...] - End point Y
     * [ez0, ez1, ...] - End point Z
     * [r0,  r1,  ...] - Radii
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
    // Batch AABB Data (SOA Layout)
    //=========================================================================
    
    /**
     * Structure of Arrays for batch AABB operations
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
    // Batch Ray Data (SOA Layout)
    //=========================================================================
    
    /**
     * Structure of Arrays for batch ray operations
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
    // Batch Query Results (SOA Layout)
    //=========================================================================
    
    /**
     * Batch collision results
     * Stores results as bit flags for memory efficiency
     */
    struct BatchCollisionResults
    {
        AlignedVector<uint8_t> hits;      // Bit-packed hit flags (1 bit per test)
        AlignedVector<float> distances;    // Optional: distances for hits
        
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
     * Batch raycast results with detailed information
     */
    struct BatchRaycastResults
    {
        AlignedVector<uint8_t> hits;       // Hit flags (bit-packed)
        AlignedVector<float> distances;     // Hit distances (t values)
        AlignedVector<float> hitPointX;     // Hit point X coordinates
        AlignedVector<float> hitPointY;     // Hit point Y coordinates
        AlignedVector<float> hitPointZ;     // Hit point Z coordinates
        AlignedVector<float> normalX;       // Hit normal X components
        AlignedVector<float> normalY;       // Hit normal Y components
        AlignedVector<float> normalZ;       // Hit normal Z components
        
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
    // Usage Example (CPU Implementation - ISPC/GPU versions separate)
    //=========================================================================
    
    /**
     * Example: Batch sphere-point distance calculation
     * This CPU version shows the algorithm structure.
     * ISPC/GPU versions would process multiple spheres per iteration.
     * 
     * ISPC equivalent would use:
     * - foreach (i = 0 ... count) for parallel processing
     * - SIMD operations on 8 spheres at once
     * 
     * GPU equivalent would use:
     * - One thread per sphere
     * - Coalesced memory access from SOA layout
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
     * Example: Batch ray-sphere intersection
     * Returns bit-packed hit flags
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
