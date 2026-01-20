#include<hgl/math/Matrix.h>
#include<hgl/math/Projection.h>
#include<hgl/math/TrigConstants.h>
#include<numbers>
#include<cmath>

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
        float f = 1.0f / std::tan( deg2rad( 0.5f * field_of_view ) );

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
        math::Vector3f forward=Normalized(target-eye);
        math::Vector3f right  =Normalized(Cross(forward,up));
        math::Vector3f nup    =Cross(right,forward);

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

                 -Dot(eye,right  ),
                 -Dot(eye,nup    ),
                  Dot(eye,forward),
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
        math::Matrix4f inv_proj_view = Inverse(projection * view);
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
        outScale.x = Length(Row[0]);
        outScale.y = Length(Row[1]);
        outScale.z = Length(Row[2]);

        // 检测镜像变换（负缩放）
        if (Dot(Cross(Row[0], Row[1]), Row[2]) < 0.0f)
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
            return glm::rotate(Matrix4f(1.0f), std::numbers::pi_v<float>, axis);
        }
        
        // 一般情况：计算旋转轴和角度
        math::Vector3f axis = glm::normalize(glm::cross(old_direction, new_direction));
        float angle = std::acos(glm::clamp(dot_product, -1.0f, 1.0f));
        
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
            return glm::angleAxis(std::numbers::pi_v<float>, axis);
        }
        
        // 一般情况：计算旋转轴和角度
        math::Vector3f axis = glm::normalize(glm::cross(old_direction, new_direction));
        float angle = std::acos(glm::clamp(dot_product, -1.0f, 1.0f));
        
        return glm::angleAxis(angle, axis);
    }

    /**
     * @brief 生成等距视角投影矩阵（Isometric Projection）
     *
     * 等距投影是一种特殊的正交投影，通过固定的旋转角度产生经典2D游戏的视觉效果。
     * 
     * 数学原理：
     * 1. 绕X轴旋转角度α（俯视角度）
     * 2. 绕Y轴旋转45°（使对角线平行于屏幕）
     * 3. 应用正交投影
     * 
     * 标准等距角度：
     * - 精确2:1比例：α = arctan(1/√2) ≈ 35.264°
     * - 传统近似值：α = 30°（Dimetric投影）
     * 
     * 坐标系说明：
     * - 使用右手坐标系（Y轴向上）
     * - 深度范围 [0, 1]（Vulkan标准）
     *
     * @param width 正交视口宽度（世界单位）
     * @param height 正交视口高度（世界单位）
     * @param znear 近裁剪面（正值）
     * @param zfar 远裁剪面（正值）
     * @param use_precise_ratio 是否使用精确2:1比例角度
     * @return Matrix4f 等距投影矩阵（列主序）
     */
    math::Matrix4f IsometricMatrix(float width, float height, float znear, float zfar, bool use_precise_ratio)
    {
        // 1. 确定俯视角度（X轴旋转）
        float x_rotation_angle;
        if (use_precise_ratio)
        {
            // 精确2:1比例：arctan(1/sqrt(2)) ≈ 35.264°
            x_rotation_angle = std::atan(1.0f / std::sqrt(2.0f));
        }
        else
        {
            // 传统30°角度（Dimetric投影）
            x_rotation_angle = deg2rad(30.0f);
        }
        
        // 2. Y轴旋转固定为45°（π/4）
        constexpr float y_rotation_angle = std::numbers::pi_v<float> / 4.0f;
        
        // 3. 计算旋转矩阵
        // 注意：矩阵乘法顺序为 Ortho * RotY * RotX
        // 这样物体先绕X轴旋转（俯视），再绕Y轴旋转（对角线对齐）
        
        const float cos_x = std::cos(x_rotation_angle);
        const float sin_x = std::sin(x_rotation_angle);
        const float cos_y = std::cos(y_rotation_angle);
        const float sin_y = std::sin(y_rotation_angle);
        
        // 组合旋转矩阵 R = RotY(45°) * RotX(α)
        // 手动展开矩阵乘法以提高效率
        const float r00 =  cos_y;
        const float r01 =  sin_y * sin_x;
        const float r02 =  sin_y * cos_x;
        
        const float r10 =  0.0f;
        const float r11 =  cos_x;
        const float r12 = -sin_x;
        
        const float r20 = -sin_y;
        const float r21 =  cos_y * sin_x;
        const float r22 =  cos_y * cos_x;
        
        // 4. 构建正交投影缩放
        const float sx = 2.0f / width;
        const float sy = 2.0f / height;
        const float sz = 1.0f / (znear - zfar);
        const float tz = znear / (znear - zfar);
        
        // 5. 组合为最终投影矩阵：P = Ortho * Rotation
        // 展开矩阵乘法（列主序）
        return Matrix4f(
            /* col0 */ sx * r00,  sy * r01,  sz * r02,  0.0f,
            /* col1 */ sx * r10,  sy * r11,  sz * r12,  0.0f,
            /* col2 */ sx * r20,  sy * r21,  sz * r22,  0.0f,
            /* col3 */ 0.0f,      0.0f,      tz,        1.0f
        );
    }

    /**
     * @brief 简化版等距投影矩阵（使用默认深度范围 0.1 ~ 100）
     */
    math::Matrix4f IsometricMatrix(float width, float height)
    {
        return IsometricMatrix(width, height, 0.1f, 100.0f, true);
    }

    /**
     * 为等距视角生成标准视图矩阵
     * @param center 场景中心点
     * @param distance 相机到中心的距离
     */
    Matrix4f IsometricViewMatrix(const Vector3f& center, float distance)
    {
        // 等距视角的标准相机位置（正上方偏移）
        Vector3f cam_pos = center + Vector3f(distance, distance, distance);
        Vector3f up(0, 1, 0);  // 或根据你的坐标系调整
        
        return LookAtMatrix(cam_pos, center, up);
    }

    /**
     * @brief 将等距投影参数转换为透视投影矩阵
     *
     * 用途：实现从等距视角到透视视角的转换。常用于游戏中从策略视角切换到第三人称视角。
     * 
     * 转换策略：
     * 1. 如果未指定FOV，根据等距视口尺寸自动计算合理的FOV（通常45-60度）
     * 2. 计算宽高比
     * 3. 使用标准透视投影矩阵
     * 
     * 注意：由于等距投影是正交的（无透视），转换后需要配合相机位置调整才能保持相似的场景覆盖范围。
     * 建议配合 CalculatePerspectiveCameraDistance 函数使用。
     *
     * @param iso_width 等距投影的世界宽度
     * @param iso_height 等距投影的世界高度
     * @param znear 近裁剪面（正值）
     * @param zfar 远裁剪面（正值）
     * @param fov_override 手动指定的垂直FOV（度），0表示自动计算
     * @return Matrix4f 透视投影矩阵
     */
    math::Matrix4f IsometricToPerspectiveMatrix(
        float iso_width, 
        float iso_height, 
        float znear, 
        float zfar,
        float fov_override)
    {
        // 1. 计算宽高比
        float aspect_ratio = iso_width / iso_height;
    
        // 2. 确定FOV
        float fov;
        if (fov_override > 0.0f)
        {
            fov = fov_override;
        }
        else
        {
            // 自动计算FOV：根据等距视口的高度来推算合理的透视FOV
            // 策略：假设相机距离场景中心的距离约为 iso_height
            // 使用反正切计算视场角：fov = 2 * atan(iso_height / (2 * distance))
            // 这里使用经验值，使得转换后的透视视角与等距视角在视觉上相近
        
            // 方案1：固定FOV（简单但可能不够灵活）
            // fov = 45.0f;
        
            // 方案2：根据视口尺寸动态计算（推荐）
            // 假设相机距离为视口高度的1.5倍，这样可以看到完整的等距场景
            float camera_distance = iso_height * 1.5f;
            float half_fov_rad = std::atan((iso_height * 0.5f) / camera_distance);
            fov = rad2deg(half_fov_rad * 2.0f);
        
            // 限制FOV在合理范围内（30-90度）
            fov = std::clamp(fov, 30.0f, 90.0f);
        }
    
        // 3. 使用标准透视投影矩阵
        return PerspectiveMatrix(fov, aspect_ratio, znear, zfar);
    }

    /**
     * 从透视投影参数估算等效的等距投影尺寸
     * 
     * @param fov 透视FOV（度）
     * @param aspect_ratio 宽高比
     * @param camera_distance 相机到场景的距离
     * @return Vector2f 推荐的等距投影宽度和高度 (width, height)
     */
    Vector2f PerspectiveToIsometricSize(float fov, float aspect_ratio, float camera_distance)
    {
        float half_fov_rad = deg2rad(fov * 0.5f);
        float height = 2.0f * camera_distance * std::tan(half_fov_rad);
        float width = height * aspect_ratio;
    
        return Vector2f(width, height);
    }

    /**
     * @brief 计算从等距投影转换到透视投影时的相机距离
     *
     * 等距投影使用正交相机，转换为透视投影后，需要将相机后退到合适的距离，
     * 以保证透视视角下能看到相似范围的场景。
     *
     * 计算原理：
     * 根据透视投影的FOV和期望的视口尺寸，通过三角函数计算相机到场景中心的距离。
     * distance = (height / 2) / tan(fov / 2)
     *
     * @param iso_width 等距投影的世界宽度
     * @param iso_height 等距投影的世界高度
     * @param target_fov 目标透视FOV（度）
     * @return float 推荐的相机距离（世界单位）
     */
    float CalculatePerspectiveCameraDistance(
        float iso_width, 
        float iso_height, 
        float target_fov)
    {
        // 使用高度作为基准计算距离
        // 公式：distance = (height / 2) / tan(fov / 2)
        float half_fov_rad = deg2rad(target_fov * 0.5f);
        float distance = (iso_height * 0.5f) / std::tan(half_fov_rad);
    
        // 可选：添加额外的偏移量以获得更好的视角
        // distance *= 1.2f;  // 稍微后退一点，避免场景边缘被裁剪
    
        return distance;
    }
}//namespace hgl::math
