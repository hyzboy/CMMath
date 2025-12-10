#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
    * 线段(Line Segment)
    * 
    * 线段是连接两个点的直线的有限部分，是最基础的几何图元之一。
    * 在3D图形学中，线段用于表示边、路径、射线的有限部分等。
    * 
    * 几何定义:
    * - 起点(Start): 线段的起始端点
    * - 终点(End): 线段的终止端点
    * - 方向: 从起点指向终点的向量
    * - 长度: 起点和终点之间的欧几里得距离
    * 
    * 参数表示:
    * 线段上的任意点可以表示为: P(t) = start + t * (end - start), 其中 t ∈ [0, 1]
    * - t = 0 时，P(t) = start
    * - t = 1 时，P(t) = end
    * - 0 < t < 1 时，P(t) 在线段内部
    * 
    * 与射线(Ray)的区别:
    * - 线段: 有限长度，两端都有端点
    * - 射线: 从起点出发，沿方向无限延伸
    * - 直线: 双向无限延伸
    * 
    * 应用场景:
    * - 边的表示（三角形、多边形的边）
    * - 路径规划中的路径片段
    * - 碰撞检测中的边缘检测
    * - 距离计算（点到线段、线段到线段）
    * - 光线追踪中的有限光线
    */
    class LineSegment
    {
    protected:

        Vector3f start;     // 线段的起始端点
        Vector3f end;       // 线段的终止端点

    public:

        /**
        * 默认构造函数
        * 不初始化起点和终点，需要后续调用Set设置
        */
        LineSegment()=default;
        
        /**
        * 参数化构造函数
        * @param s 线段起点
        * @param e 线段终点
        */
        LineSegment(const Vector3f &s,const Vector3f &e):start(s),end(e) {}

        /**
        * 设置线段的起点和终点
        * @param s 新的起点位置
        * @param e 新的终点位置
        */
        void Set(const Vector3f &s,const Vector3f &e)
        {
            start=s;
            end=e;
        }

        /**
        * 设置线段的起点
        * @param s 新的起点位置
        */
        void SetStart(const Vector3f &s)
        {
            start=s;
        }

        /**
        * 设置线段的终点
        * @param e 新的终点位置
        */
        void SetEnd(const Vector3f &e)
        {
            end=e;
        }

        /** 获取线段起点 */
        const Vector3f &GetStart()const{return start;}
        
        /** 获取线段终点 */
        const Vector3f &GetEnd()const{return end;}
        
        /**
        * 获取线段中点
        * @return 起点和终点的中点位置
        */
        const Vector3f GetCenter()const{return (start + end) * 0.5f;}

        /**
        * 获取从起点到终点的方向（归一化）
        * 这是线段的正向方向
        * @return 归一化的方向向量
        */
        const Vector3f GetStartDirection()const{return normalized(end-start);}
        
        /**
        * 获取从终点到起点的方向（归一化）
        * 这是线段的反向方向
        * @return 归一化的反方向向量
        */
        const Vector3f GetEndDirection()const{return normalized(start-end);}
        
        /**
        * 获取从起点到终点的向量（未归一化）
        * @return 位移向量 = end - start
        */
        const Vector3f GetVector()const{return end - start;}

        /**
        * 计算线段长度的平方
        * 在只需要比较距离大小时，使用平方可以避免开方运算，提高性能
        * @return 长度的平方值
        */
        const float DistanceSquared()const{return length_squared(start,end);}
        
        /**
        * 计算线段长度
        * @return 起点到终点的欧几里得距离
        */
        const float Distance()const{return length(start,end);}
        
        /**
        * 获取线段上的参数化点
        * @param t 参数值，t∈[0,1]，0表示起点，1表示终点
        * @return 线段上的点 P(t) = start + t * (end - start)
        */
        const Vector3f GetPoint(float t)const
        {
            return start + (end - start) * t;
        }
        
        /**
        * 计算点到线段的最近点
        * @param point 空间中的任意点
        * @return 线段上距离该点最近的点
        */
        const Vector3f ClosestPoint(const Vector3f &point)const
        {
            Vector3f dir = end - start;
            float len_sq = dot(dir, dir);
            
            if(len_sq < 0.0001f)  // 线段退化为点
                return start;
            
            float t = dot(point - start, dir) / len_sq;
            t = clamp(t, 0.0f, 1.0f);  // 限制在[0,1]范围内
            
            return start + dir * t;
        }
    };//class LineSegment
}//namespace hgl::math
