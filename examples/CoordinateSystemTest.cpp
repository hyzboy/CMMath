#include<hgl/math/Vector.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "坐标系与向量基本测试" << std::endl;

    Vector3f x_axis{1.0f, 0.0f, 0.0f};
    Vector3f y_axis{0.0f, 1.0f, 0.0f};
    Vector3f z_axis{0.0f, 0.0f, 1.0f};

    std::cout << "X轴:" << x_axis.x << "," << x_axis.y << "," << x_axis.z << std::endl;
    std::cout << "Y轴:" << y_axis.x << "," << y_axis.y << "," << y_axis.z << std::endl;
    std::cout << "Z轴:" << z_axis.x << "," << z_axis.y << "," << z_axis.z << std::endl;

    Vector3f v{3.0f, 4.0f, 0.0f};
    float length = v.Length();
    std::cout << "向量长度:" << length << std::endl;

    Vector3f normalized = v.Normalized();
    std::cout << "归一化:" << normalized.x << "," << normalized.y << "," << normalized.z << std::endl;

    return 0;
}
