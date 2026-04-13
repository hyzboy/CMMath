#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/Clamp.h>
#include<cmath>

namespace hgl
{
    using namespace math;

    // Lambert Azimuthal Equal-Area projection
    // http://en.wikipedia.org/wiki/Lambert_azimuthal_equal-area_projection

    /**
    * 将float3型法线数据压缩为float2型
    */
    inline Vector2f Normal3to2(const Vector3f &input)
    {
        float f=std::sqrt(8.0f*input.z+8.0f);

        return Vector2f(input.x/f+0.5f,
                        input.y/f+0.5f);
    }

    /**
    * 将float3型法线数据压缩为float2型
    */
    inline Vector2f Normal3to2(const float *input)
    {
        float f=std::sqrt(8.0f*input[2]+8.0f);

        return Vector2f(input[0]/f+0.5f,
                        input[1]/f+0.5f);
    }

    /**
    * 解压float2型法线数据
    */
    inline Vector3f Normal2to3(const Vector2f &input)
    {
        Vector2f fenc(input*4.0f-2.0f);

        float f=Dot(fenc,fenc);
        float g=std::sqrt(1.0f-f/4.0f);

        return Vector3f(fenc.x*g,
                        fenc.y*g,
                        1.0f-f/2.0f);
    }

    inline int8 ClampSNorm8(const float value)
    {
        float clamped = value;

        if(clamped < -1.0f)
            clamped = -1.0f;
        else
        if(clamped > 1.0f)
            clamped = 1.0f;

        int scaled = int(clamped * 127.0f);

        if(scaled < -127)
            scaled = -127;
        else
        if(scaled > 127)
            scaled = 127;

        return int8(scaled);
    }

    inline int32 ClampSNorm10(const float value)
    {
        float clamped = value;

        if(clamped < -1.0f)
            clamped = -1.0f;
        else
        if(clamped > 1.0f)
            clamped = 1.0f;

        int32 scaled = int32(clamped * 511.0f);

        if(scaled < -511)
            scaled = -511;
        else
        if(scaled > 511)
            scaled = 511;

        return scaled;
    }

    inline int32 ClampSNorm2(const float value)
    {
        return (value < 0.0f) ? -1 : 1;
    }

    inline uint32 PackA2B10G10R10SNorm(const float x,const float y,const float z,const float w)
    {
        const uint32 r = uint32(ClampSNorm10(x)) & 0x3FFu;
        const uint32 g = uint32(ClampSNorm10(y)) & 0x3FFu;
        const uint32 b = uint32(ClampSNorm10(z)) & 0x3FFu;
        const uint32 a = uint32(ClampSNorm2(w))  & 0x3u;

        return r | (g << 10u) | (b << 20u) | (a << 30u);
    }

    inline uint32 PackA2R10G10B10SNorm(const float x,const float y,const float z,const float w)
    {
        const uint32 r = uint32(ClampSNorm10(x)) & 0x3FFu;
        const uint32 g = uint32(ClampSNorm10(y)) & 0x3FFu;
        const uint32 b = uint32(ClampSNorm10(z)) & 0x3FFu;
        const uint32 a = uint32(ClampSNorm2(w))  & 0x3u;

        return b | (g << 10u) | (r << 20u) | (a << 30u);
    }

    /**
    * 将float3型法线数据压缩为uint16型（2个int8 packed）RG8SN格式
    */
    inline uint16 Normal3to2u8(const Vector3f &input)
    {
        const Vector2f enc = Normal3to2(input);
        const int8 x = ClampSNorm8(enc.x * 2.0f - 1.0f);
        const int8 y = ClampSNorm8(enc.y * 2.0f - 1.0f);
        
        return uint16(uint8(x)) | (uint16(uint8(y)) << 8);
    }

    /**
    * 将float3型法线数据压缩为uint16型（2个int8 packed）RG8SN格式
    */
    inline uint16 Normal3to2u8(const float *input)
    {
        return Normal3to2u8(Vector3f(input[0], input[1], input[2]));
    }

    /**
    * 将float3型法线数据压缩为uint32型 A2BGR10_SNORM格式
    */
    inline uint32 Normal3to4u10a2BGR(const Vector3f &input)
    {
        return PackA2B10G10R10SNorm(input.x, input.y, input.z, 1.0f);
    }

    /**
    * 将float3型法线数据压缩为uint32型 A2RGB10_SNORM格式
    */
    inline uint32 Normal3to4u10a2RGB(const Vector3f &input)
    {
        return PackA2R10G10B10SNorm(input.x, input.y, input.z, 1.0f);
    }

    /**
    * 将float4型法线+tangent数据压缩为uint32型 A2BGR10_SNORM格式
    */
    inline uint32 Normal3Tangent1to4u10a2BGR(const Vector3f &normal, const float tangent_w)
    {
        return PackA2B10G10R10SNorm(normal.x, normal.y, normal.z, tangent_w);
    }

    /**
    * 将float4型法线+tangent数据压缩为uint32型 A2RGB10_SNORM格式
    */
    inline uint32 Normal3Tangent1to4u10a2RGB(const Vector3f &normal, const float tangent_w)
    {
        return PackA2R10G10B10SNorm(normal.x, normal.y, normal.z, tangent_w);
    }

    inline constexpr float normal_float(const uint8 value)
    {
        return float(value)/127.5f-1.0f;
    }

    inline constexpr uint8 normal_u8(const float value)
    {
        return ClampU8((value+1.0f)*127.5f);
    }

    inline Vector3f normal_vec3(const uint8 *input)
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

    inline uint8 *normal_vec3(uint8 *output,const Vector3f &input)
    {
        *output=normal_u8(input.x);
        ++output;
        *output=normal_u8(input.y);
        ++output;
        *output=normal_u8(input.z);
        ++output;

        return output;
    }

    inline uint8 *normal_vec2(uint8 *output,const Vector2f &input)
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
} //namespace hgl
