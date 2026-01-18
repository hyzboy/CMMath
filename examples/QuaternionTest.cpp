#include<hgl/math/Quaternion.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "四元数测试" << std::endl;

    Quaternion q;
    std::cout << "单位四元数: w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;

    Quaternion rot = Quaternion::FromAxisAngle(Vector3f{0,1,0}, Angle::FromDegrees(90));
    std::cout << "旋转四元数创建完成" << std::endl;

    return 0;
}
