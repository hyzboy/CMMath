/**
 * FloatPrecision.h - 浮点数精度比较工具
 * 
 * 提供浮点数精度比较函数，用于处理浮点运算的精度问题。
 */

#pragma once

#include <cmath>
#include <limits>

namespace hgl::math
{
    /**
     * 浮点数误差阈值
     * 用于浮点数近似比较
     */
    constexpr float float_error = 1e-6f;
    constexpr double double_error = 1e-10;
    
    /**
     * @brief 检查浮点数是否近似相等
     * @param a 第一个浮点数
     * @param b 第二个浮点数
     * @param epsilon 误差阈值，默认为 float_error
     * @return 如果两个数在误差范围内相等，返回 true
     */
    inline bool IsNearlyEqual(float a, float b, float epsilon = float_error)
    {
        return std::abs(a - b) <= epsilon;
    }
    
    /**
     * @brief 检查双精度浮点数是否近似相等
     */
    inline bool IsNearlyEqual(double a, double b, double epsilon = double_error)
    {
        return std::abs(a - b) <= epsilon;
    }
    
    /**
     * @brief 检查浮点数是否近似为零
     * @param value 待检查的浮点数
     * @param epsilon 误差阈值，默认为 float_error
     * @return 如果值在零的误差范围内，返回 true
     */
    inline bool IsNearlyZero(float value, float epsilon = float_error)
    {
        return std::abs(value) <= epsilon;
    }
    
    /**
     * @brief 检查双精度浮点数是否近似为零
     */
    inline bool IsNearlyZero(double value, double epsilon = double_error)
    {
        return std::abs(value) <= epsilon;
    }
    
}//namespace hgl::math
