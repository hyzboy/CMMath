/**
 * Matrix.h - 矩阵运算操作
 *
 * 包含：
 * - 平移矩阵 (TranslateMatrix)
 * - 缩放矩阵 (ScaleMatrix)
 * - 旋转矩阵 (AxisRotate)
 * - 矩阵变换组合
 * - 向量变换
 */

#pragma once

#include<hgl/math/VectorTypes.h>
#include<hgl/math/VectorOperations.h>
#include<hgl/math/Quaternion.h>
#include<hgl/math/FloatPrecision.h>
#include<hgl/type/MemoryUtil.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/euler_angles.hpp>

namespace hgl::math
{
    /**
     * 矩阵类型定义宏
     * 为每个矩阵维度生成：
     * - 类型别名 (Matrix2f, Matrix3f 等)
     * - 字节大小常量 (Matrix2fBytes 等)
     * - 单位矩阵常量 (Identity2f 等)
     * - IsIdentityMatrix 判断函数
     * - IsNearlyEqual 比较函数
     */
#define DEFINE_MATRIX(num)  using Matrix##num##f=glm::mat##num;  \
                            constexpr const size_t Matrix##num##fBytes=sizeof(Matrix##num##f); \
                            const Matrix##num##f Identity##num##f=Matrix##num##f(1.0f); \
                            inline bool IsIdentityMatrix(const Matrix##num##f &m){return(mem_compare(m,Identity##num##f)==0);}  \
                            inline bool IsNearlyEqual(const Matrix##num##f &m1,const Matrix##num##f &m2,const float err=math::float_error)    \
                            {   \
                                float *f1=(float *)&m1;\
                                float *f2=(float *)&m2;\
                            \
                                for(int i=0;i<sizeof(Matrix##num##f)/sizeof(float);i++)  \
                                {   \
                                    if(!math::IsNearlyEqual(*f1,*f2,err))  \
                                        return(false);  \
                            \
                                    ++f1;++f2;  \
                                }   \
                            \
                                return(true);   \
                            }

    // 方阵类型定义
    DEFINE_MATRIX(2)
    DEFINE_MATRIX(3)
    DEFINE_MATRIX(4)

    // 非方阵类型定义
    DEFINE_MATRIX(2x4)
    DEFINE_MATRIX(3x4)
    DEFINE_MATRIX(4x2)
    DEFINE_MATRIX(4x3)

//#undef DEFINE_MATRIX

    // ==================== 四元数转换 ====================

    inline Matrix4f ToMatrix(const Quatf &quat)
    {
        return glm::toMat4(quat);
    }

    // ==================== 平移矩阵 ====================

    inline Matrix4f TranslateMatrix(const Vector3f &v)
    {
        return glm::translate(Matrix4f(1.0f),v);
    }

    inline Matrix4f TranslateMatrix(float x,float y,float z)
    {
        return glm::translate(Matrix4f(1.0f),Vector3f(x,y,z));
    }

    inline Matrix4f TranslateMatrix(float x,float y)
    {
        return glm::translate(Matrix4f(1.0f),Vector3f(x,y,1.0f));
    }

    // ==================== 缩放矩阵 ====================

    inline Matrix4f ScaleMatrix(const Vector3f &v)
    {
        return glm::scale(Matrix4f(1.0f),v);
    }

    inline Matrix4f ScaleMatrix(float x,float y,float z)
    {
        return glm::scale(Matrix4f(1.0f),Vector3f(x,y,z));
    }

    inline Matrix4f ScaleMatrix(float x,float y)
    {
        return glm::scale(Matrix4f(1.0f),Vector3f(x,y,1.0f));
    }

    inline Matrix4f ScaleMatrix(float s)
    {
        return glm::scale(Matrix4f(1.0f),Vector3f(s,s,s));
    }

    // ==================== 旋转矩阵 (4x4) ====================

    /**
     * 绕X轴旋转矩阵
     * @param rad 旋转弧度
     */
    inline Matrix4f AxisXRotate(float rad)
    {
        return glm::rotate(Matrix4f(1.0f),rad,AxisVector::X);
    }

    /**
     * 绕Y轴旋转矩阵
     * @param rad 旋转弧度
     */
    inline Matrix4f AxisYRotate(float rad)
    {
        return glm::rotate(Matrix4f(1.0f),rad,AxisVector::Y);
    }

    /**
     * 绕Z轴旋转矩阵
     * @param rad 旋转弧度
     */
    inline Matrix4f AxisZRotate(float rad)
    {
        return glm::rotate(Matrix4f(1.0f),rad,AxisVector::Z);
    }

    /**
     * 绕任意轴旋转矩阵
     * @param rad 旋转弧度
     * @param axis 旋转轴向量
     */
    inline Matrix4f AxisRotate(float rad,const Vector3f &axis)
    {
        return glm::rotate(Matrix4f(1.0f),rad,axis);
    }

    inline Matrix4f AxisRotate(float rad,float x,float y,float z)
    {
        return glm::rotate(Matrix4f(1.0f),rad,Vector3f(x,y,z));
    }

    inline Matrix4f AxisRotate(float rad,float x,float y)
    {
        return AxisRotate(rad,x,y,1.0f);
    }

    inline Matrix4f AxisRotate(float rad,const Vector4f &axis)
    {
        return AxisRotate(rad,Vector3f(axis.x,axis.y,axis.z));
    }

    // ==================== 旋转矩阵 (3x3) ====================

    /**
     * 绕任意轴旋转矩阵 (3x3)
     * @param rad 旋转弧度
     * @param axis 旋转轴向量
     */
    inline Matrix3f AxisRotate3(float rad,const Vector3f &axis)
    {
        return glm::mat3(glm::rotate(Matrix4f(1.0f), rad, axis));
    }

    inline Matrix3f AxisRotate3(float rad,float x,float y,float z)
    {
        return AxisRotate3(rad, Vector3f(x,y,z));
    }

    inline Matrix3f AxisRotate3(float rad,float x,float y)
    {
        return AxisRotate3(rad, Vector3f(x,y,1.0f));
    }

    inline Matrix3f AxisRotate3(float rad,const Vector4f &axis)
    {
        return AxisRotate3(rad, Vector3f(axis.x, axis.y, axis.z));
    }

    // ==================== 度数旋转矩阵 (3x3) ====================

    /**
     * 绕任意轴旋转矩阵，使用度数 (3x3)
     * @param deg 旋转度数
     * @param axis 旋转轴向量
     */
    inline Matrix3f AxisRotate3Deg(float deg,const Vector3f &axis)
    {
        return AxisRotate3(glm::radians(deg), axis);
    }

    inline Matrix3f AxisRotate3Deg(float deg,float x,float y,float z)
    {
        return AxisRotate3Deg(deg, Vector3f(x,y,z));
    }

    inline Matrix3f AxisRotate3Deg(float deg,const Vector4f &axis)
    {
        return AxisRotate3Deg(deg, Vector3f(axis.x, axis.y, axis.z));
    }

    // ==================== 向量旋转 ====================

    /**
     * 旋转3D向量
     * @param v3f 要旋转的向量
     * @param rad 旋转弧度
     * @param axis 旋转轴
     */
    inline Vector3f AxisRotate(const Vector3f &v3f,float rad,const Vector3f &axis)
    {
        Vector4f result = AxisRotate(rad, axis)*Vector4f(v3f, 1.0f);
        return Vector3f(result.x,result.y,result.z);
    }

    // ==================== 矩阵组合构建 ====================

    /**
     * 从平移、旋转（四元数）、缩放构建变换矩阵
     * @param move 平移向量
     * @param rotate_quat 旋转四元数
     * @param scale_xyz 缩放向量
     */
    inline Matrix4f MakeMatrix(const Vector3f &move,const Quatf &rotate_quat,const Vector3f &scale_xyz)
    {
        return glm::translate(Identity4f,move)
              *glm::toMat4(rotate_quat)
              *glm::scale(Identity4f,scale_xyz);
    }

    /**
     * 从平移、旋转（轴角）、缩放构建变换矩阵
     * @param move 平移向量
     * @param rotate_axis 旋转轴
     * @param rotate_angle 旋转角度（度数）
     * @param scale_xyz 缩放向量
     */
    inline Matrix4f MakeMatrix(const Vector3f &move,const Vector3f &rotate_axis,const float &rotate_angle,const Vector3f &scale_xyz)
    {
        return glm::translate(Identity4f,move)
              *glm::rotate(Identity4f,glm::radians(rotate_angle),rotate_axis)
              *glm::scale(Identity4f,scale_xyz);
    }

    // ==================== 相对矩阵 ====================

    // ==================== 矩阵运算 ====================

    /**
     * 矩阵求逆
     */
    template<typename T>
    inline T Inverse(const T &m)
    {
        return glm::inverse(m);
    }

    /**
     * 矩阵转置
     */
    template<typename T>
    inline T Transpose(const T &m)
    {
        return glm::transpose(m);
    }

    /**
     * 获取相对矩阵
     * @param self_matrix 自身矩阵
     * @param reference_matrix 参照矩阵
     */
    inline Matrix4f RelativeMatrix(const Matrix4f &self_matrix,const Matrix4f &reference_matrix)
    {
        return Inverse(reference_matrix)*self_matrix;
    }

    // ==================== 向量变换 ====================

    /**
     * 变换位置（考虑平移）
     */
    inline Vector3f TransformPosition(const Matrix4f &m,const Vector3f &v)
    {
        return Vector3f(m*Vector4f(v,1.0f));
    }

    /**
     * 变换方向（不考虑平移）
     */
    inline Vector3f TransformDirection(const Matrix4f &m,const Vector3f &v)
    {
        return Vector3f(m*Vector4f(v,0.0f));
    }

    /**
     * 变换法线（使用逆转置矩阵）
     */
    inline Vector3f TransformNormal(const Matrix4f& m, const Vector3f& v)
    {
        return Normalized(Vector3f(Transpose(Inverse(m)) * Vector4f(v, 0.0f)));
    }

    /**
     * Transform normal (3x3 matrix version)
     */
    inline Vector3f TransformNormal(const Matrix3f& m, const Vector3f& v)
    {
        return Normalized(m * v);
    }

    // ==================== 高级矩阵操作 ====================

    /**
     * 获取从旧方向到新方向的旋转矩阵
     * @param world_position 世界坐标位置
     * @param old_direction 旧方向向量
     * @param new_direction 新方向向量
     */
    const Matrix4f GetRotateMatrix(const Vector3f &world_position,const Vector3f &old_direction,const Vector3f &new_direction);

    /**
     * 分解变换矩阵为平移、旋转、缩放
     * @param transform 变换矩阵
     * @param outTranslation 输出：平移向量
     * @param outRotation 输出：旋转四元数
     * @param outScale 输出：缩放向量
     * @return 是否成功分解
     */
    bool DecomposeTransform(const Matrix4f & transform, Vector3f & outTranslation, Quatf & outRotation, Vector3f & outScale);
}//namespace hgl::math
