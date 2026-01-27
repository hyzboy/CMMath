#pragma once

/**
 * @file Lerp1D.h
 * @brief 通用标量（1D）插值函数
 *
 * 本文件提供基于模板的标量插值函数。
 * 它是多种插值算法的核心实现，包括：
 * - 线性插值
 * - 余弦插值
 * - 三次 Hermite 插值
 * - 贝塞尔曲线插值
 * - Catmull-Rom 样条插值
 * - B 样条插值
 *
 * 关于 Lerp 函数层级说明：
 * - Lerp1D.h（本文件）：通用标量插值模板
 * - Lerp2D.h：Vector2f 专用，调用 Lerp1D 算法
 * - Lerp3D.h：Vector3f 专用，调用 Lerp1D 算法
 * - VectorLerp.h：整数向量插值（u8/u16）+ 方向插值
 *
 * Lerp2D/3D 的特化本质上是对 Lerp1D 算法的包装，直接用于 2D 和 3D 向量。
 */

#include<hgl/math/Vector.h>
#include<hgl/math/LerpType.h>
#include<numbers>

namespace hgl
{
    namespace graph
    {
        // ==================== 函数指针类型定义 ====================

        /// 2 点 float 线性插值函数指针类型（线性、余弦、三次、Hermite）
        typedef float (*LerpFunc2PointFloat)(const float, const float, const float);

        /// 二次贝塞尔 float 函数指针类型
        typedef float (*LerpFuncQuadBezierFloat)(const float, const float, const float, const float);

        /// 4 点 float 插值函数指针类型（贝塞尔、CatmullRom、BSpline）
        typedef float (*LerpFunc4PointFloat)(const float, const float, const float, const float, const float);

        // ==================== 插值函数 ====================

        /**
         * @brief 线性插值模板
         * @tparam T 要插值的数据类型（需支持 +、-、* 运算）
         * @param from 起始值
         * @param to 结束值
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值结果
         */
        template<typename T>
        inline T LerpLinear(const T &from, const T &to, const float t)
        {
            return from + (to - from) * t;
        }

        /**
         * @brief 余弦插值模板
         * 使用平滑的余弦曲线实现缓动
         * @tparam T 要插值的数据类型
         * @param from 起始值
         * @param to 结束值
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值结果
         */
        template<typename T>
        inline T LerpCos(const T &from, const T &to, const float t)
        {
            float t2 = (1.0f - cos(t * std::numbers::pi_v<float>)) / 2.0f;
            return from * (1.0f - t2) + to * t2;
        }

        /**
         * @brief 三次 Hermite 插值模板
         * 平滑的三次缓动曲线
         * @tparam T 要插值的数据类型
         * @param from 起始值
         * @param to 结束值
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值结果
         */
        template<typename T>
        inline T LerpCubic(const T &from, const T &to, const float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;
            return from * (2.0f * t3 - 3.0f * t2 + 1.0f) + to * (3.0f * t2 - 2.0f * t3);
        }

        /**
         * @brief 三次缓动插值模板（LerpCubic 的别名）
         * @tparam T 要插值的数据类型
         * @param from 起始值
         * @param to 结束值
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值结果
         */
        template<typename T>
        inline T LerpCubicEase(const T &from, const T &to, const float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;
            return from * (2.0f * t3 - 3.0f * t2 + 1.0f) + to * (3.0f * t2 - 2.0f * t3);
        }

        /**
         * @brief 二次贝塞尔插值模板
         * 通过一个控制点在两点间插值
         * @tparam T 要插值的数据类型
         * @param p0 起点
         * @param p1 控制点
         * @param p2 终点
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值结果
         */
        template<typename T>
        inline T LerpQuadraticBezier(const T &p0, const T &p1, const T &p2, const float t)
        {
            float t2 = t * t;
            float it = 1.0f - t;
            float it2 = it * it;

            return p0 * it2 + p1 * 2.0f * it * t + p2 * t2;
        }

        /**
         * @brief 三次贝塞尔插值模板
         * 通过两个控制点在两点间插值
         * @tparam T 要插值的数据类型
         * @param p0 起点
         * @param p1 第一个控制点
         * @param p2 第二个控制点
         * @param p3 终点
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值结果
         */
        template<typename T>
        inline T LerpBezier(const T &p0, const T &p1, const T &p2, const T &p3, const float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;
            float it = 1.0f - t;
            float it2 = it * it;
            float it3 = it2 * it;

            return p0 * it3 + p1 * 3.0f * it2 * t + p2 * 3.0f * it * t2 + p3 * t3;
        }

        /**
         * @brief Catmull-Rom 样条插值模板
         * 平滑通过所有控制点的曲线
         * @tparam T 要插值的数据类型
         * @param p0 第一个控制点（起点前）
         * @param p1 起点
         * @param p2 终点
         * @param p3 最后一个控制点（终点后）
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值结果
         */
        template<typename T>
        inline T LerpCatmullRom(const T &p0, const T &p1, const T &p2, const T &p3, const float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;

            return p0 * ((-t3 + 2.0f * t2 - t) * 0.5f)
                 + p1 * ((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f)
                 + p2 * ((-3.0f * t3 + 4.0f * t2 + t) * 0.5f)
                 + p3 * ((t3 - t2) * 0.5f);
        }

        /**
         * @brief B 样条插值模板
         * 平滑的均匀 B 样条曲线
         * @tparam T 要插值的数据类型
         * @param p0 第一个控制点
         * @param p1 第二个控制点
         * @param p2 第三个控制点
         * @param p3 第四个控制点
         * @param t 插值参数（0.0 到 1.0）
         * @return 插值结果
         */
        template<typename T>
        inline T LerpBSpline(const T &p0, const T &p1, const T &p2, const T &p3, const float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;

            return p0 * ((-t3 + 3.0f * t2 - 3.0f * t + 1.0f) / 6.0f)
                 + p1 * ((3.0f * t3 - 6.0f * t2 + 4.0f) / 6.0f)
                 + p2 * ((-3.0f * t3 + 3.0f * t2 + 3.0f * t + 1.0f) / 6.0f)
                 + p3 * (t3 / 6.0f);
        }

        // ==================== 常用类型特化 ====================

        /**
         * @brief float 类型的线性插值
         */
        inline float LerpLinear(const float from, const float to, const float t)
        {
            return from + (to - from) * t;
        }

        /**
         * @brief float 类型的余弦插值
         */
        inline float LerpCos(const float from, const float to, const float t)
        {
            float t2 = (1.0f - cos(t * std::numbers::pi_v<float>)) / 2.0f;
            return from * (1.0f - t2) + to * t2;
        }

        /**
         * @brief float 类型的三次 Hermite 插值
         */
        inline float LerpCubic(const float from, const float to, const float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;
            return from * (2.0f * t3 - 3.0f * t2 + 1.0f) + to * (3.0f * t2 - 2.0f * t3);
        }

        /**
         * @brief float 类型的三次缓动插值
         */
        inline float LerpCubicEase(const float from, const float to, const float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;
            return from * (2.0f * t3 - 3.0f * t2 + 1.0f) + to * (3.0f * t2 - 2.0f * t3);
        }

        /**
         * @brief float 类型的二次贝塞尔插值
         */
        inline float LerpQuadraticBezier(const float p0, const float p1, const float p2, const float t)
        {
            float t2 = t * t;
            float it = 1.0f - t;
            float it2 = it * it;

            return p0 * it2 + p1 * 2.0f * it * t + p2 * t2;
        }

        /**
         * @brief float 类型的三次贝塞尔插值
         */
        inline float LerpBezier(const float p0, const float p1, const float p2, const float p3, const float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;
            float it = 1.0f - t;
            float it2 = it * it;
            float it3 = it2 * it;

            return p0 * it3 + p1 * 3.0f * it2 * t + p2 * 3.0f * it * t2 + p3 * t3;
        }

        /**
         * @brief float 类型的 Catmull-Rom 样条插值
         */
        inline float LerpCatmullRom(const float p0, const float p1, const float p2, const float p3, const float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;

            return p0 * ((-t3 + 2.0f * t2 - t) * 0.5f)
                 + p1 * ((3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f)
                 + p2 * ((-3.0f * t3 + 4.0f * t2 + t) * 0.5f)
                 + p3 * ((t3 - t2) * 0.5f);
        }

        /**
         * @brief float 类型的 B 样条插值
         */
        inline float LerpBSpline(const float p0, const float p1, const float p2, const float p3, const float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;

            return p0 * ((-t3 + 3.0f * t2 - 3.0f * t + 1.0f) / 6.0f)
                 + p1 * ((3.0f * t3 - 6.0f * t2 + 4.0f) / 6.0f)
                 + p2 * ((-3.0f * t3 + 3.0f * t2 + 3.0f * t + 1.0f) / 6.0f)
                 + p3 * (t3 / 6.0f);
        }

        // ==================== 缓动函数 ====================

        // --- 缓入（EaseIn）函数 ---

        /**
         * @brief 二次缓入插值（t^2）
         */
        inline float LerpEaseInQuad(const float from, const float to, const float t)
        {
            float eased = t * t;
            return from + (to - from) * eased;
        }

        /**
         * @brief 三次缓入插值（t^3）
         */
        inline float LerpEaseInCubic(const float from, const float to, const float t)
        {
            float eased = t * t * t;
            return from + (to - from) * eased;
        }

        /**
         * @brief 四次缓入插值（t^4）
         */
        inline float LerpEaseInQuart(const float from, const float to, const float t)
        {
            float eased = t * t * t * t;
            return from + (to - from) * eased;
        }

        /**
         * @brief 五次缓入插值（t^5）
         */
        inline float LerpEaseInQuint(const float from, const float to, const float t)
        {
            float eased = t * t * t * t * t;
            return from + (to - from) * eased;
        }

        /**
         * @brief 指数缓入插值
         */
        inline float LerpEaseInExpo(const float from, const float to, const float t)
        {
            float eased = (t == 0.0f) ? 0.0f : pow(2.0f, 10.0f * t - 10.0f);
            return from + (to - from) * eased;
        }

        /**
         * @brief 圆形缓入插值
         */
        inline float LerpEaseInCirc(const float from, const float to, const float t)
        {
            float eased = 1.0f - sqrt(1.0f - t * t);
            return from + (to - from) * eased;
        }

        // --- 缓出（EaseOut）函数 ---

        /**
         * @brief 二次缓出插值
         */
        inline float LerpEaseOutQuad(const float from, const float to, const float t)
        {
            float eased = 1.0f - (1.0f - t) * (1.0f - t);
            return from + (to - from) * eased;
        }

        /**
         * @brief 三次缓出插值
         */
        inline float LerpEaseOutCubic(const float from, const float to, const float t)
        {
            float eased = 1.0f - pow(1.0f - t, 3.0f);
            return from + (to - from) * eased;
        }

        /**
         * @brief 四次缓出插值
         */
        inline float LerpEaseOutQuart(const float from, const float to, const float t)
        {
            float eased = 1.0f - pow(1.0f - t, 4.0f);
            return from + (to - from) * eased;
        }

        /**
         * @brief 五次缓出插值
         */
        inline float LerpEaseOutQuint(const float from, const float to, const float t)
        {
            float eased = 1.0f - pow(1.0f - t, 5.0f);
            return from + (to - from) * eased;
        }

        /**
         * @brief 指数缓出插值
         */
        inline float LerpEaseOutExpo(const float from, const float to, const float t)
        {
            float eased = (t == 1.0f) ? 1.0f : 1.0f - pow(2.0f, -10.0f * t);
            return from + (to - from) * eased;
        }

        /**
         * @brief 圆形缓出插值
         */
        inline float LerpEaseOutCirc(const float from, const float to, const float t)
        {
            float eased = sqrt(1.0f - (t - 1.0f) * (t - 1.0f));
            return from + (to - from) * eased;
        }

        // --- 缓入缓出（EaseInOut）函数 ---

        /**
         * @brief 二次缓入缓出插值
         */
        inline float LerpEaseInOutQuad(const float from, const float to, const float t)
        {
            float eased = (t < 0.5f) ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
            return from + (to - from) * eased;
        }

        /**
         * @brief 三次缓入缓出插值
         */
        inline float LerpEaseInOutCubic(const float from, const float to, const float t)
        {
            float eased = (t < 0.5f) ? 4.0f * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;
            return from + (to - from) * eased;
        }

        /**
         * @brief 四次缓入缓出插值
         */
        inline float LerpEaseInOutQuart(const float from, const float to, const float t)
        {
            float eased = (t < 0.5f) ? 8.0f * t * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;
            return from + (to - from) * eased;
        }

        /**
         * @brief 五次缓入缓出插值
         */
        inline float LerpEaseInOutQuint(const float from, const float to, const float t)
        {
            float eased = (t < 0.5f) ? 16.0f * t * t * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;
            return from + (to - from) * eased;
        }

        /**
         * @brief 指数缓入缓出插值
         */
        inline float LerpEaseInOutExpo(const float from, const float to, const float t)
        {
            float eased = (t == 0.0f) ? 0.0f : (t == 1.0f) ? 1.0f :
                         (t < 0.5f) ? pow(2.0f, 20.0f * t - 10.0f) / 2.0f : (2.0f - pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
            return from + (to - from) * eased;
        }

        /**
         * @brief 圆形缓入缓出插值
         */
        inline float LerpEaseInOutCirc(const float from, const float to, const float t)
        {
            float eased = (t < 0.5f) ? (1.0f - sqrt(1.0f - pow(2.0f * t, 2.0f))) / 2.0f :
                         (sqrt(1.0f - pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
            return from + (to - from) * eased;
        }

        // --- 特殊插值函数 ---

        /**
         * @brief 平滑步进插值（3t^2 - 2t^3）
         */
        inline float LerpSmoothStep(const float from, const float to, const float t)
        {
            float eased = t * t * (3.0f - 2.0f * t);
            return from + (to - from) * eased;
        }

        /**
         * @brief 更平滑步进插值（6t^5 - 15t^4 + 10t^3）
         */
        inline float LerpSmootherStep(const float from, const float to, const float t)
        {
            float eased = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
            return from + (to - from) * eased;
        }

        /**
         * @brief 弹性缓入插值
         */
        inline float LerpEaseInElastic(const float from, const float to, const float t)
        {
            float eased = (t == 0.0f) ? 0.0f : (t == 1.0f) ? 1.0f :
                         -pow(2.0f, 10.0f * t - 10.0f) * sin((t * 10.0f - 10.75f) * (2.0f * std::numbers::pi_v<float>) / 3.0f);
            return from + (to - from) * eased;
        }

        /**
         * @brief 弹性缓出插值
         */
        inline float LerpEaseOutElastic(const float from, const float to, const float t)
        {
            float eased = (t == 0.0f) ? 0.0f : (t == 1.0f) ? 1.0f :
                         pow(2.0f, -10.0f * t) * sin((t * 10.0f - 0.75f) * (2.0f * std::numbers::pi_v<float>) / 3.0f) + 1.0f;
            return from + (to - from) * eased;
        }

        /**
         * @brief 弹性缓入缓出插值
         */
        inline float LerpEaseInOutElastic(const float from, const float to, const float t)
        {
            float eased = (t == 0.0f) ? 0.0f : (t == 1.0f) ? 1.0f :
                         (t < 0.5f) ? -(pow(2.0f, 20.0f * t - 10.0f) * sin((20.0f * t - 11.125f) * (2.0f * std::numbers::pi_v<float>) / 4.5f)) / 2.0f :
                         (pow(2.0f, -20.0f * t + 10.0f) * sin((20.0f * t - 11.125f) * (2.0f * std::numbers::pi_v<float>) / 4.5f)) / 2.0f + 1.0f;
            return from + (to - from) * eased;
        }

        /**
         * @brief 回退缓入插值（起始点超调）
         */
        inline float LerpEaseInBack(const float from, const float to, const float t)
        {
            const float c1 = 1.70158f;
            const float c3 = c1 + 1.0f;
            float eased = c3 * t * t * t - c1 * t * t;
            return from + (to - from) * eased;
        }

        /**
         * @brief 回退缓出插值（终点超调）
         */
        inline float LerpEaseOutBack(const float from, const float to, const float t)
        {
            const float c1 = 1.70158f;
            const float c3 = c1 + 1.0f;
            float eased = 1.0f + c3 * pow(t - 1.0f, 3.0f) + c1 * pow(t - 1.0f, 2.0f);
            return from + (to - from) * eased;
        }

        /**
         * @brief 回退缓入缓出插值
         */
        inline float LerpEaseInOutBack(const float from, const float to, const float t)
        {
            const float c1 = 1.70158f;
            const float c2 = c1 * 1.525f;
            float eased = (t < 0.5f) ? (pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f :
                         (pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
            return from + (to - from) * eased;
        }

        /**
         * @brief 弹跳缓入插值
         */
        inline float LerpEaseInBounce(const float from, const float to, const float t)
        {
            float eased = 1.0f - LerpEaseOutBounce(0.0f, 1.0f, 1.0f - t);
            return from + (to - from) * eased;
        }

        /**
         * @brief 弹跳缓出插值
         */
        inline float LerpEaseOutBounce(const float from, const float to, const float t)
        {
            const float n1 = 7.5625f;
            const float d1 = 2.75f;

            float eased;
            if (t < 1.0f / d1)
            {
                eased = n1 * t * t;
            }
            else if (t < 2.0f / d1)
            {
                eased = n1 * (t -= 1.5f / d1) * t + 0.75f;
            }
            else if (t < 2.5f / d1)
            {
                eased = n1 * (t -= 2.25f / d1) * t + 0.9375f;
            }
            else
            {
                eased = n1 * (t -= 2.625f / d1) * t + 0.984375f;
            }
            return from + (to - from) * eased;
        }

        /**
         * @brief 弹跳缓入缓出插值
         */
        inline float LerpEaseInOutBounce(const float from, const float to, const float t)
        {
            float eased = (t < 0.5f) ? (1.0f - LerpEaseOutBounce(0.0f, 1.0f, 1.0f - 2.0f * t)) / 2.0f :
                         (1.0f + LerpEaseOutBounce(0.0f, 1.0f, 2.0f * t - 1.0f)) / 2.0f;
            return from + (to - from) * eased;
        }

        // ==================== 函数指针获取器 ====================

        /**
         * @brief 获取 2 点插值函数指针（线性、余弦、三次、Hermite）
         * @param type 插值类型
         * @return 对应插值函数的指针，类型无效时返回 nullptr
         */
        inline LerpFunc2PointFloat GetLerpFuncPointer(LerpType type)
        {
            switch (type)
            {
                case LerpType::Linear:      return LerpLinear;
                case LerpType::Cos:         return LerpCos;
                case LerpType::Cubic:       return LerpCubic;
                case LerpType::Hermite:     return LerpCubic;  // Hermite is alias for Cubic
                default:                    return nullptr;
            }
        }

        /**
         * @brief 获取二次贝塞尔插值函数指针
         * @param type 插值类型
         * @return 匹配时返回 LerpQuadraticBezier 指针，否则返回 nullptr
         */
        inline LerpFuncQuadBezierFloat GetLerpQuadraticBezierFuncPointer(LerpType type)
        {
            if (type == LerpType::QuadraticBezier)
                return LerpQuadraticBezier;
            return nullptr;
        }

        /**
         * @brief 获取 4 点插值函数指针（贝塞尔、CatmullRom、BSpline）
         * @param type 插值类型
         * @return 对应插值函数的指针，类型无效时返回 nullptr
         */
        inline LerpFunc4PointFloat GetLerpBezierFuncPointer(LerpType type)
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
