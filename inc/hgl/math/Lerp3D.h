#pragma once

/**
 * @file Lerp3D.h
 * @brief Vector3f 插值函数
 *
 * 本文件专为 Vector3f 提供插值函数。
 * 它是对 Lerp1D.h 中通用插值算法的特化，
 * 直接应用于三维向量。
 *
 * 关于 Lerp 函数层级说明：
 * - Lerp1D.h：通用标量插值模板（核心算法）
 * - Lerp2D.h：对 Vector2f 的特化包装
 * - Lerp3D.h（本文件）：对 Vector3f 的特化包装
 * - VectorLerp.h：整数向量插值（u8/u16）及方向插值
 *
 * 此处实现的算法在数学上等价于 Lerp1D，
 * 但直接作用于 Vector3f，便于高效调用。
 */

#include<numbers>

#include<hgl/math/Vector.h>
#include<hgl/math/LerpType.h>

namespace hgl
{
    namespace graph
    {
        // ==================== 函数指针类型定义 ====================

        /// 2 点 Vector3f 插值函数指针类型（线性、余弦、三次、Hermite）
        typedef Vector3f (*LerpFunc2PointVec3f)(const math::Vector3f&, const math::Vector3f&, const float);

        /// 4 点 Vector3f 插值函数指针类型（贝塞尔、CatmullRom、BSpline）
        typedef Vector3f (*LerpFunc4PointVec3f)(const math::Vector3f&, const math::Vector3f&, const math::Vector3f&, const math::Vector3f&, const float);

        // ==================== 插值函数 ====================

        inline math::Vector3f LerpLinear(const math::Vector3f &from,const math::Vector3f &to,const float t)
        {
            return from+(to-from)*t;
        }

        inline math::Vector3f LerpCos(const math::Vector3f &from,const math::Vector3f &to,const float t)
        {
            float t2=(1.0f-cos(t*std::numbers::pi_v<float>))/2.0f;

            return from*(1.0f-t2)+to*t2;
        }

        inline math::Vector3f LerpCubic(const math::Vector3f &from,const math::Vector3f &to,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            return from*(2.0f*t3-3.0f*t2+1.0f)+to*(3.0f*t2-2.0f*t3);
        }

        inline math::Vector3f LerpCubicEase(const math::Vector3f &from,const math::Vector3f &to,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            return from*(2.0f*t3-3.0f*t2+1.0f)+to*(3.0f*t2-2.0f*t3);
        }

        inline math::Vector3f LerpBezier(const math::Vector3f &from,const math::Vector3f &cp1,const math::Vector3f &cp2,const math::Vector3f &to,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            float it=1.0f-t;
            float it2=it*it;
            float it3=it2*it;

            return from*it3+cp1*3.0f*it2*t+cp2*3.0f*it*t2+to*t3;
        }

        inline math::Vector3f LerpCatmullRom(const math::Vector3f &p0,const math::Vector3f &p1,const math::Vector3f &p2,const math::Vector3f &p3,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            return   p0*((-t3+2.0f*t2-t)*0.5f)
                    +p1*((3.0f*t3-5.0f*t2+2.0f)*0.5f)
                    +p2*((-3.0f*t3+4.0f*t2+t)*0.5f)
                    +p3*((t3-t2)*0.5f);
        }

        inline math::Vector3f LerpBSpline(const math::Vector3f &p0,const math::Vector3f &p1,const math::Vector3f &p2,const math::Vector3f &p3,const float t)
        {
            float t2=t*t;
            float t3=t2*t;

            return   p0*((-t3+3.0f*t2-3.0f*t+1.0f)/6.0f)
                    +p1*((3.0f*t3-6.0f*t2+4.0f)/6.0f)
                    +p2*((-3.0f*t3+3.0f*t2+3.0f*t+1.0f)/6.0f)
                    +p3*(t3)/6.0f;
        }

        // ==================== 函数指针获取器 ====================

        /**
         * @brief 获取 Vector3f 的 2 点插值函数指针（线性、余弦、三次、Hermite）
         * @param type 插值类型
         * @return 对应插值函数的指针，类型无效时返回 nullptr
         */
        inline LerpFunc2PointVec3f GetLerpFuncPointer(LerpType type)
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
         * @brief 获取 Vector3f 的 4 点插值函数指针（贝塞尔、CatmullRom、BSpline）
         * @param type 插值类型
         * @return 对应插值函数的指针，类型无效时返回 nullptr
         */
        inline LerpFunc4PointVec3f GetLerpBezierFuncPointer(LerpType type)
        {
            switch (type)
            {
                case LerpType::Bezier:      return LerpBezier;
                case LerpType::CatmullRom:  return LerpCatmullRom;
                case LerpType::BSpline:     return LerpBSpline;
                default:                    return nullptr;
            }
        }

        // ==================== 语义插值包装函数 ====================

        /**
         * @brief 三维位置的线性插值
         * 在三维空间中两个位置点之间插值
         * @param from 起始位置
         * @param to 结束位置
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值后的位置
         */
        inline math::Vector3f LerpPosition(const math::Vector3f &from, const math::Vector3f &to, const float t)
        {
            return LerpLinear(from, to, t);
        }

        /**
         * @brief 三维位置的余弦插值（平滑缓动）
         * @param from 起始位置
         * @param to 结束位置
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值后的位置
         */
        inline math::Vector3f LerpPositionSmooth(const math::Vector3f &from, const math::Vector3f &to, const float t)
        {
            return LerpCos(from, to, t);
        }

        /**
         * @brief 三维位置的三次插值（平滑缓动）
         * @param from 起始位置
         * @param to 结束位置
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值后的位置
         */
        inline math::Vector3f LerpPositionCubic(const math::Vector3f &from, const math::Vector3f &to, const float t)
        {
            return LerpCubic(from, to, t);
        }

        /**
         * @brief 三维方向的线性插值（归一化）
         * 结果归一化以保持方向向量单位长度
         * @param from 起始方向（应为单位向量）
         * @param to 结束方向（应为单位向量）
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值归一化后的方向
         */
        inline math::Vector3f LerpDirection(const math::Vector3f &from, const math::Vector3f &to, const float t)
        {
            math::Vector3f result = LerpLinear(from, to, t);
            return result.Normalize();  // Ensure unit length
        }

        /**
         * @brief 三维方向的余弦插值（归一化，平滑过渡）
         * 自动归一化，提供更平滑的方向插值
         * @param from 起始方向（应为单位向量）
         * @param to 结束方向（应为单位向量）
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值归一化后的方向
         */
        inline math::Vector3f LerpDirectionSmooth(const math::Vector3f &from, const math::Vector3f &to, const float t)
        {
            math::Vector3f result = LerpCos(from, to, t);
            return result.Normalize();  // Ensure unit length
        }

        /**
         * @brief 三维方向的三次插值（归一化，平滑缓动）
         * 提供三次平滑缓动的方向插值并自动归一化
         * @param from 起始方向（应为单位向量）
         * @param to 结束方向（应为单位向量）
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值归一化后的方向
         */
        inline math::Vector3f LerpDirectionCubic(const math::Vector3f &from, const math::Vector3f &to, const float t)
        {
            math::Vector3f result = LerpCubic(from, to, t);
            return result.Normalize();  // Ensure unit length
        }

        /**
         * @brief 三维方向的贝塞尔插值（归一化）
         * 平滑贝塞尔曲线插值并自动归一化
         * @param p0 起始方向点
         * @param p1 第一个控制点
         * @param p2 第二个控制点
         * @param p3 结束方向点
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值归一化后的方向
         */
        inline math::Vector3f LerpDirectionBezier(const math::Vector3f &p0, const math::Vector3f &p1, const math::Vector3f &p2, const math::Vector3f &p3, const float t)
        {
            math::Vector3f result = LerpBezier(p0, p1, p2, p3, t);
            return result.Normalize();  // Ensure unit length
        }

        /**
         * @brief 三维 RGB 颜色的线性插值
         * 直接在 RGB 空间插值颜色值
         * @param from 起始颜色
         * @param to 结束颜色
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值后的颜色
         * @deprecated 建议使用 CMCoreType 的 Color3f::lerp() 以获得更好的颜色管理
         */
        [[deprecated("Use CMCoreType's Color3f::lerp() instead for better color management")]]
        inline math::Vector3f LerpColor(const math::Vector3f &from, const math::Vector3f &to, const float t)
        {
            return LerpLinear(from, to, t);
        }

        /**
         * @brief 三维 RGB 颜色的余弦插值（平滑缓动）
         * 提供更平滑的颜色过渡
         * @param from 起始颜色
         * @param to 结束颜色
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值后的颜色
         * @deprecated 建议使用 CMCoreType 的 ColorLerp 并指定合适的缓动函数
         */
        [[deprecated("Use CMCoreType's ColorLerp with appropriate easing function instead")]]
        inline math::Vector3f LerpColorSmooth(const math::Vector3f &from, const math::Vector3f &to, const float t)
        {
            return LerpCos(from, to, t);
        }

        /**
         * @brief 三维 RGB 颜色的三次插值（平滑缓动）
         * @param from 起始颜色
         * @param to 结束颜色
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值后的颜色
         * @deprecated 建议使用 CMCoreType 的 ColorLerp 并指定三次缓动函数
         */
        [[deprecated("Use CMCoreType's ColorLerp with cubic easing function instead")]]
        inline math::Vector3f LerpColorCubic(const math::Vector3f &from, const math::Vector3f &to, const float t)
        {
            return LerpCubic(from, to, t);
        }

        /**
         * @brief 三维 RGB 颜色的贝塞尔插值
         * 沿贝塞尔曲线平滑插值颜色
         * @param c0 起始颜色
         * @param c1 第一个控制颜色
         * @param c2 第二个控制颜色
         * @param c3 结束颜色
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值后的颜色
         * @deprecated 建议使用 CMCoreType 的 ColorLerp 并支持贝塞尔曲线
         */
        [[deprecated("Use CMCoreType's ColorLerp with Bezier curve support instead")]]
        inline math::Vector3f LerpColorBezier(const math::Vector3f &c0, const math::Vector3f &c1, const math::Vector3f &c2, const math::Vector3f &c3, const float t)
        {
            return LerpBezier(c0, c1, c2, c3, t);
        }

    }//namespace graph
}//namespace hgl
