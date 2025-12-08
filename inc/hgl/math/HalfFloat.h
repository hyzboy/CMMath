#pragma once

#include<hgl/math/Clamp.h>
#include<hgl/CoreType.h>

namespace hgl::math
{
    /**
     * Half Float (16位浮点数) 转换函数
     * Bit depth: 16 - Sign: 1bit, Exponent: 5bits, Mantissa: 10bits
     */

    /**
    * 标准版float32转float16处理
    */
    void Float32toFloat16(half_float *output, const float *input, const uint count);

    /**
     * half_float转float (单个值)
     * 正确处理零、非规格化数、正常数、无穷大和NaN
     */
    inline float half2float(const half_float &hf)
    {
        union
        {
            float f;
            uint32 u;
        }x;

        uint32 sign = (hf & 0x8000) << 16;  // 符号位
        uint32 exp = (hf & 0x7C00) >> 10;   // 指数 (5位)
        uint32 mantissa = hf & 0x03FF;       // 尾数 (10位)

        if (exp == 0x1F)  // 无穷大或NaN
        {
            if (mantissa == 0)
                x.u = sign | 0x7F800000;  // 无穷大
            else
                x.u = sign | 0x7FC00000;  // NaN
        }
        else if (exp == 0)  // 零或非规格化数
        {
            if (mantissa == 0)
                x.u = sign;  // 零
            else
            {
                // 非规格化数：需要规格化
                // 找到最高位1的位置
                exp = 1;
                while ((mantissa & 0x0400) == 0)
                {
                    mantissa <<= 1;
                    exp--;
                }
                mantissa &= 0x03FF;  // 移除隐含的1
                x.u = sign | ((exp + 127 - 15) << 23) | (mantissa << 13);
            }
        }
        else  // 正常数
        {
            x.u = sign | ((exp + 127 - 15) << 23) | (mantissa << 13);
        }

        return x.f;
    }

    /**
     * half_float批量转换为float (位格式)
     * 正确处理零、非规格化数、正常数、无穷大和NaN
     */
    inline void half_to_float(uint32 *target, const half_float *source, uint32 count)
    {
        while (count--)
        {
            half_float hf = *source;
            uint32 sign = (hf & 0x8000) << 16;
            uint32 exp = (hf & 0x7C00) >> 10;
            uint32 mantissa = hf & 0x03FF;

            if (exp == 0x1F)  // 无穷大或NaN
            {
                if (mantissa == 0)
                    *target = sign | 0x7F800000;  // 无穷大
                else
                    *target = sign | 0x7FC00000;  // NaN
            }
            else if (exp == 0)  // 零或非规格化数
            {
                if (mantissa == 0)
                    *target = sign;  // 零
                else
                {
                    // 非规格化数：需要规格化
                    exp = 1;
                    while ((mantissa & 0x0400) == 0)
                    {
                        mantissa <<= 1;
                        exp--;
                    }
                    mantissa &= 0x03FF;
                    *target = sign | ((exp + 127 - 15) << 23) | (mantissa << 13);
                }
            }
            else  // 正常数
            {
                *target = sign | ((exp + 127 - 15) << 23) | (mantissa << 13);
            }

            ++target;
            ++source;
        }
    }

    /**
     * half_float转uint16 (归一化到0-65535)
     */
    inline void half2u16(uint16 *output, const half_float *input, const uint count)
    {
        union
        {
            float f;
            uint32 u;
        }x;

        for (uint i = 0; i < count; i++)
        {
            x.u = ((((*input) & 0x8000) << 16) | ((((*input) & 0x7c00) + 0x1C000) << 13) | (((*input) & 0x03FF) << 13));

            *output = Clamp(x.f) * 65535;

            ++output;
            ++input;
        }
    }

    /**
     * half_float转uint8 (归一化到0-255)
     */
    inline void half2u8(uint8 *output, const half_float *input, const uint count)
    {
        union
        {
            float f;
            uint32 u;
        }x;

        for (uint i = 0; i < count; i++)
        {
            x.u = ((((*input) & 0x8000) << 16) | ((((*input) & 0x7c00) + 0x1C000) << 13) | (((*input) & 0x03FF) << 13));

            *output = Clamp(x.f) * 255;

            ++output;
            ++input;
        }
    }
}//namespace hgl::math
