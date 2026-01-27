/**
 * SIMD.h - SIMD 优化的批量向量/矩阵操作
 *
 * 提供 SSE/AVX 加速的批量向量和矩阵运算
 * 用于性能关键的场景，如大量对象变换、物理计算等
 *
 * 参考：
 * - Intel Intrinsics Guide: https://software.intel.com/sites/landingpage/IntrinsicsGuide/
 * - Agner Fog's optimization manuals
 * - "Data-Oriented Design" by Richard Fabian
 */

#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/Matrix.h>
#include<hgl/math/geometry/AABB.h>
#include<cstddef>

namespace hgl::math::simd
{
    // ==================== SIMD 功能检测 ====================

    /**
     * 检测 CPU 是否支持 SSE 指令集
     */
    bool HasSSE() noexcept;

    /**
     * 检测 CPU 是否支持 SSE2 指令集
     */
    bool HasSSE2() noexcept;

    /**
     * 检测 CPU 是否支持 AVX 指令集
     */
    bool HasAVX() noexcept;

    /**
     * 检测 CPU 是否支持 AVX2 指令集
     */
    bool HasAVX2() noexcept;

    // ==================== 批量向量运算 (SSE/AVX 优化) ====================

    /**
     * 批量向量加法
     * @param dst 目标数组
     * @param a 输入数组 A
     * @param b 输入数组 B
     * @param count 向量数量
     *
     * 注意：为获得最佳性能，count 应为 4 的倍数，内存应 16 字节对齐
     */
    void VectorAdd_SIMD(Vector3f *dst, const Vector3f *a, const Vector3f *b, size_t count);

    /**
     * 批量向量减法
     */
    void VectorSub_SIMD(Vector3f *dst, const Vector3f *a, const Vector3f *b, size_t count);

    /**
     * 批量向量乘以标量
     */
    void VectorScale_SIMD(Vector3f *dst, const Vector3f *src, float scale, size_t count);

    /**
     * 批量向量点积
     * @param dst 输出标量数组
     * @param a 输入数组 A
     * @param b 输入数组 B
     * @param count 向量数量
     */
    void VectorDot_SIMD(float *dst, const Vector3f *a, const Vector3f *b, size_t count);

    /**
     * 批量向量叉积
     */
    void VectorCross_SIMD(Vector3f *dst, const Vector3f *a, const Vector3f *b, size_t count);

    /**
     * 批量向量归一化
     */
    void VectorNormalize_SIMD(Vector3f *dst, const Vector3f *src, size_t count);

    /**
     * 批量向量长度计算
     */
    void VectorLength_SIMD(float *dst, const Vector3f *src, size_t count);

    // ==================== 批量矩阵变换 ====================

    /**
     * 批量矩阵-向量乘法（变换点）
     * @param dst 输出向量数组
     * @param matrix 变换矩阵
     * @param src 输入向量数组
     * @param count 向量数量
     */
    void TransformPoints_SIMD(Vector3f *dst, const Matrix4f &matrix, const Vector3f *src, size_t count);

    /**
     * 批量矩阵-向量乘法（变换方向，忽略平移）
     */
    void TransformDirections_SIMD(Vector3f *dst, const Matrix4f &matrix, const Vector3f *src, size_t count);

    /**
     * 批量矩阵乘法
     */
    void MatrixMultiply_SIMD(Matrix4f *dst, const Matrix4f *a, const Matrix4f *b, size_t count);

    // ==================== AABB 批量操作 ====================

    /**
     * 批量 AABB 合并
     */
    void MergeAABBs_SIMD(AABB *dst, const AABB *a, const AABB *b, size_t count);

    /**
     * 批量扩展 AABB 以包含点
     */
    void ExpandAABBsByPoints_SIMD(AABB *aabbs, const Vector3f *points, size_t count);

}//namespace hgl::math::simd
