#pragma once

#include<hgl/math/Math.h>
#include<hgl/graph/Plane.h>

namespace hgl::graph
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
            hgl_zero(center);
            hgl_zero(axis);
            hgl_zero(half_length);
            hgl_zero(planes);
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

        OBB Transformed(const Matrix4f &)const;
    };//class OBB
}//namespace hgl::graph
