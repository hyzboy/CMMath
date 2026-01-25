/**
 * MemoryUtil.h - 内存工具函数
 *
 * 提供内存比较、清零等工具函数。
 */

#pragma once

#include <cstring>

namespace hgl
{
    /**
     * @brief 比较两块内存
     * @param a 第一块内存
     * @param b 第二块内存
     * @return 0 表示相等，非 0 表示不等
     */
    template<typename T>
    inline int mem_compare(const T &a, const T &b)
    {
        return std::memcmp(&a, &b, sizeof(T));
    }

    /**
     * @brief 将内存清零
     * @param data 待清零的数据
     */
    template<typename T>
    inline void mem_zero(T &data)
    {
        std::memset(&data, 0, sizeof(T));
    }

}//namespace hgl
