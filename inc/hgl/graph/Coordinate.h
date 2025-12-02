#pragma once

#include<hgl/math/Math.h>

namespace hgl::graph
{
    /*
        * OpenGL Coordinate System         Vulkan Coordinate System        Our Coordinate System
        *
        *                                         /Z                            | Z
        *     Y|    /Z                           /                              |
        *      |   /                            /                               |
        *      |  /                            *------------                    *------------
        *      | /                             |           X                     \         X
        *      |/                              |                                  \        
        *      *------------                   |                                   \
        *                  X                   | Y                                  \ Y
        */

//        const Matrix4f &GetOpenGL2VulkanMatrix();
//        const Matrix4f gl2vk(const Matrix4f &ogl_matrix);               //将OpenGL坐标系矩阵变换到Vulkan坐标系

    /**
        * OpenGL坐标系到Vulkan坐标系坐标转换
        */
    inline const Vector3f &gl2vk(const Vector3f &v)
    {
        return Vector3f(v.x,-v.y,v.z);
    }

    /**
    * OpenGL坐标系Y轴向上坐标到Vulkan坐标系Z轴向上坐标转换
    */
    inline const Vector3f &glY2vkZ(const Vector3f &v)
    {
        return Vector3f(v.x,v.z,v.y);
    }

    /**
        * Vulkan Z轴向上转换Y轴向下
        */
    inline const Vector3f &vkZ2vk(const Vector3f &v)
    {
        return Vector3f(v.x,-v.z,v.y);
    }
}//namespace hgl::graph
