#include<hgl/math/geometry/Triangle.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "Triangle (三角形) 基础测试" << std::endl;

    Triangle3f tri(Vector3f{0,0,0}, Vector3f{1,0,0}, Vector3f{0,1,0});
    std::cout << "三角形创建完成，顶点:" << std::endl;
    std::cout << "  V0: (0,0,0)" << std::endl;
    std::cout << "  V1: (1,0,0)" << std::endl;
    std::cout << "  V2: (0,1,0)" << std::endl;

    Vector3f p{0.25f, 0.25f, 0.0f};
    if (tri.Contains(p)) {
        std::cout << "点(0.25, 0.25, 0)在三角形内" << std::endl;
    }

    return 0;
}
