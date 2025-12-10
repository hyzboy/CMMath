/**
* 圆锥体实现文件
* 
* 实现了圆锥体的各种几何计算功能，包括:
* - 点包含性检测
* - 最近点计算
* - 距离计算
* 
* 圆锥体的数学特性:
* - 圆锥体是一个从顶点向外扩展的锥形区域
* - 在任意高度 h 处的半径为: r(h) = base_radius * (h / height)
* - 点在圆锥内当且仅当: 距离轴的径向距离 ≤ 该高度处的半径
*/
#include<hgl/math/geometry/Cone.h>
#include<hgl/math/MathUtils.h>

namespace hgl::math
{
    /**
    * 判断点是否在圆锥体内部
    * 
    * 算法步骤:
    * 1. 计算点在圆锥轴方向的投影(高度)
    * 2. 检查高度是否在[0, height]范围内
    * 3. 计算该高度处圆锥的半径(线性插值)
    * 4. 计算点到轴的径向距离
    * 5. 判断径向距离是否小于等于该高度的半径
    */
    bool Cone::ContainsPoint(const Vector3f &point) const
    {
        // 计算从顶点到目标点的向量
        Vector3f to_point = point - apex;
        
        // 计算点在圆锥轴方向的投影长度(即沿轴向的高度)
        float axis_projection = dot(to_point, axis);
        
        // 检查是否在高度范围内
        // 圆锥从顶点(h=0)开始，到底面(h=height)结束
        if (axis_projection < 0.0f || axis_projection > height)
            return false;  // 点在圆锥上下范围之外
        
        // 计算该高度处的半径(线性插值)
        // 顶点处半径为0，底面处半径为base_radius
        float radius_at_height = GetRadiusAtHeight(axis_projection);
        
        // 计算点到圆锥轴的距离(径向距离)
        Vector3f axis_point = apex + axis * axis_projection;
        float radial_distance = length(point - axis_point);
        
        // 如果径向距离小于等于该高度的半径，则点在圆锥内
        return radial_distance <= radius_at_height;
    }

    /**
    * 计算点到圆锥体表面的最近点
    * 
    * 圆锥表面包括两部分:
    * 1. 侧面(锥面): 从顶点到底面圆周的曲面
    * 2. 底面: 底部的圆形平面
    * 
    * 算法分类讨论:
    * - 点在顶点上方: 最近点是顶点
    * - 点在底面下方: 需要考虑底面圆盘
    * - 点在高度范围内: 最近点在侧面上
    */
    Vector3f Cone::ClosestPoint(const Vector3f &point) const
    {
        Vector3f to_point = point - apex;
        
        // 计算点在圆锥轴方向的投影
        float axis_projection = dot(to_point, axis);
        
        if (axis_projection <= 0.0f)
        {
            // 点在顶点上方或顶点处
            return apex;
        }
        
        if (axis_projection >= height)
        {
            // 点在底面下方
            Vector3f base_center = GetBaseCenter();
            Vector3f to_base = point - base_center;
            
            // 移除轴向分量，得到径向向量
            Vector3f radial = to_base - axis * dot(to_base, axis);
            float radial_length = length(radial);
            
            if (radial_length <= base_radius)
            {
                // 投影在底面圆内
                return point - axis * dot(to_base, axis);
            }
            else
            {
                // 投影在底面圆外，最近点在底面圆周上
                return base_center + glm::normalize(radial) * base_radius;
            }
        }
        
        // 点在圆锥高度范围内
        Vector3f axis_point = apex + axis * axis_projection;
        float radius_at_height = GetRadiusAtHeight(axis_projection);
        
        Vector3f radial = point - axis_point;
        float radial_length = length(radial);
        
        if (radial_length <= radius_at_height)
        {
            // 点在圆锥内部，需要找到最近的表面点
            // 这里简化处理，返回侧面上的点
            if (radial_length < 0.0001f)
            {
                // 点在轴上，选择任意方向
                Vector3f perpendicular;
                if (abs(axis.x) < 0.9f)
                    perpendicular = cross(axis, Vector3f(1, 0, 0));
                else
                    perpendicular = cross(axis, Vector3f(0, 1, 0));
                
                return axis_point + glm::normalize(perpendicular) * radius_at_height;
            }
            else
            {
                return axis_point + glm::normalize(radial) * radius_at_height;
            }
        }
        else
        {
            // 点在圆锥外部，最近点在侧面
            return axis_point + glm::normalize(radial) * radius_at_height;
        }
    }

    float Cone::DistanceToPoint(const Vector3f &point) const
    {
        Vector3f closest = ClosestPoint(point);
        return length(point - closest);
    }
}//namespace hgl::math
