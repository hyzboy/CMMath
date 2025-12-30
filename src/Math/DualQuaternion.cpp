/**
 * DualQuaternion.cpp - 双四元数实现
 */

#include<hgl/math/animation/DualQuaternion.h>
#include<hgl/math/VectorOperations.h>
#include<glm/gtx/quaternion.hpp>

namespace hgl::math
{
    DualQuaternion::DualQuaternion()
        : real(1.0f, 0.0f, 0.0f, 0.0f)  // 单位四元数 (w, x, y, z)
        , dual(0.0f, 0.0f, 0.0f, 0.0f)
    {
    }
    
    DualQuaternion::DualQuaternion(const Quatf &r, const Quatf &d)
        : real(r), dual(d)
    {
    }
    
    DualQuaternion DualQuaternion::FromRotationTranslation(const Quatf &rotation, const Vector3f &translation)
    {
        DualQuaternion dq;
        dq.real = rotation;
        
        // 对偶部分 = 0.5 * translation_quat * rotation_quat
        // translation_quat = (0, translation.x, translation.y, translation.z)
        Quatf t(0.0f, translation.x, translation.y, translation.z);
        dq.dual = 0.5f * t * rotation;
        
        return dq;
    }
    
    DualQuaternion DualQuaternion::FromMatrix(const Matrix4f &matrix)
    {
        // 提取旋转部分
        Quatf rotation = glm::quat_cast(matrix);
        
        // 提取平移部分
        Vector3f translation(matrix[3][0], matrix[3][1], matrix[3][2]);
        
        return FromRotationTranslation(rotation, translation);
    }
    
    Matrix4f DualQuaternion::ToMatrix() const
    {
        // 转换旋转
        Matrix4f mat = glm::toMat4(real);
        
        // 添加平移
        Vector3f translation = GetTranslation();
        mat[3][0] = translation.x;
        mat[3][1] = translation.y;
        mat[3][2] = translation.z;
        
        return mat;
    }
    
    DualQuaternion DualQuaternion::Normalized() const
    {
        float len = glm::length(real);
        if (len < 1e-8f)
            return DualQuaternion();
        
        DualQuaternion result;
        float inv_len = 1.0f / len;
        result.real = real * inv_len;
        result.dual = dual * inv_len;
        return result;
    }
    
    void DualQuaternion::Normalize()
    {
        float len = glm::length(real);
        if (len < 1e-8f)
        {
            real = Quatf(1.0f, 0.0f, 0.0f, 0.0f);
            dual = Quatf(0.0f, 0.0f, 0.0f, 0.0f);
            return;
        }
        
        float inv_len = 1.0f / len;
        real = real * inv_len;
        dual = dual * inv_len;
    }
    
    DualQuaternion DualQuaternion::operator*(const DualQuaternion &other) const
    {
        DualQuaternion result;
        result.real = real * other.real;
        result.dual = real * other.dual + dual * other.real;
        return result;
    }
    
    DualQuaternion DualQuaternion::operator+(const DualQuaternion &other) const
    {
        DualQuaternion result;
        result.real = real + other.real;
        result.dual = dual + other.dual;
        return result;
    }
    
    DualQuaternion DualQuaternion::operator*(float scalar) const
    {
        DualQuaternion result;
        result.real = real * scalar;
        result.dual = dual * scalar;
        return result;
    }
    
    DualQuaternion DualQuaternion::Blend(const DualQuaternion *dqs, const float *weights, int count)
    {
        if (count == 0)
            return DualQuaternion();
        
        DualQuaternion result = dqs[0] * weights[0];
        
        // 确保所有四元数在同一半球（避免绕远路）
        for (int i = 1; i < count; i++)
        {
            float sign = (glm::dot(result.real, dqs[i].real) < 0.0f) ? -1.0f : 1.0f;
            result = result + dqs[i] * (weights[i] * sign);
        }
        
        result.Normalize();
        return result;
    }
    
    Vector3f DualQuaternion::TransformPoint(const Vector3f &point) const
    {
        // 转换为矩阵并变换点（简化实现）
        Matrix4f mat = ToMatrix();
        Vector4f p(point, 1.0f);
        Vector4f result = mat * p;
        return Vector3f(result.x, result.y, result.z);
    }
    
    Vector3f DualQuaternion::TransformDirection(const Vector3f &direction) const
    {
        // 只应用旋转
        return real * direction;
    }
    
    Vector3f DualQuaternion::GetTranslation() const
    {
        // translation = 2 * dual * conjugate(real)
        Quatf real_conj = glm::conjugate(real);
        Quatf t = 2.0f * dual * real_conj;
        return Vector3f(t.x, t.y, t.z);
    }
    
    Quatf DualQuaternion::GetRotation() const
    {
        return real;
    }
    
    DualQuaternion DLerp(const DualQuaternion &from, const DualQuaternion &to, float t)
    {
        // 确保在同一半球
        float sign = (glm::dot(from.real, to.real) < 0.0f) ? -1.0f : 1.0f;
        
        DualQuaternion result;
        result.real = glm::mix(from.real, to.real * sign, t);
        result.dual = glm::mix(from.dual, to.dual * sign, t);
        result.Normalize();
        
        return result;
    }
    
    DualQuaternion DScLerp(const DualQuaternion &from, const DualQuaternion &to, float t)
    {
        // 球面线性插值（对旋转部分）
        float sign = (glm::dot(from.real, to.real) < 0.0f) ? -1.0f : 1.0f;
        
        DualQuaternion result;
        result.real = glm::slerp(from.real, to.real * sign, t);
        result.dual = glm::mix(from.dual, to.dual * sign, t);
        result.Normalize();
        
        return result;
    }

}//namespace hgl::math
