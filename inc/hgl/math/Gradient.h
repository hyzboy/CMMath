#pragma once

#include <hgl/math/VectorOperations.h>
#include <vector>
#include <algorithm>

namespace hgl::math
{
    /**
     * @brief 渐变停止点结构
     * @tparam P 位置类型（进度数据）
     * @tparam T 数据类型
     */
    template<typename P, typename T> 
    struct GradientStop
    {
        P pos;      ///< 进度数据
        T data;     ///< 数据
        
        bool operator<(const GradientStop& other) const
        {
            return pos < other.pos;
        }
    };

    /**
     * @brief 数据渐变类模板
     * 
     * 用于在多个关键点之间进行插值，支持任意数据类型。
     * 常用于颜色渐变、数值动画等场景。
     * 
     * @tparam P 位置类型（通常是 float 或 double）
     * @tparam T 数据类型（可以是数值、向量、颜色等）
     * 
     * 使用示例：
     * ```cpp
     * Gradient<float, Color3f> colorGradient;
     * colorGradient.Add(0.0f, Color3f(1, 0, 0));  // 红色
     * colorGradient.Add(0.5f, Color3f(0, 1, 0));  // 绿色
     * colorGradient.Add(1.0f, Color3f(0, 0, 1));  // 蓝色
     * 
     * Color3f result;
     * colorGradient.Get(result, 0.25f);  // 获取 25% 位置的颜色
     * ```
     */
    template<typename P, typename T> 
    class Gradient
    {
    private:
        mutable std::vector<GradientStop<P, T>> stop_list;
        mutable bool dirty;

    protected:
        void Sort() const
        {
            if (!dirty || stop_list.size() < 2)
                return;

            std::sort(stop_list.begin(), stop_list.end());
            dirty = false;
        }

    public:
        Gradient() : dirty(false) {}

        /**
         * @brief 添加一个渐变点
         * @param pos 位置/进度
         * @param data 该位置的数据值
         */
        void Add(const P& pos, const T& data)
        {
            GradientStop<P, T> gs;
            gs.pos = pos;
            gs.data = data;
            stop_list.push_back(gs);
            dirty = true;
        }

        /**
         * @brief 清除所有渐变点
         */
        void Clear()
        {
            stop_list.clear();
            dirty = true;
        }

        /**
         * @brief 获取渐变点数量
         */
        size_t GetCount() const
        {
            return stop_list.size();
        }

        /**
         * @brief 取得最低渐变进度
         */
        bool GetLowestPosition(P& pos) const
        {
            if (stop_list.empty())
                return false;

            Sort();
            pos = stop_list.front().pos;
            return true;
        }

        /**
         * @brief 取得最高渐变进度
         */
        bool GetHighestPosition(P& pos) const
        {
            if (stop_list.empty())
                return false;

            Sort();
            pos = stop_list.back().pos;
            return true;
        }

        /**
         * @brief 取得最低渐变结果
         */
        bool GetLowestData(T& data) const
        {
            if (stop_list.empty())
                return false;

            Sort();
            data = stop_list.front().data;
            return true;
        }

        /**
         * @brief 取得最高渐变结果
         */
        bool GetHighestData(T& data) const
        {
            if (stop_list.empty())
                return false;

            Sort();
            data = stop_list.back().data;
            return true;
        }

        /**
         * @brief 插值函数（可重写以自定义插值行为）
         * @param result 输出结果
         * @param start 起始值
         * @param end 结束值
         * @param t 插值参数 [0, 1]
         * 
         * 默认使用 hgl::math::lerp 进行线性插值
         */
        virtual void Interpolate(T& result, const T& start, const T& end, float t) const
        {
            result = lerp(start, end, t);  // 使用 hgl::math::lerp
        }

        /**
         * @brief 取得指定进度下的渐变结果
         * @param result 输出结果
         * @param pos 进度位置
         */
        void Get(T& result, const P& pos) const
        {
            if (stop_list.empty())
                return;

            Sort();

            const size_t count = stop_list.size();

            // 如果只有一个点，或者位置在第一个点之前
            if (count == 1 || pos <= stop_list[0].pos)
            {
                result = stop_list[0].data;
                return;
            }

            // 如果位置在最后一个点之后
            if (pos >= stop_list[count - 1].pos)
            {
                result = stop_list[count - 1].data;
                return;
            }

            // 在中间，找到对应的区间并插值
            for (size_t i = 1; i < count; ++i)
            {
                if (pos <= stop_list[i].pos)
                {
                    const auto& prev = stop_list[i - 1];
                    const auto& curr = stop_list[i];
                    
                    // 防止除零错误
                    P posDiff = curr.pos - prev.pos;
                    if (posDiff <= static_cast<P>(0))
                    {
                        result = prev.data;
                        return;
                    }
                    
                    float t = static_cast<float>(pos - prev.pos) / 
                             static_cast<float>(posDiff);
                    
                    Interpolate(result, prev.data, curr.data, t);
                    return;
                }
            }
        }

        /**
         * @brief 操作符重载，方便直接获取渐变值
         */
        T operator()(const P& pos) const
        {
            T result;
            Get(result, pos);
            return result;
        }
    };

    // 常用类型别名
    using FloatGradient = Gradient<float, float>;
    using ColorGradient3f = Gradient<float, Color3f>;
    using ColorGradient4f = Gradient<float, Color4f>;
    using VectorGradient2f = Gradient<float, Vector2f>;
    using VectorGradient3f = Gradient<float, Vector3f>;

} // namespace hgl::math
