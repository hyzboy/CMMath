#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "BoundingVolumes 混合体基础测试" << std::endl;

    AABB aabb(Vector3f{-1.0f,-1.0f,-1.0f}, Vector3f{1.0f,1.0f,1.0f});
    OBB obb(Vector3f{0.0f,0.0f,0.0f}, Vector3f{1.0f,2.0f,3.0f});
    Sphere sphere(Vector3f{0.0f,0.0f,0.0f}, 2.0f);

    const Vector3f &aabb_min = aabb.GetMin();
    const Vector3f &aabb_max = aabb.GetMax();

    std::cout << "AABB min:" << aabb_min.x << "," << aabb_min.y << "," << aabb_min.z << std::endl;
    std::cout << "AABB max:" << aabb_max.x << "," << aabb_max.y << "," << aabb_max.z << std::endl;

    const Vector3f &obb_center = obb.GetCenter();
    const Vector3f &obb_half_extents = obb.GetHalfExtend();

    std::cout << "OBB center:" << obb_center.x << "," << obb_center.y << "," << obb_center.z << std::endl;
    std::cout << "OBB half extents:" << obb_half_extents.x << "," << obb_half_extents.y << "," << obb_half_extents.z << std::endl;

    const Vector3f &sphere_center = sphere.GetCenter();
    float sphere_radius = sphere.GetRadius();

    std::cout << "Sphere center:" << sphere_center.x << "," << sphere_center.y << "," << sphere_center.z << std::endl;
    std::cout << "Sphere radius:" << sphere_radius << std::endl;

    return 0;
}
