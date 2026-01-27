#include<hgl/math/geometry/primitives/Sphere.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "Sphere (球体) 测试" << std::endl;

    Sphere sphere(Vector3f{0,0,0}, 5.0f);
    std::cout << "球体创建: 中心(0,0,0), 半径5" << std::endl;

    Vector3f test_point{3, 0, 0};
    if (sphere.ContainsPoint(test_point)) {
        std::cout << "点(3,0,0)在球内" << std::endl;
    }

    return 0;
}
