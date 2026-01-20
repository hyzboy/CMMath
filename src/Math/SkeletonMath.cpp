/**
 * SkeletonMath.cpp - 骨骼层级和反向运动学实现
 */

#include<hgl/math/animation/SkeletonMath.h>
#include<hgl/math/VectorOperations.h>
#include<hgl/math/Matrix.h>
#include<cmath>
#include<algorithm>

namespace hgl::math
{
    // ==================== Skeleton 实现 ====================
    
    Skeleton::Skeleton()
    {
    }
    
    int Skeleton::AddJoint(int parent_index, const Matrix4f &local_transform, const Matrix4f &inverse_bind_pose)
    {
        Joint joint;
        joint.parent_index = parent_index;
        joint.local_transform = local_transform;
        joint.inverse_bind_pose = inverse_bind_pose;
        
        joints.push_back(joint);
        return joints.size() - 1;
    }
    
    void Skeleton::ComputeGlobalTransforms(std::vector<Matrix4f> &out_global_transforms) const
    {
        out_global_transforms.resize(joints.size());
        
        for (size_t i = 0; i < joints.size(); i++)
        {
            const Joint &joint = joints[i];
            
            if (joint.parent_index < 0)
            {
                // 根关节
                out_global_transforms[i] = joint.local_transform;
            }
            else
            {
                // 子关节：全局变换 = 父全局变换 * 局部变换
                out_global_transforms[i] = out_global_transforms[joint.parent_index] * joint.local_transform;
            }
        }
    }
    
    void Skeleton::ComputeSkinMatrices(std::vector<Matrix4f> &out_skin_matrices) const
    {
        std::vector<Matrix4f> global_transforms;
        ComputeGlobalTransforms(global_transforms);
        
        out_skin_matrices.resize(joints.size());
        
        for (size_t i = 0; i < joints.size(); i++)
        {
            // 蒙皮矩阵 = 全局变换 * 绑定姿态逆矩阵
            out_skin_matrices[i] = global_transforms[i] * joints[i].inverse_bind_pose;
        }
    }
    
    int Skeleton::GetJointCount() const
    {
        return joints.size();
    }
    
    void Skeleton::SetJointLocalTransform(int joint_index, const Matrix4f &transform)
    {
        if (joint_index >= 0 && joint_index < static_cast<int>(joints.size()))
        {
            joints[joint_index].local_transform = transform;
        }
    }
    
    const Matrix4f& Skeleton::GetJointLocalTransform(int joint_index) const
    {
        static Matrix4f identity(1.0f);
        
        if (joint_index >= 0 && joint_index < static_cast<int>(joints.size()))
        {
            return joints[joint_index].local_transform;
        }
        
        return identity;
    }
    
    void Skeleton::Clear()
    {
        joints.clear();
    }
    
    // ==================== IK 求解器实现 ====================
    
    bool SolveTwoBoneIK(const Vector3f &start, const Vector3f &mid, const Vector3f &end,
                        const Vector3f &target, const Vector3f &pole_vector,
                        Vector3f &out_mid_position, Vector3f &out_end_position)
    {
        // 计算骨骼长度
        float upper_length = glm::length(mid - start);
        float lower_length = glm::length(end - mid);
        float total_length = upper_length + lower_length;
        
        // 计算到目标的距离
        Vector3f to_target = target - start;
        float target_distance = glm::length(to_target);
        
        // 检查目标是否在可达范围内
        if (target_distance > total_length * 0.999f)
        {
            // 目标太远，伸直指向目标
            Vector3f direction = glm::normalize(to_target);
            out_mid_position = start + direction * upper_length;
            out_end_position = start + direction * std::min(target_distance, total_length);
            return false;
        }
        
        if (target_distance < std::abs(upper_length - lower_length) * 1.001f)
        {
            // 目标太近
            return false;
        }
        
        // 使用余弦定理计算中间关节角度
        float cos_angle = (upper_length * upper_length + target_distance * target_distance - lower_length * lower_length) /
                         (2.0f * upper_length * target_distance);
        cos_angle = std::clamp(cos_angle, -1.0f, 1.0f);
        
        float angle = std::acos(cos_angle);
        
        // 计算指向目标的方向
        Vector3f target_dir = glm::normalize(to_target);
        
        // 计算垂直于目标方向的平面，使用极向量
        Vector3f pole_dir = Normalized(pole_vector - start);
        Vector3f normal = Normalized(Cross(target_dir, pole_dir));
        Vector3f bend_dir = Normalized(Cross(normal, target_dir));
        
        // 计算中间关节位置
        out_mid_position = start + target_dir * upper_length * std::cos(angle) +
                          bend_dir * upper_length * std::sin(angle);
        
        // 末端关节就是目标位置
        out_end_position = target;
        
        return true;
    }
    
    bool SolveFABRIK(Vector3f *joints, int joint_count, const Vector3f &target,
                     float tolerance, int max_iterations)
    {
        if (joint_count < 2)
            return false;
        
        // 保存原点（根关节）
        Vector3f root = joints[0];
        
        // 计算骨骼长度
        std::vector<float> bone_lengths(joint_count - 1);
        for (int i = 0; i < joint_count - 1; i++)
        {
            bone_lengths[i] = glm::length(joints[i + 1] - joints[i]);
        }
        
        // 检查目标是否可达
        float total_length = 0.0f;
        for (float len : bone_lengths)
            total_length += len;
        
        float distance_to_target = glm::length(target - root);
        
        if (distance_to_target > total_length * 0.999f)
        {
            // 目标太远，伸直指向目标
            Vector3f direction = Normalized(target - root);
            for (int i = 1; i < joint_count; i++)
            {
                joints[i] = joints[i - 1] + direction * bone_lengths[i - 1];
            }
            return false;
        }
        
        // FABRIK 迭代
        for (int iter = 0; iter < max_iterations; iter++)
        {
            // 检查是否已经足够接近
            if (glm::length(joints[joint_count - 1] - target) < tolerance)
                return true;
            
            // 后向传递：从末端到根
            joints[joint_count - 1] = target;
            for (int i = joint_count - 2; i >= 0; i--)
            {
                Vector3f direction = Normalized(joints[i] - joints[i + 1]);
                joints[i] = joints[i + 1] + direction * bone_lengths[i];
            }
            
            // 前向传递：从根到末端
            joints[0] = root;
            for (int i = 0; i < joint_count - 1; i++)
            {
                Vector3f direction = Normalized(joints[i + 1] - joints[i]);
                joints[i + 1] = joints[i] + direction * bone_lengths[i];
            }
        }
        
        // 达到最大迭代次数
        return glm::length(joints[joint_count - 1] - target) < tolerance;
    }

}//namespace hgl::math
