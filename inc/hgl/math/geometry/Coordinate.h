#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    /**
        * OpenGL坐标系到Vulkan坐标系坐标转换
        */
    inline const math::Vector3f &gl2vk(const math::Vector3f &v)
    {
        return Vector3f(v.x,-v.y,v.z);
    }

    /**
    * OpenGL坐标系Y轴向上坐标到Vulkan坐标系Z轴向上坐标转换
    */
    inline const math::Vector3f &glY2vkZ(const math::Vector3f &v)
    {
        return Vector3f(v.x,v.z,v.y);
    }

    /**
        * Vulkan Z轴向上转换Y轴向下
        */
    inline const math::Vector3f &vkZ2vk(const math::Vector3f &v)
    {
        return Vector3f(v.x,-v.z,v.y);
    }
}//namespace hgl::math
