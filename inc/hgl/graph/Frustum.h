#pragma once

#include<hgl/graph/AABB.h>
#include<hgl/graph/Plane.h>

namespace hgl::graph
{
    using FrustumPlanes=Vector4f[6];

    void GetFrustumPlanes(FrustumPlanes &fp,const Matrix4f &mvp);

    /**
    * 平截头裁剪处理
    */
    class Frustum
    {
        Plane pl[6];

    public:
            
        enum class Side 
        { 
            Left=0,
            Right,
            Front,
            Back,
            Top,
            Bottom
        };

        enum class Scope
        {
            OUTSIDE,    //外部
            INTERSECT,  //交错
            INSIDE      //内部
        };

    public:

        void SetMatrix(const Matrix4f &mvp);

        Scope PointIn(const Vector3f &)const;
        Scope SphereIn(const Vector3f &,float radius)const;
        Scope BoxIn(const AABB &)const;
    };//class Frustum
}//namespace hgl::graph
