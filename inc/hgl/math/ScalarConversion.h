#pragma once

/**
 * @file ScalarConversion.h
 * @brief uint8 与 float 范围的标量与向量值转换工具
 *
 * 提供标量字节与浮点数转换、颜色向量在 uint8 区间 [0-255] 与 float 区间 [0.0-1.0] 归一化表示之间的转换函数。
 *
 * @section 分类
 * - @ref scalar_conversion: 标量字节↔浮点数转换（带钳制）
 * - @ref vector3_conversion: Vector3 颜色转换（RGB）
 * - @ref vector4_conversion: Vector4 颜色转换（RGBA）
 * - @ref compatibility: 跨模块兼容的别名函数
 */

#include<hgl/math/Clamp.h>
#include<hgl/color/Color3f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/Color4f.h>
#include<hgl/color/Color4ub.h>
#include<hgl/math/VectorTypes.h>
#include<type_traits>

namespace hgl::math
{
    // ==================== 标量转换 ====================
    /// @defgroup scalar_conversion 标量字节↔浮点数转换
    /// 字节与浮点类型转换的基础操作
    /// @{

    /**
     * @brief 将 uint8 [0,255] 转换为 float [0,1]
     * @param b 输入字节值 [0-255]
     * @return 归一化浮点值 [0.0-1.0]
     * @category 归一化
     */
    inline float ByteToFloat(uint8 b)
    {
        return b / 255.0f;
    }

    /**
     * @brief 将 float [0,1] 转换为 uint8 [0,255]，带钳制
     * @param f 输入浮点值 [0.0-1.0]
     * @return 钳制后的字节值 [0-255]
     * @category 反归一化
     */
    inline uint8 FloatToByte(float f)
    {
        return ClampU8(f * 255.0f);
    }
    /// @}

    // ==================== Vector3 (RGB) 转换 ====================
    /// @defgroup vector3_conversion Vector3 RGB 颜色转换
    /// 三分量颜色向量（红、绿、蓝）的转换操作
    /// 输入支持 Vector3u8 和 Color3ub，输出支持 Vector3f 和 Color3f
    /// @{

    inline Color3f ToColorFloat(const Color3ub &v)
    {
        return Color3f(ByteToFloat(v.r),ByteToFloat(v.g),ByteToFloat(v.b));
    }

    inline Color4f ToColorFloat(const Color4ub &v)
    {
        return Color4f(ByteToFloat(v.r),ByteToFloat(v.g),ByteToFloat(v.b),ByteToFloat(v.a));
    }

    inline Color3ub ToColorByte(const Color3f &v)
    {
        return Color3ub(FloatToByte(v.r),FloatToByte(v.g),FloatToByte(v.b));
    }

    inline Color4ub ToColorByte(const Color4f &v)
    {
        return Color4ub(FloatToByte(v.r),FloatToByte(v.g),FloatToByte(v.b),FloatToByte(v.a));
    }
}
