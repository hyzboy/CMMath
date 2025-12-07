#pragma once

#include<hgl/math/VectorTypes.h>
#include<hgl/math/FloatPrecision.h>
#include<glm/glm.hpp>

namespace hgl::math
{
    /**
     * 向量基础运算
     * 包括：比较、运算符重载、基础几何运算
     */

    // ==================== 向量比较 ====================

    inline bool operator == (const math::Vector2f &lhs,const math::Vector2f &rhs)
    {
        if(!math::IsNearlyEqual(lhs.x,rhs.x))return(false);
        if(!math::IsNearlyEqual(lhs.y,rhs.y))return(false);
        return(true);
    }

    inline bool operator != (const math::Vector2f &lhs,const math::Vector2f &rhs)
    {
        if(!math::IsNearlyEqual(lhs.x,rhs.x))return(true);
        if(!math::IsNearlyEqual(lhs.y,rhs.y))return(true);
        return(false);
    }

    inline bool operator == (const math::Vector3f &lhs,const math::Vector3f &rhs)
    {
        if(!math::IsNearlyEqual(lhs.x,rhs.x))return(false);
        if(!math::IsNearlyEqual(lhs.y,rhs.y))return(false);
        if(!math::IsNearlyEqual(lhs.z,rhs.z))return(false);
        return(true);
    }

    inline bool operator != (const math::Vector3f &lhs,const math::Vector3f &rhs)
    {
        if(!math::IsNearlyEqual(lhs.x,rhs.x))return(true);
        if(!math::IsNearlyEqual(lhs.y,rhs.y))return(true);
        if(!math::IsNearlyEqual(lhs.z,rhs.z))return(true);
        return(false);
    }

    inline bool operator == (const math::Vector4f &lhs,const math::Vector4f &rhs)
    {
        if(!math::IsNearlyEqual(lhs.x,rhs.x))return(false);
        if(!math::IsNearlyEqual(lhs.y,rhs.y))return(false);
        if(!math::IsNearlyEqual(lhs.z,rhs.z))return(false);
        if(!math::IsNearlyEqual(lhs.w,rhs.w))return(false);
        return(true);
    }

    inline bool operator != (const math::Vector4f &lhs,const math::Vector4f &rhs)
    {
        if(!math::IsNearlyEqual(lhs.x,rhs.x))return(true);
        if(!math::IsNearlyEqual(lhs.y,rhs.y))return(true);
        if(!math::IsNearlyEqual(lhs.z,rhs.z))return(true);
        if(!math::IsNearlyEqual(lhs.w,rhs.w))return(true);
        return(false);
    }

    // ==================== 近似相等判断 ====================

    inline bool IsNearlyEqual(const math::Vector2f &v1,const math::Vector2f &v2,const float err=HGL_FLOAT_ERROR)
    {
        if(!math::IsNearlyEqual(v1.x,v2.x,err))return(false);
        if(!math::IsNearlyEqual(v1.y,v2.y,err))return(false);
        return(true);
    }

    inline bool IsNearlyEqual(const Vector2d &v1,const Vector2d &v2,const double err=HGL_DOUBLE_ERROR)
    {
        if(!math::IsNearlyEqual(v1.x,v2.x,err))return(false);
        if(!math::IsNearlyEqual(v1.y,v2.y,err))return(false);
        return(true);
    }

    inline bool IsNearlyEqual(const math::Vector3f &v1,const math::Vector3f &v2,const float err=HGL_FLOAT_ERROR)
    {
        if(!math::IsNearlyEqual(v1.x,v2.x,err))return(false);
        if(!math::IsNearlyEqual(v1.y,v2.y,err))return(false);
        if(!math::IsNearlyEqual(v1.z,v2.z,err))return(false);
        return(true);
    }

    inline bool IsNearlyEqual(const Vector3d &v1,const Vector3d &v2,const double err=HGL_DOUBLE_ERROR)
    {
        if(!math::IsNearlyEqual(v1.x,v2.x,err))return(false);
        if(!math::IsNearlyEqual(v1.y,v2.y,err))return(false);
        if(!math::IsNearlyEqual(v1.z,v2.z,err))return(false);
        return(true);
    }

    template<typename T,glm::qualifier Q>
    inline const bool IsNearlyZero(const glm::vec<2,T,Q> &v)
    {
        return IsNearlyZero(v.x)
             &&IsNearlyZero(v.y);
    }

    template<typename T,glm::qualifier Q>
    inline const bool IsNearlyZero(const glm::vec<3,T,Q> &v)
    {
        return IsNearlyZero(v.x)
             &&IsNearlyZero(v.y)
             &&IsNearlyZero(v.z);
    }

    // ==================== 归一化 ====================

    template<typename T>
    inline T normalized(const T &v)
    {
        return glm::normalize(v);
    }

    template<typename T>
    inline void normalize(T &v)
    {
        v=glm::normalize(v);
    }

    // ==================== 叉乘 ====================

    inline math::Vector3f cross(const math::Vector3f &v1,const math::Vector3f &v2)
    {
        return glm::cross(v1,v2);
    }

    inline math::Vector4f cross(const math::Vector4f &v1,const math::Vector4f &v2)
    {
        math::Vector3f v31=v1;
        math::Vector3f v32=v2;

        math::Vector4f result=Vector4f(glm::cross(v31,v32),1.0f);

        return result;
    }

    // ==================== 点乘 ====================

    template<typename T>
    inline float dot(const T &v1,const T &v2)
    {
        return glm::dot(v1,v2);
    }

    // ==================== 长度计算 ====================

    inline float length_squared(const math::Vector2f &v)
    {
        return (v.x*v.x) + (v.y*v.y);
    }

    inline float length_squared_2d(const math::Vector3f &v)
    {
        return (v.x*v.x) + (v.y*v.y);
    }

    inline float length_squared(const math::Vector3f &v)
    {
        return (v.x*v.x) + (v.y*v.y) + (v.z*v.z);
    }

    inline float length_squared(const math::Vector4f &v)
    {
        return (v.x*v.x) + (v.y*v.y) + (v.z*v.z);
    }

    template<typename T>
    inline float length(const T &v)
    {
        return sqrt(length_squared(v));
    }

    inline float length_2d(const math::Vector3f &v)
    {
        return sqrt(length_squared_2d(v));
    }

    template<typename T1, typename T2>
    inline float length(const T1 &v1, const T2 &v2)
    {
        return sqrt(length_squared(v1-v2));
    }

    template<typename T1, typename T2>
    inline float length_squared(const T1 &v1, const T2 &v2)
    {
        return length_squared(v1-v2);
    }

    template<typename T1, typename T2>
    inline float length_squared_2d(const T1 &v1, const T2 &v2)
    {
        const double x = double(v1.x) - double(v2.x);
        const double y = double(v1.y) - double(v2.y);

        return x*x + y*y;
    }

    template<typename T1, typename T2>
    inline float length_2d(const T1 &v1, const T2 &v2)
    {
        return sqrt(length_squared_2d(v1, v2));
    }

    // ==================== 插值 ====================

    template<typename T>
    inline T lerp(const T &from,const T &to,const float alpha)
    {
        return from + (to - from) * alpha;
    }

    // ==================== 夹值 ====================

    template<typename T>
    inline T clamp(const T v,const T min_v,const T max_v)
    {
        if(v<min_v)return min_v;
        if(v>max_v)return max_v;
        return v;
    }

    template<typename T>
    inline const T clamp(const T in)
    {
        return clamp<double>(in,0.0f,1.0f);
    }

    template<> inline const uint8 clamp(const uint8 in)
    {
        return clamp<uint8>(in,0,0xFF);
    }

    template<> inline const uint16 clamp(const uint16 in)
    {
        return clamp<uint16>(in,0,0xFFFF);
    }
}//namespace hgl::graph
