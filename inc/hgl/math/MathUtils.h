/**
 * MathUtils.h - 数学工具函数
 *
 * 包含：
 * - 浮点数精度截取 (hgl_clip_float)
 *
 * 注意：体积计算函数已移至 geometry/GeometryCalculations.h
 */

#pragma once

#include<numbers>
#include<cmath>

namespace hgl
{
    // ==================== 浮点数处理 ====================

    /**
     * 浮点数截取小数点后指定位数
     * @param value 要截取的浮点数
     * @param num 要截取的位数
     * @return 截取后的浮点数
     *
     * 示例：
     * hgl_clip_float(3.14159f, 2) 返回 3.14
     * hgl_clip_float(3.14159f, 0) 返回 3
     */
    template<typename T>
    inline T hgl_clip_float(const T value, const int num)
    {
        if (num <= 0)
            return int64(value);

        if (value == 0)
            return (0);

        double per = std::pow(10.0, num);

        const double clipped = std::floor(value * per) / per;
        return static_cast<T>(clipped);
    }
}//namespace hgl
