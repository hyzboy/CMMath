#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/MatrixTypes.h>
#include<hgl/math/geometry/Plane.h>

namespace hgl::math
{
    /**
     * Oriented Bounding Box
     */
    class OBB
    {
        Vector3f center;
        Matrix3f axis;          ///<轴矩阵
        Vector3f half_length;

        Plane planes[6];

    private:

        friend struct BoundingVolumes;

        void ComputePlanes();

        template<typename T>
        void SetFromPointsMinVolume(const T *points,size_t count,float coarseStepDeg,float fineStepDeg,float ultraStepDeg);

        void SetFromPointsMinVolumeFloat(const float *points,size_t count,uint32_t component_count,float coarseStepDeg,float fineStepDeg,float ultraStepDeg);

    public:

        const Vector3f &GetCenter()const{return center;}
        const Vector3f &GetAxis(int n)const{return axis[n];}
        const Matrix3f &GetRotationMatrix()const{return axis;}
        const Vector3f &GetHalfExtend()const{return half_length;}

        const Matrix4f GetMatrix(const float cube_size=1.0f)const;

    public:

        void Set(const Vector3f &c,const Vector3f &hl);
        void Set(const Vector3f &c,const Vector3f &a0,const Vector3f &a1,const Vector3f &a2,const Vector3f &hl);

        void SetFromPoints(const float *points,size_t count,uint32_t component_count,float coarseStepDeg=15.0f,float fineStepDeg=3.0f,float ultraStepDeg=0.5f);
        void Clear()
        {
            mem_zero(center);
            mem_zero(axis);
            mem_zero(half_length);
            mem_zero(planes);
        }

    public:

        OBB()
        {
            Set(Vector3f(0,0,0),Vector3f(1,1,1));
        }

        OBB(const Vector3f &c,const Vector3f &hl)
        {
            Set(c,hl);
        }

        OBB(const Vector3f &c,const Vector3f &a0,const Vector3f &a1,const Vector3f &a2,const Vector3f &hl)
        {
            Set(c,a0,a1,a2,hl);
        }

    public:

        bool IsEmpty()const{return IsNearlyZero(half_length);}

        const Vector3f GetMin()const{return center-half_length;}
        const Vector3f GetMax()const{return center+half_length;}

        const Plane &GetFacePlanes(int i)const{return planes[i];}

        void GetCorners(Vector3f out[8])const;

    public: // 碰撞检测 - 点相关

        /**
         * 检查点是否在OBB内
         */
        bool ContainsPoint(const math::Vector3f &point) const;

        /**
         * 计算点到OBB的最近点
         */
        math::Vector3f ClosestPoint(const math::Vector3f &point) const;

        /**
         * 计算点到OBB的距离
         */
        float DistanceToPoint(const math::Vector3f &point) const;

        /**
         * 计算点到OBB的距离平方
         */
        float DistanceToPointSquared(const math::Vector3f &point) const;

    public: // 碰撞检测 - OBB

        /**
         * 检查与另一个OBB是否相交(使用分离轴定理)
         */
        bool Intersects(const OBB &other) const;

        /**
         * 检查是否完全包含另一个OBB
         */
        bool Contains(const OBB &other) const;

        /**
         * 计算到另一个OBB的距离
         */
        float Distance(const OBB &other) const;

    public: // 碰撞检测 - 其他几何体

        /**
         * 检查与AABB是否相交
         */
        bool IntersectsAABB(const AABB &aabb) const;

        /**
         * 检查与球体是否相交
         */
        bool IntersectsSphere(const math::Vector3f &sphere_center, float sphere_radius) const;

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
         * 扩展OBB以包含指定点
         */
        void ExpandToInclude(const math::Vector3f &point);

        /**
         * 获取表面积
         */
        float GetSurfaceArea() const
        {
            Vector3f size = half_length * 2.0f;
            return 2.0f * (size.x * size.y + size.y * size.z + size.z * size.x);
        }

        /**
         * 获取体积
         */
        float GetVolume() const
        {
            return half_length.x * half_length.y * half_length.z * 8.0f;
        }

        /**
         * 合并另一个OBB(转换为AABB后合并，再转回OBB)
         */
        void Merge(const OBB &other);

    public:

        OBB Transformed(const Matrix4f &)const;
    };//class OBB
}//namespace hgl::math
