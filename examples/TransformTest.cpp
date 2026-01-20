#include<hgl/math/Matrix.h>
#include<hgl/math/Vector.h>
#include<hgl/math/Angle.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "Transform 基础测试" << std::endl;

    Matrix4f translate = TranslateMatrix(1.0f, 2.0f, 3.0f);
    std::cout << "平移矩阵创建完成" << std::endl;

    Matrix4f rotate = AxisZRotate(deg2rad(45.0f));
    std::cout << "旋转矩阵创建完成" << std::endl;

    Matrix4f scale = ScaleMatrix(2.0f, 2.0f, 2.0f);
    std::cout << "缩放矩阵创建完成" << std::endl;

    Matrix4f combined = translate * rotate * scale;
    std::cout << "综合变换矩阵创建完成" << std::endl;

    Vector4f v{1, 0, 0, 1};
    Vector4f transformed = combined * v;
    std::cout << "向量变换完成" << std::endl;

    return 0;
}
