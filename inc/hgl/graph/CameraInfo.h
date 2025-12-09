#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/MatrixTypes.h>
#include<hgl/math/Projection.h>
#include<hgl/math/MatrixOperations.h>

namespace hgl::graph
{    
    using namespace hgl::math;

    /**
     * 摄相机信息
     */
    struct CameraInfo
    {
        Matrix4f projection;
        Matrix4f inverse_projection;

        Matrix4f view;
        Matrix4f inverse_view;

        Matrix4f vp;                    ///< projection * view
        Matrix4f inverse_vp;            ///< inverse_projection * inverse_view  

        Vector4f frustum_planes[6];     ///<视锥体平面

        Matrix4f sky;                   ///<天空盒用矩阵

        alignas(16) Vector3f pos;                   ///<摄像机坐标
        alignas(16) Vector3f view_line;             ///normalize(target-pos)
        alignas(16) Vector3f world_up;

        alignas(16) Vector3f billboard_up;
        alignas(16) Vector3f billboard_right;

        float znear,zfar;

    public:

        const Vector4f WorldToViewSpace(const Vector3f &v)const
        {
            return view * Vector4f(v,1);
        }

        const Vector4f LocalToViewSpace(const Matrix4f &l2w,const Vector3f &v)const
        {
            return view * l2w * Vector4f(v,1);
        }

        const Vector4f Project(const Vector3f &v)const
        {
            return vp * Vector4f(v,1);
        }

        const Vector4f Project(const Matrix4f &l2w,const Vector3f &v)const
        {
            return vp * l2w * Vector4f(v,1);
        }

        const Vector3f UnProject(const Vector3f &v)const
        {
            return inverse_vp * Vector4f(v,1);
        }
    };//struct CameraInfo

    constexpr const size_t CameraInfoBytes=sizeof(CameraInfo);

    inline Vector3f LocalToViewSpace(const Matrix4f &l2w,const Vector3f &local_pos,const CameraInfo *ci)
    {
        if(!ci) return {};

        const Vector4f clip_pos = ci->LocalToViewSpace(l2w,local_pos);

        if(clip_pos.w == 0.0f) return {};

        return { clip_pos.x / clip_pos.w,clip_pos.y / clip_pos.w,clip_pos.z / clip_pos.w };
    }

    inline Vector2i WorldPositionToScreen(const Vector3f &wp,const CameraInfo *ci,const Vector2u &vp_size)   ///< 世界坐标→屏幕坐标
    {
        if(!ci) return {};

        return ProjectToScreen(wp,ci->view,ci->projection,vp_size);
    }

    inline Vector3f ScreenPositionToWorld(const Vector2i &sp,const CameraInfo *ci,const Vector2u &vp_size)   ///< 屏幕坐标→世界坐标
    {
        if(!ci) return {};

        return UnProjectToWorld(sp,ci->view,ci->projection,vp_size);
    }
        
    /**
     * 计算一个旋转矩阵，使原本面向X轴的模型改成面向屏幕
     */
    inline const Matrix4f CalculateFacingRotationMatrix(const Vector3f &world_position,const Matrix4f &view_matrix,const Vector3f &origin_direction)
    {
        //因为精度问题会造成偏差，所以千万不要直接使用CameraInfo中的view_line
        //const Vector3f view_line=normalized(Vector3f(view_matrix[0][2],view_matrix[1][2],view_matrix[2][2]));

        return GetRotateMatrix(world_position,
                                origin_direction,
                                normalized(math::Vector3f(view_matrix[0][2],view_matrix[1][2],view_matrix[2][2])));
    }

    /**
     * 计算一个旋转四元数，使原本面向X轴的模型改成面向屏幕
     */
    inline const Quatf CalculateFacingRotationQuat(const Vector3f &world_position,const Matrix4f &view_matrix,const Vector3f &origin_direction)
    {
        return GetRotateQuat(world_position,
                                origin_direction,
                                normalized(math::Vector3f(view_matrix[0][2],view_matrix[1][2],view_matrix[2][2])));
    }
}//namespace hgl::graph
