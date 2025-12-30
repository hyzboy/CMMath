/**
 * Collision2D.h - 2D 碰撞检测工具
 * 
 * 实现 2D 几何图元的碰撞检测函数。
 * 参考：Real-Time Collision Detection (Christer Ericson)
 * 
 * 包含：
 * - 圆与圆、圆与点碰撞
 * - 矩形与矩形、矩形与点碰撞
 * - 圆与矩形碰撞
 * - 线段相交检测
 * - 射线碰撞检测
 */

#pragma once

#include <hgl/math/VectorTypes.h>
#include <hgl/math/VectorOperations.h>
#include <hgl/math/geometry/AABB2D.h>
#include <cmath>

namespace hgl::math
{
    // ==================== 圆形碰撞检测 ====================
    
    /**
     * @brief 检测两个圆是否相交
     * @param c1 第一个圆的圆心
     * @param r1 第一个圆的半径
     * @param c2 第二个圆的圆心
     * @param r2 第二个圆的半径
     * @return 相交返回 true，否则返回 false
     */
    inline bool CircleCircleIntersection(const Vector2f &c1, float r1, 
                                          const Vector2f &c2, float r2)
    {
        Vector2f diff = c2 - c1;
        float dist_squared = glm::dot(diff, diff);
        float radius_sum = r1 + r2;
        return dist_squared <= radius_sum * radius_sum;
    }
    
    /**
     * @brief 检测点是否在圆内
     * @param center 圆心
     * @param radius 半径
     * @param point 待检测的点
     * @return 点在圆内返回 true，否则返回 false
     */
    inline bool CirclePointIntersection(const Vector2f &center, float radius, 
                                         const Vector2f &point)
    {
        Vector2f diff = point - center;
        float dist_squared = glm::dot(diff, diff);
        return dist_squared <= radius * radius;
    }
    
    // ==================== 矩形（AABB）碰撞检测 ====================
    
    /**
     * @brief 检测两个矩形（AABB）是否相交
     * @param rect1 第一个矩形
     * @param rect2 第二个矩形
     * @return 相交返回 true，否则返回 false
     */
    inline bool RectRectIntersection(const AABB2D &rect1, const AABB2D &rect2)
    {
        return rect1.Intersects(rect2);
    }
    
    /**
     * @brief 检测点是否在矩形内
     * @param rect 矩形
     * @param point 待检测的点
     * @return 点在矩形内返回 true，否则返回 false
     */
    inline bool RectPointIntersection(const AABB2D &rect, const Vector2f &point)
    {
        return rect.Contains(point);
    }
    
    // ==================== 圆与矩形碰撞 ====================
    
    /**
     * @brief 检测圆与矩形（AABB）是否相交
     * @param circle_center 圆心
     * @param radius 半径
     * @param rect 矩形
     * @return 相交返回 true，否则返回 false
     */
    inline bool CircleRectIntersection(const Vector2f &circle_center, float radius, 
                                        const AABB2D &rect)
    {
        return rect.IntersectsCircle(circle_center, radius);
    }
    
    // ==================== 线段相交检测 ====================
    
    /**
     * @brief 检测两条线段是否相交，并返回交点
     * 
     * 使用参数化方法检测线段相交。
     * 线段1: P1 + t * (P2 - P1), t ∈ [0, 1]
     * 线段2: P3 + u * (P4 - P3), u ∈ [0, 1]
     * 
     * @param p1 第一条线段的起点
     * @param p2 第一条线段的终点
     * @param p3 第二条线段的起点
     * @param p4 第二条线段的终点
     * @param out_intersection 输出交点（如果相交）
     * @return 相交返回 true 并设置 out_intersection，否则返回 false
     */
    inline bool LineSegmentIntersection(const Vector2f &p1, const Vector2f &p2,
                                         const Vector2f &p3, const Vector2f &p4,
                                         Vector2f &out_intersection)
    {
        Vector2f s1 = p2 - p1;
        Vector2f s2 = p4 - p3;
        
        // 计算叉积
        float s1_cross_s2 = s1.x * s2.y - s1.y * s2.x;
        
        // 检查平行情况（叉积为零）
        if (std::abs(s1_cross_s2) < 1e-6f)
        {
            return false;
        }
        
        Vector2f p1_to_p3 = p3 - p1;
        
        // 计算参数 t 和 u
        float t = (p1_to_p3.x * s2.y - p1_to_p3.y * s2.x) / s1_cross_s2;
        float u = (p1_to_p3.x * s1.y - p1_to_p3.y * s1.x) / s1_cross_s2;
        
        // 检查是否在线段范围内
        if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
        {
            // 计算交点
            out_intersection = p1 + s1 * t;
            return true;
        }
        
        return false;
    }
    
    /**
     * @brief 检测两条线段是否相交（不返回交点）
     */
    inline bool LineSegmentIntersection(const Vector2f &p1, const Vector2f &p2,
                                         const Vector2f &p3, const Vector2f &p4)
    {
        Vector2f dummy;
        return LineSegmentIntersection(p1, p2, p3, p4, dummy);
    }
    
    // ==================== 射线碰撞检测 ====================
    
    /**
     * @brief 检测射线与圆是否相交
     * 
     * 射线：origin + t * direction, t ≥ 0
     * 
     * @param origin 射线起点
     * @param direction 射线方向（应为归一化向量）
     * @param circle_center 圆心
     * @param radius 半径
     * @param out_t 输出参数 t（距离），如果相交则为最近交点的距离
     * @return 相交返回 true 并设置 out_t，否则返回 false
     */
    inline bool RayCircleIntersection(const Vector2f &origin, const Vector2f &direction,
                                       const Vector2f &circle_center, float radius,
                                       float &out_t)
    {
        Vector2f to_circle = circle_center - origin;
        
        // 投影距离
        float proj = glm::dot(to_circle, direction);
        
        // 圆心到射线的最近点
        Vector2f closest = origin + direction * proj;
        Vector2f closest_to_center = circle_center - closest;
        float dist_squared = glm::dot(closest_to_center, closest_to_center);
        
        // 检查是否相交
        if (dist_squared > radius * radius)
        {
            return false;
        }
        
        // 计算交点距离
        float chord_half_length = std::sqrt(radius * radius - dist_squared);
        out_t = proj - chord_half_length;
        
        // 检查交点是否在射线正方向
        if (out_t < 0.0f)
        {
            out_t = proj + chord_half_length;
            if (out_t < 0.0f)
            {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * @brief 检测射线与圆是否相交（不返回距离）
     */
    inline bool RayCircleIntersection(const Vector2f &origin, const Vector2f &direction,
                                       const Vector2f &circle_center, float radius)
    {
        float dummy;
        return RayCircleIntersection(origin, direction, circle_center, radius, dummy);
    }
    
    /**
     * @brief 检测射线与矩形（AABB）是否相交
     * 
     * 使用 slab 方法进行射线-AABB 相交检测。
     * 
     * @param origin 射线起点
     * @param direction 射线方向（应为归一化向量）
     * @param rect 矩形
     * @param out_t 输出参数 t（距离），如果相交则为最近交点的距离
     * @return 相交返回 true 并设置 out_t，否则返回 false
     */
    inline bool RayRectIntersection(const Vector2f &origin, const Vector2f &direction,
                                     const AABB2D &rect, float &out_t)
    {
        // 避免除以零
        Vector2f inv_dir(
            std::abs(direction.x) > 1e-6f ? 1.0f / direction.x : std::numeric_limits<float>::max(),
            std::abs(direction.y) > 1e-6f ? 1.0f / direction.y : std::numeric_limits<float>::max()
        );
        
        // 计算 t 的范围
        float t1 = (rect.min.x - origin.x) * inv_dir.x;
        float t2 = (rect.max.x - origin.x) * inv_dir.x;
        float t3 = (rect.min.y - origin.y) * inv_dir.y;
        float t4 = (rect.max.y - origin.y) * inv_dir.y;
        
        float tmin = std::max(std::min(t1, t2), std::min(t3, t4));
        float tmax = std::min(std::max(t1, t2), std::max(t3, t4));
        
        // 无相交
        if (tmax < 0.0f || tmin > tmax)
        {
            return false;
        }
        
        // 相交点在射线正方向
        out_t = tmin >= 0.0f ? tmin : tmax;
        return out_t >= 0.0f;
    }
    
    /**
     * @brief 检测射线与矩形是否相交（不返回距离）
     */
    inline bool RayRectIntersection(const Vector2f &origin, const Vector2f &direction,
                                     const AABB2D &rect)
    {
        float dummy;
        return RayRectIntersection(origin, direction, rect, dummy);
    }
    
    // ==================== 辅助函数 ====================
    
    /**
     * @brief 计算点到线段的最近点
     * @param point 输入点
     * @param segment_start 线段起点
     * @param segment_end 线段终点
     * @return 线段上距离输入点最近的点
     */
    inline Vector2f ClosestPointOnLineSegment(const Vector2f &point,
                                               const Vector2f &segment_start,
                                               const Vector2f &segment_end)
    {
        Vector2f segment = segment_end - segment_start;
        Vector2f to_point = point - segment_start;
        
        float segment_length_squared = glm::dot(segment, segment);
        
        // 处理退化线段（起点和终点相同）
        if (segment_length_squared < 1e-6f)
        {
            return segment_start;
        }
        
        // 计算投影参数
        float t = glm::dot(to_point, segment) / segment_length_squared;
        t = glm::clamp(t, 0.0f, 1.0f);
        
        return segment_start + segment * t;
    }
    
    /**
     * @brief 计算点到线段的距离
     */
    inline float DistanceToLineSegment(const Vector2f &point,
                                        const Vector2f &segment_start,
                                        const Vector2f &segment_end)
    {
        Vector2f closest = ClosestPointOnLineSegment(point, segment_start, segment_end);
        return glm::length(point - closest);
    }
    
    /**
     * @brief 计算点到线段的距离平方
     */
    inline float DistanceSquaredToLineSegment(const Vector2f &point,
                                               const Vector2f &segment_start,
                                               const Vector2f &segment_end)
    {
        Vector2f closest = ClosestPointOnLineSegment(point, segment_start, segment_end);
        Vector2f diff = point - closest;
        return glm::dot(diff, diff);
    }
    
}//namespace hgl::math
