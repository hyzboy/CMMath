#include<hgl/color/ColorQuantize.h>
#include<algorithm>
#include<cmath>

namespace hgl
{
    //==================================================================================================
    // Bit-based Quantization
    //==================================================================================================

    Color3f QuantizeColor3f(const Color3f &color, int bits)
    {
        if(bits < 1) bits = 1;
        if(bits > 8) bits = 8;

        int levels = (1 << bits) - 1;
        float step = 1.0f / levels;

        return Color3f(
            std::round(color.r * levels) * step,
            std::round(color.g * levels) * step,
            std::round(color.b * levels) * step
        );
    }

    Color3ub QuantizeColor3ub(const Color3ub &color, int bits)
    {
        if(bits < 1) bits = 1;
        if(bits > 8) bits = 8;

        int levels = (1 << bits) - 1;
        int mask = ~((1 << (8 - bits)) - 1);

        return Color3ub(
            uint8(color.r & mask),
            uint8(color.g & mask),
            uint8(color.b & mask)
        );
    }

    Color4f QuantizeColor4f(const Color4f &color, int bits)
    {
        if(bits < 1) bits = 1;
        if(bits > 8) bits = 8;

        int levels = (1 << bits) - 1;
        float step = 1.0f / levels;

        return Color4f(
            std::round(color.r * levels) * step,
            std::round(color.g * levels) * step,
            std::round(color.b * levels) * step,
            std::round(color.a * levels) * step
        );
    }

    Color4ub QuantizeColor4ub(const Color4ub &color, int bits)
    {
        if(bits < 1) bits = 1;
        if(bits > 8) bits = 8;

        int levels = (1 << bits) - 1;
        int mask = ~((1 << (8 - bits)) - 1);

        return Color4ub(
            uint8(color.r & mask),
            uint8(color.g & mask),
            uint8(color.b & mask),
            uint8(color.a & mask)
        );
    }

    //==================================================================================================
    // Uniform Quantization
    //==================================================================================================

    Color3f UniformQuantize3f(const Color3f &color, int levels)
    {
        if(levels < 2) levels = 2;
        if(levels > 256) levels = 256;

        float step = 1.0f / (levels - 1);
        float inv_step = 1.0f / step;

        return Color3f(
            std::round(color.r * inv_step) * step,
            std::round(color.g * inv_step) * step,
            std::round(color.b * inv_step) * step
        );
    }

    //==================================================================================================
    // Error Calculation
    //==================================================================================================

    float QuantizationError3f(const Color3f &original, const Color3f &quantized)
    {
        float dr = original.r - quantized.r;
        float dg = original.g - quantized.g;
        float db = original.b - quantized.b;

        return dr*dr + dg*dg + db*db;
    }

    float QuantizationError3ub(const Color3ub &original, const Color3ub &quantized)
    {
        float dr = (original.r - quantized.r) / 255.0f;
        float dg = (original.g - quantized.g) / 255.0f;
        float db = (original.b - quantized.b) / 255.0f;

        return dr*dr + dg*dg + db*db;
    }

    //==================================================================================================
    // Palette-based Quantization
    //==================================================================================================

    int FindNearestPaletteColor(const Color3f &color, const std::vector<Color3f> &palette)
    {
        if(palette.empty()) return -1;

        int best_idx = 0;
        float best_dist = QuantizationError3f(color, palette[0]);

        for(size_t i = 1; i < palette.size(); ++i)
        {
            float dist = QuantizationError3f(color, palette[i]);
            if(dist < best_dist)
            {
                best_dist = dist;
                best_idx = i;
            }
        }

        return best_idx;
    }

    int FindNearestPaletteColor(const Color3ub &color, const std::vector<Color3ub> &palette)
    {
        if(palette.empty()) return -1;

        int best_idx = 0;
        float best_dist = QuantizationError3ub(color, palette[0]);

        for(size_t i = 1; i < palette.size(); ++i)
        {
            float dist = QuantizationError3ub(color, palette[i]);
            if(dist < best_dist)
            {
                best_dist = dist;
                best_idx = i;
            }
        }

        return best_idx;
    }

    float GetPaletteColorDistance(const Color3f &color, const Color3f &palette_color)
    {
        return std::sqrt(QuantizationError3f(color, palette_color));
    }

} // namespace hgl
