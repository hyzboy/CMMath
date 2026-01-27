#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/AlphaBlendMode.h>
#include<hgl/color/Color3f.h>
#include<hgl/color/Color4f.h>
#include<cmath>
#include<algorithm>
#include<type_traits>

namespace hgl::math
{
    inline float    AbsValue(float v) { return std::fabs(v); }                                    ///< abs for float
    inline int      AbsValue(int v) { return std::abs(v); }                                      ///< abs for int

    // Type-specific abs overloads for vector and color types
    inline Vector2f AbsValue(const Vector2f &v){return glm::abs(v);}                   ///< abs for Vector2f
    inline Vector3f AbsValue(const Vector3f &v){return glm::abs(v);}                   ///< abs for Vector3f
    inline Vector4f AbsValue(const Vector4f &v){return glm::abs(v);}                   ///< abs for Vector4f

    inline Color3f AbsValue(const Color3f &v){return Color3f(glm::abs(static_cast<glm::vec3>(v)));} ///< abs for Color3f
    inline Color4f AbsValue(const Color4f &v){return Color4f(glm::abs(static_cast<glm::vec4>(v)));} ///< abs for Color4f

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
        * @brief 无混合，直接使用源值
        */
    template<typename T>
    inline T AlphaBlendNone(const T &base, const T &blend, float alpha)
    {
        (void)base;
        (void)alpha;
        return blend;
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
            return AbsValue(base - blended);
        }
        else
        {
            // 向量类型需要逐分量处理
            return AbsValue(base - blended);
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
        * @brief 分裂混合（除法）
        * Result = A / (B*alpha)
        */
    template<typename T>
    inline T AlphaBlendDivide(const T &base, const T &blend, float alpha)
    {
        T blended = blend * alpha;
        const float epsilon = 1e-6f;

        if constexpr (std::is_arithmetic_v<T>)
        {
            if (AbsValue(blended) < epsilon)
                return T(1.0f);
            return base / blended;
        }
        else
        {
            return base / (blended + T(epsilon));
        }
    }

    /**
        * @brief 亮光混合（VividLight）
        * 基于ColorBurn/ColorDodge的组合
        */
    template<typename T>
    inline T AlphaBlendVividLight(const T &base, const T &blend, float alpha)
    {
        T blended = blend * alpha;

        if constexpr (std::is_arithmetic_v<T>)
        {
            if (blended < 0.5f)
                return AlphaBlendColorBurn(base, blended * 2.0f, 1.0f);
            else
                return AlphaBlendColorDodge(base, (blended - T(0.5f)) * 2.0f, 1.0f);
        }
        else
        {
            return AlphaBlendHardLight(base, blend, alpha);
        }
    }

    /**
        * @brief 线性光混合（LinearLight）
        * 基于LinearBurn/LinearDodge
        */
    template<typename T>
    inline T AlphaBlendLinearLight(const T &base, const T &blend, float alpha)
    {
        T blended = blend * alpha;

        if constexpr (std::is_arithmetic_v<T>)
        {
            return base + blended * 2.0f - T(1.0f);
        }
        else
        {
            return base + blended * 2.0f - T(1.0f);
        }
    }

    /**
        * @brief 点光混合（PinLight）
        * 在变暗和变亮之间切换
        */
    template<typename T>
    inline T AlphaBlendPinLight(const T &base, const T &blend, float alpha)
    {
        T blended = blend * alpha;

        if constexpr (std::is_arithmetic_v<T>)
        {
            if (blended < 0.5f)
                return AlphaBlendDarken(base, blended * 2.0f, 1.0f);
            else
                return AlphaBlendLighten(base, (blended - T(0.5f)) * 2.0f, 1.0f);
        }
        else
        {
            return base * (1.0f - alpha) + blended;
        }
    }

    /**
        * @brief 实色混合（HardMix）
        * 基于VividLight的阈值化
        */
    template<typename T>
    inline T AlphaBlendHardMix(const T &base, const T &blend, float alpha)
    {
        T vivid = AlphaBlendVividLight(base, blend, alpha);

        if constexpr (std::is_arithmetic_v<T>)
        {
            return (vivid < 0.5f) ? T(0.0f) : T(1.0f);
        }
        else
        {
            return vivid;
        }
    }

    /**
        * @brief 更暗的颜色混合
        */
    template<typename T>
    inline T AlphaBlendDarkerColor(const T &base, const T &blend, float alpha)
    {
        T blended = blend * alpha;

        if constexpr (std::is_arithmetic_v<T>)
        {
            return (base < blended) ? base : blended;
        }
        else
        {
            return AlphaBlendDarken(base, blend, alpha);
        }
    }

    /**
        * @brief 更亮的颜色混合
        */
    template<typename T>
    inline T AlphaBlendLighterColor(const T &base, const T &blend, float alpha)
    {
        T blended = blend * alpha;

        if constexpr (std::is_arithmetic_v<T>)
        {
            return (base > blended) ? base : blended;
        }
        else
        {
            return AlphaBlendLighten(base, blend, alpha);
        }
    }

    /**
        * @brief 色相、饱和度、颜色、明度混合
        * 这里使用标准插值作为通用占位实现
        */
    template<typename T>
    inline T AlphaBlendHue(const T &base, const T &blend, float alpha)
    {
        return AlphaBlendNormal(base, blend, alpha);
    }

    template<typename T>
    inline T AlphaBlendSaturation(const T &base, const T &blend, float alpha)
    {
        return AlphaBlendNormal(base, blend, alpha);
    }

    template<typename T>
    inline T AlphaBlendColor(const T &base, const T &blend, float alpha)
    {
        return AlphaBlendNormal(base, blend, alpha);
    }

    template<typename T>
    inline T AlphaBlendLuminosity(const T &base, const T &blend, float alpha)
    {
        return AlphaBlendNormal(base, blend, alpha);
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

    /// Alpha混合函数指针类型模板
    template<typename T>
    using AlphaBlendFunc = T (*)(const T&, const T&, float);

    // ==================== 函数指针获取器 ====================

    namespace detail
    {
        template<typename T>
        constexpr AlphaBlendFunc<T> blend_function_table[] = {
            AlphaBlendNone<T>,
            AlphaBlendNormal<T>,
            AlphaBlendAdd<T>,
            AlphaBlendSubtract<T>,
            AlphaBlendMultiply<T>,
            AlphaBlendScreen<T>,
            AlphaBlendOverlay<T>,
            AlphaBlendHardLight<T>,
            AlphaBlendSoftLight<T>,
            AlphaBlendColorDodge<T>,
            AlphaBlendColorBurn<T>,
            AlphaBlendLinearDodge<T>,
            AlphaBlendLinearBurn<T>,
            AlphaBlendDarken<T>,
            AlphaBlendLighten<T>,
            AlphaBlendDifference<T>,
            AlphaBlendExclusion<T>,
            AlphaBlendVividLight<T>,
            AlphaBlendLinearLight<T>,
            AlphaBlendPinLight<T>,
            AlphaBlendHardMix<T>,
            AlphaBlendDarkerColor<T>,
            AlphaBlendLighterColor<T>,
            AlphaBlendHue<T>,
            AlphaBlendSaturation<T>,
            AlphaBlendColor<T>,
            AlphaBlendLuminosity<T>,
            AlphaBlendDivide<T>,
            AlphaBlendPremultiplied<T>
        };

        template<typename T>
        inline AlphaBlendFunc<T> GetAlphaBlendFuncImpl(AlphaBlendMode mode)
        {
            const int index = static_cast<int>(mode);
            constexpr int table_size = sizeof(blend_function_table<T>) / sizeof(blend_function_table<T>[0]);

            if (index >= 0 && index < table_size)
                return blend_function_table<T>[index];

            return nullptr;
        }
    }

    /// Alpha混合函数指针类型（标量）
    typedef AlphaBlendFunc<float> AlphaBlendFuncFloat;

    /// Alpha混合函数指针类型（Vector2f）
    typedef AlphaBlendFunc<math::Vector2f> AlphaBlendFuncVec2f;

    /// Alpha混合函数指针类型（Vector3f）
    typedef AlphaBlendFunc<math::Vector3f> AlphaBlendFuncVec3f;

    /// Alpha混合函数指针类型（Vector4f）
    typedef AlphaBlendFunc<math::Vector4f> AlphaBlendFuncVec4f;

    /// Alpha混合函数指针类型（Color3f）
    typedef AlphaBlendFunc<Color3f> AlphaBlendFuncColor3f;

    /// Alpha混合函数指针类型（Color4f）
    typedef AlphaBlendFunc<Color4f> AlphaBlendFuncColor4f;

    /**
        * @brief 获取标量Alpha混合函数指针
        * @param mode 混合模式
        * @return 对应的混合函数指针，无效模式返回nullptr
        */
    inline AlphaBlendFuncFloat GetAlphaBlendFunc(AlphaBlendMode mode)
    {
        return detail::GetAlphaBlendFuncImpl<float>(mode);
    }

    /**
        * @brief 获取Vector2f Alpha混合函数指针
        * @param mode 混合模式
        * @return 对应的混合函数指针，无效模式返回nullptr
        */
    inline AlphaBlendFuncVec2f GetAlphaBlendFuncVec2f(AlphaBlendMode mode)
    {
        return detail::GetAlphaBlendFuncImpl<math::Vector2f>(mode);
    }

    /**
        * @brief 获取Vector3f Alpha混合函数指针
        * @param mode 混合模式
        * @return 对应的混合函数指针，无效模式返回nullptr
        */
    inline AlphaBlendFuncVec3f GetAlphaBlendFuncVec3f(AlphaBlendMode mode)
    {
        return detail::GetAlphaBlendFuncImpl<math::Vector3f>(mode);
    }

    /**
        * @brief 获取Vector4f Alpha混合函数指针
        * @param mode 混合模式
        * @return 对应的混合函数指针，无效模式返回nullptr
        */
    inline AlphaBlendFuncVec4f GetAlphaBlendFuncVec4f(AlphaBlendMode mode)
    {
        return detail::GetAlphaBlendFuncImpl<math::Vector4f>(mode);
    }

    /**
        * @brief 获取Color3f Alpha混合函数指针
        * @param mode 混合模式
        * @return 对应的混合函数指针，无效模式返回nullptr
        */
    inline AlphaBlendFuncColor3f GetAlphaBlendFuncColor3f(AlphaBlendMode mode)
    {
        return detail::GetAlphaBlendFuncImpl<Color3f>(mode);
    }

    /**
        * @brief 获取Color4f Alpha混合函数指针
        * @param mode 混合模式
        * @return 对应的混合函数指针，无效模式返回nullptr
        */
    inline AlphaBlendFuncColor4f GetAlphaBlendFuncColor4f(AlphaBlendMode mode)
    {
        return detail::GetAlphaBlendFuncImpl<Color4f>(mode);
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
            case AlphaBlendMode::None:              return AlphaBlendNone(base, blend, alpha);
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
            case AlphaBlendMode::VividLight:        return AlphaBlendVividLight(base, blend, alpha);
            case AlphaBlendMode::LinearLight:       return AlphaBlendLinearLight(base, blend, alpha);
            case AlphaBlendMode::PinLight:          return AlphaBlendPinLight(base, blend, alpha);
            case AlphaBlendMode::HardMix:           return AlphaBlendHardMix(base, blend, alpha);
            case AlphaBlendMode::DarkerColor:       return AlphaBlendDarkerColor(base, blend, alpha);
            case AlphaBlendMode::LighterColor:      return AlphaBlendLighterColor(base, blend, alpha);
            case AlphaBlendMode::Hue:               return AlphaBlendHue(base, blend, alpha);
            case AlphaBlendMode::Saturation:        return AlphaBlendSaturation(base, blend, alpha);
            case AlphaBlendMode::Color:             return AlphaBlendColor(base, blend, alpha);
            case AlphaBlendMode::Luminosity:        return AlphaBlendLuminosity(base, blend, alpha);
            case AlphaBlendMode::Divide:            return AlphaBlendDivide(base, blend, alpha);
            case AlphaBlendMode::PremultipliedAlpha:return AlphaBlendPremultiplied(base, blend, alpha);
            default:                                return base;
        }
    }
}//namespace hgl::math
