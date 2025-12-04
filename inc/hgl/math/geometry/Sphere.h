#pragma once
#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
    * 球类(xyz半径一置)
    */
    class Sphere
    {
        Vector3f center;
        float radius;

    public:

        const Vector3f &GetCenter()const{return center;}
        float GetRadius()const{return radius;}

    public:

        Sphere()
        {
            Set(Vector3f(0,0,0),1);
        }

        Sphere(const Vector3f &c,float r)
        {
            Set(c,r);
        }

        void Set(const Vector3f &c,float r)
        {
            center=c;
            radius=r;
        }

        float GetVolume()const
        {
            return (4.0f/3.0f)*HGL_PI*radius*radius*radius;
        }

        /**
        * 取得指定方向上的点
        */
        Vector3f GetPoint(const Vector3f &direction)const
        {
            return center+direction*radius;
        }
    };//class Sphere

    /**
    * 椭球类(xyz半径不一置)
    */
    class EllipseSphere
    {
        Vector3f center;
        Vector3f radius;

    public:

        const Vector3f &GetCenter()const{return center;}
        const Vector3f &GetRadius()const{return radius;}

    public:

        EllipseSphere()
        {
            Set(Vector3f(0,0,0),Vector3f(1,1,1));
        }

        EllipseSphere(const Vector3f&c,const Vector3f&r)
        {
            Set(c,r);
        }

        void Set(const Vector3f&c,const Vector3f&r)
        {
            center=c;
            radius=r;
        }

        float GetVolume()const
        {
            return (4.0f/3.0f)*HGL_PI*radius.x*radius.y*radius.z;
        }

        /**
        * 取得指定方向上的点
        */
        Vector3f GetPoint(const Vector3f &direction)const
        {
            return center+direction*radius;
        }
    };//class EllipseSphere
}//namespace hgl::math
