/**
 * CatmullRomSpline3D.cpp - 3D Catmull-Rom 样条曲线实现
 */

#include<hgl/math/CatmullRomSpline3D.h>
#include<hgl/math/VectorOperations.h>
#include<cmath>
#include<algorithm>

namespace hgl::math
{
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

}//namespace hgl::math
