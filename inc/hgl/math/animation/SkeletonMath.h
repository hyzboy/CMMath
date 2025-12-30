/**
 * SkeletonMath.h - 骨骼层级和反向运动学
 * 
 * 提供骨骼动画的数学工具：
 * - 骨骼层级变换（正向运动学 FK）
 * - 反向运动学（IK）基础支持
 * 
 * 参考：
 * - "Real-Time Rendering, 4th Edition" (Skinning 章节)
 * - FABRIK: "FABRIK: A fast, iterative solver for the Inverse Kinematics problem"
 */

#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/MatrixTypes.h>
#include<vector>

namespace hgl::math
{
    /**
     * 骨骼关节数据
     */
    struct Joint
    {
        int parent_index;           // 父关节索引（根为 -1）
        Matrix4f local_transform;   // 相对父关节的变换
        Matrix4f inverse_bind_pose; // 绑定姿态的逆矩阵
        
        Joint() : parent_index(-1) {}
    };
    
    /**
     * 骨骼层级系统
     */
    class Skeleton
    {
    public:
        Skeleton();
        
        /**
         * 添加关节
         * @param parent_index 父关节索引（根为 -1）
         * @param local_transform 局部变换
         * @param inverse_bind_pose 绑定姿态逆矩阵
         * @return 新关节的索引
         */
        int AddJoint(int parent_index, const Matrix4f &local_transform, const Matrix4f &inverse_bind_pose);
        
        /**
         * 计算全局变换矩阵（正向运动学）
         * @param out_global_transforms 输出全局变换数组
         */
        void ComputeGlobalTransforms(std::vector<Matrix4f> &out_global_transforms) const;
        
        /**
         * 计算蒙皮矩阵
         * @param out_skin_matrices 输出蒙皮矩阵数组
         */
        void ComputeSkinMatrices(std::vector<Matrix4f> &out_skin_matrices) const;
        
        /**
         * 获取关节数量
         */
        int GetJointCount() const;
        
        /**
         * 设置关节局部变换
         */
        void SetJointLocalTransform(int joint_index, const Matrix4f &transform);
        
        /**
         * 获取关节局部变换
         */
        const Matrix4f& GetJointLocalTransform(int joint_index) const;
        
        /**
         * 清空骨骼
         */
        void Clear();
        
    private:
        std::vector<Joint> joints;
    };
    
    // ==================== IK (反向运动学) 基础支持 ====================
    
    /**
     * 两骨骼 IK 求解（如手臂肘关节）
     * @param start 起始点（肩膀）
     * @param mid 中间关节（肘）
     * @param end 末端关节（手腕）
     * @param target 目标位置
     * @param pole_vector 极向量（控制肘的方向）
     * @param out_mid_position 输出：计算后的中间关节位置
     * @param out_end_position 输出：计算后的末端位置
     * @return 是否求解成功
     */
    bool SolveTwoBoneIK(const Vector3f &start, const Vector3f &mid, const Vector3f &end,
                        const Vector3f &target, const Vector3f &pole_vector,
                        Vector3f &out_mid_position, Vector3f &out_end_position);
    
    /**
     * FABRIK (Forward And Backward Reaching Inverse Kinematics)
     * 通用多关节 IK 求解器
     * @param joints 关节位置数组（输入输出）
     * @param joint_count 关节数量
     * @param target 目标位置
     * @param tolerance 容差
     * @param max_iterations 最大迭代次数
     * @return 是否收敛
     */
    bool SolveFABRIK(Vector3f *joints, int joint_count, const Vector3f &target, 
                     float tolerance = 0.01f, int max_iterations = 10);

}//namespace hgl::math
