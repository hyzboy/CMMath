#pragma once

#include<hgl/math/Vector.h>

namespace hgl::graph
{
    /**
    * 面数据结构
    */
    struct Plane
    {
        Vector3f normal;
        float d;

    public:

        void Set(const Vector3f &_p,float _d)
        {
            normal=_p;
            d=_d;
        }

        void Set(const Vector3f &face_center_point,const Vector3f &face_normal)
        {
            normal=face_normal;
            d=dot(face_center_point,face_normal);
        }

        void Set(const Vector4f &p)
        {
            normal=p;
            d=p.w;
        }

        float Distance(const Vector3f &p)const
        {
            return(dot(normal,p)-d)/glm::length(normal);
        }
    };//struct Plane

    /**
    * 3D空间中的矩形<br>
    * 一般用于描述一些特殊的矩形平面区域
    */
    struct Rectangle3D
    {
        Vector3f center;
        Vector3f normal;
        Vector3f up;
        Vector3f right;
        Vector2f size;
    };

    float DistanceBetweenPointAndRectangle3D(const Vector3f &p,const Rectangle3D &rect);
}//namespace hgl::graph
