/**
 * AABB2D.h - 2D 轴对齐包围盒
 * 
 * 实现 2D 轴对齐包围盒（Axis-Aligned Bounding Box），
 * 用于 2D 游戏中的碰撞检测、空间划分等。
 * 
 * 包含：
 * - 构造与设置
 * - 属性查询（中心、尺寸、面积等）
 * - 碰撞检测（点、AABB、圆）
 * - 操作（合并、扩展、平移）
 */

#pragma once

#include <hgl/math/VectorTypes.h>
#include <hgl/math/VectorOperations.h>
#include <algorithm>
#include <limits>

namespace hgl::math
{
    /**
     * @brief 2D 轴对齐包围盒类
     */
    class AABB2D
    {
    public:
        Vector2f min;   ///< 最小点（左下角）
        Vector2f max;   ///< 最大点（右上角）
        
        // ==================== 构造函数 ====================
        
        /**
         * @brief 默认构造函数，创建无效的 AABB
         */
        AABB2D()
            : min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max())
            , max(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest())
        {
        }
        
        /**
         * @brief 从两个点构造 AABB
         * @param p_min 最小点
         * @param p_max 最大点
         */
        AABB2D(const Vector2f &p_min, const Vector2f &p_max)
            : min(p_min)
            , max(p_max)
        {
        }
        
        /**
         * @brief 从中心和尺寸创建 AABB
         * @param center 中心点
         * @param size 尺寸（宽度和高度）
         * @return AABB2D 对象
         */
        static AABB2D FromCenterSize(const Vector2f &center, const Vector2f &size)
        {
            Vector2f half_size = size * 0.5f;
            return AABB2D(center - half_size, center + half_size);
        }
        
        /**
         * @brief 从中心和半尺寸创建 AABB
         * @param center 中心点
         * @param extent 半尺寸（半宽和半高）
         * @return AABB2D 对象
         */
        static AABB2D FromCenterExtent(const Vector2f &center, const Vector2f &extent)
        {
            return AABB2D(center - extent, center + extent);
        }
        
        // ==================== 属性查询 ====================
        
        /**
         * @brief 获取中心点
         */
        Vector2f GetCenter() const
        {
            return (min + max) * 0.5f;
        }
        
        /**
         * @brief 获取尺寸（宽度和高度）
         */
        Vector2f GetSize() const
        {
            return max - min;
        }
        
        /**
         * @brief 获取半尺寸（extent）
         */
        Vector2f GetExtent() const
        {
            return (max - min) * 0.5f;
        }
        
        /**
         * @brief 获取面积
         */
        float GetArea() const
        {
            Vector2f size = GetSize();
            return size.x * size.y;
        }
        
        /**
         * @brief 获取周长
         */
        float GetPerimeter() const
        {
            Vector2f size = GetSize();
            return 2.0f * (size.x + size.y);
        }
        
        /**
         * @brief 检查 AABB 是否为空（无效）
         */
        bool IsEmpty() const
        {
            return min.x > max.x || min.y > max.y;
        }
        
        /**
         * @brief 检查 AABB 是否有效
         */
        bool IsValid() const
        {
            return !IsEmpty();
        }
        
        /**
         * @brief 获取宽度
         */
        float GetWidth() const
        {
            return max.x - min.x;
        }
        
        /**
         * @brief 获取高度
         */
        float GetHeight() const
        {
            return max.y - min.y;
        }
        
        // ==================== 碰撞检测 ====================
        
        /**
         * @brief 检查点是否在 AABB 内
         * @param point 待检测的点
         * @return 点在内部返回 true，否则返回 false
         */
        bool Contains(const Vector2f &point) const
        {
            return point.x >= min.x && point.x <= max.x &&
                   point.y >= min.y && point.y <= max.y;
        }
        
        /**
         * @brief 检查是否完全包含另一个 AABB
         * @param other 另一个 AABB
         * @return 完全包含返回 true，否则返回 false
         */
        bool Contains(const AABB2D &other) const
        {
            return other.min.x >= min.x && other.max.x <= max.x &&
                   other.min.y >= min.y && other.max.y <= max.y;
        }
        
        /**
         * @brief 检查与另一个 AABB 是否相交
         * @param other 另一个 AABB
         * @return 相交返回 true，否则返回 false
         */
        bool Intersects(const AABB2D &other) const
        {
            return !(max.x < other.min.x || min.x > other.max.x ||
                     max.y < other.min.y || min.y > other.max.y);
        }
        
        /**
         * @brief 检查与圆是否相交
         * @param center 圆心
         * @param radius 半径
         * @return 相交返回 true，否则返回 false
         */
        bool IntersectsCircle(const Vector2f &center, float radius) const
        {
            // 找到 AABB 上距离圆心最近的点
            Vector2f closest = glm::clamp(center, min, max);
            
            // 计算距离平方
            Vector2f diff = center - closest;
            float dist_squared = glm::dot(diff, diff);
            
            return dist_squared <= radius * radius;
        }
        
        // ==================== 操作 ====================
        
        /**
         * @brief 合并另一个 AABB
         * @param other 另一个 AABB
         */
        void Merge(const AABB2D &other)
        {
            min = glm::min(min, other.min);
            max = glm::max(max, other.max);
        }
        
        /**
         * @brief 扩展 AABB 以包含指定点
         * @param point 待包含的点
         */
        void Merge(const Vector2f &point)
        {
            min = glm::min(min, point);
            max = glm::max(max, point);
        }
        
        /**
         * @brief 返回合并后的新 AABB（不修改自身）
         */
        AABB2D Merged(const AABB2D &other) const
        {
            return AABB2D(glm::min(min, other.min), glm::max(max, other.max));
        }
        
        /**
         * @brief 在所有方向上扩展 AABB
         * @param delta 扩展量（可以是负数表示收缩）
         */
        void Expand(float delta)
        {
            Vector2f expansion(delta, delta);
            min -= expansion;
            max += expansion;
        }
        
        /**
         * @brief 在各方向上扩展 AABB
         * @param delta 各方向的扩展量
         */
        void Expand(const Vector2f &delta)
        {
            min -= delta;
            max += delta;
        }
        
        /**
         * @brief 平移 AABB
         * @param offset 平移向量
         */
        void Translate(const Vector2f &offset)
        {
            min += offset;
            max += offset;
        }
        
        /**
         * @brief 返回平移后的新 AABB（不修改自身）
         */
        AABB2D Translated(const Vector2f &offset) const
        {
            return AABB2D(min + offset, max + offset);
        }
        
        /**
         * @brief 缩放 AABB（从中心缩放）
         * @param scale 缩放因子
         */
        void Scale(float scale)
        {
            Vector2f center = GetCenter();
            Vector2f extent = GetExtent() * scale;
            min = center - extent;
            max = center + extent;
        }
        
        /**
         * @brief 缩放 AABB（各轴独立缩放）
         */
        void Scale(const Vector2f &scale)
        {
            Vector2f center = GetCenter();
            Vector2f extent = GetExtent() * scale;
            min = center - extent;
            max = center + extent;
        }
        
        // ==================== 辅助函数 ====================
        
        /**
         * @brief 计算点到 AABB 的最近点
         * @param point 输入点
         * @return AABB 上距离输入点最近的点
         */
        Vector2f ClosestPoint(const Vector2f &point) const
        {
            return glm::clamp(point, min, max);
        }
        
        /**
         * @brief 计算点到 AABB 的距离
         */
        float Distance(const Vector2f &point) const
        {
            Vector2f closest = ClosestPoint(point);
            return glm::length(point - closest);
        }
        
        /**
         * @brief 计算点到 AABB 的距离平方（避免开方运算）
         */
        float DistanceSquared(const Vector2f &point) const
        {
            Vector2f closest = ClosestPoint(point);
            Vector2f diff = point - closest;
            return glm::dot(diff, diff);
        }
        
        /**
         * @brief 重置为无效状态
         */
        void Clear()
        {
            min = Vector2f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
            max = Vector2f(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
        }
        
        // ==================== 运算符重载 ====================
        
        /**
         * @brief 合并运算符
         */
        AABB2D& operator+=(const AABB2D &other)
        {
            Merge(other);
            return *this;
        }
        
        /**
         * @brief 相等比较
         */
        bool operator==(const AABB2D &other) const
        {
            return min == other.min && max == other.max;
        }
        
        /**
         * @brief 不等比较
         */
        bool operator!=(const AABB2D &other) const
        {
            return !(*this == other);
        }
    };
    
}//namespace hgl::math
