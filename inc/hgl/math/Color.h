/**
 * Color.h - 颜色空间与颜色操作
 * 
 * 实现颜色空间转换和颜色操作，用于图形渲染和游戏开发。
 * 
 * 包含：
 * - 颜色类型定义（Color3f, Color4f）
 * - 预定义颜色常量
 * - 颜色空间转换（RGB-HSV, RGB-HSL, sRGB-Linear）
 * - 颜色插值和调整
 * - Tone Mapping（HDR 到 LDR）
 */

#pragma once

#include <hgl/math/VectorTypes.h>
#include <hgl/math/VectorOperations.h>
#include <hgl/math/VectorLerp.h>
#include <algorithm>
#include <cmath>

namespace hgl::math
{
    // ==================== 颜色类型定义 ====================
    
    /**
     * RGB 颜色（不含 Alpha）
     */
    using Color3f = Vector3f;
    
    /**
     * RGBA 颜色（含 Alpha）
     */
    using Color4f = Vector4f;
    
    // ==================== 预定义颜色常量 ====================
    
    namespace Colors
    {
        // 基础颜色
        const Color3f Red       (1.0f, 0.0f, 0.0f);
        const Color3f Green     (0.0f, 1.0f, 0.0f);
        const Color3f Blue      (0.0f, 0.0f, 1.0f);
        const Color3f White     (1.0f, 1.0f, 1.0f);
        const Color3f Black     (0.0f, 0.0f, 0.0f);
        const Color3f Yellow    (1.0f, 1.0f, 0.0f);
        const Color3f Cyan      (0.0f, 1.0f, 1.0f);
        const Color3f Magenta   (1.0f, 0.0f, 1.0f);
        
        // 灰度
        const Color3f Gray      (0.5f, 0.5f, 0.5f);
        const Color3f DarkGray  (0.25f, 0.25f, 0.25f);
        const Color3f LightGray (0.75f, 0.75f, 0.75f);
        
        // 常用颜色
        const Color3f Orange    (1.0f, 0.5f, 0.0f);
        const Color3f Purple    (0.5f, 0.0f, 0.5f);
        const Color3f Brown     (0.6f, 0.4f, 0.2f);
        const Color3f Pink      (1.0f, 0.75f, 0.8f);
        
        // 透明颜色（RGBA）
        const Color4f Transparent(0.0f, 0.0f, 0.0f, 0.0f);
    }
    
    // ==================== 颜色空间转换 ====================
    
    /**
     * @brief RGB 转 HSV
     * @param rgb RGB 颜色 [0, 1]
     * @return HSV 颜色，H ∈ [0, 360), S ∈ [0, 1], V ∈ [0, 1]
     */
    Vector3f RGBtoHSV(const Color3f &rgb);
    
    /**
     * @brief HSV 转 RGB
     * @param hsv HSV 颜色，H ∈ [0, 360), S ∈ [0, 1], V ∈ [0, 1]
     * @return RGB 颜色 [0, 1]
     */
    Color3f HSVtoRGB(const Vector3f &hsv);
    
    /**
     * @brief RGB 转 HSL
     * @param rgb RGB 颜色 [0, 1]
     * @return HSL 颜色，H ∈ [0, 360), S ∈ [0, 1], L ∈ [0, 1]
     */
    Vector3f RGBtoHSL(const Color3f &rgb);
    
    /**
     * @brief HSL 转 RGB
     * @param hsl HSL 颜色，H ∈ [0, 360), S ∈ [0, 1], L ∈ [0, 1]
     * @return RGB 颜色 [0, 1]
     */
    Color3f HSLtoRGB(const Vector3f &hsl);
    
    // ==================== Gamma 校正 ====================
    
    /**
     * @brief sRGB 转线性空间
     * @param srgb sRGB 颜色
     * @return 线性空间颜色
     */
    Color3f SRGBtoLinear(const Color3f &srgb);
    
    /**
     * @brief 线性空间转 sRGB
     * @param linear 线性空间颜色
     * @return sRGB 颜色
     */
    Color3f LinearToSRGB(const Color3f &linear);
    
    /**
     * @brief sRGB 转线性空间（RGBA 版本）
     * Alpha 通道不受影响
     */
    Color4f SRGBtoLinear(const Color4f &srgb);
    
    /**
     * @brief 线性空间转 sRGB（RGBA 版本）
     * Alpha 通道不受影响
     */
    Color4f LinearToSRGB(const Color4f &linear);
    
    // ==================== 颜色插值 ====================
    
    /**
     * @brief RGB 空间线性插值
     * @param c1 起始颜色
     * @param c2 目标颜色
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Color3f ColorLerp(const Color3f &c1, const Color3f &c2, float t)
    {
        return c1 + (c2 - c1) * t;
    }
    
    /**
     * @brief RGBA 空间线性插值
     */
    inline Color4f ColorLerp(const Color4f &c1, const Color4f &c2, float t)
    {
        return c1 + (c2 - c1) * t;
    }
    
    /**
     * @brief HSV 空间插值（产生更自然的过渡）
     * @param c1 起始颜色（RGB）
     * @param c2 目标颜色（RGB）
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色（RGB）
     */
    Color3f ColorLerpHSV(const Color3f &c1, const Color3f &c2, float t);
    
    // ==================== 颜色调整 ====================
    
    /**
     * @brief 去饱和度（向灰度转换）
     * @param color 输入颜色
     * @param amount 去饱和量 [0, 1]，0=原色，1=完全灰度
     * @return 调整后的颜色
     */
    Color3f Desaturate(const Color3f &color, float amount);
    
    /**
     * @brief 调整亮度
     * @param color 输入颜色
     * @param factor 亮度因子，1.0=不变，>1.0=更亮，<1.0=更暗
     * @return 调整后的颜色
     */
    inline Color3f AdjustBrightness(const Color3f &color, float factor)
    {
        return color * factor;
    }
    
    /**
     * @brief 调整对比度
     * @param color 输入颜色
     * @param factor 对比度因子，1.0=不变，>1.0=更高对比度，<1.0=更低对比度
     * @return 调整后的颜色
     */
    Color3f AdjustContrast(const Color3f &color, float factor);
    
    /**
     * @brief 调整色相
     * @param color 输入颜色
     * @param shift 色相偏移（度数），范围 [-180, 180]
     * @return 调整后的颜色
     */
    Color3f AdjustHue(const Color3f &color, float shift);
    
    /**
     * @brief 调整饱和度
     * @param color 输入颜色
     * @param factor 饱和度因子，1.0=不变，>1.0=更饱和，<1.0=更不饱和，0=灰度
     * @return 调整后的颜色
     */
    Color3f AdjustSaturation(const Color3f &color, float factor);
    
    // ==================== Tone Mapping ====================
    
    /**
     * @brief Reinhard Tone Mapping
     * 简单的 HDR 到 LDR 映射
     * @param hdr HDR 颜色
     * @return LDR 颜色 [0, 1]
     */
    inline Color3f ToneMapReinhard(const Color3f &hdr)
    {
        return hdr / (Color3f(1.0f) + hdr);
    }
    
    /**
     * @brief Extended Reinhard Tone Mapping
     * 带白点参数的 Reinhard
     * @param hdr HDR 颜色
     * @param white_point 白点亮度
     */
    Color3f ToneMapReinhardExtended(const Color3f &hdr, float white_point);
    
    /**
     * @brief ACES Filmic Tone Mapping
     * 模拟胶片效果的色调映射
     * @param hdr HDR 颜色
     * @return LDR 颜色 [0, 1]
     */
    Color3f ToneMapACES(const Color3f &hdr);
    
    /**
     * @brief Uncharted 2 Tone Mapping
     * 《神秘海域 2》使用的色调映射
     * @param hdr HDR 颜色
     * @return LDR 颜色 [0, 1]
     */
    Color3f ToneMapUncharted2(const Color3f &hdr);
    
    // ==================== 辅助函数 ====================
    
    /**
     * @brief 计算颜色的亮度（luminance）
     * 使用感知亮度权重
     * @param color RGB 颜色
     * @return 亮度值
     */
    inline float GetLuminance(const Color3f &color)
    {
        // Rec. 709 luma coefficients
        return 0.2126f * color.r + 0.7152f * color.g + 0.0722f * color.b;
    }
    
    /**
     * @brief 钳制颜色到 [0, 1] 范围
     */
    inline Color3f ClampColor(const Color3f &color)
    {
        return glm::clamp(color, Color3f(0.0f), Color3f(1.0f));
    }
    
    /**
     * @brief 钳制 RGBA 颜色到 [0, 1] 范围
     */
    inline Color4f ClampColor(const Color4f &color)
    {
        return glm::clamp(color, Color4f(0.0f), Color4f(1.0f));
    }
    
    /**
     * @brief 从 0-255 整数创建颜色
     */
    inline Color3f ColorFromBytes(uint8_t r, uint8_t g, uint8_t b)
    {
        return Color3f(r / 255.0f, g / 255.0f, b / 255.0f);
    }
    
    /**
     * @brief 从 0-255 整数创建 RGBA 颜色
     */
    inline Color4f ColorFromBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        return Color4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }
    
    // ==================== uint8 颜色插值 ====================
    
    /**
     * @brief uint8 RGB 颜色插值
     * @param c1 起始颜色
     * @param c2 目标颜色
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Vector3u8 ColorLerpU8(const Vector3u8 &c1, const Vector3u8 &c2, float t)
    {
        return Lerp(c1, c2, t);
    }

    /**
     * @brief uint8 RGBA 颜色插值
     * @param c1 起始颜色
     * @param c2 目标颜色
     * @param t 插值参数 [0, 1]
     * @return 插值后的颜色
     */
    inline Vector4u8 ColorLerpU8(const Vector4u8 &c1, const Vector4u8 &c2, float t)
    {
        return Lerp(c1, c2, t);
    }
    
}//namespace hgl::math
