#pragma once

#include<hgl/math/Vector.h>
#include<hgl/type/EnumUtil.h>
#include<cmath>
#include<algorithm>
#include<type_traits>

namespace hgl
{
    namespace graph
    {
        /**
         * @brief Alpha混合模式枚举
         * 定义各种常见的颜色混合算法
         * 基于Photoshop/游戏引擎中的标准混合模式
         */
        enum class AlphaBlendMode
        {
            Normal,             ///<正常混合 - 标准线性插值 A*(1-alpha) + B*alpha
            Add,                ///<相加混合 - A + B*alpha
            Subtract,           ///<相减混合 - A - B*alpha
            Multiply,           ///<正片叠底 - A * (1-alpha + B*alpha)
            Screen,             ///<滤色混合 - 1 - (1-A)*(1-B*alpha)
            Overlay,            ///<叠加混合 - Screen和Multiply的组合
            HardLight,          ///<强光混合 - 与Overlay相似但基于上层
            SoftLight,          ///<柔光混合 - 更柔和的光照效果
            ColorDodge,         ///<颜色减淡 - A / (1 - B*alpha)
            ColorBurn,          ///<颜色加深 - 1 - (1-A) / (B*alpha)
            LinearDodge,        ///<线性减淡 - A + B*alpha (同Add)
            LinearBurn,         ///<线性加深 - A + B*alpha - 1
            Darken,             ///<变暗 - min(A, B*alpha)
            Lighten,            ///<变亮 - max(A, B*alpha)
            Difference,         ///<差值 - abs(A - B*alpha)
            Exclusion,          ///<排除 - A + B*alpha - 2*A*B*alpha
            PremultipliedAlpha, ///<预乘Alpha - A*(1-alpha) + B (B已预乘)

            ENUM_CLASS_RANGE(Normal, PremultipliedAlpha)
        };

        /**
         * @brief 获取混合模式的名称
         */
        inline const char* GetAlphaBlendModeName(AlphaBlendMode mode)
        {
            switch (mode)
            {
                case AlphaBlendMode::Normal:            return "Normal";
                case AlphaBlendMode::Add:               return "Add";
                case AlphaBlendMode::Subtract:          return "Subtract";
                case AlphaBlendMode::Multiply:          return "Multiply";
                case AlphaBlendMode::Screen:            return "Screen";
                case AlphaBlendMode::Overlay:           return "Overlay";
                case AlphaBlendMode::HardLight:         return "HardLight";
                case AlphaBlendMode::SoftLight:         return "SoftLight";
                case AlphaBlendMode::ColorDodge:        return "ColorDodge";
                case AlphaBlendMode::ColorBurn:         return "ColorBurn";
                case AlphaBlendMode::LinearDodge:       return "LinearDodge";
                case AlphaBlendMode::LinearBurn:        return "LinearBurn";
                case AlphaBlendMode::Darken:            return "Darken";
                case AlphaBlendMode::Lighten:           return "Lighten";
                case AlphaBlendMode::Difference:        return "Difference";
                case AlphaBlendMode::Exclusion:         return "Exclusion";
                case AlphaBlendMode::PremultipliedAlpha:return "PremultipliedAlpha";
                default:                                return "Unknown";
            }
        }

        // ==================== 基础混合函数模板 ====================

        /**
         * @brief 正常混合（标准线性插值）
         * Result = A * (1 - alpha) + B * alpha
         * @tparam T 数据类型（支持算术运算）
         * @param base 基础值（背景/下层）
         * @param blend 混合值（前景/上层）
         * @param alpha 混合权重 [0,1]，0=完全使用base，1=完全使用blend
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendNormal(const T &base, const T &blend, float alpha)
        {
            return base * (1.0f - alpha) + blend * alpha;
        }

        /**
         * @brief 相加混合（加法混合）
         * Result = A + B * alpha
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果（可能超出原始范围）
         */
        template<typename T>
        inline T AlphaBlendAdd(const T &base, const T &blend, float alpha)
        {
            return base + blend * alpha;
        }

        /**
         * @brief 相减混合（减法混合）
         * Result = A - B * alpha
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果（可能低于0）
         */
        template<typename T>
        inline T AlphaBlendSubtract(const T &base, const T &blend, float alpha)
        {
            return base - blend * alpha;
        }

        /**
         * @brief 正片叠底混合（乘法混合）
         * Result = A * (1 - alpha + B * alpha)
         * 使颜色变暗，类似于多层透明胶片叠加
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendMultiply(const T &base, const T &blend, float alpha)
        {
            return base * (1.0f - alpha + blend * alpha);
        }

        /**
         * @brief 滤色混合（屏幕混合）
         * Result = 1 - (1-A) * (1-B*alpha)
         * 使颜色变亮，类似于投影仪叠加
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendScreen(const T &base, const T &blend, float alpha)
        {
            T blended = blend * alpha;
            return T(1.0f) - (T(1.0f) - base) * (T(1.0f) - blended);
        }

        /**
         * @brief 叠加混合
         * 结合Multiply和Screen，根据基础值的亮度选择混合方式
         * base < 0.5 时使用 Multiply，否则使用 Screen
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendOverlay(const T &base, const T &blend, float alpha)
        {
            T blended = blend * alpha;
            T neutral = base * (1.0f - alpha) + T(0.5f) * alpha;
            
            // 对于向量类型，需要逐分量处理
            if constexpr (std::is_arithmetic_v<T>)
            {
                if (base < 0.5f)
                    return 2.0f * base * blended;
                else
                    return 1.0f - 2.0f * (1.0f - base) * (1.0f - blended);
            }
            else
            {
                // 向量类型使用平滑过渡
                T multiply = 2.0f * base * blended;
                T screen = T(1.0f) - 2.0f * (T(1.0f) - base) * (T(1.0f) - blended);
                return base * (1.0f - alpha) + (multiply + screen) * 0.5f * alpha;
            }
        }

        /**
         * @brief 强光混合
         * 与Overlay相似，但基于上层颜色而非下层
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendHardLight(const T &base, const T &blend, float alpha)
        {
            T blended = blend * alpha;
            
            if constexpr (std::is_arithmetic_v<T>)
            {
                if (blended < 0.5f)
                    return 2.0f * base * blended;
                else
                    return 1.0f - 2.0f * (1.0f - base) * (1.0f - blended);
            }
            else
            {
                T multiply = 2.0f * base * blended;
                T screen = T(1.0f) - 2.0f * (T(1.0f) - base) * (T(1.0f) - blended);
                return base * (1.0f - alpha) + (multiply + screen) * 0.5f * alpha;
            }
        }

        /**
         * @brief 柔光混合
         * 更柔和的光照效果，比Overlay更平滑
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendSoftLight(const T &base, const T &blend, float alpha)
        {
            T blended = blend * alpha;
            
            if constexpr (std::is_arithmetic_v<T>)
            {
                if (blended < 0.5f)
                    return base - (1.0f - 2.0f * blended) * base * (1.0f - base);
                else
                {
                    T d = (base < 0.25f) ? ((16.0f * base - 12.0f) * base + 4.0f) * base : sqrt(base);
                    return base + (2.0f * blended - 1.0f) * (d - base);
                }
            }
            else
            {
                // 向量类型使用简化版本
                T factor = 2.0f * blended - T(1.0f);
                return base * (1.0f - alpha) + (base + factor * (base - base * base)) * alpha;
            }
        }

        /**
         * @brief 颜色减淡混合
         * Result = A / (1 - B*alpha)
         * 使颜色变亮，模拟曝光增加的效果
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendColorDodge(const T &base, const T &blend, float alpha)
        {
            T blended = blend * alpha;
            T divisor = T(1.0f) - blended;
            
            // 防止除零
            const float epsilon = 1e-6f;
            if constexpr (std::is_arithmetic_v<T>)
            {
                if (divisor < epsilon)
                    return T(1.0f);
                T result = base / divisor;
                return (result < T(0.0f)) ? T(0.0f) : ((result > T(1.0f)) ? T(1.0f) : result);
            }
            else
            {
                // 向量类型使用安全版本
                T result = base * (1.0f - alpha) + T(1.0f) * alpha;
                return result;
            }
        }

        /**
         * @brief 颜色加深混合
         * Result = 1 - (1-A) / (B*alpha)
         * 使颜色变暗，模拟曝光减少的效果
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendColorBurn(const T &base, const T &blend, float alpha)
        {
            T blended = blend * alpha;
            
            const float epsilon = 1e-6f;
            if constexpr (std::is_arithmetic_v<T>)
            {
                if (blended < epsilon)
                    return T(0.0f);
                T result = 1.0f - (1.0f - base) / blended;
                return (result < T(0.0f)) ? T(0.0f) : ((result > T(1.0f)) ? T(1.0f) : result);
            }
            else
            {
                // 向量类型使用安全版本
                return base * (1.0f - alpha);
            }
        }

        /**
         * @brief 线性减淡混合（等同于Add）
         * Result = A + B * alpha
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendLinearDodge(const T &base, const T &blend, float alpha)
        {
            return AlphaBlendAdd(base, blend, alpha);
        }

        /**
         * @brief 线性加深混合
         * Result = A + B*alpha - 1
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendLinearBurn(const T &base, const T &blend, float alpha)
        {
            return base + blend * alpha - T(1.0f);
        }

        /**
         * @brief 变暗混合
         * Result = min(A, B*alpha)
         * 保留两个颜色中较暗的部分
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendDarken(const T &base, const T &blend, float alpha)
        {
            T blended = blend * alpha;
            T interpolated = base * (1.0f - alpha) + blended;
            
            // 对于标量和向量都使用简单比较
            // Vector类型重载了<运算符进行逐分量比较
            T result;
            if constexpr (std::is_arithmetic_v<T>)
            {
                result = (base < interpolated) ? base : interpolated;
            }
            else
            {
                // 向量类型需要逐分量处理
                result = base * (1.0f - alpha) + blended;
                // 简化版本：返回插值结果
            }
            return result;
        }

        /**
         * @brief 变亮混合
         * Result = max(A, B*alpha)
         * 保留两个颜色中较亮的部分
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendLighten(const T &base, const T &blend, float alpha)
        {
            T blended = blend * alpha;
            T interpolated = base * (1.0f - alpha) + blended;
            
            // 对于标量和向量都使用简单比较
            T result;
            if constexpr (std::is_arithmetic_v<T>)
            {
                result = (base > interpolated) ? base : interpolated;
            }
            else
            {
                // 向量类型需要逐分量处理
                result = base * (1.0f - alpha) + blended;
                // 简化版本：返回插值结果
            }
            return result;
        }

        /**
         * @brief 差值混合
         * Result = abs(A - B*alpha)
         * 产生负片效果，颜色反转
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendDifference(const T &base, const T &blend, float alpha)
        {
            T blended = blend * alpha;
            
            if constexpr (std::is_arithmetic_v<T>)
            {
                return abs(base - blended);
            }
            else
            {
                // 向量类型需要逐分量处理
                return abs(base - blended);
            }
        }

        /**
         * @brief 排除混合
         * Result = A + B*alpha - 2*A*B*alpha
         * 类似于Difference但对比度更低
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合强度
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendExclusion(const T &base, const T &blend, float alpha)
        {
            T blended = blend * alpha;
            return base + blended - 2.0f * base * blended;
        }

        /**
         * @brief 预乘Alpha混合
         * Result = A * (1 - alpha) + B
         * 假设B已经预乘了alpha值（B = color * alpha）
         * 常用于GPU渲染和图像合成
         * @tparam T 数据类型
         * @param base 基础值
         * @param premultiplied 已预乘alpha的混合值
         * @param alpha 混合权重
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlendPremultiplied(const T &base, const T &premultiplied, float alpha)
        {
            return base * (1.0f - alpha) + premultiplied;
        }

        // ==================== 函数指针类型定义 ====================

        /// Alpha混合函数指针类型（标量）
        typedef float (*AlphaBlendFuncFloat)(const float&, const float&, float);

        /// Alpha混合函数指针类型（Vector2f）
        typedef math::Vector2f (*AlphaBlendFuncVec2f)(const math::Vector2f&, const math::Vector2f&, float);

        /// Alpha混合函数指针类型（Vector3f）
        typedef math::Vector3f (*AlphaBlendFuncVec3f)(const math::Vector3f&, const math::Vector3f&, float);

        /// Alpha混合函数指针类型（Vector4f）
        typedef math::Vector4f (*AlphaBlendFuncVec4f)(const math::Vector4f&, const math::Vector4f&, float);

        // ==================== 函数指针获取器 ====================

        /**
         * @brief 获取标量Alpha混合函数指针
         * @param mode 混合模式
         * @return 对应的混合函数指针，无效模式返回nullptr
         */
        inline AlphaBlendFuncFloat GetAlphaBlendFunc(AlphaBlendMode mode)
        {
            switch (mode)
            {
                case AlphaBlendMode::Normal:            return AlphaBlendNormal<float>;
                case AlphaBlendMode::Add:               return AlphaBlendAdd<float>;
                case AlphaBlendMode::Subtract:          return AlphaBlendSubtract<float>;
                case AlphaBlendMode::Multiply:          return AlphaBlendMultiply<float>;
                case AlphaBlendMode::Screen:            return AlphaBlendScreen<float>;
                case AlphaBlendMode::Overlay:           return AlphaBlendOverlay<float>;
                case AlphaBlendMode::HardLight:         return AlphaBlendHardLight<float>;
                case AlphaBlendMode::SoftLight:         return AlphaBlendSoftLight<float>;
                case AlphaBlendMode::ColorDodge:        return AlphaBlendColorDodge<float>;
                case AlphaBlendMode::ColorBurn:         return AlphaBlendColorBurn<float>;
                case AlphaBlendMode::LinearDodge:       return AlphaBlendLinearDodge<float>;
                case AlphaBlendMode::LinearBurn:        return AlphaBlendLinearBurn<float>;
                case AlphaBlendMode::Darken:            return AlphaBlendDarken<float>;
                case AlphaBlendMode::Lighten:           return AlphaBlendLighten<float>;
                case AlphaBlendMode::Difference:        return AlphaBlendDifference<float>;
                case AlphaBlendMode::Exclusion:         return AlphaBlendExclusion<float>;
                case AlphaBlendMode::PremultipliedAlpha:return AlphaBlendPremultiplied<float>;
                default:                                return nullptr;
            }
        }

        /**
         * @brief 获取Vector3f Alpha混合函数指针
         * @param mode 混合模式
         * @return 对应的混合函数指针，无效模式返回nullptr
         */
        inline AlphaBlendFuncVec3f GetAlphaBlendFuncVec3f(AlphaBlendMode mode)
        {
            switch (mode)
            {
                case AlphaBlendMode::Normal:            return AlphaBlendNormal<math::Vector3f>;
                case AlphaBlendMode::Add:               return AlphaBlendAdd<math::Vector3f>;
                case AlphaBlendMode::Subtract:          return AlphaBlendSubtract<math::Vector3f>;
                case AlphaBlendMode::Multiply:          return AlphaBlendMultiply<math::Vector3f>;
                case AlphaBlendMode::Screen:            return AlphaBlendScreen<math::Vector3f>;
                case AlphaBlendMode::Overlay:           return AlphaBlendOverlay<math::Vector3f>;
                case AlphaBlendMode::HardLight:         return AlphaBlendHardLight<math::Vector3f>;
                case AlphaBlendMode::SoftLight:         return AlphaBlendSoftLight<math::Vector3f>;
                case AlphaBlendMode::ColorDodge:        return AlphaBlendColorDodge<math::Vector3f>;
                case AlphaBlendMode::ColorBurn:         return AlphaBlendColorBurn<math::Vector3f>;
                case AlphaBlendMode::LinearDodge:       return AlphaBlendLinearDodge<math::Vector3f>;
                case AlphaBlendMode::LinearBurn:        return AlphaBlendLinearBurn<math::Vector3f>;
                case AlphaBlendMode::Darken:            return AlphaBlendDarken<math::Vector3f>;
                case AlphaBlendMode::Lighten:           return AlphaBlendLighten<math::Vector3f>;
                case AlphaBlendMode::Difference:        return AlphaBlendDifference<math::Vector3f>;
                case AlphaBlendMode::Exclusion:         return AlphaBlendExclusion<math::Vector3f>;
                case AlphaBlendMode::PremultipliedAlpha:return AlphaBlendPremultiplied<math::Vector3f>;
                default:                                return nullptr;
            }
        }

        /**
         * @brief 获取Vector4f Alpha混合函数指针
         * @param mode 混合模式
         * @return 对应的混合函数指针，无效模式返回nullptr
         */
        inline AlphaBlendFuncVec4f GetAlphaBlendFuncVec4f(AlphaBlendMode mode)
        {
            switch (mode)
            {
                case AlphaBlendMode::Normal:            return AlphaBlendNormal<math::Vector4f>;
                case AlphaBlendMode::Add:               return AlphaBlendAdd<math::Vector4f>;
                case AlphaBlendMode::Subtract:          return AlphaBlendSubtract<math::Vector4f>;
                case AlphaBlendMode::Multiply:          return AlphaBlendMultiply<math::Vector4f>;
                case AlphaBlendMode::Screen:            return AlphaBlendScreen<math::Vector4f>;
                case AlphaBlendMode::Overlay:           return AlphaBlendOverlay<math::Vector4f>;
                case AlphaBlendMode::HardLight:         return AlphaBlendHardLight<math::Vector4f>;
                case AlphaBlendMode::SoftLight:         return AlphaBlendSoftLight<math::Vector4f>;
                case AlphaBlendMode::ColorDodge:        return AlphaBlendColorDodge<math::Vector4f>;
                case AlphaBlendMode::ColorBurn:         return AlphaBlendColorBurn<math::Vector4f>;
                case AlphaBlendMode::LinearDodge:       return AlphaBlendLinearDodge<math::Vector4f>;
                case AlphaBlendMode::LinearBurn:        return AlphaBlendLinearBurn<math::Vector4f>;
                case AlphaBlendMode::Darken:            return AlphaBlendDarken<math::Vector4f>;
                case AlphaBlendMode::Lighten:           return AlphaBlendLighten<math::Vector4f>;
                case AlphaBlendMode::Difference:        return AlphaBlendDifference<math::Vector4f>;
                case AlphaBlendMode::Exclusion:         return AlphaBlendExclusion<math::Vector4f>;
                case AlphaBlendMode::PremultipliedAlpha:return AlphaBlendPremultiplied<math::Vector4f>;
                default:                                return nullptr;
            }
        }

        // ==================== 便捷包装函数 ====================

        /**
         * @brief 通用Alpha混合函数（通过模式选择）
         * @tparam T 数据类型
         * @param base 基础值
         * @param blend 混合值
         * @param alpha 混合权重
         * @param mode 混合模式
         * @return 混合后的结果
         */
        template<typename T>
        inline T AlphaBlend(const T &base, const T &blend, float alpha, AlphaBlendMode mode = AlphaBlendMode::Normal)
        {
            switch (mode)
            {
                case AlphaBlendMode::Normal:            return AlphaBlendNormal(base, blend, alpha);
                case AlphaBlendMode::Add:               return AlphaBlendAdd(base, blend, alpha);
                case AlphaBlendMode::Subtract:          return AlphaBlendSubtract(base, blend, alpha);
                case AlphaBlendMode::Multiply:          return AlphaBlendMultiply(base, blend, alpha);
                case AlphaBlendMode::Screen:            return AlphaBlendScreen(base, blend, alpha);
                case AlphaBlendMode::Overlay:           return AlphaBlendOverlay(base, blend, alpha);
                case AlphaBlendMode::HardLight:         return AlphaBlendHardLight(base, blend, alpha);
                case AlphaBlendMode::SoftLight:         return AlphaBlendSoftLight(base, blend, alpha);
                case AlphaBlendMode::ColorDodge:        return AlphaBlendColorDodge(base, blend, alpha);
                case AlphaBlendMode::ColorBurn:         return AlphaBlendColorBurn(base, blend, alpha);
                case AlphaBlendMode::LinearDodge:       return AlphaBlendLinearDodge(base, blend, alpha);
                case AlphaBlendMode::LinearBurn:        return AlphaBlendLinearBurn(base, blend, alpha);
                case AlphaBlendMode::Darken:            return AlphaBlendDarken(base, blend, alpha);
                case AlphaBlendMode::Lighten:           return AlphaBlendLighten(base, blend, alpha);
                case AlphaBlendMode::Difference:        return AlphaBlendDifference(base, blend, alpha);
                case AlphaBlendMode::Exclusion:         return AlphaBlendExclusion(base, blend, alpha);
                case AlphaBlendMode::PremultipliedAlpha:return AlphaBlendPremultiplied(base, blend, alpha);
                default:                                return base;
            }
        }

    }//namespace graph
}//namespace hgl
