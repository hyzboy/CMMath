/**
 * DualQuaternion.h - 双四元数
 * 
 * 用于骨骼蒙皮动画，避免传统 LBS（线性混合蒙皮）的糖果包装效应
 * 双四元数可以更自然地插值刚体变换
 * 
 * 参考：
 * - Kavan et al.: "Dual Quaternions for Rigid Transformation Blending" (2006)
 * - "Real-Time Rendering, 4th Edition" (Skinning 章节)
 */

#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/Quaternion.h>
#include<hgl/math/MatrixTypes.h>

namespace hgl::math
{
    /**
     * 双四元数类
     * 用于骨骼蒙皮，避免传统 LBS（线性混合蒙皮）的糖果包装效应
     */
    class DualQuaternion
    {
    public:
        Quatf real;      // 实部四元数（旋转）
        Quatf dual;      // 对偶部分（平移）
        
        /**
         * 构造函数
         */
        DualQuaternion();
        DualQuaternion(const Quatf &real, const Quatf &dual);
        
        /**
         * 从旋转和平移构造
         * @param rotation 旋转四元数
         * @param translation 平移向量
         */
        static DualQuaternion FromRotationTranslation(const Quatf &rotation, const Vector3f &translation);
        
        /**
         * 从变换矩阵构造
         */
        static DualQuaternion FromMatrix(const Matrix4f &matrix);
        
        /**
         * 转换为变换矩阵
         */
        Matrix4f ToMatrix() const;
        
        /**
         * 归一化双四元数
         */
        DualQuaternion Normalized() const;
        void Normalize();
        
        /**
         * 双四元数乘法（组合变换）
         */
        DualQuaternion operator*(const DualQuaternion &other) const;
        
        /**
         * 双四元数加法（用于混合）
         */
        DualQuaternion operator+(const DualQuaternion &other) const;
        
        /**
         * 标量乘法（用于混合）
         */
        DualQuaternion operator*(float scalar) const;
        
        /**
         * 双四元数线性混合（DLB - Dual quaternion Linear Blending）
         * @param dqs 双四元数数组
         * @param weights 权重数组
         * @param count 数量
         * @return 混合后的双四元数
         */
        static DualQuaternion Blend(const DualQuaternion *dqs, const float *weights, int count);
        
        /**
         * 变换点
         */
        Vector3f TransformPoint(const Vector3f &point) const;
        
        /**
         * 变换方向（忽略平移）
         */
        Vector3f TransformDirection(const Vector3f &direction) const;
        
        /**
         * 获取平移部分
         */
        Vector3f GetTranslation() const;
        
        /**
         * 获取旋转部分
         */
        Quatf GetRotation() const;
    };
    
    /**
     * 双四元数线性插值
     */
    DualQuaternion DLerp(const DualQuaternion &from, const DualQuaternion &to, float t);
    
    /**
     * 双四元数球面线性插值（更平滑）
     */
    DualQuaternion DScLerp(const DualQuaternion &from, const DualQuaternion &to, float t);

}//namespace hgl::math
