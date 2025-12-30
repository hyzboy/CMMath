/**
 * SIMD_AVX.cpp - AVX/AVX2 优化实现
 * 
 * 使用 AVX/AVX2 指令集加速向量和矩阵运算
 * AVX 提供 256 位宽寄存器，可同时处理 8 个 float
 * 编译时需要添加 -mavx2 或 /arch:AVX2 标志
 * 
 * 注意：本文件中的函数在支持 AVX 时会自动使用，否则回退到 SSE 版本
 */

#include<hgl/math/SIMD.h>

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #if defined(__AVX__) || defined(__AVX2__)
        #define HGL_SIMD_AVX_AVAILABLE
        #include<immintrin.h>  // AVX/AVX2
    #endif
#endif

namespace hgl::math::simd
{
    // AVX 特定的优化实现
    // 注意：由于 Vector3f 不是 8 的倍数，AVX 的优势不如预期
    // 在实际使用中，SSE 版本对 Vector3f 可能已经足够好
    // AVX 更适合处理对齐的 Vector4f 或纯标量数组
    
#ifdef HGL_SIMD_AVX_AVAILABLE
    
    // 目前 AVX 版本主要用于 CPU 特性检测
    // 具体的 AVX 优化函数可以在需要时添加
    // 例如处理 8 个 float 的数组操作等
    
    /**
     * AVX 示例：批量标量加法（8 个 float 一次）
     * 这是演示 AVX 使用的示例函数
     */
    static void AddFloatArrayAVX(float *dst, const float *a, const float *b, size_t count)
    {
        size_t i = 0;
        
        // 每次处理 8 个 float
        for (; i + 8 <= count; i += 8)
        {
            __m256 va = _mm256_loadu_ps(&a[i]);
            __m256 vb = _mm256_loadu_ps(&b[i]);
            __m256 result = _mm256_add_ps(va, vb);
            _mm256_storeu_ps(&dst[i], result);
        }
        
        // 处理剩余元素
        for (; i < count; i++)
        {
            dst[i] = a[i] + b[i];
        }
    }
    
    /**
     * AVX 版本的批量向量点积
     * 使用 AVX 可以同时处理更多数据
     */
    void VectorDot_AVX(float *dst, const Vector3f *a, const Vector3f *b, size_t count)
    {
        // 简化实现：实际上对 Vector3f，AVX 优势不明显
        // 因为 Vector3f 是 12 字节，不是 32 字节的倍数
        // 这里保留为 SSE 实现的占位符
        
        for (size_t i = 0; i < count; i++)
        {
            dst[i] = dot(a[i], b[i]);
        }
    }
    
    /**
     * AVX 版本的批量向量长度计算
     */
    void VectorLength_AVX(float *dst, const Vector3f *src, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            dst[i] = length(src[i]);
        }
    }
    
#else
    
    // 如果没有 AVX 支持，这些函数不会被调用
    // 因为主 SIMD.h 中的函数会回退到 SSE 版本
    
#endif

    // ==================== AVX 特定的辅助函数 ====================
    
    /**
     * 检测并返回最佳 SIMD 实现级别
     * @return 0=标量, 1=SSE, 2=AVX
     */
    int GetBestSIMDLevel() noexcept
    {
#ifdef HGL_SIMD_AVX_AVAILABLE
        if (HasAVX2())
            return 2;
#endif
        if (HasSSE2())
            return 1;
        return 0;
    }
    
}//namespace hgl::math::simd
