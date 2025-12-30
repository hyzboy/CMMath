/**
 * SIMD_SSE.cpp - SSE/SSE2 优化实现
 * 
 * 使用 SSE/SSE2 指令集加速向量和矩阵运算
 * 编译时需要添加 -msse4.1 或 /arch:SSE2 标志
 */

#include<hgl/math/SIMD.h>
#include<cstring>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #define HGL_SIMD_SSE_AVAILABLE
    #include<xmmintrin.h>  // SSE
    #include<emmintrin.h>  // SSE2
    #include<pmmintrin.h>  // SSE3
    #include<smmintrin.h>  // SSE4.1
    
    #ifdef _MSC_VER
        #include<intrin.h>
    #else
        #include<cpuid.h>
    #endif
#endif

namespace hgl::math::simd
{
    // ==================== SIMD 检测 ====================
    
#ifdef HGL_SIMD_SSE_AVAILABLE
    
    static void cpuid(int info[4], int function_id)
    {
#ifdef _MSC_VER
        __cpuid(info, function_id);
#else
        __cpuid_count(function_id, 0, info[0], info[1], info[2], info[3]);
#endif
    }
    
    bool HasSSE() noexcept
    {
        int info[4];
        cpuid(info, 1);
        return (info[3] & (1 << 25)) != 0;
    }
    
    bool HasSSE2() noexcept
    {
        int info[4];
        cpuid(info, 1);
        return (info[3] & (1 << 26)) != 0;
    }
    
    bool HasAVX() noexcept
    {
        int info[4];
        cpuid(info, 1);
        return (info[2] & (1 << 28)) != 0;
    }
    
    bool HasAVX2() noexcept
    {
        int info[4];
        cpuid(info, 7);
        return (info[1] & (1 << 5)) != 0;
    }
    
#else
    
    bool HasSSE() noexcept { return false; }
    bool HasSSE2() noexcept { return false; }
    bool HasAVX() noexcept { return false; }
    bool HasAVX2() noexcept { return false; }
    
#endif

    // ==================== 批量向量运算实现 ====================
    
#ifdef HGL_SIMD_SSE_AVAILABLE
    
    void VectorAdd_SIMD(Vector3f *dst, const Vector3f *a, const Vector3f *b, size_t count)
    {
        size_t i = 0;
        
        // 处理 4 向量对齐的部分 (实际上处理的是单个 Vector3f，每次一个)
        for (; i + 1 <= count; i++)
        {
            __m128 va = _mm_loadu_ps(&a[i].x);  // 加载 a[i] 的 x, y, z (+ 填充)
            __m128 vb = _mm_loadu_ps(&b[i].x);  // 加载 b[i] 的 x, y, z (+ 填充)
            __m128 result = _mm_add_ps(va, vb);
            _mm_storeu_ps(&dst[i].x, result);
        }
        
        // 处理剩余元素（虽然上面已经处理了所有）
        for (; i < count; i++)
        {
            dst[i].x = a[i].x + b[i].x;
            dst[i].y = a[i].y + b[i].y;
            dst[i].z = a[i].z + b[i].z;
        }
    }
    
    void VectorSub_SIMD(Vector3f *dst, const Vector3f *a, const Vector3f *b, size_t count)
    {
        size_t i = 0;
        
        for (; i + 1 <= count; i++)
        {
            __m128 va = _mm_loadu_ps(&a[i].x);
            __m128 vb = _mm_loadu_ps(&b[i].x);
            __m128 result = _mm_sub_ps(va, vb);
            _mm_storeu_ps(&dst[i].x, result);
        }
        
        for (; i < count; i++)
        {
            dst[i].x = a[i].x - b[i].x;
            dst[i].y = a[i].y - b[i].y;
            dst[i].z = a[i].z - b[i].z;
        }
    }
    
    void VectorScale_SIMD(Vector3f *dst, const Vector3f *src, float scale, size_t count)
    {
        __m128 vscale = _mm_set1_ps(scale);
        size_t i = 0;
        
        for (; i + 1 <= count; i++)
        {
            __m128 v = _mm_loadu_ps(&src[i].x);
            __m128 result = _mm_mul_ps(v, vscale);
            _mm_storeu_ps(&dst[i].x, result);
        }
        
        for (; i < count; i++)
        {
            dst[i].x = src[i].x * scale;
            dst[i].y = src[i].y * scale;
            dst[i].z = src[i].z * scale;
        }
    }
    
    void VectorDot_SIMD(float *dst, const Vector3f *a, const Vector3f *b, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            __m128 va = _mm_loadu_ps(&a[i].x);
            __m128 vb = _mm_loadu_ps(&b[i].x);
            __m128 mul = _mm_mul_ps(va, vb);
            
            // 水平求和 (x*x + y*y + z*z)
            // 使用 SSE3 的 hadd 或手动实现
            __m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 1, 0, 3));
            __m128 sums = _mm_add_ps(mul, shuf);
            shuf = _mm_movehl_ps(shuf, sums);
            sums = _mm_add_ss(sums, shuf);
            
            _mm_store_ss(&dst[i], sums);
        }
    }
    
    void VectorCross_SIMD(Vector3f *dst, const Vector3f *a, const Vector3f *b, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            __m128 va = _mm_loadu_ps(&a[i].x);
            __m128 vb = _mm_loadu_ps(&b[i].x);
            
            // 叉积: (ay*bz - az*by, az*bx - ax*bz, ax*by - ay*bx)
            __m128 a_yzx = _mm_shuffle_ps(va, va, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 b_yzx = _mm_shuffle_ps(vb, vb, _MM_SHUFFLE(3, 0, 2, 1));
            __m128 a_zxy = _mm_shuffle_ps(va, va, _MM_SHUFFLE(3, 1, 0, 2));
            __m128 b_zxy = _mm_shuffle_ps(vb, vb, _MM_SHUFFLE(3, 1, 0, 2));
            
            __m128 result = _mm_sub_ps(
                _mm_mul_ps(a_yzx, b_zxy),
                _mm_mul_ps(a_zxy, b_yzx)
            );
            
            _mm_storeu_ps(&dst[i].x, result);
        }
    }
    
    void VectorNormalize_SIMD(Vector3f *dst, const Vector3f *src, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            __m128 v = _mm_loadu_ps(&src[i].x);
            __m128 sq = _mm_mul_ps(v, v);
            
            // 水平求和
            __m128 shuf = _mm_shuffle_ps(sq, sq, _MM_SHUFFLE(2, 1, 0, 3));
            __m128 sums = _mm_add_ps(sq, shuf);
            shuf = _mm_movehl_ps(shuf, sums);
            sums = _mm_add_ss(sums, shuf);
            
            // 平方根和倒数
            __m128 len = _mm_sqrt_ss(sums);
            len = _mm_shuffle_ps(len, len, _MM_SHUFFLE(0, 0, 0, 0));
            
            // 避免除以零
            __m128 result = _mm_div_ps(v, _mm_max_ps(len, _mm_set1_ps(1e-8f)));
            
            _mm_storeu_ps(&dst[i].x, result);
        }
    }
    
    void VectorLength_SIMD(float *dst, const Vector3f *src, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            __m128 v = _mm_loadu_ps(&src[i].x);
            __m128 sq = _mm_mul_ps(v, v);
            
            // 水平求和
            __m128 shuf = _mm_shuffle_ps(sq, sq, _MM_SHUFFLE(2, 1, 0, 3));
            __m128 sums = _mm_add_ps(sq, shuf);
            shuf = _mm_movehl_ps(shuf, sums);
            sums = _mm_add_ss(sums, shuf);
            
            __m128 len = _mm_sqrt_ss(sums);
            _mm_store_ss(&dst[i], len);
        }
    }
    
    // ==================== 批量矩阵变换 ====================
    
    void TransformPoints_SIMD(Vector3f *dst, const Matrix4f &matrix, const Vector3f *src, size_t count)
    {
        // 加载矩阵的列
        __m128 col0 = _mm_loadu_ps(&matrix[0][0]);
        __m128 col1 = _mm_loadu_ps(&matrix[1][0]);
        __m128 col2 = _mm_loadu_ps(&matrix[2][0]);
        __m128 col3 = _mm_loadu_ps(&matrix[3][0]);
        
        for (size_t i = 0; i < count; i++)
        {
            __m128 v = _mm_set_ps(1.0f, src[i].z, src[i].y, src[i].x);
            
            // 矩阵乘法
            __m128 x = _mm_mul_ps(col0, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));
            __m128 y = _mm_mul_ps(col1, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
            __m128 z = _mm_mul_ps(col2, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
            __m128 w = _mm_mul_ps(col3, _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)));
            
            __m128 result = _mm_add_ps(_mm_add_ps(x, y), _mm_add_ps(z, w));
            
            _mm_storeu_ps(&dst[i].x, result);
        }
    }
    
    void TransformDirections_SIMD(Vector3f *dst, const Matrix4f &matrix, const Vector3f *src, size_t count)
    {
        // 加载矩阵的列（忽略平移部分）
        __m128 col0 = _mm_loadu_ps(&matrix[0][0]);
        __m128 col1 = _mm_loadu_ps(&matrix[1][0]);
        __m128 col2 = _mm_loadu_ps(&matrix[2][0]);
        
        for (size_t i = 0; i < count; i++)
        {
            __m128 v = _mm_set_ps(0.0f, src[i].z, src[i].y, src[i].x);
            
            __m128 x = _mm_mul_ps(col0, _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)));
            __m128 y = _mm_mul_ps(col1, _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)));
            __m128 z = _mm_mul_ps(col2, _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)));
            
            __m128 result = _mm_add_ps(_mm_add_ps(x, y), z);
            
            _mm_storeu_ps(&dst[i].x, result);
        }
    }
    
    void MatrixMultiply_SIMD(Matrix4f *dst, const Matrix4f *a, const Matrix4f *b, size_t count)
    {
        // 简化实现：逐个矩阵相乘
        for (size_t i = 0; i < count; i++)
        {
            dst[i] = a[i] * b[i];  // 使用 GLM 的矩阵乘法
        }
    }
    
    void MergeAABBs_SIMD(AABB *dst, const AABB *a, const AABB *b, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            __m128 a_min = _mm_loadu_ps(&a[i].GetMinPoint().x);
            __m128 a_max = _mm_loadu_ps(&a[i].GetMaxPoint().x);
            __m128 b_min = _mm_loadu_ps(&b[i].GetMinPoint().x);
            __m128 b_max = _mm_loadu_ps(&b[i].GetMaxPoint().x);
            
            __m128 new_min = _mm_min_ps(a_min, b_min);
            __m128 new_max = _mm_max_ps(a_max, b_max);
            
            Vector3f min_point, max_point;
            _mm_storeu_ps(&min_point.x, new_min);
            _mm_storeu_ps(&max_point.x, new_max);
            
            dst[i].SetMinMax(min_point, max_point);
        }
    }
    
    void ExpandAABBsByPoints_SIMD(AABB *aabbs, const Vector3f *points, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            aabbs[i].AddPoint(points[i]);
        }
    }
    
#else
    
    // 非 SSE 平台的回退实现
    
    void VectorAdd_SIMD(Vector3f *dst, const Vector3f *a, const Vector3f *b, size_t count)
    {
        for (size_t i = 0; i < count; i++)
            dst[i] = a[i] + b[i];
    }
    
    void VectorSub_SIMD(Vector3f *dst, const Vector3f *a, const Vector3f *b, size_t count)
    {
        for (size_t i = 0; i < count; i++)
            dst[i] = a[i] - b[i];
    }
    
    void VectorScale_SIMD(Vector3f *dst, const Vector3f *src, float scale, size_t count)
    {
        for (size_t i = 0; i < count; i++)
            dst[i] = src[i] * scale;
    }
    
    void VectorDot_SIMD(float *dst, const Vector3f *a, const Vector3f *b, size_t count)
    {
        for (size_t i = 0; i < count; i++)
            dst[i] = dot(a[i], b[i]);
    }
    
    void VectorCross_SIMD(Vector3f *dst, const Vector3f *a, const Vector3f *b, size_t count)
    {
        for (size_t i = 0; i < count; i++)
            dst[i] = cross(a[i], b[i]);
    }
    
    void VectorNormalize_SIMD(Vector3f *dst, const Vector3f *src, size_t count)
    {
        for (size_t i = 0; i < count; i++)
            dst[i] = normalize(src[i]);
    }
    
    void VectorLength_SIMD(float *dst, const Vector3f *src, size_t count)
    {
        for (size_t i = 0; i < count; i++)
            dst[i] = length(src[i]);
    }
    
    void TransformPoints_SIMD(Vector3f *dst, const Matrix4f &matrix, const Vector3f *src, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            Vector4f v(src[i], 1.0f);
            Vector4f result = matrix * v;
            dst[i] = Vector3f(result.x, result.y, result.z);
        }
    }
    
    void TransformDirections_SIMD(Vector3f *dst, const Matrix4f &matrix, const Vector3f *src, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            Vector4f v(src[i], 0.0f);
            Vector4f result = matrix * v;
            dst[i] = Vector3f(result.x, result.y, result.z);
        }
    }
    
    void MatrixMultiply_SIMD(Matrix4f *dst, const Matrix4f *a, const Matrix4f *b, size_t count)
    {
        for (size_t i = 0; i < count; i++)
            dst[i] = a[i] * b[i];
    }
    
    void MergeAABBs_SIMD(AABB *dst, const AABB *a, const AABB *b, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            dst[i] = a[i];
            dst[i].Merge(b[i]);
        }
    }
    
    void ExpandAABBsByPoints_SIMD(AABB *aabbs, const Vector3f *points, size_t count)
    {
        for (size_t i = 0; i < count; i++)
            aabbs[i].AddPoint(points[i]);
    }
    
#endif

}//namespace hgl::math::simd
