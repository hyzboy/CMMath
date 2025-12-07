/**
 * Quaternion.h - 四元数操作
 * 
 * 包含：
 * - 四元数类型定义
 * - 四元数与矩阵的转换
 * - 四元数插值 (Lerp, SLerp)
 * - 四元数提取轴角
 */

#pragma once

#include<hgl/math/VectorTypes.h>
#include<hgl/math/MatrixTypes.h>
#include<hgl/math/FloatPrecision.h>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>

namespace hgl::math
{
    // ==================== 四元数类型定义 ====================
    
    using Quatf=glm::quat;
    const math::Quatf IdentityQuatf(1,0,0,0);     //w,x,y,z

    /**
     * 四元数精度比较
     */
    inline const bool IsNearlyEqual(const math::Quatf &q1,const math::Quatf &q2,const float err=HGL_FLOAT_ERROR)
    {
        return glm::all(glm::epsilonEqual(q1,q2,err));
    }

    // ==================== 四元数创建 ====================
    
    /**
     * 从轴角创建旋转四元数
     * @param angle 旋转角度（度数）
     * @param axis 旋转轴
     */
    inline Quatf RotationQuat(const float angle,const math::Vector3f &axis)
    {
        return glm::angleAxis(glm::radians(angle),axis);
    }

    // ==================== 四元数与矩阵转换 ====================
    
    /**
     * 四元数转换为4x4矩阵
     */
    inline Matrix4f ToMatrix(const math::Quatf &quat)
    {
        return glm::toMat4(quat);
    }

    // ==================== 四元数分解 ====================
    
    /**
     * 从四元数提取旋转轴和角度
     * @param quat 四元数
     * @param axis 输出：旋转轴
     * @param angle 输出：旋转角度（度数）
     */
    inline void ExtractedQuat(const math::Quatf &quat,Vector3f &axis,float &angle)
    {
        angle=glm::degrees(glm::angle(quat));
        axis=glm::axis(quat);
    }

    /**
     * 获取四元数的旋转轴
     */
    inline const math::Vector3f &GetRotateAxis(const math::Quatf &quat)
    {
        return glm::axis(quat);
    }

    /**
     * 获取四元数的旋转角度（度数）
     */
    inline const float GetRotateAngle(const math::Quatf &quat)
    {
        return glm::degrees(glm::angle(quat));
    }

    // ==================== 四元数插值 ====================
    
    /**
     * 四元数线性插值
     */
    inline Quatf LerpQuat(const math::Quatf &from,const math::Quatf &to,const float t)
    {
        return glm::lerp(from,to,t);
    }

    /**
     * 四元数球面线性插值（推荐用于旋转动画）
     */
    inline Quatf SLerpQuat(const math::Quatf &from,const math::Quatf &to,const float t)
    {
        return glm::slerp(from,to,t);
    }

    // ==================== 高级四元数操作 ====================
    
    /**
     * 获取从旧方向到新方向的旋转四元数
     * @param world_position 世界坐标位置
     * @param old_direction 旧方向向量
     * @param new_direction 新方向向量
     */
    const math::Quatf GetRotateQuat(const math::Vector3f &world_position,const math::Vector3f &old_direction,const math::Vector3f &new_direction);
}//namespace hgl::math
