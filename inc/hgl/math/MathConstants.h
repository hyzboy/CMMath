/**
 * MathConstants.h - 基础数学常量
 * 
 * 包含：
 * - 自然常数 e 及其对数
 * - 圆周率 π 及其派生常量
 * - 根号 2 及其派生常量
 * 
 * C++20 风格：使用小写命名 + 下划线，inline constexpr
 * 保留大写别名以保持向后兼容
 */

#pragma once

namespace hgl::math
{
    // ==================== 自然常数 e ====================
    
    inline constexpr double e           = 2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274;  ///< 欧拉数(自然对数的底数)
    inline constexpr double log2e       = 1.44269504088896340736;                              ///< log₂(e)
    inline constexpr double log10e      = 0.434294481903251827651;                             ///< log₁₀(e)
    inline constexpr double ln2         = 0.693147180559945309417;                             ///< ln(2)
    inline constexpr double ln10        = 2.30258509299404568402;                              ///< ln(10)

    // ==================== 圆周率 π ====================
   
    inline constexpr double pi          = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068;  ///< 圆周率 π
    inline constexpr float  pi_f        = 3.14159265358979323846f;                           ///< 圆周率 π (float 版本)
    inline constexpr double pi_2        = 1.57079632679489661923;                              ///< π/2
    inline constexpr double pi_4        = 0.785398163397448309616;                             ///< π/4
    inline constexpr double pi_3_4      = 2.356194490192344928938;                             ///< 3π/4
    inline constexpr double inv_pi      = 0.318309886183790671538;                             ///< 1/π
    inline constexpr double two_pi      = 0.636619772367581343076;                             ///< 2/π
    inline constexpr double two_sqrtpi  = 1.12837916709551257390;                              ///< 2/√π

    // ==================== 根号常量 ====================
    
    inline constexpr double sqrt2       = 1.41421356237309504880168872420969807;              ///< √2
    inline constexpr double sqrt3       = 1.73205080756887729352744634150587236694280525381038; ///< √3
    inline constexpr double sqrt1_2     = 0.707106781186547524401;                             ///< √(1/2) = 1/√2

    // ==================== 黄金比例 ====================
    
    inline constexpr double phi         = 1.61803398874989484820458683436563811772030917980576; ///< 黄金比例 φ = (1 + √5)/2

    // ==================== 特殊数学常数 ====================
    
    inline constexpr double euler_gamma = 0.57721566490153286060651209008240243104215933593992; ///< 欧拉-马斯凯罗尼常数 γ
    inline constexpr double catalan_g   = 0.91596559417721901505460351493238411077414937428167; ///< 卡塔兰常数 G
    inline constexpr double apery_zeta3 = 1.20205690315959428539973816151144999076498629234049; ///< 阿培里常数 ζ(3)
}//namespace hgl::math
