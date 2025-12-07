#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
    * 胶囊体
    */
    class Capsule
    {
    protected:

        math::Vector3f start;
        math::Vector3f end;

        float radius;

    public:

        const math::Vector3f &GetStart()const{return start;}
        const math::Vector3f &GetEnd()const{return end;}
        const float GetRadius()const{return radius;}
    };//class Capsule
}//namespace hgl::math
