#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/AABB.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "OBB (有向包围盒) 测试" << std::endl;

    AABB aabb(Vector3f{-1,-1,-1}, Vector3f{1,1,1});
    OBB obb(aabb);

    std::cout << "OBB从AABB创建完成" << std::endl;

    return 0;
}
