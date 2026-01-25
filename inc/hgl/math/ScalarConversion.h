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
#include<hgl/math/VectorTypes.h>

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
    /// @{

    /**
     * @brief Convert Vector3u8 [0-255] to Vector3f [0.0-1.0]
     * @param v Input vector with uint8 components (RGB)
     * @return Output vector with float components (normalized)
     * @category Normalization
     */
    inline Vector3f Vector3u8ToFloat(const Vector3u8 &v)
    {
        return Vector3f(
            ByteToFloat(v.r),
            ByteToFloat(v.g),
            ByteToFloat(v.b)
        );
    }

    /**
     * @brief Convert Vector3f [0.0-1.0] to Vector3u8 [0-255]
     * @param v Input vector with float components (normalized)
     * @return Output vector with uint8 components (clamped)
     * @category Denormalization
     */
    inline Vector3u8 Vector3fToByte(const Vector3f &v)
    {
        Vector3u8 result;
        result.r = FloatToByte(v.r);
        result.g = FloatToByte(v.g);
        result.b = FloatToByte(v.b);
        return result;
    }
    /// @}

    // ==================== Vector4 (RGBA) Conversion ====================
    /// @defgroup vector4_conversion Vector4 RGBA Color Conversion
    /// Conversion operations for 4-component color vectors (Red, Green, Blue, Alpha)
    /// @{

    /**
     * @brief Convert Vector4u8 [0-255] to Vector4f [0.0-1.0]
     * @param v Input vector with uint8 components (RGBA)
     * @return Output vector with float components (normalized)
     * @category Normalization
     */
    inline Vector4f Vector4u8ToFloat(const Vector4u8 &v)
    {
        return Vector4f(
            ByteToFloat(v.r),
            ByteToFloat(v.g),
            ByteToFloat(v.b),
            ByteToFloat(v.a)
        );
    }

    /**
     * @brief Convert Vector4f [0.0-1.0] to Vector4u8 [0-255]
     * @param v Input vector with float components (normalized)
     * @return Output vector with uint8 components (clamped)
     * @category Denormalization
     */
    inline Vector4u8 Vector4fToByte(const Vector4f &v)
    {
        Vector4u8 result;
        result.r = FloatToByte(v.r);
        result.g = FloatToByte(v.g);
        result.b = FloatToByte(v.b);
        result.a = FloatToByte(v.a);
        return result;
    }
    /// @}
}
