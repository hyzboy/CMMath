#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    struct BoundingSphere
    {
        Vector3f center { 0.0f };

        float radius=-1.0f;

    public:

        void Clear()
        {
            center=Vector3f(0.0f);
            radius=-1.0f;
        }

        bool IsEmpty() const
        {
            return radius<=0.0f;
        }

    public:

        void SetFromPoints(const float *pts,const uint32 count,const uint32 component_count);
    };//struct BoundingSphere
}//namespace hgl::math
