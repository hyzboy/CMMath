#include<hgl/math/Quaternion.h>
#include<hgl/math/Angle.h>
#include<iostream>

using namespace hgl::math;

int main()
{
    std::cout << "四元数测试" << std::endl;

    Quatf q;
    std::cout << "单位四元数: w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << std::endl;

    Quatf rot = RotationQuat(90,Vector3f{0,1,0});
    std::cout << "旋转四元数创建完成" << std::endl;

    return 0;
}
