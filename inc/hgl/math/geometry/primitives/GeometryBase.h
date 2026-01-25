/**
 * GeometryBase.h - 几何图元基础接口定义
 *
 * 本文件定义了所有几何图元应实现的通用接口。
 * 我们采用 C++20 concepts 进行编译期多态，而非虚函数，
 * 以保证零运行时开销并保持一致接口。
 *
 * 各类几何体需实现的方法：
 * - GetCenter() -> Vector3f          ：返回几何中心
 * - GetBoundingBox() -> AABB         ：返回用于粗略检测的轴对齐包围盒
 * - ContainsPoint(point) -> bool     ：判断点是否在几何体内
 * - ClosestPoint(point) -> Vector3f  ：求表面最近点
 * - DistanceToPoint(point) -> float  ：计算到表面距离
 * - GetVolume() -> float             ：返回体积（实体几何体）
 * - GetSurfaceArea() -> float        ：返回表面积
 *
 * 设计理念：
 * - 各几何类保持轻量，仅做简单查询
 * - 复杂碰撞逻辑交由专用工具处理
 * - 模板静态多态（无虚函数开销）
 * - 两阶段检测：粗略（AABB）+精确
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>

namespace hgl::math
{
#if __cplusplus >= 202002L  // C++20 concepts

    /**
     * 定义基础几何图元所需满足的概念
     *
     * 满足该概念的类型可用于各种查询工具
     * （如 CollisionDetector、RaycastQuery 等）
     */
    template<typename T>
    concept GeometryPrimitive = requires(T geom, const Vector3f& point)
    {
        { geom.GetCenter() } -> std::convertible_to<Vector3f>;
        { geom.GetBoundingBox() } -> std::convertible_to<AABB>;
        { geom.ContainsPoint(point) } -> std::convertible_to<bool>;
        { geom.ClosestPoint(point) } -> std::convertible_to<Vector3f>;
        { geom.DistanceToPoint(point) } -> std::convertible_to<float>;
    };

    /**
     * 实体几何体（具有体积和表面积）的概念
     */
    template<typename T>
    concept SolidGeometry = GeometryPrimitive<T> && requires(T geom)
    {
        { geom.GetVolume() } -> std::convertible_to<float>;
        { geom.GetSurfaceArea() } -> std::convertible_to<float>;
    };

#else  // C++17 fallback using SFINAE

    /**
     * 类型萃取：检查类型是否具备基础几何接口
     * 用法：std::enable_if_t<is_geometry_primitive_v<T>>
     */
    template<typename T, typename = void>
    struct is_geometry_primitive : std::false_type {};

    template<typename T>
    struct is_geometry_primitive<T, std::void_t<
        decltype(std::declval<T>().GetCenter()),
        decltype(std::declval<T>().GetBoundingBox()),
        decltype(std::declval<T>().ContainsPoint(std::declval<Vector3f>())),
        decltype(std::declval<T>().ClosestPoint(std::declval<Vector3f>())),
        decltype(std::declval<T>().DistanceToPoint(std::declval<Vector3f>()))
    >> : std::true_type {};

    template<typename T>
    inline constexpr bool is_geometry_primitive_v = is_geometry_primitive<T>::value;

#endif

}//namespace hgl::math
