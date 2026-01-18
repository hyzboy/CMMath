#include<hgl/math/LineSegment.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "线段基础测试" << std::endl;

    LineSegment2f seg({0.0f,0.0f}, {3.0f,4.0f});
    std::cout << "长度:" << seg.Length() << std::endl;

    return 0;
}
