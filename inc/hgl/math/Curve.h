/**
 * Curve.h - 曲线系统
 * 
 * 包含高级曲线和样条功能：
 * - Bezier 曲线（二次、三次）
 * - Catmull-Rom 样条
 * - B 样条
 * - 曲线工具函数（长度、切线、法线）
 * 
 * 参考：
 * - "Computer Graphics: Principles and Practice" (Bezier & B-Spline 章节)
 * - "The NURBS Book" by Les Piegl and Wayne Tiller
 */

#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/CatmullRomSpline3D.h>
#include<hgl/math/CatmullRomSpline2D.h>
#include<vector>

namespace hgl::math
{
    // ==================== Bezier 曲线 ====================
    
    /**
     * 二次 Bezier 曲线（3 控制点）
     * @param p0, p1, p2 控制点
     * @param t 参数 [0, 1]
     * @return 曲线上的点
     */
    Vector3f QuadraticBezier(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, float t);
    
    /**
     * 三次 Bezier 曲线（4 控制点）
     * @param p0, p1, p2, p3 控制点
     * @param t 参数 [0, 1]
     * @return 曲线上的点
     */
    Vector3f CubicBezier(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, float t);
    
    /**
     * 计算 Bezier 曲线长度（数值积分）
     * @param p0, p1, p2, p3 控制点
     * @param steps 积分步数（越大越精确）
     * @return 曲线长度
     */
    float CubicBezierLength(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, int steps = 100);
    
    /**
     * Bezier 曲线切线（归一化）
     * @param p0, p1, p2, p3 控制点
     * @param t 参数 [0, 1]
     * @return 切线方向
     */
    Vector3f CubicBezierTangent(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, float t);
    
    /**
     * Bezier 曲线法线（Frenet 标架）
     * @param p0, p1, p2, p3 控制点
     * @param t 参数 [0, 1]
     * @param up 参考上向量
     * @return 法线方向
     */
    Vector3f CubicBezierNormal(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, float t, const Vector3f &up);
    
    // ==================== Catmull-Rom Spline ====================
    // 注意：CatmullRomSpline3D 和 CatmullRomSpline2D 类已移至独立头文件
    
    // ==================== B-Spline ====================
    
    /**
     * 均匀 B 样条曲线
     * @param points 控制点数组
     * @param count 控制点数量
     * @param t 参数 [0, 1]
     * @param degree 曲线阶数（通常为 3）
     * @return 曲线上的点
     */
    Vector3f UniformBSpline(const Vector3f *points, int count, float t, int degree = 3);
    
    /**
     * B 样条基函数（Cox-de Boor 递归公式）
     * @param i 基函数索引
     * @param k 阶数
     * @param t 参数
     * @param knots 节点向量
     * @return 基函数值
     */
    float BSplineBasis(int i, int k, float t, const std::vector<float> &knots);

}//namespace hgl::math
