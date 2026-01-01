/**
 * VectorLerp.h - 向量线性插值函数
 * 
 * 为所有向量类型提供统一的 Lerp 插值函数，包括整数向量和浮点向量。
 * 
 * 特点：
 * - 整数向量（Vector2u8, Vector3u8, Vector4u8）插值时自动钳制到有效范围
 * - 浮点向量使用现有的 lerp 函数
 * - 支持颜色插值（RGB 和 RGBA）
 * - 提供标量插值函数
 */

#pragma once

#include <hgl/math/Vector.h>
#include <hgl/math/Clamp.h>
#include <hgl/math/VectorOperations.h>

namespace hgl::math
{
    // ==================== 整数向量的 Lerp ====================
    
    /**
     * @brief Vector2u8 线性插值（带钳制）
     * @param a 起始向量
     * @param b 目标向量
     * @param t 插值参数 [0, 1]
     * @return 插值后的向量
     */
    inline Vector2u8 Lerp(const Vector2u8 &a, const Vector2u8 &b, float t)
    {
        return Vector2u8(
            ClampU8(float(a.x) + (float(b.x) - float(a.x)) * t),
            ClampU8(float(a.y) + (float(b.y) - float(a.y)) * t)
        );
    }
    
    /**
     * @brief Vector3u8 线性插值（带钳制）- RGB 颜色插值
     * @param a 起始向量
     * @param b 目标向量
     * @param t 插值参数 [0, 1]
     * @return 插值后的向量
     */
    inline Vector3u8 Lerp(const Vector3u8 &a, const Vector3u8 &b, float t)
    {
        return Vector3u8(
            ClampU8(float(a.r) + (float(b.r) - float(a.r)) * t),
            ClampU8(float(a.g) + (float(b.g) - float(a.g)) * t),
            ClampU8(float(a.b) + (float(b.b) - float(a.b)) * t)
        );
    }
    
    /**
     * @brief Vector4u8 线性插值（带钳制）- RGBA 颜色插值
     * @param a 起始向量
     * @param b 目标向量
     * @param t 插值参数 [0, 1]
     * @return 插值后的向量
     */
    inline Vector4u8 Lerp(const Vector4u8 &a, const Vector4u8 &b, float t)
    {
        return Vector4u8(
            ClampU8(float(a.r) + (float(b.r) - float(a.r)) * t),
            ClampU8(float(a.g) + (float(b.g) - float(a.g)) * t),
            ClampU8(float(a.b) + (float(b.b) - float(a.b)) * t),
            ClampU8(float(a.a) + (float(b.a) - float(a.a)) * t)
        );
    }
    
    // ==================== 浮点向量的 Lerp ====================
    
    /**
     * @brief Vector2f 线性插值
     * @param a 起始向量
     * @param b 目标向量
     * @param t 插值参数 [0, 1]
     * @return 插值后的向量
     */
    inline Vector2f Lerp(const Vector2f &a, const Vector2f &b, float t)
    {
        return lerp(a, b, t);
    }
    
    /**
     * @brief Vector3f 线性插值
     * @param a 起始向量
     * @param b 目标向量
     * @param t 插值参数 [0, 1]
     * @return 插值后的向量
     */
    inline Vector3f Lerp(const Vector3f &a, const Vector3f &b, float t)
    {
        return lerp(a, b, t);
    }
    
    /**
     * @brief Vector4f 线性插值
     * @param a 起始向量
     * @param b 目标向量
     * @param t 插值参数 [0, 1]
     * @return 插值后的向量
     */
    inline Vector4f Lerp(const Vector4f &a, const Vector4f &b, float t)
    {
        return lerp(a, b, t);
    }
    
    /**
     * @brief Vector2d 线性插值
     * @param a 起始向量
     * @param b 目标向量
     * @param t 插值参数
     * @return 插值后的向量
     */
    inline Vector2d Lerp(const Vector2d &a, const Vector2d &b, double t)
    {
        return lerp(a, b, static_cast<float>(t));
    }
    
    /**
     * @brief Vector3d 线性插值
     * @param a 起始向量
     * @param b 目标向量
     * @param t 插值参数
     * @return 插值后的向量
     */
    inline Vector3d Lerp(const Vector3d &a, const Vector3d &b, double t)
    {
        return lerp(a, b, static_cast<float>(t));
    }
    
    /**
     * @brief Vector4d 线性插值
     * @param a 起始向量
     * @param b 目标向量
     * @param t 插值参数
     * @return 插值后的向量
     */
    inline Vector4d Lerp(const Vector4d &a, const Vector4d &b, double t)
    {
        return lerp(a, b, static_cast<float>(t));
    }
    
    // ==================== 标量 Lerp ====================
    
    /**
     * @brief 标量 uint8 线性插值（带钳制）
     * @param a 起始值
     * @param b 目标值
     * @param t 插值参数 [0, 1]
     * @return 插值后的值
     */
    inline uint8_t Lerp(uint8_t a, uint8_t b, float t)
    {
        return ClampU8(float(a) + (float(b) - float(a)) * t);
    }
    
    /**
     * @brief 标量 float 线性插值
     * @param a 起始值
     * @param b 目标值
     * @param t 插值参数 [0, 1]
     * @return 插值后的值
     */
    inline float Lerp(float a, float b, float t)
    {
        return lerp(a, b, t);
    }
    
    /**
     * @brief 标量 double 线性插值
     * @param a 起始值
     * @param b 目标值
     * @param t 插值参数
     * @return 插值后的值
     */
    inline double Lerp(double a, double b, double t)
    {
        return lerp(a, b, static_cast<float>(t));
    }

} // namespace hgl::math
