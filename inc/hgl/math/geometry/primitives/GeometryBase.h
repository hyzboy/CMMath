/**
 * GeometryBase.h - Base interface definitions for geometry primitives
 * 
 * This file defines the common interface that all geometry primitives should implement.
 * We use C++20 concepts for compile-time polymorphism instead of virtual functions,
 * ensuring zero runtime overhead while maintaining a consistent interface.
 * 
 * Required Methods for Geometry Types:
 * - GetCenter() -> Vector3f          : Returns the geometric center
 * - GetBoundingBox() -> AABB         : Returns axis-aligned bounding box for broad phase
 * - ContainsPoint(point) -> bool     : Tests if a point is inside the geometry
 * - ClosestPoint(point) -> Vector3f  : Finds the closest point on surface
 * - DistanceToPoint(point) -> float  : Calculates distance to surface
 * - GetVolume() -> float             : Returns volume (for solid geometries)
 * - GetSurfaceArea() -> float        : Returns surface area
 * 
 * Design Philosophy:
 * - Geometry classes remain lightweight data containers with simple queries
 * - Complex collision logic is delegated to specialized query tools
 * - Static polymorphism via templates (no virtual function overhead)
 * - Two-phase detection: broad phase (AABB) + narrow phase (precise)
 */
#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/geometry/AABB.h>

namespace hgl::math
{
#if __cplusplus >= 202002L  // C++20 concepts
    
    /**
     * Concept defining the requirements for a basic geometry primitive
     * 
     * Any type satisfying this concept can be used with the query tools
     * (CollisionDetector, RaycastQuery, etc.)
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
     * Concept for solid geometries (with volume and surface area)
     */
    template<typename T>
    concept SolidGeometry = GeometryPrimitive<T> && requires(T geom)
    {
        { geom.GetVolume() } -> std::convertible_to<float>;
        { geom.GetSurfaceArea() } -> std::convertible_to<float>;
    };
    
#else  // C++17 fallback using SFINAE
    
    /**
     * Type trait to check if a type has the basic geometry interface
     * Use: std::enable_if_t<is_geometry_primitive_v<T>>
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
