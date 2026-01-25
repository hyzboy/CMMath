#pragma once

/**
 * @file ScalarConversion.h
 * @brief Scalar and vector value conversion utilities between uint8 and float ranges
 * 
 * Provides conversion functions for scalar byte-float conversions and color vectors
 * between uint8 range [0-255] and float range [0.0-1.0] normalized representations.
 * 
 * @section Categories
 * - @ref scalar_conversion: Scalar byte↔float conversions with clamping
 * - @ref vector3_conversion: Vector3 color conversions (RGB)
 * - @ref vector4_conversion: Vector4 color conversions (RGBA)
 * - @ref compatibility: Alias functions for cross-module compatibility
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
    // ==================== Scalar Conversion ====================
    /// @defgroup scalar_conversion Scalar Byte↔Float Conversion
    /// Fundamental conversion operations for byte-float type conversions
    /// @{

    /**
     * @brief Convert uint8 [0,255] to float [0,1]
     * @param b Input byte value [0-255]
     * @return Normalized float value [0.0-1.0]
     * @category Normalization
     */
    inline float ByteToFloat(uint8 b)
    {
        return b / 255.0f;
    }

    /**
     * @brief Convert float [0,1] to uint8 [0,255] with clamping
     * @param f Input float value [0.0-1.0]
     * @return Clamped byte value [0-255]
     * @category Denormalization
     */
    inline uint8 FloatToByte(float f)
    {
        return ClampU8(f * 255.0f);
    }
    /// @}

    // ==================== Vector3 (RGB) Conversion ====================
    /// @defgroup vector3_conversion Vector3 RGB Color Conversion
    /// Conversion operations for 3-component color vectors (Red, Green, Blue)
    /// Supports both Vector3u8 and Color3ub for input, and Vector3f and Color3f for output
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
