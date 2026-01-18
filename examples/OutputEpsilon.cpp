#include<hgl/math/Epsilon.h>
#include<iostream>
#include<limits>

int main()
{
    std::cout << "浮点精度常数输出测试" << std::endl;
    
    std::cout << "Float epsilon: " << std::numeric_limits<float>::epsilon() << std::endl;
    std::cout << "Double epsilon: " << std::numeric_limits<double>::epsilon() << std::endl;
    
    return 0;
}
