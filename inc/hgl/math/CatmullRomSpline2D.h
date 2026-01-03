/**
 * CatmullRomSpline2D.h - 2D Catmull-Rom 样条曲线
 * 
 * 通过控制点序列，生成平滑的二维曲线
 */

#pragma once

#include<hgl/math/Vector.h>
#include<vector>

namespace hgl::math
{
    /**
     * Catmull-Rom 样条曲线类 (2D)
     * 通过控制点序列，生成平滑曲线
     */
    class CatmullRomSpline2D
    {
    public:
        CatmullRomSpline2D();
        
        /**
         * 添加控制点
         */
        void AddPoint(const Vector2f &point);
        
        /**
         * 设置控制点序列
         */
        void SetPoints(const std::vector<Vector2f> &points);
        
        /**
         * 在曲线上采样
         * @param t 全局参数 [0, point_count - 1]
         * @return 曲线上的点
         */
        Vector2f Evaluate(float t) const;
        
        /**
         * 获取切线
         */
        Vector2f GetTangent(float t) const;
        
        /**
         * 获取曲线长度（近似）
         */
        float GetLength(int samples_per_segment = 10) const;
        
        /**
         * 获取控制点数量
         */
        int GetPointCount() const;
        
        /**
         * 清空控制点
         */
        void Clear();
        
    private:
        std::vector<Vector2f> control_points;
        
        // 计算单个线段的值
        Vector2f EvaluateSegment(int segment, float t) const;
        Vector2f GetSegmentTangent(int segment, float t) const;
    };

}//namespace hgl::math
