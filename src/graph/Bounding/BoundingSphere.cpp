#include <hgl/graph/BoundingSphere.h>

namespace hgl::graph
{
    void BoundingSphere::SetFromPoints(const float *pts,const uint32 count,const uint32 component_count)
    {
        Clear();

        if(pts==nullptr||count==0)
            return;
    
        glm::vec3 c(0.0f);

        const float *p=pts;
        for(uint32 i=0;i<count;++i)
        {
            c.x += p[0];
            c.y += p[1];
            c.z += p[2];

            p+=component_count;
        }
        c /= static_cast<float>(count);
    
        float r=0.0f;

        p=pts;

        for(uint32 i=0;i<count;++i)
        {
            r = std::max(r, glm::length(Vector3f(p[0] - c.x,
                                                 p[1] - c.y,
                                                 p[2] - c.z)));

            p+=component_count;
        }
    
        center = c;
        radius = r;
    }
}//namespace hgl::graph

