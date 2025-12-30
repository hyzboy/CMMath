/**
 * Clamp.h - 数值钳制工具
 * 
 * 提供将数值限制在指定范围内的函数。
 */

#pragma once

#include <algorithm>
#include <cstdint>

namespace hgl::math
{
    /**
     * @brief 将值钳制到 [0, 1] 范围
     * @param value 输入值
     * @return 钳制后的值
     */
    inline float Clamp(float value)
    {
        return std::clamp(value, 0.0f, 1.0f);
    }
    
    /**
     * @brief 将双精度浮点数钳制到 [0, 1] 范围
     */
    inline double Clamp(double value)
    {
        return std::clamp(value, 0.0, 1.0);
    }
    
    /**
     * @brief 将值钳制到指定范围
     * @param value 输入值
     * @param min_val 最小值
     * @param max_val 最大值
     * @return 钳制后的值
     */
    template<typename T>
    inline T Clamp(T value, T min_val, T max_val)
    {
        return std::clamp(value, min_val, max_val);
    }
    
    /**
     * @brief 将浮点数钳制到 [0, 255] 范围并转换为 uint8_t
     */
    inline uint8_t ClampU8(float value)
    {
        if (value <= 0.0f) return 0;
        if (value >= 255.0f) return 255;
        return static_cast<uint8_t>(value + 0.5f);  // 四舍五入
    }
    
    /**
     * @brief 将浮点数钳制到 [0, 65535] 范围并转换为 uint16_t
     */
    inline uint16_t ClampU16(float value)
    {
        if (value <= 0.0f) return 0;
        if (value >= 65535.0f) return 65535;
        return static_cast<uint16_t>(value + 0.5f);  // 四舍五入
    }
    
}//namespace hgl::math
