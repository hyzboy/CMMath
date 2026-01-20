#include<hgl/math/Matrix.h>
#include<hgl/math/Angle.h>
#include<iostream>
#include<chrono>

using namespace hgl::math;

int main()
{
    std::cout << "Transform 性能基准测试" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    
    int iterations = 100000;
    for (int i = 0; i < iterations; ++i) {
        Matrix4f T = TranslateMatrix(1.0f + i*0.001f, 2.0f, 3.0f);
        Matrix4f R = AxisZRotate(glm::radians(i * 0.1f));
        Matrix4f result = T * R;
        (void)result;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "完成 " << iterations << " 次变换: " << duration.count() << " ms" << std::endl;

    return 0;
}
