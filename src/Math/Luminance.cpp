/**
 * Luminance.cpp - 亮度与光度单位转换实现
 */

#include <hgl/math/Luminance.h>
#include <algorithm>
#include <cmath>

namespace hgl::math
{
    // 单位转换常量
    constexpr float FOOTLAMBERT_TO_NIT = 3.426f;        // 1 fL = 3.426 cd/m² = 3.426 nit
    constexpr float NIT_TO_FOOTLAMBERT = 1.0f / 3.426f; // 1 nit = 0.2919 fL (反向转换)

    // ==================== Luminance 类实现 ====================

    Luminance::Luminance(float value, LuminanceUnit unit) noexcept
    {
        // 转换各种单位到尼特
        switch (unit)
        {
            case LuminanceUnit::Nit:
                nits_ = value;
                break;

            case LuminanceUnit::FootLambert:
                nits_ = value * FOOTLAMBERT_TO_NIT;
                break;
        }
    }

    float Luminance::AsFootLambert() const noexcept
    {
        return nits_ * NIT_TO_FOOTLAMBERT;
    }

    float Luminance::As(LuminanceUnit unit) const noexcept
    {
        switch (unit)
        {
            case LuminanceUnit::Nit:
                return AsNit();
            case LuminanceUnit::FootLambert:
                return AsFootLambert();
            default:
                return 0.0f;
        }
    }

    // ==================== 色温转换实现 ====================

    Color3f ColorTemperatureToRGB(float temperature)
    {
        // 限制温度范围
        temperature = std::clamp(temperature, 1000.0f, 40000.0f);

        // 将温度除以100以便计算
        float temp = temperature / 100.0f;

        Color3f color;

        // 计算红色分量
        if (temp <= 66.0f)
        {
            color.r = 1.0f;
        }
        else
        {
            float red = temp - 60.0f;
            red = 329.698727446f * std::pow(red, -0.1332047592f);
            color.r = std::clamp(red / 255.0f, 0.0f, 1.0f);
        }

        // 计算绿色分量
        if (temp <= 66.0f)
        {
            float green = temp;
            green = 99.4708025861f * std::log(green) - 161.1195681661f;
            color.g = std::clamp(green / 255.0f, 0.0f, 1.0f);
        }
        else
        {
            float green = temp - 60.0f;
            green = 288.1221695283f * std::pow(green, -0.0755148492f);
            color.g = std::clamp(green / 255.0f, 0.0f, 1.0f);
        }

        // 计算蓝色分量
        if (temp >= 66.0f)
        {
            color.b = 1.0f;
        }
        else if (temp <= 19.0f)
        {
            color.b = 0.0f;
        }
        else
        {
            float blue = temp - 10.0f;
            blue = 138.5177312231f * std::log(blue) - 305.0447927307f;
            color.b = std::clamp(blue / 255.0f, 0.0f, 1.0f);
        }

        return color;
    }

}//namespace hgl::math
