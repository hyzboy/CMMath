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
 * @brief 生成一个正交投影矩阵（列主序，兼容 Vulkan 深度范围 [0,1]）
 *
 * 该函数返回的矩阵将把给定的裁剪体（left,right,bottom,top,znear,zfar）映射到标准裁剪空间。
 * 与 OpenGL 的 [-1,1] 深度不同，此处采用 0..1 深度范围（常用于 Vulkan）。
 * 注意：此处的坐标系为右手，视空间中摄像机朝向为 -Z。
 *
 * @param left  裁剪体左边界（x）
 * @param right 裁剪体右边界（x）
 * @param bottom 裁剪体下边界（y）
 * @param top 裁剪体上边界（y）
 * @param znear 近平面深度（正值），视空间中近平面对应 z=-znear
 * @param zfar  远平面深度（正值），视空间中远平面对应 z=-zfar
 * @return Matrix4f 正交投影矩阵，列主序构造
 */
    math::Matrix4f OrthoMatrix( float left,
                    float right,
                    float bottom,
                    float top,
                    float znear,
                    float zfar )
    {
        return Matrix4f(
          2.0f / (right - left),
          0.0f,
          0.0f,
          0.0f,

          0.0f,
          2.0f / (bottom - top),
          0.0f,
          0.0f,

          0.0f,
          0.0f,
          1.0f / (znear - zfar),
          0.0f,

          -(right + left) / (right - left),
          -(bottom + top) / (bottom - top),
          znear / (znear - zfar),
          1.0f
        );
    }

    /**
     * @brief 便捷重载：基于宽高构造正交投影矩阵（左上原点到右下）
     *
     * 与 OrthoMatrix(left,right,bottom,top,...) 等价，这里按 (0,width,height,0) 构造。
     */
    math::Matrix4f OrthoMatrix(float width,float height,float znear,float zfar)
    {
        return OrthoMatrix(0.0f,width,height,0.0f,znear,zfar);
    }

    /**
     * @brief 更简便的正交投影（默认 znear=0, zfar=1）
     */
    math::Matrix4f OrthoMatrix(float width,float height)
    {
        return OrthoMatrix(width,height,0.0f,1.0f);
    }

    /**
     * @brief 生成一个透视投影矩阵（depth 0..1，右手坐标系）
     *
     * 矩阵采用列主序构造，返回的深度映射将近平面映射到 NDC.z=0，远平面映射到 NDC.z=1，
     * 与 Vulkan  的深度范围一致。
     *
     * 参数 field_of_view 单位为度（degrees），表示垂直方向的视场角（FOV）。
     *
     * 矩阵形式与 glm::perspectiveRH_ZO 等价（右手、Z 0..1）。
     *
     * @param field_of_view 垂直 FOV（度）
     * @param aspect_ratio 宽高比（width/height）
     * @param znear 近平面（正值）
     * @param zfar  远平面（正值）
     * @return Matrix4f 透视投影矩阵
     */
    math::Matrix4f PerspectiveMatrix( float field_of_view,
                                float aspect_ratio,
                                float znear,
                                float zfar)
    {
        float f = 1.0f / tanf( deg2rad( 0.5f * field_of_view ) );

        return Matrix4f(
          -f / aspect_ratio,
          0.0f,
          0.0f,
          0.0f,

          0.0f,
          -f,
          0.0f,
          0.0f,

          0.0f,
          0.0f,
          zfar/(znear-zfar),
          -1.0f,

          0.0f,
          0.0f,
          (znear * zfar) / (znear-zfar),
          0.0f
        );
    }

    /**
     * @brief 构造一个在 z==0 时与正交投影一致，但随 z 引入透视效果的投影矩阵
     *
     * 用途：需要在屏幕平面（z==0）与 OrthoMatrix 行为严格一致，但当物体沿视深方向
     * 发生偏离时，產生近大远小的透视效果。
     *
     * 说明：矩阵以列主序构造。alpha 控制透视强度（一般取负值以在 camera-space z<0 时产生缩放），
     * alpha = 0 时退化为正交投影（无透视）。
     *
     * @param left  正交体左边界
     * @param right 右边界
     * @param bottom 下边界
     * @param top 上边界
     * @param znear 近平面（正值）
     * @param zfar  远平面（正值）
     * @param alpha 透视强度（建议负值，例如 -1/focus）
     * @return Matrix4f 自定义透视矩阵
     */
    math::Matrix4f PerspectiveMatchOrtho(
        float left, float right,
        float bottom, float top,
        float znear, float zfar,
        float alpha /* 透视强度，默认可设为 -(2/(zfar-znear)) 或者 -(1/focus) */)
    {
        // orthographic params (same as OrthoMatrix)
        const float m00 = 2.0f / (right - left);                     // scale X
        const float m11 = 2.0f / (bottom - top);                     // scale Y
        const float tx  = -(right + left) / (right - left);         // translate X
        const float ty  = -(bottom + top) / (bottom - top);         // translate Y

        // depth mapping: keep RH, Z in [0,1] behaviour compatible with existing PerspectiveMatrix
        const float C = zfar / (znear - zfar);
        const float D = (znear * zfar) / (znear - zfar);

        // Matrix is column-major: columns passed sequentially
        // column0, column1, column2, column3
        // column2 contains C at row2 and alpha at row3 (w = 1 + alpha*z)
        return Matrix4f(
            /* col0 */ m00, 0.0f, 0.0f, 0.0f,
            /* col1 */ 0.0f, m11, 0.0f, 0.0f,
            /* col2 */ 0.0f, 0.0f, C,    alpha,
            /* col3 */ tx,   ty,   D,    1.0f
        );
    }

    /**
     * @brief 生成视图矩阵（等价于 glm::lookAtRH）
     *
     * 约定：右手坐标系，摄像机朝向为 -Z。函数参数为摄像机位置 eye、目标位置 target 和上向量 up。
     * 返回的矩阵将世界坐标变换到摄像机/视图空间。
     *
     * @param eye 摄像机位置
     * @param target 摄像机关注点
     * @param up 摄像机上向量（一般为 (0,1,0) 或 (0,0,1) 取决于坐标系）
     * @return Matrix4f 视图矩阵（列主序）
     */
    math::Matrix4f LookAtMatrix(const math::Vector3f &eye,const math::Vector3f &target,const math::Vector3f &up)
    {
        math::Vector3f forward=normalize(target-eye);
        math::Vector3f right  =normalize(cross(forward,up));
        math::Vector3f nup    =cross(right,forward);

        return Matrix4f(   right.x,
                             nup.x,
                        -forward.x,
                              0.0f,

                           right.y,
                             nup.y,
                        -forward.y,
                              0.0f,

                           right.z,
                             nup.z,
                        -forward.z,
                              0.0f,

                 -dot(eye,right  ),
                 -dot(eye,nup    ),
                  dot(eye,forward),
                              1.0f
        );

        //经查证，此代码完全等于glm::lookAtRH，无任何差别
    }

    /**
     * @brief 将世界坐标点投影到屏幕坐标（像素）
     *
     * 计算流程：
     *  1. world -> view -> clip = projection * view * vec4(world_pos,1)
     *  2. clip -> NDC 通过齐次除法 (x/w,y/w,z/w)。本实现假定 NDC z 在 [0,1]
     *  3. NDC -> 屏幕空间：x,y 从 [-1,1] 映射到 [0,width] 与 [0,height] 与，并假设屏幕 Y 向下为正
     *
     * 注意：若要获得深度值，可返回 ndc.z（范围 0..1）。
     *
     * @param world_pos 世界坐标位置
     * @param view 视图矩阵（world->view）
     * @param projection 投影矩阵
     * @param vp_size 视口尺寸（像素）
     * @return Vector2i 屏幕坐标（像素，整数）
     */
    Vector2i ProjectToScreen(
        const math::Vector3f &world_pos,
        const math::Matrix4f &view,
        const math::Matrix4f &projection,
        const math::Vector2u &vp_size)
    {
        // 1. 世界坐标 -> 裁剪空间
        math::Vector4f clip = projection * view * math::Vector4f(world_pos.x,world_pos.y,world_pos.z, 1.0f);       //需要转换到OPENGL坐标系

        // 2. 齐次除法，得到 NDC（注意Z为0~1）
        if (clip.w == 0.0f)
            return Vector2i(0, 0);

        math::Vector3f ndc;

        ndc.x = clip.x / clip.w;
        ndc.y = clip.y / clip.w;
        ndc.z = clip.z / clip.w; // 0~1

        // 3. NDC -> 屏幕空间
        float screen_x = (ndc.x + 1.0f) * 0.5f * float(vp_size.x);
        float screen_y = (ndc.y + 1.0f) * 0.5f * float(vp_size.y); // Y轴向下为正

        return Vector2i(screen_x, screen_y);
    }

    /**
     * @brief 将屏幕坐标反投影回世界坐标（假设深度在近平面）
     *
     * 本函数把屏幕像素坐标转换为 NDC，再通过逆投影（inverse(projection*view)）得到世界位置。
     * 当前实现默认使用近平面（ndc_z = 0）；若需要任意深度，请改造为传入深度参数并设置 ndc_z。
     *
     * @param win_pos 屏幕像素坐标
     * @param view 视图矩阵
     * @param projection 投影矩阵
     * @param vp_size 视口尺寸
     * @return Vector3f 世界坐标
     */
    math::Vector3f UnProjectToWorld(
        const math::Vector2i &win_pos,
        const math::Matrix4f &view,
        const math::Matrix4f &projection,
        const math::Vector2u &vp_size)
    {
        // 1. 归一化到 NDC（[-1, 1]）
        float ndc_x = (2.0f * float(win_pos.x)) / float(vp_size.x) - 1.0f;
        float ndc_y = (2.0f * float(win_pos.y)) / float(vp_size.y) - 1.0f;
        // 这里假设在近平面（z=0），如需支持深度可加参数
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

    /**
     * @brief 从 4x4 变换矩阵中分解出平移、旋转（四元数）和缩放
     *
     * 该实现基于对上三角 3x3 子矩阵（行向量）进行长度归一化来提取缩放，
     * 然后使用 quat_cast 从正交旋转矩阵构造四元数。
     *
     * 注意：此函数对非匀缩放、剪切、奇异矩阵具有限制；对包含反射（负缩放）的情况，会把 X 缩放符号取为负以指示镜像。
     *
     * @param transform 输入 4x4 变换矩阵（列主序）
     * @param outTranslation 输出平移向量
     * @param outRotation 输出旋转四元数
     * @param outScale 输出缩放向量
     * @return bool 始终返回 true（对于非法矩阵未做额外检查）
     */
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

        // 检测镜像变换（负缩放）
        if (dot(cross(Row[0], Row[1]), Row[2]) < 0.0f)
        {
            outScale.x = -outScale.x;
        }

        constexpr float epsilon = 1e-8f;

        // 归一化行向量以提取旋转（使用epsilon避免除零）
        if (outScale.x > epsilon) Row[0] /= outScale.x;
        if (outScale.y > epsilon) Row[1] /= outScale.y;
        if (outScale.z > epsilon) Row[2] /= outScale.z;

        // 提取旋转
        mat3 RotationMatrix(Row[0], Row[1], Row[2]);
        outRotation = quat_cast(RotationMatrix);

        return true;
    }

    /**
     * @brief 计算将 old_direction 旋转到 new_direction 的旋转矩阵（4x4）
     *
     * 处理了特殊情况：当两个方向几乎相同（返回单位矩阵）或几乎相反（旋转180度，选择任意垂直轴）。
     * 返回列主序矩阵，可直接用于变换向量（右乘列向量）或与其他 4x4 矩阵组合。
     *
     * @param old_direction 起始方向（应已归一化）
     * @param new_direction 目标方向（应已归一化）
     * @return Matrix4f 旋转矩阵
     */
    math::Matrix4f GetRotateMatrix(const math::Vector3f &old_direction, const math::Vector3f &new_direction)
    {
        constexpr float epsilon = 1e-6f;
        
        float dot_product = glm::dot(old_direction, new_direction);
        
        // 如果两个方向几乎相同，返回单位矩阵
        if (dot_product > 1.0f - epsilon)
            return Matrix4f(1.0f);
        
        // 如果两个方向几乎相反（180度）
        if (dot_product < -1.0f + epsilon)
        {
            // 找一个垂直于old_direction的轴
            math::Vector3f axis = glm::abs(old_direction.x) > 0.9f 
                ? glm::cross(old_direction, math::Vector3f(0.0f, 1.0f, 0.0f))
                : glm::cross(old_direction, math::Vector3f(1.0f, 0.0f, 0.0f));
            
            axis = glm::normalize(axis);
            return glm::rotate(Matrix4f(1.0f), float(pi), axis);
        }
        
        // 一般情况：计算旋转轴和角度
        math::Vector3f axis = glm::normalize(glm::cross(old_direction, new_direction));
        float angle = acos(glm::clamp(dot_product, -1.0f, 1.0f));
        
        return glm::rotate(Matrix4f(1.0f), angle, axis);
    }

    /**
     * @brief 计算将 old_direction 旋转到 new_direction 的四元数表示
     *
     * 对 near-equal 和 nearly-opposite 情况做了处理，返回单位四元数或 180 度旋转四元数作为特例。
     *
     * @param old_direction 起始方向（应已归一化）
     * @param new_direction 目标方向（应已归一化）
     * @return Quatf 旋转四元数
     */
    math::Quatf GetRotateQuat(const math::Vector3f &old_direction, const math::Vector3f &new_direction)
    {
        constexpr float epsilon = 1e-6f;
        
        float dot_product = glm::dot(old_direction, new_direction);
        
        // 如果两个方向几乎相同，返回单位四元数
        if (dot_product > 1.0f - epsilon)
            return Quatf(1.0f, 0.0f, 0.0f, 0.0f);
        
        // 如果两个方向几乎相反（180度）
        if (dot_product < -1.0f + epsilon)
        {
            // 找一个垂直于old_direction的轴
            math::Vector3f axis = glm::abs(old_direction.x) > 0.9f 
                ? glm::cross(old_direction, math::Vector3f(0.0f, 1.0f, 0.0f))
                : glm::cross(old_direction, math::Vector3f(1.0f, 0.0f, 0.0f));
            
            axis = glm::normalize(axis);
            return glm::angleAxis(float(pi), axis);
        }
        
        // 一般情况：计算旋转轴和角度
        math::Vector3f axis = glm::normalize(glm::cross(old_direction, new_direction));
        float angle = acos(glm::clamp(dot_product, -1.0f, 1.0f));
        
        return glm::angleAxis(angle, axis);
    }
}//namespace hgl::math
