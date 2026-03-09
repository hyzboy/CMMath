#include<hgl/math/Quaternion.h>

namespace hgl::math
{
    Quatf DirectionToRotation(const Vector3f &dir)
    {
        const float len2 = glm::dot(dir, dir);
        const math::Vector3f forward = (len2 > 1e-8f) ? glm::normalize(dir) : math::AxisVector::Y;

        const math::Vector3f world_up = math::AxisVector::Z;
        math::Vector3f right = glm::cross(world_up, forward);
        if (glm::dot(right, right) < 1e-8f)
        {
            const math::Vector3f fallback_up = math::AxisVector::X;
            right = glm::cross(fallback_up, forward);
        }

        right = glm::normalize(right);
        const math::Vector3f up = glm::normalize(glm::cross(forward, right));

        glm::mat3 basis(1.0f);
        basis[0] = right;
        basis[1] = up;
        basis[2] = forward;
        return glm::normalize(glm::quat_cast(basis));
    }
}//namespace hgl::math

