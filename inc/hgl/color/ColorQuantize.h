#pragma once

#include<hgl/color/Color3f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/Color4f.h>
#include<hgl/color/Color4ub.h>
#include<vector>

namespace hgl
{
    /**
     * @brief 颜色量化算法
     *
     * 此模块提供各种颜色量化方法，用于减少图像中的颜色数量，同时保持视觉质量。
     *
     * 实现：见 src/Color/ColorQuantize.cpp
     */

    //==================================================================================================
    // 基于位的量化
    //==================================================================================================

    /**
     * @brief 将颜色量化到指定的每通道位数
     * @param color 输入颜色 [0, 1]
     * @param bits 每通道的位数 (1-8)
     * @return 量化后的颜色
     */
    Color3f QuantizeColor3f(const Color3f &color, int bits);

    /**
     * @brief 将颜色量化到指定的每通道位数
     * @param color 输入颜色 [0, 255]
     * @param bits 每通道的位数 (1-8)
     * @return 量化后的颜色
     */
    Color3ub QuantizeColor3ub(const Color3ub &color, int bits);

    /**
     * @brief 将颜色量化到指定的每通道位数
     * @param color 输入颜色 [0, 1]
     * @param bits 每通道的位数 (1-8)
     * @return 量化后的颜色
     */
    Color4f QuantizeColor4f(const Color4f &color, int bits);

    /**
     * @brief 将颜色量化到指定的每通道位数
     * @param color 输入颜色 [0, 255]
     * @param bits 每通道的位数 (1-8)
     * @return 量化后的颜色
     */
    Color4ub QuantizeColor4ub(const Color4ub &color, int bits);

    //==================================================================================================
    // 均匀量化
    //==================================================================================================

    /**
     * @brief 均匀颜色量化 - 将调色板缩减为均匀网格
     * @param color 输入颜色 [0, 1]
     * @param levels 每通道的级数 (2-256)
     * @return 量化后的颜色
     */
    Color3f UniformQuantize3f(const Color3f &color, int levels);

    //==================================================================================================
    // 误差计算
    //==================================================================================================

    /**
     * @brief 计算量化误差（平方欧几里得距离）
     * @param original 原始颜色
     * @param quantized 量化后的颜色
     * @return 量化误差值
     */
    float QuantizationError3f(const Color3f &original, const Color3f &quantized);

    /**
     * @brief 计算 uint8 颜色的量化误差
     * @param original 原始颜色
     * @param quantized 量化后的颜色
     * @return 量化误差值
     */
    float QuantizationError3ub(const Color3ub &original, const Color3ub &quantized);

    //==================================================================================================
    // 基于调色板的量化
    //==================================================================================================

    /**
     * @brief 在调色板中查找最近的颜色
     * @param color 目标颜色
     * @param palette 调色板颜色向量
     * @return 调色板中最近颜色的索引
     */
    int FindNearestPaletteColor(const Color3f &color, const std::vector<Color3f> &palette);

    /**
     * @brief 在调色板中查找最近的颜色 (uint8)
     * @param color 目标颜色
     * @param palette 调色板颜色向量
     * @return 调色板中最近颜色的索引
     */
    int FindNearestPaletteColor(const Color3ub &color, const std::vector<Color3ub> &palette);

    /**
     * @brief 获取调色板中的颜色距离（用于调试）
     * @param color 目标颜色
     * @param palette_color 调色板颜色
     * @return 距离值（[0,1] 范围内的欧几里得距离）
     */
    float GetPaletteColorDistance(const Color3f &color, const Color3f &palette_color);

} // namespace hgl
