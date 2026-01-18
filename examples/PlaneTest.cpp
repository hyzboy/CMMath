#include<hgl/math/Plane.h>
#include<hgl/math/Vector.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "平面测试" << std::endl;

    Plane plane{Vector3f{0,1,0}, 0.0f};
    std::cout << "平面创建: 法线(0,1,0), 距离=0" << std::endl;

    Vector3f p{0.0f, 2.0f, 0.0f};
    float d = plane.Distance(p);
    std::cout << "点到平面距离: " << d << std::endl;

    return 0;
}
