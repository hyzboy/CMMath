/**
 * Luminance.h - 亮度与光度单位转换
 * 
 * 提供亮度和光度相关的数据类型、单位转换以及常见光源数据表。
 * 
 * 包含：
 * - 亮度类型定义（支持多种单位输入输出）
 * - 光度单位转换（流明、坎德拉、尼特、勒克斯等）
 * - 常见光源亮度和色温数据表
 * - 颜色-亮度关联定义
 */

#pragma once

#include <hgl/math/VectorTypes.h>
#include <hgl/math/Color.h>
#include <cmath>

namespace hgl::math
{
    // ==================== 亮度单位枚举 ====================
    
    /**
     * 光度单位类型
     */
    enum class LuminanceUnit
    {
        Candela,        // 坎德拉 (cd) - 光强度，SI基本单位
        Lumen,          // 流明 (lm) - 光通量
        Nit,            // 尼特 (cd/m²) - 亮度
        Lux,            // 勒克斯 (lx) - 照度
        FootLambert,    // 英尺朗伯 (fL) - 亮度（美制单位）
        Footcandle      // 英尺烛光 (fc) - 照度（美制单位）
    };
    
    // ==================== 亮度数据类 ====================
    
    /**
     * @brief 亮度数据类
     * 
     * 存储亮度值，支持多种单位的输入和输出转换。
     * 内部使用坎德拉(Candela)作为标准单位存储。
     */
    class Luminance
    {
    private:
        float candela_;  // 内部存储：坎德拉 (cd)
        
    public:
        // ==================== 构造函数 ====================
        
        /**
         * 默认构造函数，初始化为0
         */
        constexpr Luminance() noexcept : candela_(0.0f) {}
        
        /**
         * 从指定单位的数值构造
         * @param value 亮度数值
         * @param unit 亮度单位
         */
        Luminance(float value, LuminanceUnit unit) noexcept;
        
        /**
         * 从坎德拉值直接构造
         */
        static constexpr Luminance FromCandela(float cd) noexcept
        {
            Luminance lum;
            lum.candela_ = cd;
            return lum;
        }
        
        /**
         * 从流明值构造（假设为全向光源，4π立体角）
         */
        static Luminance FromLumen(float lm) noexcept
        {
            return Luminance(lm, LuminanceUnit::Lumen);
        }
        
        /**
         * 从尼特值构造
         */
        static constexpr Luminance FromNit(float nit) noexcept
        {
            return Luminance::FromCandela(nit);  // 1 nit = 1 cd/m²
        }
        
        /**
         * 从勒克斯值构造（假设距离为1米）
         */
        static Luminance FromLux(float lux) noexcept
        {
            return Luminance(lux, LuminanceUnit::Lux);
        }
        
        // ==================== 单位转换输出 ====================
        
        /**
         * 获取坎德拉值
         */
        constexpr float AsCandela() const noexcept { return candela_; }
        
        /**
         * 获取流明值（假设为全向光源）
         */
        float AsLumen() const noexcept;
        
        /**
         * 获取尼特值
         */
        constexpr float AsNit() const noexcept { return candela_; }
        
        /**
         * 获取勒克斯值（假设距离为1米）
         */
        float AsLux() const noexcept;
        
        /**
         * 获取英尺朗伯值
         */
        float AsFootLambert() const noexcept;
        
        /**
         * 获取英尺烛光值
         */
        float AsFootcandle() const noexcept;
        
        /**
         * 按指定单位获取值
         */
        float As(LuminanceUnit unit) const noexcept;
        
        // ==================== 操作符重载 ====================
        
        constexpr Luminance operator+(const Luminance& other) const noexcept
        {
            return FromCandela(candela_ + other.candela_);
        }
        
        constexpr Luminance operator-(const Luminance& other) const noexcept
        {
            return FromCandela(candela_ - other.candela_);
        }
        
        constexpr Luminance operator*(float scale) const noexcept
        {
            return FromCandela(candela_ * scale);
        }
        
        constexpr Luminance operator/(float scale) const noexcept
        {
            return FromCandela(candela_ / scale);
        }
        
        constexpr bool operator<(const Luminance& other) const noexcept
        {
            return candela_ < other.candela_;
        }
        
        constexpr bool operator>(const Luminance& other) const noexcept
        {
            return candela_ > other.candela_;
        }
        
        constexpr bool operator<=(const Luminance& other) const noexcept
        {
            return candela_ <= other.candela_;
        }
        
        constexpr bool operator>=(const Luminance& other) const noexcept
        {
            return candela_ >= other.candela_;
        }
        
        constexpr bool operator==(const Luminance& other) const noexcept
        {
            return candela_ == other.candela_;
        }
        
        constexpr bool operator!=(const Luminance& other) const noexcept
        {
            return candela_ != other.candela_;
        }
    };
    
    // ==================== 光源数据结构 ====================
    
    /**
     * 光源特性数据
     */
    struct LightSourceData
    {
        const char* name;           // 光源名称
        Luminance luminance;        // 亮度
        float colorTemperature;     // 色温 (开尔文)
        Color3f color;              // 近似颜色 (RGB)
        float powerWatts;           // 功率 (瓦特)
    };
    
    // ==================== 常见光源数据表 ====================
    
    namespace LightSources
    {
        // ==================== 自然光源 ====================
        
        /**
         * 阳光（正午，海平面）
         */
        const LightSourceData Sunlight
        {
            "阳光",
            Luminance::FromLux(100000.0f),
            5500.0f,
            Color3f(1.0f, 1.0f, 0.95f),
            0.0f  // 不适用
        };
        
        /**
         * 满月
         */
        const LightSourceData Moonlight
        {
            "满月",
            Luminance::FromLux(0.25f),
            4100.0f,
            Color3f(0.7f, 0.75f, 0.85f),
            0.0f  // 不适用
        };
        
        /**
         * 黎明/黄昏
         */
        const LightSourceData Twilight
        {
            "黎明/黄昏",
            Luminance::FromLux(400.0f),
            4500.0f,
            Color3f(1.0f, 0.8f, 0.6f),
            0.0f  // 不适用
        };
        
        /**
         * 阴天
         */
        const LightSourceData Overcast
        {
            "阴天",
            Luminance::FromLux(1000.0f),
            6500.0f,
            Color3f(0.8f, 0.85f, 0.9f),
            0.0f  // 不适用
        };
        
        // ==================== 白炽灯泡 ====================
        
        /**
         * 40W 白炽灯泡
         */
        const LightSourceData Incandescent40W
        {
            "40W 白炽灯泡",
            Luminance::FromLumen(450.0f),
            2700.0f,
            Color3f(1.0f, 0.85f, 0.6f),
            40.0f
        };
        
        /**
         * 60W 白炽灯泡
         */
        const LightSourceData Incandescent60W
        {
            "60W 白炽灯泡",
            Luminance::FromLumen(800.0f),
            2700.0f,
            Color3f(1.0f, 0.85f, 0.6f),
            60.0f
        };
        
        /**
         * 100W 白炽灯泡
         */
        const LightSourceData Incandescent100W
        {
            "100W 白炽灯泡",
            Luminance::FromLumen(1600.0f),
            2850.0f,
            Color3f(1.0f, 0.87f, 0.65f),
            100.0f
        };
        
        // ==================== LED 灯泡 ====================
        
        /**
         * 9W LED 灯泡（相当于60W白炽灯）
         */
        const LightSourceData LED9W_WarmWhite
        {
            "9W LED 灯泡（暖白）",
            Luminance::FromLumen(800.0f),
            3000.0f,
            Color3f(1.0f, 0.9f, 0.75f),
            9.0f
        };
        
        /**
         * 9W LED 灯泡（冷白）
         */
        const LightSourceData LED9W_CoolWhite
        {
            "9W LED 灯泡（冷白）",
            Luminance::FromLumen(850.0f),
            5000.0f,
            Color3f(0.95f, 0.98f, 1.0f),
            9.0f
        };
        
        /**
         * 13W LED 灯泡（相当于100W白炽灯）
         */
        const LightSourceData LED13W
        {
            "13W LED 灯泡",
            Luminance::FromLumen(1600.0f),
            4000.0f,
            Color3f(1.0f, 0.95f, 0.85f),
            13.0f
        };
        
        // ==================== 荧光灯 ====================
        
        /**
         * 18W 紧凑型荧光灯（CFL）
         */
        const LightSourceData CFL18W
        {
            "18W 荧光灯",
            Luminance::FromLumen(1200.0f),
            4100.0f,
            Color3f(0.9f, 0.95f, 1.0f),
            18.0f
        };
        
        /**
         * 23W 紧凑型荧光灯（相当于100W白炽灯）
         */
        const LightSourceData CFL23W
        {
            "23W 荧光灯",
            Luminance::FromLumen(1600.0f),
            4100.0f,
            Color3f(0.9f, 0.95f, 1.0f),
            23.0f
        };
        
        // ==================== 卤素灯 ====================
        
        /**
         * 50W 卤素射灯
         */
        const LightSourceData Halogen50W
        {
            "50W 卤素射灯",
            Luminance::FromLumen(900.0f),
            3000.0f,
            Color3f(1.0f, 0.92f, 0.78f),
            50.0f
        };
        
        // ==================== 蜡烛 ====================
        
        /**
         * 标准蜡烛
         */
        const LightSourceData Candle
        {
            "蜡烛",
            Luminance::FromCandela(1.0f),
            1850.0f,
            Color3f(1.0f, 0.65f, 0.3f),
            0.0f  // 不适用
        };
        
        // ==================== 显示器 ====================
        
        /**
         * 标准显示器
         */
        const LightSourceData StandardMonitor
        {
            "标准显示器",
            Luminance::FromNit(250.0f),
            6500.0f,
            Color3f(1.0f, 1.0f, 1.0f),
            50.0f  // 估计
        };
        
        /**
         * HDR 显示器
         */
        const LightSourceData HDRMonitor
        {
            "HDR 显示器",
            Luminance::FromNit(1000.0f),
            6500.0f,
            Color3f(1.0f, 1.0f, 1.0f),
            100.0f  // 估计
        };
        
    }//namespace LightSources
    
    // ==================== 色温转换函数 ====================
    
    /**
     * @brief 根据色温计算近似颜色（使用黑体辐射模型）
     * @param temperature 色温（开尔文），范围 1000K - 40000K
     * @return RGB 颜色
     * 
     * 使用 Tanner Helland 算法的改进版本
     */
    Color3f ColorTemperatureToRGB(float temperature);
    
    /**
     * @brief 常见色温值
     */
    namespace ColorTemperature
    {
        constexpr float Candle          = 1850.0f;  // 蜡烛
        constexpr float Incandescent    = 2700.0f;  // 白炽灯
        constexpr float WarmWhite       = 3000.0f;  // 暖白
        constexpr float Halogen         = 3200.0f;  // 卤素灯
        constexpr float Fluorescent     = 4100.0f;  // 荧光灯
        constexpr float CoolWhite       = 5000.0f;  // 冷白
        constexpr float Daylight        = 5500.0f;  // 日光
        constexpr float CloudySky       = 6500.0f;  // 阴天
        constexpr float ClearSky        = 10000.0f; // 晴空
    }
    
    // ==================== 亮度相关工具函数 ====================
    
    /**
     * @brief 计算点光源在指定距离处的照度（勒克斯）
     * @param source 光源亮度（流明）
     * @param distance 距离（米）
     * @return 照度（勒克斯）
     */
    inline float CalculateIlluminance(const Luminance& source, float distance)
    {
        if (distance <= 0.0f) return 0.0f;
        // 照度 = 光通量 / (4π * 距离²)
        float lumen = source.AsLumen();
        return lumen / (4.0f * 3.14159265f * distance * distance);
    }
    
    /**
     * @brief 根据亮度调整颜色
     * @param baseColor 基础颜色
     * @param luminance 亮度值
     * @return 调整后的颜色
     */
    inline Color3f ApplyLuminance(const Color3f& baseColor, float luminance)
    {
        return baseColor * luminance;
    }
    
}//namespace hgl::math
