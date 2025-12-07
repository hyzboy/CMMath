#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
    * 线段
    */
    class LineSegment
    {
    protected:

        math::Vector3f start;
        math::Vector3f end;

    public:

        LineSegment()=default;
        LineSegment(const math::Vector3f &s,const math::Vector3f &e):start(s),end(e) {}

        void Set(const math::Vector3f &s,const math::Vector3f &e)
        {
            start=s;
            end=e;
        }

        void SetStart(const math::Vector3f &s)
        {
            start=s;
        }

        void SetEnd(const math::Vector3f &e)
        {
            end=e;
        }

        const math::Vector3f &GetStart()const{return start;}
        const math::Vector3f &GetEnd()const{return end;}

        const math::Vector3f GetStartDirection()const{return normalized(end-start);}
        const math::Vector3f GetEndDirection()const{return normalized(start-end);}

        const float DistanceSquared()const{return length_squared(start,end);}
        const float Distance()const{return length(start,end);}
    };//class LineSegment
}//namespace hgl::math
