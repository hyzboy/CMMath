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
#include <hgl/math/MathConstants.h>
#include <cmath>

namespace hgl::math
{
    // ==================== 光通量单位枚举 ====================
    
    /**
     * 光通量单位类型
     */
    enum class LuminousFluxUnit
    {
        Lumen           // 流明 (lm) - 光通量，SI单位
    };
    
    // ==================== 亮度单位枚举 ====================
    
    /**
     * 光度单位类型
     * 注意：本类主要处理亮度（Luminance）相关单位
     */
    enum class LuminanceUnit
    {
        Nit,            // 尼特 (cd/m²) - 亮度，SI单位
        FootLambert     // 英尺朗伯 (fL) - 亮度（美制单位）
    };
    
    // ==================== 光通量数据类 ====================
    
    /**
     * @brief 光通量数据类
     * 
     * 存储光通量值（Luminous Flux），以流明(Lumen)为单位。
     * 光通量表示光源发出的可见光总量。
     * 
     * 用途：
     * - 灯泡、LED等点光源的光输出
     * - 计算照度（需要结合距离和面积）
     * - 评估光源效率（流明/瓦特）
     */
    class LuminousFlux
    {
    private:
        float lumens_;  // 内部存储：流明 (lm)
        
    public:
        // ==================== 构造函数 ====================
        
        /**
         * 默认构造函数，初始化为0
         */
        constexpr LuminousFlux() noexcept : lumens_(0.0f) {}
        
        /**
         * 从指定单位的数值构造
         * @param value 光通量数值
         * @param unit 光通量单位
         */
        constexpr LuminousFlux(float value, LuminousFluxUnit unit) noexcept : lumens_(value) {}
        
        /**
         * 从流明值直接构造
         */
        static constexpr LuminousFlux FromLumen(float lumen) noexcept
        {
            LuminousFlux lf;
            lf.lumens_ = lumen;
            return lf;
        }
        
        // ==================== 单位转换输出 ====================
        
        /**
         * 获取流明值
         */
        constexpr float AsLumen() const noexcept { return lumens_; }
        
        /**
         * 按指定单位获取值
         */
        constexpr float As(LuminousFluxUnit unit) const noexcept
        {
            return lumens_;
        }
        
        // ==================== 光度学转换 ====================
        
        /**
         * @brief 转换为光强度（坎德拉）
         * @return 光强度（假设均匀全向发光）
         * 
         * 对于均匀全向发光的点光源：I = Φ / 4π
         */
        constexpr float ToCandela() const noexcept
        {
            return lumens_ / (4.0f * 3.14159265359f);
        }
        
        /**
         * @brief 计算在指定距离处的照度
         * @param distance 距离（米）
         * @return 照度（勒克斯）
         * 
         * 对于均匀全向发光的点光源：E = Φ / (4π * r²)
         */
        float CalculateIlluminance(float distance) const noexcept
        {
            if (distance <= 0.0f) return 0.0f;
            return lumens_ / (4.0f * 3.14159265359f * distance * distance);
        }
        
        // ==================== 操作符重载 ====================
        
        constexpr LuminousFlux operator+(const LuminousFlux& other) const noexcept
        {
            return FromLumen(lumens_ + other.lumens_);
        }
        
        constexpr LuminousFlux operator-(const LuminousFlux& other) const noexcept
        {
            return FromLumen(lumens_ - other.lumens_);
        }
        
        constexpr LuminousFlux operator*(float scale) const noexcept
        {
            return FromLumen(lumens_ * scale);
        }
        
        constexpr LuminousFlux operator/(float scale) const noexcept
        {
            return FromLumen(lumens_ / scale);
        }
        
        constexpr bool operator<(const LuminousFlux& other) const noexcept
        {
            return lumens_ < other.lumens_;
        }
        
        constexpr bool operator>(const LuminousFlux& other) const noexcept
        {
            return lumens_ > other.lumens_;
        }
        
        constexpr bool operator<=(const LuminousFlux& other) const noexcept
        {
            return lumens_ <= other.lumens_;
        }
        
        constexpr bool operator>=(const LuminousFlux& other) const noexcept
        {
            return lumens_ >= other.lumens_;
        }
        
        constexpr bool operator==(const LuminousFlux& other) const noexcept
        {
            return lumens_ == other.lumens_;
        }
        
        constexpr bool operator!=(const LuminousFlux& other) const noexcept
        {
            return lumens_ != other.lumens_;
        }
    };
    
    // ==================== 亮度数据类 ====================
    
    /**
     * @brief 亮度数据类
     * 
     * 存储亮度值（Luminance），支持多种单位的输入和输出转换。
     * 内部使用尼特(Nit = cd/m²)作为标准单位存储。
     * 
     * 注意：本类专注于亮度（Luminance）单位。
     * 对于光通量（Lumen）、光强度（Candela）、照度（Lux）等其他光度学量，
     * 请使用相应的转换函数，这些转换需要额外的几何参数（距离、面积等）。
     */
    class Luminance
    {
    private:
        float nits_;  // 内部存储：尼特 (cd/m²)
        
    public:
        // ==================== 构造函数 ====================
        
        /**
         * 默认构造函数，初始化为0
         */
        constexpr Luminance() noexcept : nits_(0.0f) {}
        
        /**
         * 从指定单位的数值构造
         * @param value 亮度数值
         * @param unit 亮度单位
         */
        Luminance(float value, LuminanceUnit unit) noexcept;
        
        /**
         * 从尼特值直接构造
         */
        static constexpr Luminance FromNit(float nit) noexcept
        {
            Luminance lum;
            lum.nits_ = nit;
            return lum;
        }
        
        /**
         * 从英尺朗伯值构造
         */
        static Luminance FromFootLambert(float fL) noexcept
        {
            return Luminance(fL, LuminanceUnit::FootLambert);
        }
        
        // ==================== 单位转换输出 ====================
        
        /**
         * 获取尼特值
         */
        constexpr float AsNit() const noexcept { return nits_; }
        
        /**
         * 获取英尺朗伯值
         */
        float AsFootLambert() const noexcept;
        
        /**
         * 按指定单位获取值
         */
        float As(LuminanceUnit unit) const noexcept;
        
        // ==================== 操作符重载 ====================
        
        constexpr Luminance operator+(const Luminance& other) const noexcept
        {
            return FromNit(nits_ + other.nits_);
        }
        
        constexpr Luminance operator-(const Luminance& other) const noexcept
        {
            return FromNit(nits_ - other.nits_);
        }
        
        constexpr Luminance operator*(float scale) const noexcept
        {
            return FromNit(nits_ * scale);
        }
        
        constexpr Luminance operator/(float scale) const noexcept
        {
            return FromNit(nits_ / scale);
        }
        
        constexpr bool operator<(const Luminance& other) const noexcept
        {
            return nits_ < other.nits_;
        }
        
        constexpr bool operator>(const Luminance& other) const noexcept
        {
            return nits_ > other.nits_;
        }
        
        constexpr bool operator<=(const Luminance& other) const noexcept
        {
            return nits_ <= other.nits_;
        }
        
        constexpr bool operator>=(const Luminance& other) const noexcept
        {
            return nits_ >= other.nits_;
        }
        
        constexpr bool operator==(const Luminance& other) const noexcept
        {
            return nits_ == other.nits_;
        }
        
        constexpr bool operator!=(const Luminance& other) const noexcept
        {
            return nits_ != other.nits_;
        }
    };
    
    // ==================== 光源数据结构 ====================
    
    /**
     * 光源特性数据
     * 注意：不同类型的光源使用不同的光度学量：
     * - luminousFlux: 光通量（流明），用于灯泡等光源
     * - luminance: 亮度（尼特），用于显示器等发光表面
     */
    struct LightSourceData
    {
        const char* name;           // 光源名称
        LuminousFlux luminousFlux;  // 光通量（流明），用于灯泡等
        Luminance luminance;        // 亮度 (尼特)，用于显示器等发光表面
        float colorTemperature;     // 色温 (开尔文)
        Color3f color;              // 近似颜色 (RGB)
        float powerWatts;           // 功率 (瓦特)
    };
    
    // ==================== 常见光源数据表 ====================
    
    namespace LightSources
    {
        // ==================== 自然光源 ====================
        // 注意：环境光照使用照度（勒克斯）表示，这里给出参考值
        // 实际使用时可用 CalculateIlluminance 函数
        
        /**
         * 阳光（正午，海平面）- 照度约 100,000 lux
         */
        const LightSourceData Sunlight
        {
            "阳光",
            LuminousFlux::FromLumen(0.0f),  // 环境光照，非点光源
            Luminance::FromNit(0.0f),  // 不适用
            5500.0f,
            Color3f(1.0f, 1.0f, 0.95f),
            0.0f  // 不适用
        };
        
        /**
         * 满月 - 照度约 0.25 lux
         */
        const LightSourceData Moonlight
        {
            "满月",
            LuminousFlux::FromLumen(0.0f),  // 环境光照，非点光源
            Luminance::FromNit(0.0f),  // 不适用
            4100.0f,
            Color3f(0.7f, 0.75f, 0.85f),
            0.0f  // 不适用
        };
        
        /**
         * 黎明/黄昏 - 照度约 400 lux
         */
        const LightSourceData Twilight
        {
            "黎明/黄昏",
            LuminousFlux::FromLumen(0.0f),  // 环境光照，非点光源
            Luminance::FromNit(0.0f),  // 不适用
            4500.0f,
            Color3f(1.0f, 0.8f, 0.6f),
            0.0f  // 不适用
        };
        
        /**
         * 阴天 - 照度约 1,000 lux
         */
        const LightSourceData Overcast
        {
            "阴天",
            LuminousFlux::FromLumen(0.0f),  // 环境光照，非点光源
            Luminance::FromNit(0.0f),  // 不适用
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
            LuminousFlux::FromLumen(450.0f),
            Luminance::FromNit(0.0f),  // 点光源，不使用亮度
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
            LuminousFlux::FromLumen(800.0f),
            Luminance::FromNit(0.0f),  // 点光源，不使用亮度
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
            LuminousFlux::FromLumen(1600.0f),
            Luminance::FromNit(0.0f),
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
            LuminousFlux::FromLumen(800.0f),
            Luminance::FromNit(0.0f),
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
            LuminousFlux::FromLumen(850.0f),
            Luminance::FromNit(0.0f),
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
            LuminousFlux::FromLumen(1600.0f),
            Luminance::FromNit(0.0f),
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
            LuminousFlux::FromLumen(1200.0f),
            Luminance::FromNit(0.0f),
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
            LuminousFlux::FromLumen(1600.0f),
            Luminance::FromNit(0.0f),
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
            LuminousFlux::FromLumen(900.0f),
            Luminance::FromNit(0.0f),
            3000.0f,
            Color3f(1.0f, 0.92f, 0.78f),
            50.0f
        };
        
        // ==================== 蜡烛 ====================
        
        /**
         * 标准蜡烛 - 约12.5流明
         */
        const LightSourceData Candle
        {
            "蜡烛",
            LuminousFlux::FromLumen(12.5f),
            Luminance::FromNit(0.0f),
            1850.0f,
            Color3f(1.0f, 0.65f, 0.3f),
            0.0f  // 不适用
        };
        
        // ==================== 显示器 ====================
        
        /**
         * 标准显示器 - 亮度通常以尼特为单位
         */
        const LightSourceData StandardMonitor
        {
            "标准显示器",
            LuminousFlux::FromLumen(0.0f),  // 不使用光通量
            Luminance::FromNit(250.0f),  // 250 nits
            6500.0f,
            Color3f(1.0f, 1.0f, 1.0f),
            50.0f  // 估计
        };
        
        /**
         * HDR 显示器 - 高亮度显示器
         */
        const LightSourceData HDRMonitor
        {
            "HDR 显示器",
            LuminousFlux::FromLumen(0.0f),  // 不使用光通量
            Luminance::FromNit(1000.0f),  // 1000 nits
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
     * @param luminousFlux 光通量（流明），必须为非负值
     * @param distance 距离（米），必须为正值
     * @return 照度（勒克斯 lx）
     * 
     * 对于点光源，光强度 I = 光通量 / 4π
     * 然后照度 E = I / 距离²
     * 因此 E = 光通量 / (4π * 距离²)
     * 
     * 注意：此公式假设光源为均匀全向发光的点光源。
     * 对于有方向性的光源（如射灯），需要考虑光束角度和方向因子。
     */
    inline float CalculateIlluminance(const LuminousFlux& luminousFlux, float distance)
    {
        if (distance <= 0.0f) return 0.0f;
        // E = Φ / (4π * r²)
        return luminousFlux.AsLumen() / (4.0f * pi_f * distance * distance);
    }
    
    /**
     * @brief 计算点光源在指定距离处的照度（勒克斯）- 浮点数重载版本
     * @param luminousFluxLumens 光通量（流明），必须为非负值
     * @param distance 距离（米），必须为正值
     * @return 照度（勒克斯 lx）
     */
    inline float CalculateIlluminance(float luminousFluxLumens, float distance)
    {
        if (distance <= 0.0f || luminousFluxLumens < 0.0f) return 0.0f;
        // E = Φ / (4π * r²)
        return luminousFluxLumens / (4.0f * pi_f * distance * distance);
    }
    
    /**
     * @brief 根据亮度调整颜色
     * @param baseColor 基础颜色
     * @param luminance 亮度值，必须为非负值
     * @return 调整后的颜色
     */
    inline Color3f ApplyLuminance(const Color3f& baseColor, float luminance)
    {
        if (luminance < 0.0f) return Color3f(0.0f);
        return baseColor * luminance;
    }

}//namespace hgl::math
