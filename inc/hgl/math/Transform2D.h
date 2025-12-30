/**
 * Transform2D.h - 2D 变换矩阵
 * 
 * 实现 2D 变换矩阵（3x3 齐次坐标），用于 2D 游戏开发。
 * 
 * 包含：
 * - Matrix3f 类型定义
 * - 2D 变换构造函数（平移、旋转、缩放、组合）
 * - 2D 点/向量变换
 * - 矩阵分解
 */

#pragma once

#include <hgl/math/MatrixTypes.h>
#include <hgl/math/VectorTypes.h>
#include <hgl/math/Angle.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace hgl::math
{
    // ==================== 类型定义 ====================
    
    /**
     * Identity 3x3 单位矩阵（已在 MatrixTypes.h 中定义为 Identity3f）
     */
    
    // ==================== 2D 变换矩阵构造 ====================
    
    /**
     * @brief 创建 2D 平移矩阵
     * @param x X 轴平移量
     * @param y Y 轴平移量
     * @return 3x3 平移矩阵
     */
    inline Matrix3f Translate2D(float x, float y)
    {
        Matrix3f mat(1.0f);
        mat[2][0] = x;
        mat[2][1] = y;
        return mat;
    }
    
    /**
     * @brief 创建 2D 平移矩阵
     * @param translation 平移向量
     * @return 3x3 平移矩阵
     */
    inline Matrix3f Translate2D(const Vector2f &translation)
    {
        return Translate2D(translation.x, translation.y);
    }
    
    /**
     * @brief 创建 2D 旋转矩阵（绕 Z 轴旋转）
     * @param angle_degrees 旋转角度（角度制）
     * @return 3x3 旋转矩阵
     */
    inline Matrix3f Rotate2D(float angle_degrees)
    {
        float angle_rad = deg2rad(angle_degrees);
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        
        Matrix3f mat(1.0f);
        mat[0][0] = c;
        mat[0][1] = s;
        mat[1][0] = -s;
        mat[1][1] = c;
        return mat;
    }
    
    /**
     * @brief 创建 2D 缩放矩阵
     * @param sx X 轴缩放因子
     * @param sy Y 轴缩放因子
     * @return 3x3 缩放矩阵
     */
    inline Matrix3f Scale2D(float sx, float sy)
    {
        Matrix3f mat(1.0f);
        mat[0][0] = sx;
        mat[1][1] = sy;
        return mat;
    }
    
    /**
     * @brief 创建 2D 缩放矩阵
     * @param scale 缩放向量
     * @return 3x3 缩放矩阵
     */
    inline Matrix3f Scale2D(const Vector2f &scale)
    {
        return Scale2D(scale.x, scale.y);
    }
    
    /**
     * @brief 创建均匀缩放矩阵
     * @param scale 统一缩放因子
     * @return 3x3 缩放矩阵
     */
    inline Matrix3f Scale2D(float scale)
    {
        return Scale2D(scale, scale);
    }
    
    /**
     * @brief 创建 2D TRS (Translation-Rotation-Scale) 组合变换矩阵
     * 变换顺序：缩放 -> 旋转 -> 平移
     * @param translation 平移向量
     * @param rotation 旋转角度（角度制）
     * @param scale 缩放向量
     * @return 组合变换矩阵
     */
    inline Matrix3f TRS2D(const Vector2f &translation, float rotation, const Vector2f &scale)
    {
        // 组合顺序：T * R * S
        return Translate2D(translation) * Rotate2D(rotation) * Scale2D(scale);
    }
    
    /**
     * @brief 创建 2D TRS 变换矩阵（均匀缩放）
     */
    inline Matrix3f TRS2D(const Vector2f &translation, float rotation, float scale)
    {
        return TRS2D(translation, rotation, Vector2f(scale, scale));
    }
    
    // ==================== 2D 变换应用 ====================
    
    /**
     * @brief 变换 2D 点（考虑平移）
     * 使用齐次坐标 (x, y, 1) 进行变换
     * @param matrix 变换矩阵
     * @param point 待变换的点
     * @return 变换后的点
     */
    inline Vector2f TransformPoint2D(const Matrix3f &matrix, const Vector2f &point)
    {
        Vector3f homogeneous(point.x, point.y, 1.0f);
        Vector3f transformed = matrix * homogeneous;
        return Vector2f(transformed.x, transformed.y);
    }
    
    /**
     * @brief 变换 2D 方向向量（忽略平移）
     * 使用齐次坐标 (x, y, 0) 进行变换，只应用旋转和缩放
     * @param matrix 变换矩阵
     * @param direction 待变换的方向
     * @return 变换后的方向
     */
    inline Vector2f TransformDirection2D(const Matrix3f &matrix, const Vector2f &direction)
    {
        Vector3f homogeneous(direction.x, direction.y, 0.0f);
        Vector3f transformed = matrix * homogeneous;
        return Vector2f(transformed.x, transformed.y);
    }
    
    // ==================== 矩阵分解 ====================
    
    /**
     * @brief 从 2D 变换矩阵中提取 TRS 分量
     * 
     * 注意：此函数假设矩阵是标准的 TRS 变换矩阵。
     * 对于包含剪切或非均匀变换的矩阵，结果可能不准确。
     * 
     * @param matrix 输入的变换矩阵
     * @param out_translation 输出的平移分量
     * @param out_rotation 输出的旋转角度（角度制）
     * @param out_scale 输出的缩放分量
     * @return 成功返回 true，失败返回 false
     */
    inline bool DecomposeTransform2D(const Matrix3f &matrix, 
                                     Vector2f &out_translation, 
                                     float &out_rotation, 
                                     Vector2f &out_scale)
    {
        // 提取平移（第三列的前两个分量）
        out_translation.x = matrix[2][0];
        out_translation.y = matrix[2][1];
        
        // 提取缩放（变换后的基向量的长度）
        Vector2f scale_x(matrix[0][0], matrix[0][1]);
        Vector2f scale_y(matrix[1][0], matrix[1][1]);
        
        float sx = glm::length(scale_x);
        float sy = glm::length(scale_y);
        
        // 检查行列式以确定是否有镜像
        float det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        if (det < 0.0f)
        {
            sx = -sx;
        }
        
        out_scale.x = sx;
        out_scale.y = sy;
        
        // 提取旋转（从归一化的旋转矩阵）
        if (sx != 0.0f && sy != 0.0f)
        {
            float inv_sx = 1.0f / sx;
            float rot_mat_00 = matrix[0][0] * inv_sx;
            float rot_mat_01 = matrix[0][1] * inv_sx;
            
            // 使用 atan2 提取旋转角度
            out_rotation = rad2deg(std::atan2(rot_mat_01, rot_mat_00));
            return true;
        }
        
        // 缩放为零，无法提取旋转
        out_rotation = 0.0f;
        return false;
    }
    
    // ==================== 辅助函数 ====================
    
    /**
     * @brief 获取 2D 变换矩阵的逆矩阵
     * @param matrix 输入矩阵
     * @return 逆矩阵
     */
    inline Matrix3f Inverse2D(const Matrix3f &matrix)
    {
        return glm::inverse(matrix);
    }
    
    /**
     * @brief 创建 2D 观察矩阵（类似相机变换）
     * @param position 观察位置
     * @param target 目标位置
     * @param zoom 缩放因子
     * @return 观察矩阵
     */
    inline Matrix3f LookAt2D(const Vector2f &position, const Vector2f &target, float zoom = 1.0f)
    {
        Vector2f direction = glm::normalize(target - position);
        float angle = rad2deg(std::atan2(direction.y, direction.x));
        
        // 创建视图变换：先平移到原点，然后旋转对齐，最后缩放
        Matrix3f view = Scale2D(zoom) * Rotate2D(-angle) * Translate2D(-position);
        return view;
    }
    
}//namespace hgl::math
