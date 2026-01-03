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
    
    // ========== 3D 版本 ========= =
    
    CatmullRomSpline3D::CatmullRomSpline3D()
    {
    }
    
    void CatmullRomSpline3D::AddPoint(const Vector3f &point)
    {
        control_points.push_back(point);
    }
    
    void CatmullRomSpline3D::SetPoints(const std::vector<Vector3f> &points)
    {
        control_points = points;
    }
    
    Vector3f CatmullRomSpline3D::EvaluateSegment(int segment, float t) const
    {
        // Catmull-Rom 需要 4 个点：P-1, P0, P1, P2
        // 对于边界情况，复制端点
        
        Vector3f p0, p1, p2, p3;
        
        if (segment == 0)
        {
            p0 = control_points[0];  // 复制第一个点
            p1 = control_points[0];
            p2 = control_points[1];
            p3 = (control_points.size() > 2) ? control_points[2] : control_points[1];
        }
        else if (segment >= static_cast<int>(control_points.size()) - 2)
        {
            int last = control_points.size() - 1;
            p0 = (segment > 0) ? control_points[segment - 1] : control_points[0];
            p1 = control_points[segment];
            p2 = control_points[last];
            p3 = control_points[last];  // 复制最后一个点
        }
        else
        {
            p0 = control_points[segment - 1];
            p1 = control_points[segment];
            p2 = control_points[segment + 1];
            p3 = control_points[segment + 2];
        }
        
        // Catmull-Rom 矩阵形式
        float t2 = t * t;
        float t3 = t2 * t;
        
        Vector3f result = 0.5f * (
            (2.0f * p1) +
            (-p0 + p2) * t +
            (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
            (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
        );
        
        return result;
    }
    
    Vector3f CatmullRomSpline3D::Evaluate(float t) const
    {
        if (control_points.empty())
            return Vector3f(0.0f);
        
        if (control_points.size() == 1)
            return control_points[0];
        
        // 将 t 映射到线段和局部参数
        float total_length = static_cast<float>(control_points.size() - 1);
        t = std::clamp(t, 0.0f, total_length);
        
        int segment = static_cast<int>(std::floor(t));
        float local_t = t - segment;
        
        if (segment >= static_cast<int>(control_points.size()) - 1)
        {
            segment = control_points.size() - 2;
            local_t = 1.0f;
        }
        
        return EvaluateSegment(segment, local_t);
    }
    
    Vector3f CatmullRomSpline3D::GetSegmentTangent(int segment, float t) const
    {
        Vector3f p0, p1, p2, p3;
        
        if (segment == 0)
        {
            p0 = control_points[0];
            p1 = control_points[0];
            p2 = control_points[1];
            p3 = (control_points.size() > 2) ? control_points[2] : control_points[1];
        }
        else if (segment >= static_cast<int>(control_points.size()) - 2)
        {
            int last = control_points.size() - 1;
            p0 = (segment > 0) ? control_points[segment - 1] : control_points[0];
            p1 = control_points[segment];
            p2 = control_points[last];
            p3 = control_points[last];
        }
        else
        {
            p0 = control_points[segment - 1];
            p1 = control_points[segment];
            p2 = control_points[segment + 1];
            p3 = control_points[segment + 2];
        }
        
        float t2 = t * t;
        
        Vector3f tangent = 0.5f * (
            (-p0 + p2) +
            (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * 2.0f * t +
            (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * 3.0f * t2
        );
        
        return glm::normalize(tangent);
    }
    
    Vector3f CatmullRomSpline3D::GetTangent(float t) const
    {
        if (control_points.size() < 2)
            return Vector3f(1.0f, 0.0f, 0.0f);
        
        float total_length = static_cast<float>(control_points.size() - 1);
        t = std::clamp(t, 0.0f, total_length);
        
        int segment = static_cast<int>(std::floor(t));
        float local_t = t - segment;
        
        if (segment >= static_cast<int>(control_points.size()) - 1)
        {
            segment = control_points.size() - 2;
            local_t = 1.0f;
        }
        
        return GetSegmentTangent(segment, local_t);
    }
    
    float CatmullRomSpline3D::GetLength(int samples_per_segment) const
    {
        if (control_points.size() < 2)
            return 0.0f;
        
        float length = 0.0f;
        int segments = control_points.size() - 1;
        
        for (int seg = 0; seg < segments; seg++)
        {
            Vector3f prev = EvaluateSegment(seg, 0.0f);
            
            for (int i = 1; i <= samples_per_segment; i++)
            {
                float t = static_cast<float>(i) / static_cast<float>(samples_per_segment);
                Vector3f current = EvaluateSegment(seg, t);
                length += glm::length(current - prev);
                prev = current;
            }
        }
        
        return length;
    }
    
    int CatmullRomSpline3D::GetPointCount() const
    {
        return control_points.size();
    }
    
    void CatmullRomSpline3D::Clear()
    {
        control_points.clear();
    }
    
    // ========== 2D 版本 ========= =
    
    CatmullRomSpline2D::CatmullRomSpline2D()
    {
    }
    
    void CatmullRomSpline2D::AddPoint(const Vector2f &point)
    {
        control_points.push_back(point);
    }
    
    void CatmullRomSpline2D::SetPoints(const std::vector<Vector2f> &points)
    {
        control_points = points;
    }
    
    Vector2f CatmullRomSpline2D::EvaluateSegment(int segment, float t) const
    {
        // Catmull-Rom 需要 4 个点：P-1, P0, P1, P2
        // 对于边界情况，复制端点
        
        Vector2f p0, p1, p2, p3;
        
        if (segment == 0)
        {
            p0 = control_points[0];  // 复制第一个点
            p1 = control_points[0];
            p2 = control_points[1];
            p3 = (control_points.size() > 2) ? control_points[2] : control_points[1];
        }
        else if (segment >= static_cast<int>(control_points.size()) - 2)
        {
            int last = control_points.size() - 1;
            p0 = (segment > 0) ? control_points[segment - 1] : control_points[0];
            p1 = control_points[segment];
            p2 = control_points[last];
            p3 = control_points[last];  // 复制最后一个点
        }
        else
        {
            p0 = control_points[segment - 1];
            p1 = control_points[segment];
            p2 = control_points[segment + 1];
            p3 = control_points[segment + 2];
        }
        
        // Catmull-Rom 矩阵形式
        float t2 = t * t;
        float t3 = t2 * t;
        
        Vector2f result = 0.5f * (
            (2.0f * p1) +
            (-p0 + p2) * t +
            (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
            (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
        );
        
        return result;
    }
    
    Vector2f CatmullRomSpline2D::Evaluate(float t) const
    {
        if (control_points.empty())
            return Vector2f(0.0f);
        
        if (control_points.size() == 1)
            return control_points[0];
        
        // 将 t 映射到线段和局部参数
        float total_length = static_cast<float>(control_points.size() - 1);
        t = std::clamp(t, 0.0f, total_length);
        
        int segment = static_cast<int>(std::floor(t));
        float local_t = t - segment;
        
        if (segment >= static_cast<int>(control_points.size()) - 1)
        {
            segment = control_points.size() - 2;
            local_t = 1.0f;
        }
        
        return EvaluateSegment(segment, local_t);
    }
    
    Vector2f CatmullRomSpline2D::GetSegmentTangent(int segment, float t) const
    {
        Vector2f p0, p1, p2, p3;
        
        if (segment == 0)
        {
            p0 = control_points[0];
            p1 = control_points[0];
            p2 = control_points[1];
            p3 = (control_points.size() > 2) ? control_points[2] : control_points[1];
        }
        else if (segment >= static_cast<int>(control_points.size()) - 2)
        {
            int last = control_points.size() - 1;
            p0 = (segment > 0) ? control_points[segment - 1] : control_points[0];
            p1 = control_points[segment];
            p2 = control_points[last];
            p3 = control_points[last];
        }
        else
        {
            p0 = control_points[segment - 1];
            p1 = control_points[segment];
            p2 = control_points[segment + 1];
            p3 = control_points[segment + 2];
        }
        
        float t2 = t * t;
        
        Vector2f tangent = 0.5f * (
            (-p0 + p2) +
            (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * 2.0f * t +
            (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * 3.0f * t2
        );
        
        return glm::normalize(tangent);
    }
    
    Vector2f CatmullRomSpline2D::GetTangent(float t) const
    {
        if (control_points.size() < 2)
            return Vector2f(1.0f, 0.0f);
        
        float total_length = static_cast<float>(control_points.size() - 1);
        t = std::clamp(t, 0.0f, total_length);
        
        int segment = static_cast<int>(std::floor(t));
        float local_t = t - segment;
        
        if (segment >= static_cast<int>(control_points.size()) - 1)
        {
            segment = control_points.size() - 2;
            local_t = 1.0f;
        }
        
        return GetSegmentTangent(segment, local_t);
    }
    
    float CatmullRomSpline2D::GetLength(int samples_per_segment) const
    {
        if (control_points.size() < 2)
            return 0.0f;
        
        float length = 0.0f;
        int segments = control_points.size() - 1;
        
        for (int seg = 0; seg < segments; seg++)
        {
            Vector2f prev = EvaluateSegment(seg, 0.0f);
            
            for (int i = 1; i <= samples_per_segment; i++)
            {
                float t = static_cast<float>(i) / static_cast<float>(samples_per_segment);
                Vector2f current = EvaluateSegment(seg, t);
                length += glm::length(current - prev);
                prev = current;
            }
        }
        
        return length;
    }
    
    int CatmullRomSpline2D::GetPointCount() const
    {
        return control_points.size();
    }
    
    void CatmullRomSpline2D::Clear()
    {
        control_points.clear();
    }
    
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
