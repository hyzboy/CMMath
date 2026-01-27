/**
 * Curve.cpp - 曲线系统实现
 */

#include<hgl/math/Curve.h>
#include<hgl/math/VectorOperations.h>
#include<cmath>
#include<algorithm>

namespace hgl::math
{
    // ==================== Bezier 曲线实现 ====================

    Vector3f QuadraticBezier(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, float t)
    {
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;

        // B(t) = (1-t)²P0 + 2(1-t)tP1 + t²P2
        return uu * p0 + 2.0f * u * t * p1 + tt * p2;
    }

    Vector3f CubicBezier(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, float t)
    {
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        // B(t) = (1-t)³P0 + 3(1-t)²tP1 + 3(1-t)t²P2 + t³P3
        return uuu * p0 + 3.0f * uu * t * p1 + 3.0f * u * tt * p2 + ttt * p3;
    }

    float CubicBezierLength(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, int steps)
    {
        float length = 0.0f;
        Vector3f prev = p0;

        for (int i = 1; i <= steps; i++)
        {
            float t = static_cast<float>(i) / static_cast<float>(steps);
            Vector3f current = CubicBezier(p0, p1, p2, p3, t);
            length += glm::length(current - prev);
            prev = current;
        }

        return length;
    }

    Vector3f CubicBezierTangent(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, float t)
    {
        float u = 1.0f - t;
        float uu = u * u;
        float tt = t * t;

        // B'(t) = 3(1-t)²(P1-P0) + 6(1-t)t(P2-P1) + 3t²(P3-P2)
        Vector3f tangent = 3.0f * uu * (p1 - p0) +
                          6.0f * u * t * (p2 - p1) +
                          3.0f * tt * (p3 - p2);

        return glm::normalize(tangent);
    }

    Vector3f CubicBezierNormal(const Vector3f &p0, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, float t, const Vector3f &up)
    {
        Vector3f tangent = CubicBezierTangent(p0, p1, p2, p3, t);
        Vector3f binormal = Normalized(Cross(tangent, up));
        Vector3f normal = Cross(binormal, tangent);
        return glm::normalize(normal);
    }

    // ==================== Catmull-Rom Spline 实现 ====================
    // 注意：CatmullRomSpline3D 和 CatmullRomSpline2D 实现已移至独立的 .cpp 文件

    // ==================== B-Spline 实现 ====================

    float BSplineBasis(int i, int k, float t, const std::vector<float> &knots)
    {
        // Cox-de Boor 递归公式
        if (k == 1)
        {
            return (t >= knots[i] && t < knots[i + 1]) ? 1.0f : 0.0f;
        }

        float denom1 = knots[i + k - 1] - knots[i];
        float denom2 = knots[i + k] - knots[i + 1];

        float term1 = 0.0f;
        float term2 = 0.0f;

        if (denom1 > 1e-8f)
        {
            term1 = ((t - knots[i]) / denom1) * BSplineBasis(i, k - 1, t, knots);
        }

        if (denom2 > 1e-8f)
        {
            term2 = ((knots[i + k] - t) / denom2) * BSplineBasis(i + 1, k - 1, t, knots);
        }

        return term1 + term2;
    }

    Vector3f UniformBSpline(const Vector3f *points, int count, float t, int degree)
    {
        if (count < degree + 1)
            return Vector3f(0.0f);

        // 创建均匀节点向量
        int knot_count = count + degree + 1;
        std::vector<float> knots(knot_count);

        for (int i = 0; i < knot_count; i++)
        {
            knots[i] = static_cast<float>(i);
        }

        // 将 t 映射到有效范围
        float t_min = knots[degree];
        float t_max = knots[count];
        t = t_min + t * (t_max - t_min);

        // 计算曲线点
        Vector3f result(0.0f);

        for (int i = 0; i < count; i++)
        {
            float basis = BSplineBasis(i, degree + 1, t, knots);
            result += basis * points[i];
        }

        return result;
    }

}//namespace hgl::math
