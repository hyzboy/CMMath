/**
* 圆柱体实现文件
* 
* 实现了圆柱体的各种几何计算功能，包括:
* - 点包含性检测
* - 最近点计算
* - 距离计算
* 
* 这些函数常用于碰撞检测、射线相交测试、距离查询等场景。
*/
#include<hgl/math/geometry/Cylinder.h>
#include<hgl/math/MathUtils.h>

namespace hgl::math
{
    /**
    * 判断点是否在圆柱体内部
    * 
    * 算法步骤:
    * 1. 计算从中心点到目标点的向量
    * 2. 将该向量投影到圆柱轴上，得到轴向分量
    * 3. 检查轴向分量是否在[-h/2, h/2]范围内
    * 4. 计算点到轴上投影点的距离(径向距离)
    * 5. 检查径向距离是否小于等于半径
    */
    bool Cylinder::ContainsPoint(const Vector3f &point) const
    {
        // 计算从圆柱中心到目标点的向量
        Vector3f to_point = point - center;
        
        // 计算点在圆柱轴方向的投影长度
        // 这个值表示点沿轴向的位置，0表示在中心平面
        float axis_projection = dot(to_point, axis);
        
        // 检查是否在高度范围内
        // 圆柱的高度从中心向两侧延伸各 height/2
        float half_height = height * 0.5f;
        if (abs(axis_projection) > half_height)
            return false;  // 点在圆柱上下端面之外
        
        // 计算点到圆柱轴的距离(径向距离)
        // 先找到轴上的投影点，然后计算目标点到投影点的距离
        Vector3f axis_point = center + axis * axis_projection;
        float radial_distance = length(point - axis_point);
        
        // 检查径向距离是否在半径之内
        return radial_distance <= radius;
    }

    /**
    * 计算点到圆柱体表面的最近点
    * 
    * 算法分析:
    * 圆柱体的表面包括三部分: 侧面、顶面、底面
    * 
    * 1. 对于圆柱外部的点:
    *    - 如果在高度范围外，最近点可能在端面上
    *    - 如果在高度范围内，最近点在侧面上
    * 
    * 2. 对于圆柱内部的点:
    *    需要比较到三个表面的距离，选择最小的
    *    - 到侧面的距离 = radius - radial_distance
    *    - 到顶面的距离 = half_height - axis_projection
    *    - 到底面的距离 = half_height + axis_projection
    */
    Vector3f Cylinder::ClosestPoint(const Vector3f &point) const
    {
        // 计算从中心到目标点的向量
        Vector3f to_point = point - center;
        
        // 计算点在圆柱轴方向的投影
        float axis_projection = dot(to_point, axis);
        
        // 限制在圆柱高度范围内
        // clamp 确保投影值不超出 [-half_height, half_height]
        float half_height = height * 0.5f;
        axis_projection = clamp(axis_projection, -half_height, half_height);
        
        // 计算轴上的投影点
        Vector3f axis_point = center + axis * axis_projection;
        
        // 计算径向方向(从轴上的点指向目标点)
        Vector3f radial = point - axis_point;
        float radial_length = length(radial);
        
        if (radial_length <= radius)
        {
            // 点在圆柱内部，需要找最近的表面点
            
            // 计算到三个表面的距离
            float top_dist = half_height - axis_projection;      // 到顶面的距离
            float bottom_dist = half_height + axis_projection;   // 到底面的距离
            float radial_dist = radius - radial_length;          // 到侧面的距离
            
            if (radial_dist < top_dist && radial_dist < bottom_dist)
            {
                // 最近的是侧面，沿径向投影到侧面
                return axis_point + glm::normalize(radial) * radius;
            }
            else if (top_dist < bottom_dist)
            {
                // 最近的是顶面，垂直投影到顶面
                return center + axis * half_height + radial;
            }
            else
            {
                // 最近的是底面，垂直投影到底面
                return center - axis * half_height + radial;
            }
        }
        else
        {
            // 点在圆柱外部，最近点一定在侧面上
            // 沿径向投影到半径边界
            return axis_point + glm::normalize(radial) * radius;
        }
    }

    /**
    * 计算点到圆柱体的距离
    * 
    * 通过先找到最近点，然后计算欧几里得距离
    * - 如果点在圆柱内，返回到表面的距离
    * - 如果点在圆柱外，返回到最近表面点的距离
    */
    float Cylinder::DistanceToPoint(const Vector3f &point) const
    {
        Vector3f closest = ClosestPoint(point);
        return length(point - closest);
    }
}//namespace hgl::math
