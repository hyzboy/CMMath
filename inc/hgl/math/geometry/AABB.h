#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/Matrix.h>
#include<hgl/math/geometry/Plane.h>
#include<hgl/math/geometry/Triangle.h>
#include<cfloat>

namespace hgl::math
{
    const Vector3f AABBFaceNormal[6]=
    {
        Vector3f(-1,  0,  0),
        Vector3f( 1,  0,  0),
        Vector3f( 0, -1,  0),
        Vector3f( 0,  1,  0),
        Vector3f( 0,  0, -1),
        Vector3f( 0,  0,  1)
    };

    class Ray;
    class OBB;

    /**
    * Axis Aligned Bounding Box
    */
    class AABB
    {
        Vector3f minPoint;
        Vector3f center;
        Vector3f length;
        Vector3f maxPoint;

        Vector3f face_center_point[6];

        Plane planes[6];

    private:

        friend struct BoundingVolumes;

        void Update();

    public:

        AABB()
        {
            SetCornerLength(Vector3f(0,0,0),Vector3f(1,1,1));
        }

        AABB(const Vector3f &c,const Vector3f &l)
        {
            SetCornerLength(c,l);
        }

        /**
         * 按最小点和尺寸设置盒子范围
         * @param min_point 最小点（盒子的左下后角）
         * @param size 盒子尺寸（长宽高）
         */
        void SetCornerLength(const Vector3f &c,const Vector3f &l)
        {
            minPoint=c;
            length=l;
            maxPoint=c+l;
            center=(minPoint+maxPoint)/2.0f;
            Update();
        }

        void SetMinMax(const Vector3f &min_v,const Vector3f &max_v)             ///<按最小最大值设置盒子范围
        {
            minPoint=min_v;
            maxPoint=max_v;
            length=max_v-min_v;
            center=(min_v+max_v)/2.0f;
            Update();
        }

        void SetFromPoints(const float *pts,const uint32_t count,const uint32_t component_count);

        /**
         * 清空 AABB，设置为无效状态
         * @note 清空后 IsValid() 返回 false，IsEmpty() 返回 true
         */
        void Clear()
        {
            minPoint = Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
            maxPoint = Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
            center = Vector3f(0, 0, 0);
            length = Vector3f(0, 0, 0);
            mem_zero(planes);
            mem_zero(face_center_point);
        }

        const   Vector3f &  GetMin      ()const{return minPoint;}
        const   Vector3f &  GetMax      ()const{return maxPoint;}

        const   Vector3f &  GetCenter   ()const{return center;}
        const   Vector3f &  GetLength   ()const{return length;}

        /**
         * 获取尺寸（等同于 GetLength()，但更直观）
         */
        const   Vector3f &  GetSize     ()const{return length;}
        
        /**
         * 获取半尺寸（extent）
         */
        Vector3f GetExtent() const { return length * 0.5f; }

        /**
         * 获取对角线长度
         */
        float GetDiagonal() const { return glm::length(length); }

                Vector3f    GetVertexP  (const Vector3f &)const;
                Vector3f    GetVertexN  (const Vector3f &)const;

        void Merge(const AABB &box)
        {
            SetMinMax(MinVector(minPoint,box.minPoint)
                     ,MaxVector(maxPoint,box.maxPoint));
        }

        const Plane &GetFacePlanes(int i)const{return planes[i];}

        /**
         * 检查 AABB 是否为空（尺寸接近零）
         */
        bool IsEmpty()const{return IsNearlyZero(length);}

        /**
         * 检查 AABB 是否有效（min <= max）
         * @note 刚 Clear() 后的 AABB 是无效的
         */
        bool IsValid()const
        {
            return minPoint.x <= maxPoint.x && 
                   minPoint.y <= maxPoint.y && 
                   minPoint.z <= maxPoint.z;
        }

    public: // 静态工厂方法

        /**
         * 从两个点创建 AABB（自动计算 min/max）
         */
        static AABB FromTwoPoints(const Vector3f &p1, const Vector3f &p2)
        {
            AABB result;
            result.SetMinMax(MinVector(p1, p2), MaxVector(p1, p2));
            return result;
        }

        /**
         * 从中心和尺寸创建 AABB
         */
        static AABB FromCenterAndSize(const Vector3f &center, const Vector3f &size)
        {
            Vector3f half_size = size * 0.5f;
            AABB result;
            result.SetMinMax(center - half_size, center + half_size);
            return result;
        }

        /**
         * 创建空的无效 AABB
         */
        static AABB Empty()
        {
            AABB result;
            result.Clear();
            return result;
        }

    public: // 碰撞检测 - 点相关

        /**
         * 检查点是否在AABB内
         */
        bool ContainsPoint(const Vector3f &point) const
        {
            return point.x >= minPoint.x && point.x <= maxPoint.x &&
                   point.y >= minPoint.y && point.y <= maxPoint.y &&
                   point.z >= minPoint.z && point.z <= maxPoint.z;
        }

        /**
         * 计算点到AABB的最近点
         * 将点"钳制"到AABB边界内。如果点在AABB内部，返回该点本身；
         * 如果在外部，返回AABB表面上距离该点最近的点
         */
        Vector3f ClampPoint(const Vector3f &point) const
        {
            return glm::clamp(point, minPoint, maxPoint);
        }
        
        Vector3f ClosestPoint(const Vector3f &point) const;

        /**
         * 计算点到AABB的距离
         */
        float DistanceToPoint(const Vector3f &point) const;

        /**
         * 计算点到AABB的距离平方
         */
        float DistanceToPointSquared(const Vector3f &point) const;

    public: // 碰撞检测 - AABB

        /**
         * 检查与另一个AABB是否相交
         * 注意：边界接触不算相交（遵循Unreal Engine等游戏引擎的惯例）
         */
        bool Intersects(const AABB &other) const
        {
            return !(maxPoint.x <= other.minPoint.x || minPoint.x >= other.maxPoint.x ||
                     maxPoint.y <= other.minPoint.y || minPoint.y >= other.maxPoint.y ||
                     maxPoint.z <= other.minPoint.z || minPoint.z >= other.maxPoint.z);
        }

        /**
         * 检查是否完全包含另一个AABB
         */
        bool Contains(const AABB &other) const
        {
            return other.minPoint.x >= minPoint.x && other.maxPoint.x <= maxPoint.x &&
                   other.minPoint.y >= minPoint.y && other.maxPoint.y <= maxPoint.y &&
                   other.minPoint.z >= minPoint.z && other.maxPoint.z <= maxPoint.z;
        }

        /**
         * 计算与另一个AABB的交集
         */
        bool GetIntersection(const AABB &other, AABB &intersection) const;

        /**
         * 计算到另一个AABB的距离
         */
        float Distance(const AABB &other) const;

    public: // 碰撞检测 - 其他几何体

        /**
         * 检查与球体是否相交
         */
        bool IntersectsSphere(const Vector3f &sphere_center, float sphere_radius) const;

        /**
         * 检查与OBB是否相交
         */
        bool IntersectsOBB(const OBB &obb) const;

        /**
         * 检查与射线是否相交
         */
        bool IntersectsRay(const Ray &ray, float &distance) const;
        bool IntersectsRay(const Ray &ray) const;
        bool IntersectsRay(const Ray &ray, float &t_min, float &t_max) const;

        /**
         * 检查与平面的关系
         * @return <0: 完全在平面后面, 0: 相交, >0: 完全在平面前面
         */
        int ClassifyPlane(const Plane &plane) const;

        /**
         * 检查是否与平面相交
         */
        bool IntersectsPlane(const Plane &plane) const;

        /**
         * 检查与三角形是否相交
         * @warning 当前实现不完整，可能产生假阴性（漏掉某些相交情况）
         * @note 只检查三角形顶点是否在 AABB 内，不检查边和面相交
         * TODO: 实现完整的 SAT (Separating Axis Theorem) 测试
         */
        bool IntersectsTriangle(const Triangle3f &triangle) const;

    public: // 工具函数

        /**
         * 扩展AABB以包含指定点
         */
        void ExpandToInclude(const Vector3f &point);

        /**
         * 返回缩放后的 AABB（从中心缩放）
         */
        AABB Scaled(float factor) const
        {
            Vector3f half_size = length * 0.5f * factor;
            AABB result;
            result.SetMinMax(center - half_size, center + half_size);
            return result;
        }

        /**
         * 返回缩放后的 AABB（非均匀缩放）
         */
        AABB Scaled(const Vector3f &scale) const
        {
            Vector3f half_size = length * 0.5f;
            Vector3f new_half_size(
                half_size.x * scale.x,
                half_size.y * scale.y,
                half_size.z * scale.z
            );
            AABB result;
            result.SetMinMax(center - new_half_size, center + new_half_size);
            return result;
        }

        /**
         * 返回偏移后的 AABB
         */
        AABB Offset(const Vector3f &offset) const
        {
            AABB result;
            result.SetMinMax(minPoint + offset, maxPoint + offset);
            return result;
        }

        /**
         * 返回扩展/收缩后的 AABB（每个方向扩展相同距离）
         */
        AABB Expanded(float amount) const
        {
            Vector3f expansion(amount, amount, amount);
            AABB result;
            result.SetMinMax(minPoint - expansion, maxPoint + expansion);
            return result;
        }

        /**
         * 原地缩放
         */
        void Scale(float factor)
        {
            *this = Scaled(factor);
        }

        /**
         * 原地偏移（平移）
         */
        void Translate(const Vector3f &offset)
        {
            SetMinMax(minPoint + offset, maxPoint + offset);
        }

        /**
         * 获取AABB的8个顶点坐标
         * @param corners 输出数组，必须至少有8个元素
         * 顶点顺序：
         * 0-3: 底面（z=minPoint.z），逆时针
         * 4-7: 顶面（z=maxPoint.z），逆时针
         */
        void GetCorners(Vector3f corners[8]) const
        {
            corners[0] = Vector3f(minPoint.x, minPoint.y, minPoint.z);
            corners[1] = Vector3f(maxPoint.x, minPoint.y, minPoint.z);
            corners[2] = Vector3f(maxPoint.x, maxPoint.y, minPoint.z);
            corners[3] = Vector3f(minPoint.x, maxPoint.y, minPoint.z);
            corners[4] = Vector3f(minPoint.x, minPoint.y, maxPoint.z);
            corners[5] = Vector3f(maxPoint.x, minPoint.y, maxPoint.z);
            corners[6] = Vector3f(maxPoint.x, maxPoint.y, maxPoint.z);
            corners[7] = Vector3f(minPoint.x, maxPoint.y, maxPoint.z);
        }

        /**
         * 获取表面积
         */
        float GetSurfaceArea() const
        {
            return 2.0f * (length.x * length.y + length.y * length.z + length.z * length.x);
        }

        /**
         * 获取体积
         */
        float GetVolume() const
        {
            return length.x * length.y * length.z;
        }

    public:

        void operator += (const AABB &aabb){Merge(aabb);}                       ///<融合另一个AABox

        AABB Transformed(const Matrix4f &m)const;                               ///<返回变换后的AABox
    };//class AABB
}//namespace hgl::math
