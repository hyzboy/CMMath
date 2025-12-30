/**
 * GeometryUtils.h - 几何计算工具函数
 * 
 * 提供常用的几何计算函数，包括：
 * - 三角形法线计算
 * - 2D直线相交检测
 * - 向量夹角计算
 * 
 * 这些函数常用于：
 * - 网格生成和处理
 * - 2D/3D图形算法
 * - 碰撞检测
 * - 几何分析
 */

#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
     * 计算三角形的法线向量（未归一化）
     * 
     * 使用叉积计算法线：AB × AC，其中A、B、C是三角形的三个顶点。
     * 结果向量的方向遵循右手定则，长度等于三角形面积的两倍。
     * 
     * @param A 三角形的第一个顶点
     * @param B 三角形的第二个顶点
     * @param C 三角形的第三个顶点
     * @return 三角形的法线向量（未归一化）
     * 
     * @note 如果需要单位法线向量，请对结果调用 normalize()
     * @note 法线方向取决于顶点的顺序（顺时针或逆时针）
     * 
     * 使用场景：
     * - 计算面光照
     * - 背面剔除
     * - 碰撞检测法线
     */
    Vector3f TriangleNormal(const Vector3f &A, const Vector3f &B, const Vector3f &C);
    
    /**
     * 计算两条2D直线的交点参数
     * 
     * 使用参数方程表示两条直线：
     * - L1 = p + t*r
     * - L2 = q + u*s
     * 
     * 其中 p 和 q 是起点，r 和 s 是方向向量，t 和 u 是参数。
     * 
     * @param p 第一条直线的起点
     * @param r 第一条直线的方向向量
     * @param q 第二条直线的起点
     * @param s 第二条直线的方向向量
     * @param t [输出] 第一条直线上的交点参数
     * @param u [输出] 第二条直线上的交点参数
     * @return true 如果直线相交（不平行），false 如果直线平行
     * 
     * @note 如果返回true，交点可计算为：p + t*r 或 q + u*s
     * @note t和u的值范围：
     *       - [0,1] 表示交点在线段内
     *       - <0 或 >1 表示交点在线段延长线上
     * 
     * 使用场景：
     * - 2D碰撞检测
     * - 射线投射
     * - 路径规划
     * - 多边形裁剪
     */
    bool LineIntersect(const Vector2f &p, const Vector2f &r, 
                       const Vector2f &q, const Vector2f &s, 
                       float &t, float &u);
    
    /**
     * 计算两个2D向量之间的夹角（度数）
     * 
     * 使用向量点积和长度计算夹角：
     * angle = arccos(dot(a,b) / (|a|*|b|))
     * 
     * @param ax 第一个向量的x分量
     * @param ay 第一个向量的y分量
     * @param bx 第二个向量的x分量
     * @param by 第二个向量的y分量
     * @return 两向量之间的夹角，单位为度，范围 [0, 180]
     * 
     * @note 如果任一向量长度接近零，返回0
     * @note 结果为无符号角度（始终为正值）
     * 
     * 使用场景：
     * - 方向判断
     * - 角度约束
     * - 转向计算
     * - 相似度判断
     */
    float AngleBetween(const float ax, const float ay, const float bx, const float by);
    
    /**
     * 计算两个2D向量之间的夹角（度数）- Vector2f重载版本
     * 
     * 这是AngleBetween的便捷重载，接受Vector2f参数。
     * 
     * @param a 第一个向量
     * @param b 第二个向量
     * @return 两向量之间的夹角，单位为度，范围 [0, 180]
     * 
     * @see AngleBetween(float, float, float, float)
     */
    inline float AngleBetween(const Vector2f &a, const Vector2f &b)
    {
        return AngleBetween(a.x, a.y, b.x, b.y);
    }
}//namespace hgl::math
