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

        Vector3f start;
        Vector3f end;

    public:

        LineSegment()=default;
        LineSegment(const Vector3f &s,const Vector3f &e):start(s),end(e) {}

        void Set(const Vector3f &s,const Vector3f &e)
        {
            start=s;
            end=e;
        }

        void SetStart(const Vector3f &s)
        {
            start=s;
        }

        void SetEnd(const Vector3f &e)
        {
            end=e;
        }

        const Vector3f &GetStart()const{return start;}
        const Vector3f &GetEnd()const{return end;}

        const Vector3f GetStartDirection()const{return normalized(end-start);}
        const Vector3f GetEndDirection()const{return normalized(start-end);}

        const float DistanceSquared()const{return length_squared(start,end);}
        const float Distance()const{return length(start,end);}
    };//class LineSegment
}//namespace hgl::math
