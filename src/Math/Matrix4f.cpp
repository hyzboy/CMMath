#include<hgl/math/MatrixOperations.h>
#include<hgl/math/Projection.h>
#include<hgl/math/TrigConstants.h>

// Vulkan Cookbook
// ISBN: 9781786468154
// Packt Publishing Limited
// git: https://github.com/PacktPublishing/Vulkan-Cookbook.git
//
// Author:   Pawel Lapinski
// LinkedIn: https://www.linkedin.com/in/pawel-lapinski-84522329
//
// Chapter: 10 Helper Recipes
// Recipe:  04 Preparing a perspective projection matrix
// Recipe:  05 Preparing an orthographic projection matrix

namespace hgl::math
{
    /**
     * 生成正交投影矩阵 (Vulkan, Z-up)
     * 
     * 坐标系约定:
     * - Vulkan NDC: x[-1,1], y[-1,1] (y向下), z[0,1]
     * - 世界空间: X右, Y前, Z上
     * 
     * @param left 左边界
     * @param right 右边界
     * @param bottom 底边界 (屏幕下方，y值较大)
     * @param top 顶边界 (屏幕上方，y值较小)
     * @param znear 近平面z值
     * @param zfar 远平面z值
     */
    math::Matrix4f OrthoMatrix( float left,
                    float right,
                    float bottom,
                    float top,
                    float znear,
                    float zfar )
    {
        // Vulkan正交投影：NDC y轴向下，所以top<bottom
        return Matrix4f(
          2.0f / (right - left),
          0.0f,
          0.0f,
          0.0f,

          0.0f,
          2.0f / (top - bottom),  // 注意：Vulkan y轴翻转，用(top-bottom)而非(bottom-top)
          0.0f,
          0.0f,

          0.0f,
          0.0f,
          1.0f / (znear - zfar),
          0.0f,

          -(right + left) / (right - left),
          -(top + bottom) / (top - bottom),  // 对应y轴翻转
          znear / (znear - zfar),
          1.0f
        );
    }

    /**
     * 生成正交投影矩阵 (简化版本，Vulkan)
     * @param width 宽度
     * @param height 高度
     * @param znear 近平面z值
     * @param zfar 远平面z值
     * 
     * 坐标系：左上角(0,0)，右下角(width,height)
     * - left=0, right=width
     * - top=0, bottom=height (y向下为正)
     */
    math::Matrix4f OrthoMatrix(float width,float height,float znear,float zfar)
    {
        return OrthoMatrix(0.0f,width,height,0.0f,znear,zfar);  // left, right, bottom, top
    }

    /**
     * 生成一个正角视图矩阵
     * @param width 宽
     * @param height 高
     */
    math::Matrix4f OrthoMatrix(float width,float height)
    {
        return OrthoMatrix(width,height,0.0f,1.0f);
    }

    /**
     * 生成一个透视矩阵 (Vulkan, Z-up, 右手坐标系)
     * @param field_of_view 视野(角度)
     * @param aspect_ratio 宽高比
     * @param znear 近截面
     * @param zfar 远截面
     * 
     * 坐标系约定:
     * - X轴向右 (+X = right)
     * - Y轴向前 (+Y = forward, 摄像机朝向)
     * - Z轴向上 (+Z = up)
     * - Vulkan NDC: x[-1,1], y[-1,1] (y向下), z[0,1] (0=近, 1=远)
     */
    math::Matrix4f PerspectiveMatrix( float field_of_view,
                                float aspect_ratio,
                                float znear,
                                float zfar)
    {
        float f = 1.0f / tanf( deg2rad( 0.5f * field_of_view ) );

        return Matrix4f(
          f / aspect_ratio,  // X轴不翻转
          0.0f,
          0.0f,
          0.0f,

          0.0f,
          -f,                // Y轴翻转 (Vulkan NDC y向下)
          0.0f,
          0.0f,

          0.0f,
          0.0f,
          zfar/(znear-zfar), // Vulkan深度范围[0,1]
          -1.0f,

          0.0f,
          0.0f,
          (znear * zfar) / (znear-zfar),
          0.0f
        );
    }

    /**
     * 生成视图矩阵 (Vulkan Z-up 右手坐标系)
     * 
     * 坐标系约定:
     * - 世界空间: X右, Y前, Z上 (right-handed, Z-up)
     * - 摄像机空间: forward沿+Y轴, right沿+X轴, up沿+Z轴
     * - 摄像机朝向: 从eye指向target的方向 (+Y方向)
     * 
     * @param eye 摄像机位置
     * @param target 目标点
     * @param up 向上方向 (通常是世界空间的+Z, 即Vector3f(0,0,1))
     */
    math::Matrix4f LookAtMatrix(const math::Vector3f &eye,const math::Vector3f &target,const math::Vector3f &up)
    {
        math::Vector3f forward=normalize(target-eye);  // 摄像机前方(+Y)
        math::Vector3f right  =normalize(cross(forward,up));  // 摄像机右方(+X)
        math::Vector3f nup    =cross(right,forward);   // 摄像机上方(+Z)

        // 构建视图矩阵 (世界到摄像机空间的变换)
        // 注意: forward保持正向 (摄像机朝向+Y, 不是OpenGL的-Z)
        return Matrix4f(   right.x,
                             nup.x,
                          forward.x,  // 不翻转forward (Z-up系统摄像机朝+Y)
                              0.0f,

                           right.y,
                             nup.y,
                          forward.y,  // 不翻转
                              0.0f,

                           right.z,
                             nup.z,
                          forward.z,  // 不翻转
                              0.0f,

                 -dot(eye,right  ),
                 -dot(eye,nup    ),
                 -dot(eye,forward),  // 改为负值
                              1.0f
        );

        // 注意: 此代码与glm::lookAtRH不同！
        // glm::lookAtRH是Y-up系统, 摄像机朝-Z
        // 此实现是Z-up系统, 摄像机朝+Y
    }

    /**
     * 世界坐标投影到屏幕坐标 (Vulkan Z-up)
     * 
     * @param world_pos 世界空间坐标
     * @param view 视图矩阵
     * @param projection 投影矩阵
     * @param vp_size 视口大小
     * @return 屏幕坐标 (左上角为原点，x右，y下)
     */
    Vector2i ProjectToScreen(
        const math::Vector3f &world_pos,
        const math::Matrix4f &view,
        const math::Matrix4f &projection,
        const math::Vector2u &vp_size)
    {
        // 1. 世界坐标 -> 裁剪空间
        math::Vector4f clip = projection * view * math::Vector4f(world_pos.x,world_pos.y,world_pos.z, 1.0f);

        // 2. 齐次除法，得到 NDC
        if (clip.w == 0.0f)
            return Vector2i(0, 0);

        math::Vector3f ndc;
        ndc.x = clip.x / clip.w;  // [-1, 1]
        ndc.y = clip.y / clip.w;  // [-1, 1] (Vulkan: -1=上, +1=下)
        ndc.z = clip.z / clip.w;  // [0, 1] (Vulkan深度范围)

        // 3. NDC -> 屏幕空间
        // Vulkan NDC: y=-1在上，y=+1在下
        // 屏幕空间: (0,0)在左上角
        float screen_x = (ndc.x + 1.0f) * 0.5f * float(vp_size.x);
        float screen_y = (ndc.y + 1.0f) * 0.5f * float(vp_size.y);  // NDC的+1对应屏幕底部

        return Vector2i(screen_x, screen_y);
    }

    /**
     * 屏幕坐标反投影到世界坐标 (Vulkan Z-up)
     * 
     * @param win_pos 屏幕坐标 (左上角为原点)
     * @param view 视图矩阵
     * @param projection 投影矩阵
     * @param vp_size 视口大小
     * @return 世界空间坐标 (近平面上的点)
     */
    math::Vector3f UnProjectToWorld(
        const math::Vector2i &win_pos,
        const math::Matrix4f &view,
        const math::Matrix4f &projection,
        const math::Vector2u &vp_size)
    {
        // 1. 屏幕坐标 -> NDC
        // 屏幕: (0,0)左上角 -> NDC: (-1,-1)左上角（Vulkan）
        float ndc_x = (2.0f * float(win_pos.x)) / float(vp_size.x) - 1.0f;
        float ndc_y = (2.0f * float(win_pos.y)) / float(vp_size.y) - 1.0f;
        // 近平面z=0 (Vulkan深度范围[0,1])
        float ndc_z = 0.0f;

        math::Vector4f ndc_pos(ndc_x, ndc_y, ndc_z, 1.0f);

        // 2. 逆变换到世界空间
        math::Matrix4f inv_proj_view = inverse(projection * view);
        math::Vector4f world_pos = inv_proj_view * ndc_pos;

        // 3. 齐次除法
        if (world_pos.w == 0.0f)
            return Vector3f(0, 0, 0);

        return Vector3f(world_pos.x / world_pos.w, world_pos.y / world_pos.w, world_pos.z / world_pos.w);
    }

    // 函数用于从 glm::mat4 中提取平移、旋转和缩放
    bool DecomposeTransform(const math::Matrix4f & transform, Vector3f & outTranslation, Quatf & outRotation, Vector3f & outScale)
    {
        using namespace glm;
        mat4 LocalMatrix(transform);

        // 提取平移
        outTranslation = vec3(LocalMatrix[3]);

        // 提取缩放和旋转
        vec3 Row[3];

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                Row[i][j] = LocalMatrix[i][j];

        // 提取缩放
        outScale.x = length(Row[0]);
        outScale.y = length(Row[1]);
        outScale.z = length(Row[2]);

        // 归一化行向量以提取旋转
        if (outScale.x) Row[0] /= outScale.x;
        if (outScale.y) Row[1] /= outScale.y;
        if (outScale.z) Row[2] /= outScale.z;

        // 提取旋转
        mat3 RotationMatrix(Row[0], Row[1], Row[2]);
        outRotation = quat_cast(RotationMatrix);

        return true;
    }

    /**
    * 计算一个方向旋转成另一个方向的变换矩阵
    */
    const math::Matrix4f GetRotateMatrix(const math::Vector3f &world_position,const math::Vector3f &old_direction,const math::Vector3f &new_direction)
    {
        math::Vector3f axis=glm::cross(old_direction,new_direction);

        if(glm::dot(axis,axis)<0.0001f)
            return Matrix4f(1.0f);

        axis=glm::normalize(axis);

        float angle=acos(glm::dot(old_direction,new_direction));

        return glm::rotate(Matrix4f(1.0f),angle,axis);
    }

    /**
    * 计算一个方向旋转成另一个方向的四元数
    */
    const math::Quatf GetRotateQuat(const math::Vector3f &world_position,const math::Vector3f &old_direction,const math::Vector3f &new_direction)
    {
        math::Vector3f axis=glm::cross(old_direction,new_direction);

        if(glm::dot(axis,axis)<0.0001f)
            return Quatf(1.0f,0.0f,0.0f,0.0f);

        axis=glm::normalize(axis);

        float angle=acos(glm::dot(old_direction,new_direction));

        return glm::angleAxis(angle,axis);
    }
}//namespace hgl::math
