#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/MatrixTypes.h>
#include<hgl/math/geometry/Plane.h>

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
            hgl_zero(minPoint);
            hgl_zero(maxPoint);
            hgl_zero(center);
            hgl_zero(length);
            hgl_zero(planes);
            hgl_zero(face_center_point);
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

    public:

        void operator += (const AABB &aabb){Merge(aabb);}                       ///<融合另一个AABox

        AABB Transformed(const Matrix4f &m)const;                               ///<返回变换后的AABox
    };//class AABB
}//namespace hgl::math
