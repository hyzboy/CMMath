#pragma once

#include<hgl/math/Vector.h>

namespace hgl
{
    namespace graph
    {
        /**
        * 胶囊体
        */
        class Capsule
        {
        protected:

            Vector3f start;
            Vector3f end;

            float radius;

        public:

            const Vector3f &GetStart()const{return start;}
            const Vector3f &GetEnd()const{return end;}
            const float GetRadius()const{return radius;}
        };//class Capsule
    }//namespace graph
}//namespace hgl
