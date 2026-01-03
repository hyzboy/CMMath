# Batch Query Optimization Guide

## SOA (Structure of Arrays) Design for SIMD/GPU Acceleration

This document describes the SOA data layout strategy for batch geometry queries, designed for efficient ISPC (Intel SPMD Program Compiler) and GPU (CUDA/OpenCL) acceleration.

## Why SOA Layout?

### Traditional AOS (Array of Structures)
```cpp
// BAD: Poor SIMD/GPU performance
struct Sphere {
    Vector3f center;  // x, y, z
    float radius;
};
Sphere spheres[1000];

// Memory layout: [cx0,cy0,cz0,r0, cx1,cy1,cz1,r1, ...]
// Problem: Non-contiguous access when processing X coordinates
```

### SOA (Structure of Arrays)
```cpp
// GOOD: Optimal for SIMD/GPU
struct BatchSphereSOA {
    float centerX[1000];  // All X coordinates together
    float centerY[1000];  // All Y coordinates together
    float centerZ[1000];  // All Z coordinates together
    float radius[1000];   // All radii together
};

// Memory layout: [cx0,cx1,cx2,...] [cy0,cy1,cy2,...] [cz0,cz1,cz2,...] [r0,r1,r2,...]
// Benefit: Sequential access, perfect for SIMD vectorization
```

## Performance Benefits

### 1. SIMD Vectorization (CPU)
```cpp
// AVX2 can process 8 floats at once
// With SOA, load 8 sphere X coordinates in one instruction:
__m256 centerX = _mm256_load_ps(&spheres.centerX[i]);

// With AOS, would need 8 separate loads or complex shuffling
```

### 2. GPU Coalesced Access
```cpp
// GPU threads access consecutive memory locations
// Thread 0 reads centerX[0]
// Thread 1 reads centerX[1]
// Thread 2 reads centerX[2]
// ...all in one memory transaction
```

### 3. Cache Efficiency
- Sequential access pattern = better cache utilization
- Prefetcher can predict access patterns
- Reduced cache misses

## ISPC Integration

### Example ISPC Kernel
```ispc
// batch_sphere_distance.ispc
export void BatchSpherePointDistance(
    uniform float px, uniform float py, uniform float pz,  // Query point
    const uniform float centerX[], const uniform float centerY[], const uniform float centerZ[],
    const uniform float radius[],
    uniform float outDistances[],
    uniform int count)
{
    // Process 'programCount' spheres in parallel (typically 8 or 16)
    foreach (i = 0 ... count) {
        float dx = px - centerX[i];
        float dy = py - centerY[i];
        float dz = pz - centerZ[i];
        
        float distSq = dx * dx + dy * dy + dz * dz;
        float dist = sqrt(distSq);
        
        outDistances[i] = max(0.0f, dist - radius[i]);
    }
}
```

### C++ Integration
```cpp
#include "batch_sphere_distance_ispc.h"

void ComputeDistances(const BatchSphereSOA& spheres, const Vector3f& point, float* results) {
    ispc::BatchSpherePointDistance(
        point.x, point.y, point.z,
        spheres.GetCenterXData(),
        spheres.GetCenterYData(),
        spheres.GetCenterZData(),
        spheres.GetRadiusData(),
        results,
        spheres.count
    );
}
```

## GPU (CUDA) Integration

### Example CUDA Kernel
```cuda
// batch_sphere_distance.cu
__global__ void BatchSpherePointDistance(
    float px, float py, float pz,
    const float* centerX, const float* centerY, const float* centerZ,
    const float* radius,
    float* outDistances,
    int count)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (i < count) {
        float dx = px - centerX[i];  // Coalesced memory access
        float dy = py - centerY[i];
        float dz = pz - centerZ[i];
        
        float distSq = dx * dx + dy * dy + dz * dz;
        float dist = sqrtf(distSq);
        
        outDistances[i] = fmaxf(0.0f, dist - radius[i]);
    }
}
```

### Launch Configuration
```cpp
void ComputeDistancesGPU(const BatchSphereSOA& spheres, const Vector3f& point, float* results) {
    // Copy data to GPU
    float *d_centerX, *d_centerY, *d_centerZ, *d_radius, *d_results;
    cudaMalloc(&d_centerX, spheres.count * sizeof(float));
    // ... allocate others
    
    cudaMemcpy(d_centerX, spheres.GetCenterXData(), 
               spheres.count * sizeof(float), cudaMemcpyHostToDevice);
    // ... copy others
    
    // Launch kernel
    int blockSize = 256;
    int gridSize = (spheres.count + blockSize - 1) / blockSize;
    BatchSpherePointDistance<<<gridSize, blockSize>>>(
        point.x, point.y, point.z,
        d_centerX, d_centerY, d_centerZ, d_radius,
        d_results, spheres.count
    );
    
    // Copy results back
    cudaMemcpy(results, d_results, spheres.count * sizeof(float), 
               cudaMemcpyDeviceToHost);
    
    // Free GPU memory
    cudaFree(d_centerX);
    // ... free others
}
```

## Usage Patterns

### Pattern 1: Single Query, Multiple Targets
```cpp
// Find which spheres contain a point
BatchSphereSOA spheres;
for (auto& obj : scene.objects) {
    spheres.Add(obj.position, obj.radius);
}

Vector3f testPoint(10, 5, 3);
BatchCollisionResults results;
BatchPointSphereContainment(testPoint, spheres, results);

// Check results
for (size_t i = 0; i < spheres.count; ++i) {
    if (results.GetHit(i)) {
        // Sphere i contains the point
    }
}
```

### Pattern 2: Multiple Rays, Multiple Targets
```cpp
// Ray casting against scene
BatchRaySOA rays;
BatchSphereSOA spheres;

// Add rays
for (auto& ray : playerRays) {
    rays.Add(ray.origin, ray.direction);
}

// Add spheres
for (auto& sphere : scene.spheres) {
    spheres.Add(sphere.center, sphere.radius);
}

// Process (one-to-one or all-pairs depending on algorithm)
BatchRaycastResults results;
BatchRaySphereIntersection(rays, spheres, results);
```

### Pattern 3: Streaming Updates
```cpp
// For dynamic scenes with moving objects
BatchSphereSOA spheres;
spheres.Reserve(MAX_OBJECTS);

while (running) {
    spheres.Clear();
    
    // Rebuild each frame
    for (auto& obj : activeObjects) {
        spheres.Add(obj.GetPosition(), obj.GetRadius());
    }
    
    // Run queries
    ProcessCollisions(spheres);
}
```

## Memory Alignment

### Importance
- SIMD instructions require aligned memory (16/32/64 bytes)
- GPU global memory transactions are more efficient with aligned access
- Modern CPUs can cache-line align (64 bytes)

### Implementation
```cpp
// Use aligned allocators
constexpr size_t SIMD_ALIGNMENT = 32;  // AVX2
template<typename T>
using AlignedVector = std::vector<T, std::aligned_allocator<T, SIMD_ALIGNMENT>>;

AlignedVector<float> centerX;  // Guaranteed 32-byte aligned
```

## Performance Guidelines

### When to Use Batch Queries

✅ **Good Use Cases:**
- 1000+ geometry queries per frame
- Uniform query types (e.g., all ray-sphere tests)
- Queries can be grouped together
- Target SIMD/GPU platforms

❌ **Poor Use Cases:**
- Few queries (<100)
- Heterogeneous query types
- Results needed immediately (can't batch)
- CPU-only with no SIMD

### Expected Performance

**CPU (AVX2) vs Scalar:**
- 4-8x speedup for well-vectorized operations
- Best case: ~16 spheres processed per iteration (2 AVX2 ops)

**GPU vs CPU:**
- 10-100x speedup for large batches (10,000+)
- Memory transfer overhead significant for small batches

**ISPC vs C++:**
- 3-5x speedup typically
- Easier to write than intrinsics
- Near-GPU performance for CPU-bound tasks

## Integration with Existing Code

### Hybrid Approach
```cpp
// Single queries use existing API
bool hit = CollisionDetector::Intersects(sphere, capsule);

// Batch queries use SOA API
BatchSphereSOA sphereBatch;
// ... populate batch
BatchCollisionResults results = BatchCollisionDetector::Intersects(sphereBatch, testSphere);
```

### Conversion Helpers
```cpp
// Convert from AOS to SOA
BatchSphereSOA ToSOA(const std::vector<Sphere>& spheres) {
    BatchSphereSOA result;
    result.Reserve(spheres.size());
    for (const auto& s : spheres) {
        result.Add(s.GetCenter(), s.GetRadius());
    }
    return result;
}
```

## Future Extensions

1. **Spatial Acceleration + SOA**
   - BVH nodes in SOA layout
   - Parallel tree traversal

2. **Persistent GPU Buffers**
   - Keep geometry on GPU between frames
   - Update only changed objects

3. **Multi-GPU Support**
   - Partition geometry across GPUs
   - Parallel query processing

4. **Compressed Formats**
   - Half-precision (fp16) for mobile
   - Quantized positions for distant objects

## Build Integration

### CMake Configuration
```cmake
# ISPC support
find_program(ISPC_COMPILER ispc)
if(ISPC_COMPILER)
    add_library(batch_queries_ispc 
        batch_sphere_distance.ispc
        batch_raycast.ispc)
    target_compile_options(batch_queries_ispc PRIVATE --target=avx2)
endif()

# CUDA support
if(CUDA_FOUND)
    cuda_add_library(batch_queries_cuda
        batch_sphere_distance.cu
        batch_raycast.cu)
endif()
```

## Testing

See `tests/test_batch_queries.cpp` for:
- Correctness validation (SOA vs scalar)
- Performance benchmarks
- Memory alignment verification

## References

- [ISPC Documentation](https://ispc.github.io/)
- [CUDA Programming Guide](https://docs.nvidia.com/cuda/)
- [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/)
- "Data-Oriented Design" by Richard Fabian
- "Computer Systems: A Programmer's Perspective" (Memory Hierarchy chapter)
