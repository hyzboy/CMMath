#include<hgl/math/Geometry.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "几何基础测试" << std::endl;

    Vector3f a{0.0f,0.0f,0.0f};
    Vector3f b{1.0f,1.0f,0.0f};

    float d = Length(a, b);
    std::cout << "两点距离:" << d << std::endl;

    return 0;
}
