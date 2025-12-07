#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/MatrixTypes.h>
#include<hgl/math/Projection.h>
#include<hgl/math/MatrixOperations.h>

namespace hgl::graph
{
    /**
     * 摄相机信息
     */
    struct CameraInfo
    {
        math::Matrix4f projection;
        math::Matrix4f inverse_projection;

        math::Matrix4f view;
        math::Matrix4f inverse_view;

        math::Matrix4f vp;                    ///< projection * view
        math::Matrix4f inverse_vp;            ///< inverse_projection * inverse_view  

        math::Vector4f frustum_planes[6];     ///<视锥体平面

        math::Matrix4f sky;                   ///<天空盒用矩阵

        alignas(16) math::Vector3f pos;                   ///<摄像机坐标
        alignas(16) math::Vector3f view_line;             ///normalize(target-pos)
        alignas(16) math::Vector3f world_up;

        alignas(16) math::Vector3f billboard_up;
        alignas(16) math::Vector3f billboard_right;

        float znear,zfar;

    public:

        const math::Vector4f WorldToViewSpace(const math::Vector3f &v)const
        {
            return view * math::Vector4f(v,1);
        }

        const math::Vector4f LocalToViewSpace(const math::Matrix4f &l2w,const math::Vector3f &v)const
        {
            return view * l2w * math::Vector4f(v,1);
        }

        const math::Vector4f Project(const math::Vector3f &v)const
        {
            return vp * math::Vector4f(v,1);
        }

        const math::Vector4f Project(const math::Matrix4f &l2w,const math::Vector3f &v)const
        {
            return vp * l2w * math::Vector4f(v,1);
        }

        const math::Vector3f UnProject(const math::Vector3f &v)const
        {
            return inverse_vp * math::Vector4f(v,1);
        }
    };//struct CameraInfo

    constexpr const size_t CameraInfoBytes=sizeof(CameraInfo);

    inline math::Vector3f LocalToViewSpace(const math::Matrix4f &l2w,const math::Vector3f &local_pos,const CameraInfo *ci)
    {
        if(!ci) return {};

        const math::Vector4f clip_pos = ci->LocalToViewSpace(l2w,local_pos);

        if(clip_pos.w == 0.0f) return {};

        return { clip_pos.x / clip_pos.w,clip_pos.y / clip_pos.w,clip_pos.z / clip_pos.w };
    }

    inline math::Vector2i WorldPositionToScreen(const math::Vector3f &wp,const CameraInfo *ci,const math::Vector2u &vp_size)   ///< 世界坐标→屏幕坐标
    {
        if(!ci) return {};

        return math::ProjectToScreen(wp,ci->view,ci->projection,vp_size);
    }

    inline math::Vector3f ScreenPositionToWorld(const math::Vector2i &sp,const CameraInfo *ci,const math::Vector2u &vp_size)   ///< 屏幕坐标→世界坐标
    {
        if(!ci) return {};

        return math::UnProjectToWorld(sp,ci->view,ci->projection,vp_size);
    }
        
    /**
     * 计算一个旋转矩阵，使原本面向X轴的模型改成面向屏幕
     */
    inline const math::Matrix4f CalculateFacingRotationMatrix(const math::Vector3f &world_position,const math::Matrix4f &view_matrix,const math::Vector3f &origin_direction)
    {
        //因为精度问题会造成偏差，所以千万不要直接使用CameraInfo中的view_line
        //const math::Vector3f view_line=normalized(Vector3f(view_matrix[0][2],view_matrix[1][2],view_matrix[2][2]));

        return math::GetRotateMatrix(world_position,
                                origin_direction,
                                math::normalized(math::Vector3f(view_matrix[0][2],view_matrix[1][2],view_matrix[2][2])));
    }

    /**
     * 计算一个旋转四元数，使原本面向X轴的模型改成面向屏幕
     */
    inline const math::Quatf CalculateFacingRotationQuat(const math::Vector3f &world_position,const math::Matrix4f &view_matrix,const math::Vector3f &origin_direction)
    {
        return math::GetRotateQuat(world_position,
                                origin_direction,
                                math::normalized(math::Vector3f(view_matrix[0][2],view_matrix[1][2],view_matrix[2][2])));
    }
}//namespace hgl::graph
