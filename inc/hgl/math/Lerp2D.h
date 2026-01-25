#pragma once

/**
 * @file Lerp2D.h
 * @brief Vector2f 插值函数
 *
 * 本文件专为 Vector2f 提供插值函数。
 * 它是对 Lerp1D.h 中通用插值算法的特化，
 * 直接应用于二维向量。
 *
 * 关于 Lerp 函数层级说明：
 * - Lerp1D.h：通用标量插值模板（核心算法）
 * - Lerp2D.h（本文件）：对 Vector2f 的特化包装
 * - Lerp3D.h：对 Vector3f 的特化包装
 * - VectorLerp.h：整数向量插值（u8/u16）及方向插值
 *
 * 此处实现的算法在数学上等价于 Lerp1D，
 * 但直接作用于 Vector2f，便于高效调用。
 */

#include<numbers>

#include<hgl/math/Vector.h>
#include<hgl/math/LerpType.h>

namespace hgl
{
    namespace graph
    {
        // ==================== 函数指针类型定义 ====================

        /// 2 点 Vector2f 插值函数指针类型（线性、余弦、三次、Hermite）
        typedef Vector2f (*LerpFunc2PointVec2f)(const math::Vector2f&, const math::Vector2f&, const float);

        /// 4 点 Vector2f 插值函数指针类型（贝塞尔、CatmullRom、BSpline）
        typedef Vector2f (*LerpFunc4PointVec2f)(const math::Vector2f&, const math::Vector2f&, const math::Vector2f&, const math::Vector2f&, const float);

        // ==================== 插值函数 ====================

        inline float LerpLinear(const float from,const float to,const float t)
        {
            return from+(to-from)*t;
        }

        inline float LerpCos(const float from,const float to,const float t)
        {
            float t2=(1.0f-cos(t*std::numbers::pi_v<float>))/2.0f;

            return from*(1.0f-t2)+to*t2;
        }

        inline float LerpCubic(const float from,const float to,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            return from*(2.0f*t3-3.0f*t2+1.0f)+to*(3.0f*t2-2.0f*t3);
        }

        inline float LerpCubicEase(const float from,const float to,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            return from*(2.0f*t3-3.0f*t2+1.0f)+to*(3.0f*t2-2.0f*t3);
        }

        inline Vector2f LerpLinear(const math::Vector2f &from,const math::Vector2f &to,const float t)
        {
            return from+(to-from)*t;
        }

        inline Vector2f LerpCos(const math::Vector2f &from,const math::Vector2f &to,const float t)
        {
            float t2=(1.0f-cos(t*std::numbers::pi_v<float>))/2.0f;

            return from*(1.0f-t2)+to*t2;
        }

        inline Vector2f LerpCubic(const math::Vector2f &from,const math::Vector2f &to,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            return from*(2.0f*t3-3.0f*t2+1.0f)+to*(3.0f*t2-2.0f*t3);
        }

        inline Vector2f LerpCubicEase(const math::Vector2f &from,const math::Vector2f &to,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            return from*(2.0f*t3-3.0f*t2+1.0f)+to*(3.0f*t2-2.0f*t3);
        }

        inline Vector2f LerpBezier(const math::Vector2f &p0,const math::Vector2f &p1,const math::Vector2f &p2,const math::Vector2f &p3,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            float t_1=1.0f-t;
            float t2_1=t_1*t_1;
            float t3_1=t2_1*t_1;

            return p0*t3_1+3.0f*p1*t*t2_1+3.0f*p2*t2*t_1+p3*t3;
        }

        inline Vector2f LerpCatmullRom(const math::Vector2f &p0,const math::Vector2f &p1,const math::Vector2f &p2,const math::Vector2f &p3,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            float t_1=1.0f-t;
            float t2_1=t_1*t_1;
            float t3_1=t2_1*t_1;

            return   p0*((-t3+2.0f*t2-t)*0.5f)
                    +p1*((3.0f*t3-5.0f*t2+2.0f)*0.5f)
                    +p2*((-3.0f*t3+4.0f*t2+t)*0.5f)
                    +p3*((t3-t2)*0.5f);
        }

        inline Vector2f LerpBSpline(const math::Vector2f &p0,const math::Vector2f &p1,const math::Vector2f &p2,const math::Vector2f &p3,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            float t_1=1.0f-t;
            float t2_1=t_1*t_1;
            float t3_1=t2_1*t_1;

            return   p0*((-t3+3.0f*t2-3.0f*t+1.0f)/6.0f)
                    +p1*((3.0f*t3-6.0f*t2+4.0f)/6.0f)
                    +p2*((-3.0f*t3+3.0f*t2+3.0f*t+1.0f)/6.0f)
                    +p3*(t3)/6.0f;
        }

        // ==================== 函数指针获取器 ====================

        /**
         * @brief 获取 Vector2f 的 2 点插值函数指针（线性、余弦、三次、Hermite）
         * @param type 插值类型
         * @return 对应插值函数的指针，类型无效时返回 nullptr
         */
        inline LerpFunc2PointVec2f GetLerpFuncPointer(LerpType type)
        {
            switch (type)
            {
                case LerpType::Linear:      return LerpLinear;
                case LerpType::Cos:         return LerpCos;
                case LerpType::Cubic:       return LerpCubic;
                case LerpType::Hermite:     return LerpCubicEase;  // Hermite is alias for Cubic
                default:                    return nullptr;
            }
        }

        /**
         * @brief 获取 Vector2f 的 4 点插值函数指针（贝塞尔、CatmullRom、BSpline）
         * @param type 插值类型
         * @return 对应插值函数的指针，类型无效时返回 nullptr
         */
        inline LerpFunc4PointVec2f GetLerpBezierFuncPointer(LerpType type)
        {
            switch (type)
            {
                case LerpType::Bezier:      return LerpBezier;
                case LerpType::CatmullRom:  return LerpCatmullRom;
                case LerpType::BSpline:     return LerpBSpline;
                default:                    return nullptr;
            }
        }

    }//namespace graph
}//namespace hgl
