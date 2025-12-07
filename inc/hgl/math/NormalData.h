#pragma once
#include<hgl/math/Math.h>

namespace hgl
{
    // Lambert Azimuthal Equal-Area projection
    // http://en.wikipedia.org/wiki/Lambert_azimuthal_equal-area_projection

    /**
    * 将float3型法线数据压缩为float2型
    */
    inline Vector2f Normal3to2(const math::Vector3f &input)
    {
        float f=sqrt(8.0f*input.z+8.0f);

        return Vector2f(input.x/f+0.5f,
                        input.y/f+0.5f);
    }

    /**
    * 将float3型法线数据压缩为float2型
    */
    inline Vector2f Normal3to2(const float *input)
    {
        float f=sqrt(8.0f*input[2]+8.0f);

        return Vector2f(input[0]/f+0.5f,
                        input[1]/f+0.5f);
    }

    /**
    * 解压float2型法线数据
    */
    inline math::Vector3f Normal2to3(const math::Vector2f &input)
    {
        math::Vector2f fenc(input*4.0f-2.0f);

        float f=dot(fenc,fenc);
        float g=sqrt(1.0f-f/4.0f);

        return Vector3f(fenc.x*g,
                        fenc.y*g,
                        1.0f-f/2.0f);
    }

    inline constexpr float normal_float(const uint8 value)
    {
        return float(value)/127.5f-1.0f;
    }

    inline constexpr uint8 normal_u8(const float value)
    {
        return ClampU8((value+1.0f)*127.5f);
    }

    inline math::Vector3f normal_vec3(const uint8 *input)
    {
        return Vector3f(normal_float(input[0]),
                        normal_float(input[1]),
                        normal_float(input[2]));
    }

    inline Vector2f normal_vec2(const uint8 *input)
    {
        return Vector2f(normal_float(input[0]),
                        normal_float(input[1]));
    }

    inline uint8 *normal_vec3(uint8 *output,const math::Vector3f &input)
    {
        *output=normal_u8(input.x);
        ++output;
        *output=normal_u8(input.y);
        ++output;
        *output=normal_u8(input.z);
        ++output;

        return output;
    }

    inline uint8 *normal_vec2(uint8 *output,const math::Vector2f &input)
    {
        *output=normal_u8(input.x);
        ++output;
        *output=normal_u8(input.y);
        ++output;

        return output;
    }

    /*
    * 批量将3字节法线数据压缩成2字节数据
    */
    inline void Normal3to2(uint8 *output,const uint8 *input,const uint count)
    {
        for(uint i=0;i<count;i++)
        {
            output=normal_vec2(output,Normal3to2(normal_vec3(input)));

            input+=3;
        }
    }

    /**
    * 批量将2字节法线数据解压成3字节法线数据
    */
    inline void Normal2to3(uint8 *output,const uint8 *input,const uint count)
    {
        for(uint i=0;i<count;i++)
        {
            output=normal_vec3(output,Normal2to3(normal_vec2(input)));

            input+=2;
        }
    }

    /*
    * 批量将float3型法线数据压缩成2字节数据
    */
    inline void Normal3to2(uint8 *output,const float *input,const uint count)
    {
        for(uint i=0;i<count;i++)
        {
            output=normal_vec2(output,Normal3to2(input));

            input+=3;
        }
    }
}//namespace hgl
