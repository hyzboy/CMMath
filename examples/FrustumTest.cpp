#include<hgl/math/Frustum.h>
#include<hgl/math/Matrix.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "视锥体测试" << std::endl;

    Matrix4f proj = Matrix4f::Perspective(Angle::FromDegrees(60.0), 16.0f/9.0f, 0.1f, 100.0f);
    Frustum fr(proj);

    std::cout << "Frustum 创建成功" << std::endl;

    return 0;
}
