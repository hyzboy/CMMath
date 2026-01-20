#include<hgl/math/geometry/Triangle.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "Triangle (三角形) 基础测试" << std::endl;

    Triangle2f tri(Vector2f{0,0}, Vector2f{1,0}, Vector2f{0,1});
    std::cout << "三角形创建完成，顶点:" << std::endl;
    std::cout << "  V0: (0,0)" << std::endl;
    std::cout << "  V1: (1,0)" << std::endl;
    std::cout << "  V2: (0,1)" << std::endl;

    Vector2f p{0.25f, 0.25f};
    if (tri.PointIn(p))
    {
        std::cout << "点(0.25, 0.25)在三角形内" << std::endl;
    }

    return 0;
}
