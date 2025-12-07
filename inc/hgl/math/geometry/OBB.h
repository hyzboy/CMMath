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
        math::Vector3f center;
        Matrix3f axis;          ///<轴矩阵
        math::Vector3f half_length;

        Plane planes[6];

    private:

        friend struct BoundingVolumes;

        void ComputePlanes();

        template<typename T>
        void SetFromPointsMinVolume(const T *points,size_t count,float coarseStepDeg,float fineStepDeg,float ultraStepDeg);
        
        void SetFromPointsMinVolumeFloat(const float *points,size_t count,uint32_t component_count,float coarseStepDeg,float fineStepDeg,float ultraStepDeg);

    public:

        const math::Vector3f &GetCenter()const{return center;}
        const math::Vector3f &GetAxis(int n)const{return axis[n];}
        const Matrix3f &GetRotationMatrix()const{return axis;}
        const math::Vector3f &GetHalfExtend()const{return half_length;}

        const math::Matrix4f GetMatrix(const float cube_size=1.0f)const;

    public:

        void Set(const math::Vector3f &c,const math::Vector3f &hl);
        void Set(const math::Vector3f &c,const math::Vector3f &a0,const math::Vector3f &a1,const math::Vector3f &a2,const math::Vector3f &hl);

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

        OBB(const math::Vector3f &c,const math::Vector3f &hl)
        {
            Set(c,hl);
        }

        OBB(const math::Vector3f &c,const math::Vector3f &a0,const math::Vector3f &a1,const math::Vector3f &a2,const math::Vector3f &hl)
        {
            Set(c,a0,a1,a2,hl);
        }

    public:

        bool IsEmpty()const{return IsNearlyZero(half_length);}

        const math::Vector3f GetMin()const{return center-half_length;}
        const math::Vector3f GetMax()const{return center+half_length;}

        const Plane &GetFacePlanes(int i)const{return planes[i];}

        void GetCorners(Vector3f out[8])const;

        OBB Transformed(const math::Matrix4f &)const;
    };//class OBB
}//namespace hgl::math
