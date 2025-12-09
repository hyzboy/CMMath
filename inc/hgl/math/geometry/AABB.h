#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/MatrixTypes.h>
#include<hgl/math/geometry/Plane.h>
#include<hgl/math/geometry/Triangle.h>

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

        void SetCornerLength(const Vector3f &c,const Vector3f &l)               ///<按顶角和长度设置盒子范围
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

        void Clear()
        {
            mem_zero(minPoint);
            mem_zero(maxPoint);
            mem_zero(center);
            mem_zero(length);
            mem_zero(planes);
            mem_zero(face_center_point);
        }

        const   Vector3f &  GetMin      ()const{return minPoint;}
        const   Vector3f &  GetMax      ()const{return maxPoint;}

        const   Vector3f &  GetCenter   ()const{return center;}
        const   Vector3f &  GetLength   ()const{return length;}

                Vector3f    GetVertexP  (const Vector3f &)const;
                Vector3f    GetVertexN  (const Vector3f &)const;

        void Merge(const AABB &box)
        {
            SetMinMax(MinVector(minPoint,box.minPoint)
                     ,MaxVector(maxPoint,box.maxPoint));
        }

        const Plane &GetFacePlanes(int i)const{return planes[i];}

        bool IsEmpty()const{return IsNearlyZero(length);}

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
         */
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
         */
        bool Intersects(const AABB &other) const
        {
            return !(maxPoint.x < other.minPoint.x || minPoint.x > other.maxPoint.x ||
                     maxPoint.y < other.minPoint.y || minPoint.y > other.maxPoint.y ||
                     maxPoint.z < other.minPoint.z || minPoint.z > other.maxPoint.z);
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
         */
        bool IntersectsTriangle(const Triangle3f &triangle) const;

    public: // 工具函数

        /**
         * 扩展AABB以包含指定点
         */
        void ExpandToInclude(const Vector3f &point);

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
