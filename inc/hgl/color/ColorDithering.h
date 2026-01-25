#pragma once

#include<hgl/color/Color3f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/Color4f.h>
#include<hgl/color/Color4ub.h>
#include<hgl/color/ColorQuantize.h>
#include<vector>
#include<cstdint>

namespace hgl
{
    /**
     * @brief 颜色抖动算法
     *
     * 此模块提供各种抖动技术，通过在相邻像素间分散量化误差来改善量化颜色的视觉质量。
     *
     * 实现：见 src/Color/ColorDithering.cpp
     */

    //==================================================================================================
    // 误差扩散抖动
    //==================================================================================================

    /**
     * @brief Floyd-Steinberg 抖动误差系数
     * 
     * 经典的误差扩散抖动算法：
     *
     *      [当前]  7/16
     *  3/16  5/16  1/16
     */
    class FloydSteinbergDitherer
    {
    public:
        /**
         * @brief 根据位置对颜色应用 Floyd-Steinberg 抖动
         * @param original 原始颜色 [0, 1]
         * @param error_x X 坐标误差（像素位置）
         * @param error_y Y 坐标误差（像素位置）
         * @param bits 每通道的量化位数
         * @return 抖动后的颜色
         */
        static Color3f Dither(const Color3f &original, int error_x, int error_y, int bits);

        /**
         * @brief 应用 Floyd-Steinberg 抖动 (uint8 版本)
         */
        static Color3ub Dither(const Color3ub &original, int error_x, int error_y, int bits);
    };

    //==================================================================================================
    // 有序抖动（Bayer 矩阵）
    //==================================================================================================

    /**
     * @brief Bayer 矩阵抖动 - 使用预计算的阈值矩阵
     */
    class BayerDitherer
    {
    public:
        /**
         * @brief 对颜色应用 Bayer 抖动
         * @param original 原始颜色 [0, 1]
         * @param x 像素 X 坐标
         * @param y 像素 Y 坐标
         * @param levels 每通道的输出级数
         * @return 抖动后的颜色
         */
        static Color3f Dither(const Color3f &original, int x, int y, int levels);

        /**
         * @brief 应用 Bayer 抖动 (uint8 版本)
         */
        static Color3ub Dither(const Color3ub &original, int x, int y, int levels);

        /**
         * @brief 获取 Bayer 矩阵阈值
         */
        static float GetThreshold(int x, int y);
    };

    //==================================================================================================
    // 随机抖动
    //==================================================================================================

    /**
     * @brief 随机抖动 - 在量化前添加随机噪声
     */
    class RandomDitherer
    {
    private:
        class RandomImpl;
        RandomImpl* pImpl;

    public:
        /**
         * @brief 构造函数，可选种子
         */
        RandomDitherer(uint32_t seed = 0);

        /**
         * @brief 析构函数
         */
        ~RandomDitherer();

        /**
         * @brief 应用随机抖动
         * @param original 原始颜色 [0, 1]
         * @param noise_amount 噪声量 [0, 1]
         * @param bits 每通道的量化位数
         * @return 抖动后的颜色
         */
        Color3f Dither(const Color3f &original, float noise_amount = 0.05f, int bits = 5);

        /**
         * @brief 应用随机抖动 (uint8 版本)
         */
        Color3ub Dither(const Color3ub &original, float noise_amount = 0.05f, int bits = 5);

        /**
         * @brief 重新设置生成器种子
         */
        void SetSeed(uint32_t seed);
    };

    //==================================================================================================
    // 基于调色板的抖动
    //==================================================================================================

    /**
     * @brief 对颜色进行抖动并量化到特定的调色板
     * @param original 原始颜色
     * @param palette 颜色调色板
     * @param dither_pattern 可选的抖动值 [-0.5, 0.5]
     * @return 选中的调色板颜色的索引
     */
    int PaletteDither(const Color3f &original, const std::vector<Color3f> &palette, 
                     float dither_pattern = 0.0f);

} // namespace hgl
