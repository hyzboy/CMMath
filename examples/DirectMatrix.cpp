#include<hgl/math/Matrix.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "直接矩阵运算基础测试" << std::endl;

    Matrix4f I = Matrix4f::Identity();
    std::cout << "单位矩阵创建完成" << std::endl;

    Matrix4f T = Matrix4f::Translation(1.0f, 2.0f, 3.0f);
    std::cout << "平移矩阵: (1,2,3)" << std::endl;

    Matrix4f S = Matrix4f::Scale(2.0f, 2.0f, 2.0f);
    std::cout << "缩放矩阵: x2" << std::endl;

    Matrix4f R = Matrix4f::RotationZ(Angle::FromDegrees(90.0));
    std::cout << "Z轴旋转矩阵: 90°" << std::endl;

    Matrix4f M = I * T * S * R;
    std::cout << "综合矩阵计算完成" << std::endl;

    return 0;
}
