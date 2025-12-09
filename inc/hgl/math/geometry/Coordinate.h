#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
     * 坐标系转换函数
     * 
     * 坐标系约定:
     * - OpenGL标准: X右, Y上, Z后 (右手坐标系, Y-up, 摄像机朝-Z)
     * - Vulkan标准: X右, Y下, Z前 (右手坐标系, Y-down, 摄像机朝+Z) [NDC空间]
     * - Vulkan Z-up: X右, Y前, Z上 (右手坐标系, Z-up, 摄像机朝+Y) [世界空间自定义]
     */

    /**
     * OpenGL坐标(Y-up)到Vulkan NDC坐标(Y-down)的转换
     * OpenGL: (x,y,z) -> Vulkan NDC: (x,-y,z)
     */
    inline const math::Vector3f gl2vk(const math::Vector3f &v)
    {
        return Vector3f(v.x,-v.y,v.z);
    }

    /**
     * OpenGL坐标(Y-up, 摄像机朝-Z)到Vulkan Z-up坐标(摄像机朝+Y)的转换
     * OpenGL: X右, Y上, Z后 -> Vulkan Z-up: X右, Y前(深度), Z上
     * 映射: GL.x->VK.x, GL.y->VK.z, GL.z->VK.y
     */
    inline const math::Vector3f glYUp2vkZUp(const math::Vector3f &v)
    {
        return Vector3f(v.x,-v.z,v.y);  // 注意: GL的+Z是后方, 所以需要取负变成VK的+Y(前方)
    }

    /**
     * Vulkan Z-up坐标到OpenGL Y-up坐标的转换 (逆变换)
     * Vulkan Z-up: X右, Y前, Z上 -> OpenGL: X右, Y上, Z后
     * 映射: VK.x->GL.x, VK.y->-GL.z, VK.z->GL.y
     */
    inline const math::Vector3f vkZUp2glYUp(const math::Vector3f &v)
    {
        return Vector3f(v.x,v.z,-v.y);
    }

    // 保留旧函数名作为兼容
    [[deprecated("Use glYUp2vkZUp instead")]]
    inline const math::Vector3f glY2vkZ(const math::Vector3f &v)
    {
        return glYUp2vkZUp(v);
    }

    [[deprecated("Use vkZUp2glYUp instead")]]
    inline const math::Vector3f vkZ2vk(const math::Vector3f &v)
    {
        return Vector3f(v.x,-v.z,v.y);  // 保持原有行为
    }
}//namespace hgl::math
