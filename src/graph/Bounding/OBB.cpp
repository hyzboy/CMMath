#include<hgl/graph/OBB.h>

namespace hgl::graph
{
    void OBB::ComputePlanes()
    {
        planes[0].Set(center + axis[0] * half_length.x,  axis[0]);      // axis[0] 的正方向面
        planes[1].Set(center - axis[0] * half_length.x, -axis[0]);      // axis[0] 的负方向面
        planes[2].Set(center + axis[1] * half_length.y,  axis[1]);      // axis[1] 的正方向面
        planes[3].Set(center - axis[1] * half_length.y, -axis[1]);      // axis[1] 的负方向面
        planes[4].Set(center + axis[2] * half_length.z,  axis[2]);      // axis[2] 的正方向面
        planes[5].Set(center - axis[2] * half_length.z, -axis[2]);      // axis[2] 的负方向面
    }

    void OBB::Set(const Vector3f &c,const Vector3f &hl)
    {
        Set(c,Vector3f(1,0,0),Vector3f(0,1,0),Vector3f(0,0,1),hl);
    }

    void OBB::Set(const Vector3f &c,const Vector3f &a0,const Vector3f &a1,const Vector3f &a2,const Vector3f &hl)
    {
        center=c;
        axis[0]=a0;
        axis[1]=a1;
        axis[2]=a2;
        half_length=hl;

        ComputePlanes();
    }

    const Matrix4f OBB::GetMatrix(const float cube_size)const
    {
        // 这段代码也是正确的，留着做参考吧！
        {        
            //Matrix4f translate_matrix   =TranslateMatrix(center);
            //Matrix4f rotate_matrix      =axis;
            //Matrix4f scale_matrix       =ScaleMatrix(half_length*(cube_size/0.5f));

            //return translate_matrix*rotate_matrix*scale_matrix;
        }

        Matrix4f result(axis);

        const float scale=cube_size/0.5f;

        result[0]*=half_length.x*scale;
        result[1]*=half_length.y*scale;
        result[2]*=half_length.z*scale;

        result[3]=Vector4f(center,1.0f);

        return result;
    }

    void OBB::GetCorners(Vector3f out[8])const
    {        
        const glm::vec3 ex=axis[0]*half_length.x;
        const glm::vec3 ey=axis[1]*half_length.y;
        const glm::vec3 ez=axis[2]*half_length.z;

        out[0]=center-ex-ey-ez;
        out[1]=center+ex-ey-ez;
        out[2]=center-ex+ey-ez;
        out[3]=center+ex+ey-ez;
        out[4]=center-ex-ey+ez;
        out[5]=center+ex-ey+ez;
        out[6]=center-ex+ey+ez;
        out[7]=center+ex+ey+ez;
    }

    OBB OBB::Transformed(const Matrix4f &m)const
    {
        if(IsEmpty()) return *this;
        OBB out;
        out.center=glm::vec3(m*glm::vec4(center,1.0f));
        const glm::mat3 L(m);

        const glm::vec3 v0=L*axis[0];
        const glm::vec3 v1=L*axis[1];
        const glm::vec3 v2=L*axis[2];

        const float l0=glm::length(v0);
        const float l1=glm::length(v1);
        const float l2=glm::length(v2);

        out.axis[0]=(l0>0.0f)?(v0/l0):axis[0];
        out.axis[1]=(l1>0.0f)?(v1/l1):axis[1];
        out.axis[2]=(l2>0.0f)?(v2/l2):axis[2];
        out.half_length=glm::vec3(half_length.x*l0,half_length.y*l1,half_length.z*l2);
        out.ComputePlanes();
        return out;
    }
}//namespace hgl::graph
