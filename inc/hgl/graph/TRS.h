#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace hgl::graph
{
    // Restore TRS-based transform representation (float for real-time use)
    struct TRS
    {
        glm::vec3 translation{ 0.0f };
        glm::quat rotation{ 1.0f,0.0f,0.0f,0.0f }; // w,x,y,z
        glm::vec3 scale{ 1.0f };

    public:

        void init()
        {
            translation=glm::vec3(0.0f);
            rotation=glm::quat(1.0f,0.0f,0.0f,0.0f);
            scale=glm::vec3(1.0f);
        }

        // 判断是否为空（单位变换）
        bool empty() const
        {
            return translation==glm::vec3(0.0f)
                 &&rotation==glm::quat(1.0f,0.0f,0.0f,0.0f)
                 &&scale==glm::vec3(1.0f);
        }

        bool operator == (const TRS &rhs) const
        {
            return translation==rhs.translation
                 &&rotation==rhs.rotation
                 &&scale==rhs.scale;
        }

        bool operator != (const TRS &rhs) const
        {
            return !(*this==rhs);
        }

        // 转换为glm::mat4
        glm::mat4 toMat4() const;
    };
}//namespace hgl::graph
