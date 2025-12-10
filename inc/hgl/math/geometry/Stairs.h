#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>
#include<vector>

namespace hgl::math
{
    /**
    * 楼梯台阶(Stair Step)
    * 
    * 表示楼梯中的单个台阶，包含台阶的位置和尺寸信息。
    * 台阶是一个长方体，通常具有：
    * - 宽度(Width): 垂直于前进方向的尺寸
    * - 高度(Rise): 垂直方向的尺寸
    * - 深度(Run/Tread): 沿前进方向的尺寸
    */
    struct StairStep
    {
        Vector3f position;    // 台阶位置(台阶底部中心点)
        Vector3f size;        // 台阶尺寸(宽度、高度、深度)
        
        StairStep()
        {
            position = Vector3f(0, 0, 0);
            size = Vector3f(1, 0.2f, 0.3f);
        }
        
        StairStep(const Vector3f &pos, const Vector3f &s)
        {
            position = pos;
            size = s;
        }
        
        /**
        * 获取台阶的AABB包围盒
        */
        AABB GetAABB() const
        {
            Vector3f half_size = size * 0.5f;
            Vector3f min_point = position - Vector3f(half_size.x, 0, half_size.z);
            Vector3f max_point = position + Vector3f(half_size.x, size.y, half_size.z);
            
            return AABB(min_point, max_point);
        }
    };

    /**
    * 直线楼梯(Straight Stairs)
    * 
    * 表示沿直线方向上升的楼梯，是最常见的楼梯类型。
    * 每个台阶依次排列，形成一条连续的上升路径。
    * 
    * 楼梯参数:
    * - 起始位置: 第一个台阶的底部位置
    * - 前进方向: 楼梯延伸的水平方向(归一化)
    * - 向上方向: 楼梯上升的方向(归一化)
    * - 台阶尺寸: 宽度(Width)、高度(Rise)、深度(Run)
    * - 台阶数量: 总共有多少个台阶
    * 
    * 建筑学规范:
    * - 舒适的台阶比例: 2*Rise + Run ≈ 600-650mm
    * - 常见Rise: 150-200mm
    * - 常见Run: 250-300mm
    * 
    * 应用场景: 室内楼梯、室外台阶、平台连接等。
    */
    class Stairs
    {
    protected:

        Vector3f start_position;  // 起始位置(第一个台阶的底部中心)
        Vector3f direction;       // 楼梯前进方向(归一化的水平方向向量)
        Vector3f up;              // 楼梯向上方向(归一化的垂直方向向量)
        
        float step_width;               // 台阶宽度(垂直于前进方向的尺寸)
        float step_height;              // 台阶高度/踏步高(Rise，垂直上升高度)
        float step_depth;               // 台阶深度/踏步宽(Run，沿前进方向的深度)
        int step_count;                 // 台阶数量(总共有多少级台阶)
        
        std::vector<StairStep> steps;   // 所有台阶的集合

    public:

        Stairs()
        {
            start_position = Vector3f(0, 0, 0);
            direction = Vector3f(0, 0, 1);
            up = Vector3f(0, 1, 0);
            step_width = 1.0f;
            step_height = 0.2f;
            step_depth = 0.3f;
            step_count = 10;
        }

        Stairs(const Vector3f &start, const Vector3f &dir, const Vector3f &up_dir,
               float width, float height, float depth, int count)
        {
            Set(start, dir, up_dir, width, height, depth, count);
        }

        void Set(const Vector3f &start, const Vector3f &dir, const Vector3f &up_dir,
                 float width, float height, float depth, int count)
        {
            start_position = start;
            direction = normalize(dir);
            up = normalize(up_dir);
            step_width = width;
            step_height = height;
            step_depth = depth;
            step_count = count;
            
            GenerateSteps();
        }

        const Vector3f &GetStartPosition() const { return start_position; }
        const Vector3f &GetDirection() const { return direction; }
        const Vector3f &GetUp() const { return up; }
        float GetStepWidth() const { return step_width; }
        float GetStepHeight() const { return step_height; }
        float GetStepDepth() const { return step_depth; }
        int GetStepCount() const { return step_count; }
        const std::vector<StairStep> &GetSteps() const { return steps; }

        /**
        * 获取楼梯总高度
        * 即从第一级台阶底部到最后一级台阶顶部的垂直高度
        * @return 总高度 = step_height * step_count
        */
        float GetTotalHeight() const
        {
            return step_height * step_count;
        }

        /**
        * 获取楼梯总长度(沿前进方向)
        * 即楼梯在水平方向上的投影长度
        * @return 总长度 = step_depth * step_count
        */
        float GetTotalLength() const
        {
            return step_depth * step_count;
        }

        /**
        * 获取楼梯终点位置
        * 即最后一个台阶顶部的中心点位置
        * @return 终点位置 = 起点 + 水平位移 + 垂直位移
        */
        Vector3f GetEndPosition() const
        {
            return start_position + direction * GetTotalLength() + up * GetTotalHeight();
        }

        /**
        * 生成所有台阶
        * 根据楼梯参数自动生成所有台阶的位置和尺寸
        * 每个台阶依次排列，前一个台阶之上并沿方向前进一个深度单位
        */
        void GenerateSteps()
        {
            steps.clear();
            steps.reserve(step_count);
            
            for (int i = 0; i < step_count; i++)
            {
                Vector3f step_pos = start_position + 
                                   direction * (step_depth * i) + 
                                   up * (step_height * i);
                
                Vector3f step_size(step_width, step_height, step_depth);
                
                steps.push_back(StairStep(step_pos, step_size));
            }
        }

        /**
        * 获取楼梯的整体AABB包围盒
        */
        AABB GetAABB() const
        {
            if (steps.empty())
                return AABB();
            
            AABB result = steps[0].GetAABB();
            for (size_t i = 1; i < steps.size(); i++)
            {
                result.Merge(steps[i].GetAABB());
            }
            
            return result;
        }

        /**
        * 判断点在哪个台阶上(-1表示不在任何台阶上)
        */
        int GetStepIndexAtPoint(const Vector3f &point) const
        {
            for (size_t i = 0; i < steps.size(); i++)
            {
                if (steps[i].GetAABB().IsPointInBox(point))
                    return static_cast<int>(i);
            }
            return -1;
        }
    };//class Stairs
}//namespace hgl::math
