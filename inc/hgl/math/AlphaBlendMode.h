#pragma once

#include<hgl/platform/Platform.h>
#include<hgl/type/EnumUtil.h>

namespace hgl::math
{
    /**
     * @brief Alpha blending modes for color composition
     * 
     * These modes define how source and destination colors are combined.
     * This enum serves as the authoritative definition for alpha blending modes
     * in both CMCoreType and CMMath libraries.
     */
    enum class AlphaBlendMode
    {
        None = 0,           ///< No blending, use source color directly
        Normal,             ///< Standard alpha blending (src over dst)
        Add,                ///< Additive blending (src + dst)
        Subtract,           ///< Subtractive blending (dst - src)
        Multiply,           ///< Multiplicative blending (src * dst)
        Screen,             ///< Screen blending (1 - (1-src)*(1-dst))
        Overlay,            ///< Overlay blending (combines multiply and screen)
        Darken,             ///< Select darker of two colors (min)
        Lighten,            ///< Select lighter of two colors (max)
        ColorDodge,         ///< Dodge blending mode (brighten)
        ColorBurn,          ///< Burn blending mode (darken)
        HardLight,          ///< Hard light blending
        SoftLight,          ///< Soft light blending
        Difference,         ///< Difference blending (abs(dst - src))
        Exclusion,          ///< Exclusion blending (similar to difference)
        LinearBurn,         ///< Linear burn: dst + src - 1
        LinearDodge,        ///< Linear dodge: dst + src
        VividLight,         ///< Vivid light blending
        LinearLight,        ///< Linear light blending
        PinLight,           ///< Pin light blending
        HardMix,            ///< Hard mix (posterize to 0 or 1)
        DarkerColor,        ///< Select color with lower luminance
        LighterColor,       ///< Select color with higher luminance
        Hue,                ///< Use source hue with destination saturation and luminosity
        Saturation,         ///< Use source saturation with destination hue and luminosity
        Color,              ///< Use source hue and saturation with destination luminosity
        Luminosity,         ///< Use source luminosity with destination hue and saturation
        Divide,             ///< Division blending: dst / src
        PremultipliedAlpha,
        
        ENUM_CLASS_RANGE(None, PremultipliedAlpha)
    };

    /**
     * @brief Get the name of an alpha blend mode
     * @param mode The alpha blend mode
     * @return The name of the blend mode as a C string
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
