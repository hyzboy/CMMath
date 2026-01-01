/**
 * Clamp.h - 数值钳制工具
 * 
 * 提供将数值限制在指定范围内的函数。
 */

#pragma once

#include <algorithm>
#include <cstdint>

namespace hgl::math
{
    /**
     * @brief 将值钳制到 [0, 1] 范围
     * @param value 输入值
     * @return 钳制后的值
     */
    inline float Clamp(float value)
    {
        return std::clamp(value, 0.0f, 1.0f);
    }
    
    /**
     * @brief 将双精度浮点数钳制到 [0, 1] 范围
     */
    inline double Clamp(double value)
    {
        return std::clamp(value, 0.0, 1.0);
    }
    
    /**
     * @brief 将值钳制到指定范围
     * @param value 输入值
     * @param min_val 最小值
     * @param max_val 最大值
     * @return 钳制后的值
     */
    template<typename T>
    inline T Clamp(T value, T min_val, T max_val)
    {
        return std::clamp(value, min_val, max_val);
    }
    
    /**
     * @brief 将浮点数钳制到 [0, 255] 范围并转换为 uint8_t
     */
    inline uint8_t ClampU8(float value)
    {
        if (value <= 0.0f) return 0;
        if (value >= 255.0f) return 255;
        return static_cast<uint8_t>(value + 0.5f);  // 四舍五入
    }
    
    /**
     * @brief 将浮点数钳制到 [0, 65535] 范围并转换为 uint16_t
     */
    inline uint16_t ClampU16(float value)
    {
        if (value <= 0.0f) return 0;
        if (value >= 65535.0f) return 65535;
        return static_cast<uint16_t>(value + 0.5f);  // 四舍五入
    }
    
}//namespace hgl::math

// Include vector types for vector clamp functions
#include <hgl/math/VectorTypes.h>

namespace hgl::math
{
    // ==================== Vector Clamp Functions ====================
    
    /**
     * @brief 将 Vector2u8 的每个分量钳制到 [0, 255] 范围
     * @note Vector2u8 已经在 [0, 255] 范围内，直接返回
     */
    inline Vector2u8 ClampVector(const Vector2u8 &v)
    {
        return v; // Vector2u8 已经在 [0, 255] 范围内
    }

    /**
     * @brief 将 Vector3u8 的每个分量钳制到 [0, 255] 范围
     * @note Vector3u8 已经在 [0, 255] 范围内，直接返回
     */
    inline Vector3u8 ClampVector(const Vector3u8 &v)
    {
        return v; // Vector3u8 已经在 [0, 255] 范围内
    }

    /**
     * @brief 将 Vector4u8 的每个分量钳制到 [0, 255] 范围
     * @note Vector4u8 已经在 [0, 255] 范围内，直接返回
     */
    inline Vector4u8 ClampVector(const Vector4u8 &v)
    {
        return v; // Vector4u8 已经在 [0, 255] 范围内
    }

    /**
     * @brief 将浮点向量的每个分量钳制到 [0, 255] 范围并转换为 uint8
     */
    inline Vector2u8 ClampVectorU8(const Vector2f &v)
    {
        return Vector2u8(ClampU8(v.x), ClampU8(v.y));
    }

    /**
     * @brief 将浮点向量的每个分量钳制到 [0, 255] 范围并转换为 uint8
     */
    inline Vector3u8 ClampVectorU8(const Vector3f &v)
    {
        return Vector3u8(ClampU8(v.r), ClampU8(v.g), ClampU8(v.b));
    }

    /**
     * @brief 将浮点向量的每个分量钳制到 [0, 255] 范围并转换为 uint8
     */
    inline Vector4u8 ClampVectorU8(const Vector4f &v)
    {
        return Vector4u8(ClampU8(v.r), ClampU8(v.g), ClampU8(v.b), ClampU8(v.a));
    }
    
}//namespace hgl::math
