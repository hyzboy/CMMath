#include<hgl/math/geometry/LineSegment.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "线段基础测试" << std::endl;

    LineSegment seg(Vector3f(0.0f,0.0f,0.0f), Vector3f(3.0f,4.0f,0.0f));
    std::cout << "长度:" << seg.Distance() << std::endl;

    return 0;
}
