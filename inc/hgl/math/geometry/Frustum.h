#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/MatrixTypes.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/Plane.h>

namespace hgl::math
{
    using FrustumPlanes=Vector4f[6];

    void GetFrustumPlanes(FrustumPlanes &fp,const math::Matrix4f &mvp);

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

        void SetMatrix(const math::Matrix4f &mvp);

        Scope PointIn(const math::Vector3f &)const;
        Scope SphereIn(const math::Vector3f &,float radius)const;
        Scope BoxIn(const AABB &)const;
    };//class Frustum
}//namespace hgl::math
