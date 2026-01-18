#include<hgl/math/Matrix.h>
#include<hgl/math/Angle.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "旋转矩阵测试" << std::endl;

    Angle angle = Angle::FromDegrees(90);

    Matrix4f rx = Matrix4f::RotationX(angle);
    std::cout << "X轴旋转矩阵创建: 90°" << std::endl;

    Matrix4f ry = Matrix4f::RotationY(angle);
    std::cout << "Y轴旋转矩阵创建: 90°" << std::endl;

    Matrix4f rz = Matrix4f::RotationZ(angle);
    std::cout << "Z轴旋转矩阵创建: 90°" << std::endl;

    return 0;
}
