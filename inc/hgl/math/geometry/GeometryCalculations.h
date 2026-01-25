#pragma once

/**
 * @file GeometryCalculations.h
 * @brief 各类几何体体积计算
 *
 * 本文件包含各种几何体体积的计算。
 * 之前属于 MathUtils.h，为了更好的代码组织已独立。
 */

#include<numbers>
#include<cmath>

namespace hgl::math
{
    // ==================== 体积计算 ====================

    /**
     * @brief 计算球体体积
     * @param radius 球体半径
     * @return 球体体积 = (4/3)πr³
     */
    constexpr double SphereVolume(const double radius)
    {
        return (radius * radius * radius * (std::numbers::pi_v<double> * 4.0)) / 3.0;
    }

    /**
     * @brief 计算椭球体体积
     * @param x_radius x轴半径
     * @param y_radius y轴半径
     * @param z_radius z轴半径
     * @return 椭球体体积 = (4/3)π·x·y·z
     */
    constexpr double EllipsoidVolume(const double x_radius, const double y_radius, const double z_radius)
    {
        return (x_radius * y_radius * z_radius * (std::numbers::pi_v<double> * 4.0)) / 3.0;
    }

}//namespace hgl::math
