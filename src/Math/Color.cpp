/**
 * Color.cpp - 颜色空间转换和颜色操作实现
 */

#include <hgl/math/Color.h>
#include <algorithm>
#include <cmath>

namespace hgl::math
{
    // ==================== RGB-HSV 转换 ====================

    Vector3f RGBtoHSV(const Color3f &rgb)
    {
        float r = rgb.r;
        float g = rgb.g;
        float b = rgb.b;

        float max_val = std::max({r, g, b});
        float min_val = std::min({r, g, b});
        float delta = max_val - min_val;

        Vector3f hsv;

        // Value
        hsv.z = max_val;

        // Saturation
        if (max_val > 1e-6f)
        {
            hsv.y = delta / max_val;
        }
        else
        {
            hsv.y = 0.0f;
            hsv.x = 0.0f;  // Hue is undefined
            return hsv;
        }

        // Hue
        if (delta < 1e-6f)
        {
            hsv.x = 0.0f;  // Gray color, hue is undefined
        }
        else
        {
            if (r >= max_val)
            {
                hsv.x = (g - b) / delta;  // Between yellow & magenta
            }
            else if (g >= max_val)
            {
                hsv.x = 2.0f + (b - r) / delta;  // Between cyan & yellow
            }
            else
            {
                hsv.x = 4.0f + (r - g) / delta;  // Between magenta & cyan
            }

            hsv.x *= 60.0f;  // Convert to degrees

            if (hsv.x < 0.0f)
            {
                hsv.x += 360.0f;
            }
        }

        return hsv;
    }

    Color3f HSVtoRGB(const Vector3f &hsv)
    {
        float h = hsv.x;
        float s = hsv.y;
        float v = hsv.z;

        if (s <= 1e-6f)
        {
            // Achromatic (gray)
            return Color3f(v, v, v);
        }

        // Normalize hue to [0, 360)
        h = std::fmod(h, 360.0f);
        if (h < 0.0f)
            h += 360.0f;

        h /= 60.0f;  // Sector 0 to 5
        int sector = static_cast<int>(std::floor(h));
        float f = h - sector;  // Fractional part

        float p = v * (1.0f - s);
        float q = v * (1.0f - s * f);
        float t = v * (1.0f - s * (1.0f - f));

        switch (sector)
        {
            case 0:  return Color3f(v, t, p);
            case 1:  return Color3f(q, v, p);
            case 2:  return Color3f(p, v, t);
            case 3:  return Color3f(p, q, v);
            case 4:  return Color3f(t, p, v);
            default: return Color3f(v, p, q);
        }
    }

    // ==================== RGB-HSL 转换 ====================

    Vector3f RGBtoHSL(const Color3f &rgb)
    {
        float r = rgb.r;
        float g = rgb.g;
        float b = rgb.b;

        float max_val = std::max({r, g, b});
        float min_val = std::min({r, g, b});
        float delta = max_val - min_val;

        Vector3f hsl;

        // Lightness
        hsl.z = (max_val + min_val) / 2.0f;

        if (delta < 1e-6f)
        {
            // Achromatic
            hsl.x = 0.0f;
            hsl.y = 0.0f;
        }
        else
        {
            // Saturation
            if (hsl.z < 0.5f)
            {
                hsl.y = delta / (max_val + min_val);
            }
            else
            {
                hsl.y = delta / (2.0f - max_val - min_val);
            }

            // Hue
            if (r >= max_val)
            {
                hsl.x = (g - b) / delta;
            }
            else if (g >= max_val)
            {
                hsl.x = 2.0f + (b - r) / delta;
            }
            else
            {
                hsl.x = 4.0f + (r - g) / delta;
            }

            hsl.x *= 60.0f;

            if (hsl.x < 0.0f)
            {
                hsl.x += 360.0f;
            }
        }

        return hsl;
    }

    static float HueToRGB(float p, float q, float t)
    {
        if (t < 0.0f) t += 1.0f;
        if (t > 1.0f) t -= 1.0f;
        if (t < 1.0f / 6.0f) return p + (q - p) * 6.0f * t;
        if (t < 1.0f / 2.0f) return q;
        if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
        return p;
    }

    Color3f HSLtoRGB(const Vector3f &hsl)
    {
        float h = hsl.x / 360.0f;  // Normalize to [0, 1]
        float s = hsl.y;
        float l = hsl.z;

        if (s <= 1e-6f)
        {
            // Achromatic
            return Color3f(l, l, l);
        }

        float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
        float p = 2.0f * l - q;

        float r = HueToRGB(p, q, h + 1.0f / 3.0f);
        float g = HueToRGB(p, q, h);
        float b = HueToRGB(p, q, h - 1.0f / 3.0f);

        return Color3f(r, g, b);
    }

    // ==================== Gamma 校正 ====================

    static float SRGBToLinearComponent(float srgb)
    {
        if (srgb <= 0.04045f)
        {
            return srgb / 12.92f;
        }
        else
        {
            return std::pow((srgb + 0.055f) / 1.055f, 2.4f);
        }
    }

    static float LinearToSRGBComponent(float linear)
    {
        if (linear <= 0.0031308f)
        {
            return 12.92f * linear;
        }
        else
        {
            return 1.055f * std::pow(linear, 1.0f / 2.4f) - 0.055f;
        }
    }

    Color3f SRGBtoLinear(const Color3f &srgb)
    {
        return Color3f(
            SRGBToLinearComponent(srgb.r),
            SRGBToLinearComponent(srgb.g),
            SRGBToLinearComponent(srgb.b)
        );
    }

    Color3f LinearToSRGB(const Color3f &linear)
    {
        return Color3f(
            LinearToSRGBComponent(linear.r),
            LinearToSRGBComponent(linear.g),
            LinearToSRGBComponent(linear.b)
        );
    }

    Color4f SRGBtoLinear(const Color4f &srgb)
    {
        return Color4f(
            SRGBToLinearComponent(srgb.r),
            SRGBToLinearComponent(srgb.g),
            SRGBToLinearComponent(srgb.b),
            srgb.a  // Alpha 不变
        );
    }

    Color4f LinearToSRGB(const Color4f &linear)
    {
        return Color4f(
            LinearToSRGBComponent(linear.r),
            LinearToSRGBComponent(linear.g),
            LinearToSRGBComponent(linear.b),
            linear.a  // Alpha 不变
        );
    }

    // ==================== 颜色插值 ====================

    Color3f ColorLerpHSV(const Color3f &c1, const Color3f &c2, float t)
    {
        Vector3f hsv1 = RGBtoHSV(c1);
        Vector3f hsv2 = RGBtoHSV(c2);

        // 处理色相的环形插值
        float h1 = hsv1.x;
        float h2 = hsv2.x;
        float hue_diff = h2 - h1;

        if (hue_diff > 180.0f)
        {
            h1 += 360.0f;
        }
        else if (hue_diff < -180.0f)
        {
            h2 += 360.0f;
        }

        Vector3f hsv_lerp(
            h1 + (h2 - h1) * t,
            hsv1.y + (hsv2.y - hsv1.y) * t,
            hsv1.z + (hsv2.z - hsv1.z) * t
        );

        return HSVtoRGB(hsv_lerp);
    }

    // ==================== 颜色调整 ====================

    Color3f Desaturate(const Color3f &color, float amount)
    {
        float luminance = GetLuminance(color);
        Color3f gray(luminance, luminance, luminance);
        return ColorLerp(color, gray, amount);
    }

    Color3f AdjustContrast(const Color3f &color, float factor)
    {
        const Color3f mid(0.5f, 0.5f, 0.5f);
        return Color3f(mid + (color - mid) * factor);
    }

    Color3f AdjustHue(const Color3f &color, float shift)
    {
        Vector3f hsv = RGBtoHSV(color);
        hsv.x += shift;
        return HSVtoRGB(hsv);
    }

    Color3f AdjustSaturation(const Color3f &color, float factor)
    {
        Vector3f hsv = RGBtoHSV(color);
        hsv.y *= factor;
        hsv.y = std::clamp(hsv.y, 0.0f, 1.0f);
        return HSVtoRGB(hsv);
    }

    // ==================== Tone Mapping ====================

    Color3f ToneMapReinhardExtended(const Color3f &hdr, float white_point)
    {
        float white_sq = white_point * white_point;
        return Color3f((hdr * (Color3f(1.0f) + hdr / white_sq)) / (Color3f(1.0f) + hdr));
    }

    Color3f ToneMapACES(const Color3f &hdr)
    {
        // ACES Filmic tone mapping curve
        // Reference: https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/

        const float a = 2.51f;
        const float b = 0.03f;
        const float c = 2.43f;
        const float d = 0.59f;
        const float e = 0.14f;

        Color3f x = hdr;
        return Color3f(glm::clamp(static_cast<glm::vec3>((x * (a * x + b)) / (x * (c * x + d) + e)),
                          glm::vec3(0.0f), glm::vec3(1.0f)));
    }

    static float Uncharted2ToneMapFunc(float x)
    {
        const float A = 0.15f;
        const float B = 0.50f;
        const float C = 0.10f;
        const float D = 0.20f;
        const float E = 0.02f;
        const float F = 0.30f;

        return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
    }

    Color3f ToneMapUncharted2(const Color3f &hdr)
    {
        const float exposure_bias = 2.0f;
        const float white_point = 11.2f;

        Color3f curr = hdr * exposure_bias;

        Color3f result(
            Uncharted2ToneMapFunc(curr.r),
            Uncharted2ToneMapFunc(curr.g),
            Uncharted2ToneMapFunc(curr.b)
        );

        float white_scale = 1.0f / Uncharted2ToneMapFunc(white_point);
        result *= white_scale;

        return Color3f(glm::clamp(static_cast<glm::vec3>(result), glm::vec3(0.0f), glm::vec3(1.0f)));
    }

}//namespace hgl::math
