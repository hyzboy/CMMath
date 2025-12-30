#include<hgl/math/geometry/GeometryUtils.h>
#include<hgl/math/MathConstants.h>
#include<hgl/math/FloatPrecision.h>
#include<glm/glm.hpp>
#include<cmath>
#include<algorithm>

namespace hgl::math
{
    Vector3f TriangleNormal(const Vector3f &A, const Vector3f &B, const Vector3f &C)
    {
        // 计算两条边向量
        Vector3f AB = B - A;
        Vector3f AC = C - A;
        
        // 使用GLM的叉积函数计算法线
        return glm::cross(AB, AC);
    }

    bool LineIntersect(const Vector2f &p, const Vector2f &r, 
                       const Vector2f &q, const Vector2f &s, 
                       float &t, float &u)
    {
        // 计算 r × s (2D叉积的标量结果)
        float rxs = r.x * s.y - r.y * s.x;
        
        // 如果 r × s 接近零，说明两直线平行或共线
        if(std::fabs(rxs) < float_error) 
            return false;
        
        // 计算 q - p
        Vector2f qp = q - p;
        
        // 计算参数 t 和 u
        // t = (q - p) × s / (r × s)
        t = (qp.x * s.y - qp.y * s.x) / rxs;
        
        // u = (q - p) × r / (r × s)
        u = (qp.x * r.y - qp.y * r.x) / rxs;
        
        return true;
    }

    float AngleBetween(const float ax, const float ay, const float bx, const float by)
    {
        // 计算向量长度
        float la = std::sqrt(ax*ax + ay*ay);
        float lb = std::sqrt(bx*bx + by*by);
        
        // 如果任一向量长度接近零，返回0
        if(la < float_error || lb < float_error) 
            return 0.0f;
        
        // 计算归一化点积
        float dot = (ax*bx + ay*by) / (la*lb);
        
        // 将点积钳制到[-1, 1]范围，避免浮点误差导致的数值域错误
        dot = std::clamp(dot, -1.0f, 1.0f);
        
        // 计算夹角并转换为度数
        // 使用 hgl::math::pi_f 常量
        return std::acos(dot) * 180.0f / pi_f;
    }
}//namespace hgl::math
