#include<hgl/math/AABB.h>
#include<hgl/math/OBB.h>
#include<hgl/math/Sphere.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "BoundingVolumes 混合体基础测试" << std::endl;

    AABB aabb(Vector3f{-1.0f,-1.0f,-1.0f}, Vector3f{1.0f,1.0f,1.0f});
    OBB obb(Vector3f{0.0f,0.0f,0.0f}, Vector3f{1.0f,2.0f,3.0f});
    Sphere sphere(Vector3f{0.0f,0.0f,0.0f}, 2.0f);

    std::cout << "AABB min:" << aabb.min.x << "," << aabb.min.y << "," << aabb.min.z << std::endl;
    std::cout << "AABB max:" << aabb.max.x << "," << aabb.max.y << "," << aabb.max.z << std::endl;

    std::cout << "OBB center:" << obb.center.x << "," << obb.center.y << "," << obb.center.z << std::endl;
    std::cout << "OBB half extents:" << obb.halfExtents.x << "," << obb.halfExtents.y << "," << obb.halfExtents.z << std::endl;

    std::cout << "Sphere center:" << sphere.center.x << "," << sphere.center.y << "," << sphere.center.z << std::endl;
    std::cout << "Sphere radius:" << sphere.radius << std::endl;

    return 0;
}
