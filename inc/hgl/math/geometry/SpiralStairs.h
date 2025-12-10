#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<vector>

namespace hgl::math
{
    /**
    * 旋转楼梯台阶(Spiral Stair Step)
    * 
    * 表示螺旋楼梯中的单个台阶。
    * 与直线楼梯台阶不同，螺旋楼梯的台阶呈扇形或梯形，
    * 围绕中心轴旋转排列，每个台阶都有自己的旋转角度。
    */
    struct SpiralStairStep
    {
        Vector3f position;    // 台阶中心位置
        Vector3f size;        // 台阶尺寸(宽度、高度、径向深度)
        float angle;                // 台阶角度(弧度)
        
        SpiralStairStep()
        {
            position = Vector3f(0, 0, 0);
            size = Vector3f(0.8f, 0.2f, 0.5f);
            angle = 0.0f;
        }
        
        SpiralStairStep(const Vector3f &pos, const Vector3f &s, float a)
        {
            position = pos;
            size = s;
            angle = a;
        }
    };

    /**
    * 旋转楼梯/螺旋楼梯(Spiral Stairs/Helical Stairs)
    * 
    * 围绕中心轴螺旋上升的楼梯结构，是一种节省空间的楼梯设计。
    * 台阶呈扇形排列，内侧连接中心柱，外侧形成螺旋曲线。
    * 
    * 几何定义:
    * - 中心轴: 楼梯围绕旋转的垂直轴线
    * - 内半径: 中心柱的半径(台阶内沿到中心轴的距离)
    * - 外半径: 台阶外沿到中心轴的距离
    * - 台阶高度: 每个台阶的垂直上升高度
    * - 旋转角度: 每个台阶相对于前一个台阶的旋转角度
    * 
    * 螺旋参数:
    * - 螺距(Pitch): 旋转一圈(360°)上升的高度 = step_height * (2π / step_angle)
    * - 旋转方向: 顺时针或逆时针
    * - 起始角度: 第一个台阶的角度位置
    * 
    * 建筑学特点:
    * - 节省空间，占地面积小
    * - 视觉效果优雅
    * - 通常需要中心柱支撑
    * - 内侧台阶较窄，外侧较宽
    * 
    * 应用场景: 室内旋转楼梯、观景塔、灯塔、紧凑空间等。
    */
    class SpiralStairs
    {
    protected:

        Vector3f center;          // 旋转中心轴的底部位置(楼梯起点)
        Vector3f axis;            // 旋转轴方向(归一化，通常指向上方)
        
        float inner_radius;             // 内半径(中心柱半径，台阶内沿到轴的距离)
        float outer_radius;             // 外半径(台阶外沿到轴的距离)
        float step_height;              // 每个台阶的垂直高度(Rise)
        float step_angle;               // 每个台阶的旋转角度(弧度，如30°=π/6)
        int step_count;                 // 台阶总数
        
        float start_angle;              // 起始角度(弧度，第一个台阶的角度位置)
        bool clockwise;                 // 旋转方向: true=顺时针，false=逆时针
        
        std::vector<SpiralStairStep> steps; // 所有台阶的集合

    public:

        SpiralStairs()
        {
            center = Vector3f(0, 0, 0);
            axis = Vector3f(0, 1, 0);
            inner_radius = 0.5f;
            outer_radius = 1.5f;
            step_height = 0.2f;
            step_angle = math::pi / 6.0f;  // 30度
            step_count = 20;
            start_angle = 0.0f;
            clockwise = false;
        }

        SpiralStairs(const Vector3f &c, const Vector3f &ax,
                    float inner_r, float outer_r, float height, float angle,
                    int count, float start_a = 0.0f, bool cw = false)
        {
            Set(c, ax, inner_r, outer_r, height, angle, count, start_a, cw);
        }

        void Set(const Vector3f &c, const Vector3f &ax,
                float inner_r, float outer_r, float height, float angle,
                int count, float start_a = 0.0f, bool cw = false)
        {
            center = c;
            axis = normalize(ax);
            inner_radius = inner_r;
            outer_radius = outer_r;
            step_height = height;
            step_angle = angle;
            step_count = count;
            start_angle = start_a;
            clockwise = cw;
            
            GenerateSteps();
        }

        const Vector3f &GetCenter() const { return center; }
        const Vector3f &GetAxis() const { return axis; }
        float GetInnerRadius() const { return inner_radius; }
        float GetOuterRadius() const { return outer_radius; }
        float GetStepHeight() const { return step_height; }
        float GetStepAngle() const { return step_angle; }
        int GetStepCount() const { return step_count; }
        float GetStartAngle() const { return start_angle; }
        bool IsClockwise() const { return clockwise; }
        const std::vector<SpiralStairStep> &GetSteps() const { return steps; }

        /**
        * 获取楼梯总高度
        * 即从底部到顶部的垂直距离
        * @return 总高度 = step_height * step_count
        */
        float GetTotalHeight() const
        {
            return step_height * step_count;
        }

        /**
        * 获取楼梯总旋转角度
        * 即从第一个台阶到最后一个台阶旋转的总角度(弧度)
        * 例如: 如果每个台阶30°，共12个台阶，则总旋转360°(一圈)
        * @return 总旋转角度(弧度) = step_angle * step_count
        */
        float GetTotalAngle() const
        {
            return step_angle * step_count;
        }

        /**
        * 获取楼梯顶部位置
        * 即最后一个台阶顶部中心轴上的位置
        * @return 顶部位置 = center + axis * 总高度
        */
        Vector3f GetTopPosition() const
        {
            return center + axis * GetTotalHeight();
        }

        /**
        * 生成所有台阶
        * 
        * 算法步骤:
        * 1. 计算垂直于轴的两个正交基向量(right 和 forward)
        * 2. 对每个台阶:
        *    a. 计算当前旋转角度 = start_angle + step_angle * i
        *    b. 计算当前高度 = step_height * i
        *    c. 根据角度计算径向方向 = cos(θ)*right + sin(θ)*forward
        *    d. 计算台阶中心位置 = center + 高度 + 径向偏移
        *    e. 创建台阶对象并添加到列表
        * 
        * 台阶排列遵循螺旋线方程: r(θ) = (x, y, z)
        * 其中 x = R*cos(θ), y = h*θ/(2π), z = R*sin(θ)
        */
        void GenerateSteps()
        {
            steps.clear();
            steps.reserve(step_count);
            
            // 计算垂直于轴的两个正交向量
            Vector3f right;
            if (abs(axis.x) < 0.9f)
                right = normalize(cross(axis, Vector3f(1, 0, 0)));
            else
                right = normalize(cross(axis, Vector3f(0, 1, 0)));
            
            Vector3f forward = normalize(cross(right, axis));
            
            float angle_direction = clockwise ? -1.0f : 1.0f;
            
            for (int i = 0; i < step_count; i++)
            {
                float current_angle = start_angle + angle_direction * step_angle * i;
                float current_height = step_height * i;
                
                // 计算台阶中心位置(在内外半径中间)
                float step_radius = (inner_radius + outer_radius) * 0.5f;
                
                Vector3f radial_direction = cos(current_angle) * right + sin(current_angle) * forward;
                Vector3f step_pos = center + 
                                   axis * current_height + 
                                   radial_direction * step_radius;
                
                // 台阶尺寸: 宽度(弧长方向)、高度、径向深度
                float step_width = step_radius * step_angle;  // 近似弧长
                float radial_depth = outer_radius - inner_radius;
                Vector3f step_size(step_width, step_height, radial_depth);
                
                steps.push_back(SpiralStairStep(step_pos, step_size, current_angle));
            }
        }

        /**
        * 获取指定台阶的OBB包围盒
        * 
        * 由于螺旋楼梯的台阶是旋转的，使用OBB(Oriented Bounding Box)
        * 比AABB更精确地表示台阶的实际形状和方向。
        * 
        * OBB的三个轴对应:
        * - 切向(Tangent): 沿圆周方向(台阶宽度方向)
        * - 轴向(Axial): 沿楼梯上升方向(台阶高度方向)
        * - 径向(Radial): 从中心轴指向外侧(台阶深度方向)
        * 
        * @param step_index 台阶索引(0到step_count-1)
        * @return 该台阶的OBB包围盒，如果索引无效则返回空OBB
        */
        OBB GetStepOBB(int step_index) const
        {
            if (step_index < 0 || step_index >= static_cast<int>(steps.size()))
                return OBB();
            
            const SpiralStairStep &step = steps[step_index];
            
            // 计算旋转矩阵
            Vector3f right;
            if (abs(axis.x) < 0.9f)
                right = normalize(cross(axis, Vector3f(1, 0, 0)));
            else
                right = normalize(cross(axis, Vector3f(0, 1, 0)));
            
            Vector3f forward = normalize(cross(right, axis));
            
            Vector3f radial = cos(step.angle) * right + sin(step.angle) * forward;
            Vector3f tangent = -sin(step.angle) * right + cos(step.angle) * forward;
            
            OBB obb;
            obb.Set(step.position, 
                   tangent,                     // 沿弧线方向
                   axis,                        // 向上方向
                   radial,                      // 径向方向
                   step.size * 0.5f);
            
            return obb;
        }

        /**
        * 获取楼梯的整体AABB包围盒(近似)
        * 
        * 注意: 这是一个保守的近似包围盒，会包含整个螺旋楼梯。
        * 由于螺旋楼梯是旋转的，AABB会包含许多空白区域。
        * 实际应用中可能需要更精确的碰撞检测方法。
        * 
        * 包围盒范围:
        * - XZ平面: 以center为中心，边长为2*outer_radius的正方形
        * - Y轴: 从center.y到center.y+总高度
        * 
        * @return 包含整个螺旋楼梯的轴对齐包围盒
        */
        AABB GetAABB() const
        {
            Vector3f min_point = center - Vector3f(outer_radius, 0, outer_radius);
            Vector3f max_point = center + Vector3f(outer_radius, GetTotalHeight(), outer_radius);
            
            return AABB(min_point, max_point);
        }

        /**
        * 判断点在哪个台阶上
        * 
        * 遍历所有台阶，检查点是否在某个台阶的OBB内。
        * 这个方法可用于:
        * - 角色站立位置检测
        * - 碰撞检测
        * - AI寻路
        * 
        * @param point 待检测的3D空间点
        * @return 台阶索引(0到step_count-1)，如果不在任何台阶上则返回-1
        */
        int GetStepIndexAtPoint(const Vector3f &point) const
        {
            for (int i = 0; i < static_cast<int>(steps.size()); i++)
            {
                OBB step_obb = GetStepOBB(i);
                if (step_obb.ContainsPoint(point))
                    return i;
            }
            return -1;
        }
    };//class SpiralStairs
}//namespace hgl::math
