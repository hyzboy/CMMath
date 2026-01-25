#pragma once

/**
 * @file GeometryCalculations.h
 * @brief Geometric volume calculations
 *
 * This file contains calculations for the volumes of various geometric shapes.
 * Previously part of MathUtils.h, now separated for better code organization.
 */

#include<numbers>
#include<cmath>

namespace hgl::math
{
    // ==================== Volume Calculations ====================

    /**
     * @brief Calculate the volume of a sphere
     * @param radius The radius of the sphere
     * @return The volume of the sphere = (4/3)πr³
     */
    constexpr double SphereVolume(const double radius)
    {
        return (radius * radius * radius * (std::numbers::pi_v<double> * 4.0)) / 3.0;
    }

    /**
     * @brief Calculate the volume of an ellipsoid
     * @param x_radius The radius along the x-axis
     * @param y_radius The radius along the y-axis
     * @param z_radius The radius along the z-axis
     * @return The volume of the ellipsoid = (4/3)π·x·y·z
     */
    constexpr double EllipsoidVolume(const double x_radius, const double y_radius, const double z_radius)
    {
        return (x_radius * y_radius * z_radius * (std::numbers::pi_v<double> * 4.0)) / 3.0;
    }

}//namespace hgl::math
