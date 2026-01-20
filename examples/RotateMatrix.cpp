#include<hgl/math/Matrix.h>
#include<hgl/math/Angle.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "旋转矩阵测试" << std::endl;

    float rad = glm::radians(90.0f);

    Matrix4f rx = AxisXRotate(rad);
    std::cout << "X轴旋转矩阵创建: 90°" << std::endl;

    Matrix4f ry = AxisYRotate(rad);
    std::cout << "Y轴旋转矩阵创建: 90°" << std::endl;

    Matrix4f rz = AxisZRotate(rad);
    std::cout << "Z轴旋转矩阵创建: 90°" << std::endl;

    return 0;
}
