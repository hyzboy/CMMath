/**
 * Projection.h - 投影与视图变换
 *
 * 包含：
 * - 正交投影矩阵 (OrthoMatrix)
 * - 透视投影矩阵 (PerspectiveMatrix)
 * - 视图矩阵 (LookAtMatrix)
 * - 屏幕坐标投影/反投影
 */

#pragma once

#include<hgl/math/VectorTypes.h>
#include<hgl/math/Matrix.h>

namespace hgl::math
{
    // ==================== 正交投影矩阵 ====================

    /**
     * 生成一个正交投影矩阵（6参数版本）
     * @param left 左边界
     * @param right 右边界
     * @param bottom 底边界
     * @param top 顶边界
     * @param znear 近平面z值
     * @param zfar 远平面z值
     */
    Matrix4f OrthoMatrix( float left,
                          float right,
                          float bottom,
                          float top,
                          float znear,
                          float zfar );

    /**
     * 生成一个正交投影矩阵（4参数版本）
     * @param width 宽度
     * @param height 高度
     * @param znear 近平面z值
     * @param zfar 远平面z值
     */
    Matrix4f OrthoMatrix(float width,float height,float znear,float zfar);

    /**
     * 生成一个正交投影矩阵（2参数版本，近平面=0，远平面=1）
     * @param width 宽度
     * @param height 高度
     */
    Matrix4f OrthoMatrix(float width,float height);

    // ==================== 透视投影矩阵 ====================

    /**
     * 生成一个透视投影矩阵
     * @param field_of_view 视野角度（FOV，度数）
     * @param aspect_ratio 宽高比（width/height）
     * @param znear 近截面
     * @param zfar 远截面
     */
    Matrix4f PerspectiveMatrix( float field_of_view,
                                float aspect_ratio,
                                float znear,
                                float zfar);


    /**
     * @brief 构造一个在 z==0 时与正交投影一致，但随 z 引入透视效果的投影矩阵
     *
     * 用途：需要在屏幕平面（z==0）与 OrthoMatrix 行为严格一致，但当物体沿视深方向
     * 发生偏离时，產生近大远小的透视效果。
     *
     * 说明：矩阵以列主序构造。alpha 控制透视强度（一般取负值以在 camera-space z<0 时产生缩放），
     * alpha = 0 时退化为正交投影（无透视）。
     */
    math::Matrix4f PerspectiveMatchOrtho(
        float left, float right,
        float bottom, float top,
        float znear, float zfar,
        float alpha /* 透视强度，默认可设为 -(2/(zfar-znear)) 或者 -(1/focus) */);

    // ==================== 视图矩阵 ====================

    /**
     * 产生一个LookAt变换矩阵
     * @param eye 相机位置
     * @param target 目标位置
     * @param up 向上向量（默认为Z轴）
     */
    Matrix4f LookAtMatrix(const Vector3f &eye,const Vector3f &target,const Vector3f &up=AxisVector::Z);

    // ==================== 坐标投影 ====================

    /**
     * 将世界坐标投影到屏幕坐标
     * @param world_pos 世界坐标
     * @param view 视图矩阵
     * @param projection 投影矩阵
     * @param viewport_size 视口大小
     * @return 屏幕坐标
     */
    Vector2i ProjectToScreen(
        const Vector3f& world_pos,
        const Matrix4f& view,
        const Matrix4f& projection,
        const Vector2u& viewport_size);

    /**
     * 将屏幕坐标反投影到世界坐标
     * @param win_pos 屏幕坐标
     * @param view 视图矩阵
     * @param projection 投影矩阵
     * @param viewport_size 视口大小
     * @return 世界坐标
     */
    Vector3f UnProjectToWorld(
        const Vector2i &win_pos,
        const Matrix4f &view,
        const Matrix4f &projection,
        const Vector2u &viewport_size);

    /**
     * 生成等距视角投影矩阵（Isometric Projection）
     *
     * 实现经典2D游戏风格的2:1等距视角（如《模拟城市》《红色警戒》等）
     *
     * @param width 视口宽度（世界单位）
     * @param height 视口高度（世界单位）
     * @param znear 近裁剪面
     * @param zfar 远裁剪面
     * @param use_precise_ratio 是否使用精确的2:1比例（默认true）
     *        true: 使用 arctan(1/√2) ≈ 35.264°（精确2:1）
     *        false: 使用传统的30°（更平缓，但不是严格2:1）
     * @return Matrix4f 等距投影矩阵
     */
    Matrix4f IsometricMatrix(float width, float height, float znear, float zfar, bool use_precise_ratio = true);

    /**
     * 生成等距视角投影矩阵（简化版本，使用默认深度范围）
     */
    Matrix4f IsometricMatrix(float width, float height);

    /**
     * 为等距视角生成标准视图矩阵
     * @param center 场景中心点
     * @param distance 相机到中心的距离
     */
    Matrix4f IsometricViewMatrix(const Vector3f& center, float distance);

    /**
     * 将等距投影参数转换为透视投影矩阵
     *
     * 该函数用于从等距视角平滑过渡到标准3D透视视角。
     * 它会自动计算合适的FOV和相机位置，使得透视投影能够显示相似的场景范围。
     *
     * @param iso_width 等距投影的宽度（世界单位）
     * @param iso_height 等距投影的高度（世界单位）
     * @param znear 近裁剪面
     * @param zfar 远裁剪面
     * @param fov_override 可选：手动指定FOV（度），0表示自动计算
     * @return Matrix4f 透视投影矩阵
     */
    Matrix4f IsometricToPerspectiveMatrix(
        float iso_width,
        float iso_height,
        float znear,
        float zfar,
        float fov_override = 0.0f);

    /**
     * 计算从等距视角到透视视角的相机位置偏移
     *
     * 等距投影通常使用正交相机，转换为透视投影后需要调整相机位置以保持相似的视野范围。
     *
     * @param iso_width 等距投影宽度
     * @param iso_height 等距投影高度
     * @param target_fov 目标透视FOV（度）
     * @return float 推荐的相机后退距离
     */
    float CalculatePerspectiveCameraDistance(
        float iso_width,
        float iso_height,
        float target_fov);
}//namespace hgl::math
