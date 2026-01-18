#include<hgl/math/geometry/Ray.h>
#include<hgl/math/geometry/Plane.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "Ray (射线) 测试" << std::endl;

    Ray ray(Vector3f{0,0,0}, Vector3f{1,0,0});
    std::cout << "射线从(0,0,0)出发，方向(1,0,0)" << std::endl;

    Plane plane(Vector3f{0,1,0}, 0.0f);
    std::cout << "平面法线(0,1,0)，经过原点" << std::endl;

    return 0;
}
