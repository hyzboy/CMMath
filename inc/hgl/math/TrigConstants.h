/**
 * TrigConstants.h - 三角函数常量与角度转换
 * 
 * 包含：
 * - 常用角度的 sin/cos 预定义值 (C++20 constexpr)
 * - 常用角度的弧度值
 * - 类型安全的角度与弧度转换函数
 * - 支持编译期计算
 */

#pragma once

#include<hgl/math/MathConstants.h>
#include<numbers>
#include<concepts>

namespace hgl::math
{    
    // ==================== 常用三角函数值（C++20 constexpr） ====================
    
    // √2/2 的精确值
    inline constexpr double sqrt2_over_2 = std::numbers::sqrt2_v<double> / 2.0;
    inline constexpr float sqrt2_over_2_f = std::numbers::sqrt2_v<float> / 2.0f;
    
    // 余弦值
    namespace cos_values {
        inline constexpr double deg_0   = 1.0;
        inline constexpr double deg_45  = sqrt2_over_2;
        inline constexpr double deg_90  = 0.0;
        inline constexpr double deg_135 = -sqrt2_over_2;
        inline constexpr double deg_180 = -1.0;
        inline constexpr double deg_225 = -sqrt2_over_2;
        inline constexpr double deg_270 = 0.0;
        inline constexpr double deg_315 = sqrt2_over_2;
    }
    
    // 正弦值
    namespace sin_values {
        inline constexpr double deg_0   = 0.0;
        inline constexpr double deg_45  = sqrt2_over_2;
        inline constexpr double deg_90  = 1.0;
        inline constexpr double deg_135 = sqrt2_over_2;
        inline constexpr double deg_180 = 0.0;
        inline constexpr double deg_225 = -sqrt2_over_2;
        inline constexpr double deg_270 = -1.0;
        inline constexpr double deg_315 = -sqrt2_over_2;
    }
    
    // ==================== 常用角度的弧度值 ====================
    
    namespace radians {
        inline constexpr double deg_0   = 0.0;
        inline constexpr double deg_45  = pi / 4.0;
        inline constexpr double deg_90  = pi / 2.0;
        inline constexpr double deg_135 = 3.0 * pi / 4.0;
        inline constexpr double deg_180 = pi;
        inline constexpr double deg_225 = 5.0 * pi / 4.0;
        inline constexpr double deg_270 = 3.0 * pi / 2.0;
        inline constexpr double deg_315 = 7.0 * pi / 4.0;
        inline constexpr double deg_360 = 2.0 * pi;
        
        // float 版本
        inline constexpr float deg_0_f   = 0.0f;
        inline constexpr float deg_45_f  = pi_f / 4.0f;
        inline constexpr float deg_90_f  = pi_f / 2.0f;
        inline constexpr float deg_135_f = 3.0f * pi_f / 4.0f;
        inline constexpr float deg_180_f = pi_f;
        inline constexpr float deg_225_f = 5.0f * pi_f / 4.0f;
        inline constexpr float deg_270_f = 3.0f * pi_f / 2.0f;
        inline constexpr float deg_315_f = 7.0f * pi_f / 4.0f;
        inline constexpr float deg_360_f = 2.0f * pi_f;
    }
    
    // ==================== 角度与弧度转换（C++20 Concepts） ====================
    
    /**
     * 角度转弧度（泛型版本，支持所有浮点类型）
     * @param degrees 角度值
     * @return 对应的弧度值
     */
    template<std::floating_point T>
    [[nodiscard]] constexpr T degrees_to_radians(T degrees) noexcept
    {
        return degrees * (std::numbers::pi_v<T> / T(180));
    }
    
    /**
     * 弧度转角度（泛型版本，支持所有浮点类型）
     * @param radians 弧度值
     * @return 对应的角度值
     */
    template<std::floating_point T>
    [[nodiscard]] constexpr T radians_to_degrees(T radians) noexcept
    {
        return radians * (T(180) / std::numbers::pi_v<T>);
    }
    
    // ==================== 向后兼容的别名 ====================
    
    /**
     * 角度转弧度（向后兼容）
     * @deprecated 建议使用 degrees_to_radians
     */
    [[nodiscard]] constexpr float deg2rad(float deg) noexcept
    {
        return degrees_to_radians(deg);
    }
    
    /**
     * 弧度转角度（向后兼容）
     * @deprecated 建议使用 radians_to_degrees
     */
    [[nodiscard]] constexpr float rad2deg(float rad) noexcept
    {
        return radians_to_degrees(rad);
    }
    
    // ==================== 旧常量别名（向后兼容） ====================
    
    inline constexpr double HGL_COS_0   = cos_values::deg_0;
    inline constexpr double HGL_COS_45  = cos_values::deg_45;
    inline constexpr double HGL_COS_90  = cos_values::deg_90;
    inline constexpr double HGL_COS_135 = cos_values::deg_135;
    inline constexpr double HGL_COS_180 = cos_values::deg_180;
    inline constexpr double HGL_COS_225 = cos_values::deg_225;
    inline constexpr double HGL_COS_270 = cos_values::deg_270;
    inline constexpr double HGL_COS_315 = cos_values::deg_315;
    
    inline constexpr double HGL_SIN_0   = sin_values::deg_0;
    inline constexpr double HGL_SIN_45  = sin_values::deg_45;
    inline constexpr double HGL_SIN_90  = sin_values::deg_90;
    inline constexpr double HGL_SIN_135 = sin_values::deg_135;
    inline constexpr double HGL_SIN_180 = sin_values::deg_180;
    inline constexpr double HGL_SIN_225 = sin_values::deg_225;
    inline constexpr double HGL_SIN_270 = sin_values::deg_270;
    inline constexpr double HGL_SIN_315 = sin_values::deg_315;
    
    inline constexpr double HGL_RAD_0   = radians::deg_0;
    inline constexpr double HGL_RAD_45  = radians::deg_45;
    inline constexpr double HGL_RAD_90  = radians::deg_90;
    inline constexpr double HGL_RAD_135 = radians::deg_135;
    inline constexpr double HGL_RAD_180 = radians::deg_180;
    inline constexpr double HGL_RAD_225 = radians::deg_225;
    inline constexpr double HGL_RAD_270 = radians::deg_270;
    inline constexpr double HGL_RAD_315 = radians::deg_315;
    inline constexpr double HGL_RAD_360 = radians::deg_360;
    
}//namespace hgl::math
