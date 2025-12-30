/**
 * Easing.h - 缓动函数库
 * 
 * 实现常见的缓动函数用于游戏动画、UI过渡、相机移动等。
 * 基于 Robert Penner's easing functions 标准实现。
 * 
 * 参考：https://easings.net/
 * 
 * 包含：
 * - 线性：Linear
 * - 二次方：EaseInQuad, EaseOutQuad, EaseInOutQuad
 * - 三次方：EaseInCubic, EaseOutCubic, EaseInOutCubic
 * - 四次方：EaseInQuart, EaseOutQuart, EaseInOutQuart
 * - 五次方：EaseInQuint, EaseOutQuint, EaseInOutQuint
 * - 正弦：EaseInSine, EaseOutSine, EaseInOutSine
 * - 指数：EaseInExpo, EaseOutExpo, EaseInOutExpo
 * - 圆形：EaseInCirc, EaseOutCirc, EaseInOutCirc
 * - 弹性：EaseInElastic, EaseOutElastic, EaseInOutElastic
 * - 回弹：EaseInBack, EaseOutBack, EaseInOutBack
 * - 弹跳：EaseInBounce, EaseOutBounce, EaseInOutBounce
 */

#pragma once

#include <hgl/math/MathConstants.h>
#include <cmath>

namespace hgl::math
{
    // ==================== 缓动函数类型定义 ====================
    
    /**
     * 缓动函数指针类型
     * @param t 插值参数，范围 [0.0, 1.0]
     * @return 缓动后的值
     */
    using EasingFunction = float(*)(float);
    
    // ==================== 线性缓动 ====================
    
    /**
     * @brief 线性缓动（无缓动）
     * @param t 插值参数 [0.0, 1.0]
     * @return 缓动值，等于 t
     */
    constexpr inline float EaseLinear(float t)
    {
        return t;
    }
    
    // ==================== 二次方缓动 (Quadratic) ====================
    
    /**
     * @brief 二次方缓入 (t^2)
     * 开始缓慢，然后加速
     */
    constexpr inline float EaseInQuad(float t)
    {
        return t * t;
    }
    
    /**
     * @brief 二次方缓出 (1 - (1-t)^2)
     * 开始快速，然后减速
     */
    constexpr inline float EaseOutQuad(float t)
    {
        return 1.0f - (1.0f - t) * (1.0f - t);
    }
    
    /**
     * @brief 二次方缓入缓出
     * 开始和结束都缓慢，中间快速
     */
    constexpr inline float EaseInOutQuad(float t)
    {
        return t < 0.5f ? 2.0f * t * t : 1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) / 2.0f;
    }
    
    // ==================== 三次方缓动 (Cubic) ====================
    
    /**
     * @brief 三次方缓入 (t^3)
     */
    constexpr inline float EaseInCubic(float t)
    {
        return t * t * t;
    }
    
    /**
     * @brief 三次方缓出
     */
    constexpr inline float EaseOutCubic(float t)
    {
        float f = 1.0f - t;
        return 1.0f - f * f * f;
    }
    
    /**
     * @brief 三次方缓入缓出
     */
    constexpr inline float EaseInOutCubic(float t)
    {
        return t < 0.5f ? 4.0f * t * t * t : 1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) / 2.0f;
    }
    
    // ==================== 四次方缓动 (Quartic) ====================
    
    /**
     * @brief 四次方缓入 (t^4)
     */
    constexpr inline float EaseInQuart(float t)
    {
        return t * t * t * t;
    }
    
    /**
     * @brief 四次方缓出
     */
    constexpr inline float EaseOutQuart(float t)
    {
        float f = 1.0f - t;
        return 1.0f - f * f * f * f;
    }
    
    /**
     * @brief 四次方缓入缓出
     */
    constexpr inline float EaseInOutQuart(float t)
    {
        return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) / 2.0f;
    }
    
    // ==================== 五次方缓动 (Quintic) ====================
    
    /**
     * @brief 五次方缓入 (t^5)
     */
    constexpr inline float EaseInQuint(float t)
    {
        return t * t * t * t * t;
    }
    
    /**
     * @brief 五次方缓出
     */
    constexpr inline float EaseOutQuint(float t)
    {
        float f = 1.0f - t;
        return 1.0f - f * f * f * f * f;
    }
    
    /**
     * @brief 五次方缓入缓出
     */
    constexpr inline float EaseInOutQuint(float t)
    {
        return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) / 2.0f;
    }
    
    // ==================== 正弦缓动 (Sine) ====================
    
    /**
     * @brief 正弦缓入
     * 使用正弦曲线进行缓动
     */
    inline float EaseInSine(float t)
    {
        return 1.0f - std::cos(t * pi_2);
    }
    
    /**
     * @brief 正弦缓出
     */
    inline float EaseOutSine(float t)
    {
        return std::sin(t * pi_2);
    }
    
    /**
     * @brief 正弦缓入缓出
     */
    inline float EaseInOutSine(float t)
    {
        return -(std::cos(pi * t) - 1.0f) / 2.0f;
    }
    
    // ==================== 指数缓动 (Exponential) ====================
    
    /**
     * @brief 指数缓入
     * 使用 2^(10t-10) 指数曲线
     */
    inline float EaseInExpo(float t)
    {
        return t == 0.0f ? 0.0f : std::pow(2.0f, 10.0f * t - 10.0f);
    }
    
    /**
     * @brief 指数缓出
     */
    inline float EaseOutExpo(float t)
    {
        return t == 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
    }
    
    /**
     * @brief 指数缓入缓出
     */
    inline float EaseInOutExpo(float t)
    {
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        return t < 0.5f ? std::pow(2.0f, 20.0f * t - 10.0f) / 2.0f 
                        : (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
    }
    
    // ==================== 圆形缓动 (Circular) ====================
    
    /**
     * @brief 圆形缓入
     * 使用圆形曲线 (1 - sqrt(1 - t^2))
     */
    inline float EaseInCirc(float t)
    {
        return 1.0f - std::sqrt(1.0f - t * t);
    }
    
    /**
     * @brief 圆形缓出
     */
    inline float EaseOutCirc(float t)
    {
        float f = t - 1.0f;
        return std::sqrt(1.0f - f * f);
    }
    
    /**
     * @brief 圆形缓入缓出
     */
    inline float EaseInOutCirc(float t)
    {
        return t < 0.5f 
            ? (1.0f - std::sqrt(1.0f - 4.0f * t * t)) / 2.0f
            : (std::sqrt(1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f)) + 1.0f) / 2.0f;
    }
    
    // ==================== 弹性缓动 (Elastic) ====================
    
    /**
     * @brief 弹性缓入
     * 产生类似弹簧被压缩后释放的效果
     */
    inline float EaseInElastic(float t)
    {
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        
        constexpr float c4 = (2.0f * pi) / 3.0f;
        return -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
    }
    
    /**
     * @brief 弹性缓出
     * 产生类似弹簧振荡后停止的效果
     */
    inline float EaseOutElastic(float t)
    {
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        
        constexpr float c4 = (2.0f * pi) / 3.0f;
        return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
    }
    
    /**
     * @brief 弹性缓入缓出
     */
    inline float EaseInOutElastic(float t)
    {
        if (t == 0.0f) return 0.0f;
        if (t == 1.0f) return 1.0f;
        
        constexpr float c5 = (2.0f * pi) / 4.5f;
        
        return t < 0.5f
            ? -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f
            : (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
    }
    
    // ==================== 回弹缓动 (Back) ====================
    
    /**
     * @brief 回弹缓入
     * 稍微后退然后前进，产生"蓄力"效果
     */
    constexpr inline float EaseInBack(float t)
    {
        constexpr float c1 = 1.70158f;
        constexpr float c3 = c1 + 1.0f;
        return c3 * t * t * t - c1 * t * t;
    }
    
    /**
     * @brief 回弹缓出
     * 超过目标然后回弹
     */
    constexpr inline float EaseOutBack(float t)
    {
        constexpr float c1 = 1.70158f;
        constexpr float c3 = c1 + 1.0f;
        float f = t - 1.0f;
        return 1.0f + c3 * f * f * f + c1 * f * f;
    }
    
    /**
     * @brief 回弹缓入缓出
     */
    constexpr inline float EaseInOutBack(float t)
    {
        constexpr float c1 = 1.70158f;
        constexpr float c2 = c1 * 1.525f;
        
        return t < 0.5f
            ? ((2.0f * t) * (2.0f * t) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
            : ((2.0f * t - 2.0f) * (2.0f * t - 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
    }
    
    // ==================== 弹跳缓动 (Bounce) ====================
    
    /**
     * @brief 弹跳缓出
     * 产生类似球落地弹跳的效果
     */
    constexpr inline float EaseOutBounce(float t)
    {
        constexpr float n1 = 7.5625f;
        constexpr float d1 = 2.75f;
        
        if (t < 1.0f / d1)
        {
            return n1 * t * t;
        }
        else if (t < 2.0f / d1)
        {
            t -= 1.5f / d1;
            return n1 * t * t + 0.75f;
        }
        else if (t < 2.5f / d1)
        {
            t -= 2.25f / d1;
            return n1 * t * t + 0.9375f;
        }
        else
        {
            t -= 2.625f / d1;
            return n1 * t * t + 0.984375f;
        }
    }
    
    /**
     * @brief 弹跳缓入
     */
    constexpr inline float EaseInBounce(float t)
    {
        return 1.0f - EaseOutBounce(1.0f - t);
    }
    
    /**
     * @brief 弹跳缓入缓出
     */
    constexpr inline float EaseInOutBounce(float t)
    {
        return t < 0.5f
            ? (1.0f - EaseOutBounce(1.0f - 2.0f * t)) / 2.0f
            : (1.0f + EaseOutBounce(2.0f * t - 1.0f)) / 2.0f;
    }
    
    // ==================== 通用插值函数 ====================
    
    /**
     * @brief 使用缓动函数进行插值
     * @tparam T 插值的数据类型（必须支持 +、-、* 运算符）
     * @param from 起始值
     * @param to 目标值
     * @param t 插值参数 [0.0, 1.0]
     * @param easing 缓动函数
     * @return 插值结果
     */
    template<typename T>
    inline T EaseLerp(const T &from, const T &to, float t, EasingFunction easing)
    {
        float eased_t = easing(t);
        return from + (to - from) * eased_t;
    }
    
    /**
     * @brief 使用缓动函数进行浮点数插值
     */
    inline float EaseLerp(float from, float to, float t, EasingFunction easing)
    {
        float eased_t = easing(t);
        return from + (to - from) * eased_t;
    }
    
}//namespace hgl::math
