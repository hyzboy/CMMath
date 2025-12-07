#include<hgl/math/geometry/AABB.h>

namespace hgl::math
{
    math::Vector3f AABB::GetVertexP(const math::Vector3f &normal) const
    {
        math::Vector3f res = minPoint;

        if (normal[0] > 0)res[0] += length[0];
        if (normal[1] > 0)res[1] += length[1];
        if (normal[2] > 0)res[2] += length[2];

        return(res);
    }

    math::Vector3f AABB::GetVertexN(const math::Vector3f &normal) const
    {
        math::Vector3f res = minPoint;

        if (normal[0] < 0)res[0] += length[0];
        if (normal[1] < 0)res[1] += length[1];
        if (normal[2] < 0)res[2] += length[2];

        return(res);
    }

    void AABB::Update()
    {
        face_center_point[0]=Vector3f(minPoint.x, center.y, center.z);
        face_center_point[1]=Vector3f(maxPoint.x, center.y, center.z);
        face_center_point[2]=Vector3f(center.x, minPoint.y, center.z);
        face_center_point[3]=Vector3f(center.x, maxPoint.y, center.z);
        face_center_point[4]=Vector3f(center.x, center.y, minPoint.z);
        face_center_point[5]=Vector3f(center.x, center.y, maxPoint.z);

        for(uint i=0;i<6;i++)
            planes[i].Set(face_center_point[i],AABBFaceNormal[i]);
    }

    void AABB::SetFromPoints(const float *pts,const uint32_t count,const uint32_t component_count)
    {
        Clear();

        if(pts==nullptr||count==0)
            return;

        const float *p=pts;
            
        math::Vector3f minp(p[0],p[1],p[2]);
        math::Vector3f maxp(p[0],p[1],p[2]);
        p+=component_count;

        for(uint32_t i=1;i<count;++i)
        {
            math::Vector3f v(p[0],p[1],p[2]);
            minp=glm::min(minp,v);
            maxp=glm::max(maxp,v);
            p+=component_count;
        }

        SetMinMax(minp,maxp);
    }

    AABB AABB::Transformed(const math::Matrix4f &m)const
    {
        if(IsEmpty())
            return *this;

        const math::Vector3f corners[8]=
        {
            minPoint,
            math::Vector3f(maxPoint.x,minPoint.y,minPoint.z),
            math::Vector3f(minPoint.x,maxPoint.y,minPoint.z),
            math::Vector3f(maxPoint.x,maxPoint.y,minPoint.z),
            math::Vector3f(minPoint.x,minPoint.y,maxPoint.z),
            math::Vector3f(maxPoint.x,minPoint.y,maxPoint.z),
            math::Vector3f(minPoint.x,maxPoint.y,maxPoint.z),
            maxPoint
        };

        math::Vector3f transformed[8];

        for(int i=0;i<8;++i)
            transformed[i] = Vector3f(m * math::Vector4f(corners[i], 1.0f));

        AABB result;
        result.SetFromPoints((const float *)&transformed,8,
                             sizeof(Vector3f)/sizeof(float));       //这里不能直接写3，因为Vector3f可能为了硬件SIMD指令加速而对齐保存。
        return result;
    }
}//namespace hgl::math
