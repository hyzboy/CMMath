#include<hgl/math/geometry/Frustum.h>
#include<hgl/math/Projection.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "视锥体测试" << std::endl;

    Matrix4f proj = PerspectiveMatrix(60.0, 16.0f/9.0f, 0.1f, 100.0f);
    Frustum fr(proj);

    std::cout << "Frustum 创建成功" << std::endl;

    return 0;
}
