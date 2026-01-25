#pragma once

#include<hgl/platform/Platform.h>
#include<hgl/type/EnumUtil.h>

namespace hgl::math
{
    /**
     * @brief 颜色合成的 Alpha 混合模式
     *
     * 这些模式定义了源色和目标色如何组合。
     * 该枚举作为 CMCoreType 和 CMMath 库中 Alpha 混合模式的权威定义。
     */
    enum class AlphaBlendMode
    {
        None = 0,           ///< 无混合，直接使用源颜色
        Normal,             ///< 标准 alpha 混合（src 覆盖 dst）
        Add,                ///< 相加混合（src + dst）
        Subtract,           ///< 相减混合（dst - src）
        Multiply,           ///< 正片叠底（src * dst）
        Screen,             ///< 滤色（1 - (1-src)*(1-dst)）
        Overlay,            ///< 叠加（结合 Multiply 与 Screen）
        Darken,             ///< 取较暗颜色（最小值）
        Lighten,            ///< 取较亮颜色（最大值）
        ColorDodge,         ///< 颜色减淡（变亮）
        ColorBurn,          ///< 颜色加深（变暗）
        HardLight,          ///< 强光混合
        SoftLight,          ///< 柔光混合
        Difference,         ///< 差值混合（abs(dst - src)）
        Exclusion,          ///< 排除混合（类似差值）
        LinearBurn,         ///< 线性加深：dst + src - 1
        LinearDodge,        ///< 线性减淡：dst + src
        VividLight,         ///< 亮光混合
        LinearLight,        ///< 线性光混合
        PinLight,           ///< 点光混合
        HardMix,            ///< 强混合（阈值化为 0 或 1）
        DarkerColor,        ///< 选择亮度更低的颜色
        LighterColor,       ///< 选择亮度更高的颜色
        Hue,                ///< 色相模式：使用源色相，保留目标饱和度和明度
        Saturation,         ///< 饱和度模式：使用源饱和度，保留目标色相和明度
        Color,              ///< 颜色模式：使用源色相和饱和度，保留目标明度
        Luminosity,         ///< 明度模式：使用源明度，保留目标色相和饱和度
        Divide,             ///< 除法混合：dst / src
        PremultipliedAlpha,

        ENUM_CLASS_RANGE(None, PremultipliedAlpha)
    };

    /**
     * @brief 获取 Alpha 混合模式的名称
     * @param mode 混合模式
     * @return 返回混合模式的名称（C 字符串）
     */
    inline const char *GetAlphaBlendModeName(AlphaBlendMode mode)
    {
        switch(mode)
        {
            case AlphaBlendMode::None:          return "None";
            case AlphaBlendMode::Normal:        return "Normal";
            case AlphaBlendMode::Add:           return "Add";
            case AlphaBlendMode::Subtract:      return "Subtract";
            case AlphaBlendMode::Multiply:      return "Multiply";
            case AlphaBlendMode::Screen:        return "Screen";
            case AlphaBlendMode::Overlay:       return "Overlay";
            case AlphaBlendMode::Darken:        return "Darken";
            case AlphaBlendMode::Lighten:       return "Lighten";
            case AlphaBlendMode::ColorDodge:    return "ColorDodge";
            case AlphaBlendMode::ColorBurn:     return "ColorBurn";
            case AlphaBlendMode::HardLight:     return "HardLight";
            case AlphaBlendMode::SoftLight:     return "SoftLight";
            case AlphaBlendMode::Difference:    return "Difference";
            case AlphaBlendMode::Exclusion:     return "Exclusion";
            case AlphaBlendMode::LinearBurn:    return "LinearBurn";
            case AlphaBlendMode::LinearDodge:   return "LinearDodge";
            case AlphaBlendMode::VividLight:    return "VividLight";
            case AlphaBlendMode::LinearLight:   return "LinearLight";
            case AlphaBlendMode::PinLight:      return "PinLight";
            case AlphaBlendMode::HardMix:       return "HardMix";
            case AlphaBlendMode::DarkerColor:   return "DarkerColor";
            case AlphaBlendMode::LighterColor:  return "LighterColor";
            case AlphaBlendMode::Hue:           return "Hue";
            case AlphaBlendMode::Saturation:    return "Saturation";
            case AlphaBlendMode::Color:         return "Color";
            case AlphaBlendMode::Luminosity:    return "Luminosity";
            case AlphaBlendMode::Divide:        return "Divide";
            case AlphaBlendMode::PremultipliedAlpha:return "PremultipliedAlpha";
            default:                            return "Unknown";
        }
    }
}//namespace hgl::math
