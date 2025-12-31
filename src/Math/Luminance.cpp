/**
 * Luminance.cpp - 亮度与光度单位转换实现
 */

#include <hgl/math/Luminance.h>
#include <hgl/math/MathConstants.h>
#include <algorithm>
#include <cmath>

namespace hgl::math
{
    // ==================== Luminance 类实现 ====================
    
    Luminance::Luminance(float value, LuminanceUnit unit) noexcept
    {
        // 转换各种单位到坎德拉
        switch (unit)
        {
            case LuminanceUnit::Candela:
                candela_ = value;
                break;
                
            case LuminanceUnit::Lumen:
                // 流明转坎德拉（假设全向光源，立体角 4π）
                candela_ = value / (4.0f * pi);
                break;
                
            case LuminanceUnit::Nit:
                // 尼特就是坎德拉每平方米 (cd/m²)
                candela_ = value;
                break;
                
            case LuminanceUnit::Lux:
                // 勒克斯转坎德拉（假设1米距离，立体角计算）
                candela_ = value;  // 简化：1 lux ≈ 1 cd at 1m
                break;
                
            case LuminanceUnit::FootLambert:
                // 英尺朗伯转尼特：1 fL = 3.426 cd/m²
                candela_ = value * 3.426f;
                break;
                
            case LuminanceUnit::Footcandle:
                // 英尺烛光转勒克斯：1 fc = 10.764 lux
                candela_ = value * 10.764f;
                break;
        }
    }
    
    float Luminance::AsLumen() const noexcept
    {
        // 坎德拉转流明（假设全向光源）
        return candela_ * 4.0f * pi;
    }
    
    float Luminance::AsLux() const noexcept
    {
        // 坎德拉转勒克斯（假设1米距离）
        return candela_;  // 简化：1 cd ≈ 1 lux at 1m
    }
    
    float Luminance::AsFootLambert() const noexcept
    {
        // 尼特转英尺朗伯：1 cd/m² = 0.2919 fL
        return candela_ * 0.2919f;
    }
    
    float Luminance::AsFootcandle() const noexcept
    {
        // 勒克斯转英尺烛光：1 lux = 0.0929 fc
        float lux = AsLux();
        return lux * 0.0929f;
    }
    
    float Luminance::As(LuminanceUnit unit) const noexcept
    {
        switch (unit)
        {
            case LuminanceUnit::Candela:
                return AsCandela();
            case LuminanceUnit::Lumen:
                return AsLumen();
            case LuminanceUnit::Nit:
                return AsNit();
            case LuminanceUnit::Lux:
                return AsLux();
            case LuminanceUnit::FootLambert:
                return AsFootLambert();
            case LuminanceUnit::Footcandle:
                return AsFootcandle();
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
