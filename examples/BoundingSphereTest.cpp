#include<hgl/math/Sphere.h>
#include<hgl/math/Vector.h>
#include<iostream>
#include<iomanip>
#include<cmath>

using namespace hgl::math;

#define TEST_ASSERT(condition) \
    if (!(condition)) { \
        std::cout << "  ✗ 断言失败: " #condition << " (行 " << __LINE__ << ")" << std::endl; \
        ++test_failed; \
    } else { \
        ++test_passed; \
    }

#define RUN_TEST(test_func) \
    std::cout << "\n测试: " #test_func << std::endl; \
    test_func(); \
    ++test_total;

int test_passed = 0;
int test_failed = 0;
int test_total = 0;

void test_BasicSphere()
{
    std::cout << "验证基本球体构造..." << std::endl;
    Sphere s;
    TEST_ASSERT(s.center == Vector3f{});
    TEST_ASSERT(s.radius == 0.0f);

    Sphere s2(Vector3f{1.0f,2.0f,3.0f}, 5.0f);
    TEST_ASSERT(s2.center == Vector3f{1.0f,2.0f,3.0f});
    TEST_ASSERT(s2.radius == 5.0f);
}

void test_ContainsPoint()
{
    std::cout << "验证点包含关系..." << std::endl;
    Sphere s(Vector3f{0.0f,0.0f,0.0f}, 2.0f);
    TEST_ASSERT(s.Contains(Vector3f{0.0f,0.0f,0.0f}));
    TEST_ASSERT(s.Contains(Vector3f{1.0f,1.0f,1.0f}));
    TEST_ASSERT(!s.Contains(Vector3f{3.0f,0.0f,0.0f}));
}

void test_IntersectSphere()
{
    std::cout << "验证球体相交..." << std::endl;
    Sphere s1(Vector3f{0.0f,0.0f,0.0f}, 2.0f);
    Sphere s2(Vector3f{3.0f,0.0f,0.0f}, 2.0f);
    TEST_ASSERT(s1.Intersects(s2));
    Sphere s3(Vector3f{5.0f,0.0f,0.0f}, 1.0f);
    TEST_ASSERT(!s1.Intersects(s3));
}

void test_Merge()
{
    std::cout << "验证球体合并..." << std::endl;
    Sphere s1(Vector3f{0.0f,0.0f,0.0f}, 2.0f);
    Sphere s2(Vector3f{4.0f,0.0f,0.0f}, 2.0f);
    Sphere merged = Sphere::Merge(s1, s2);
    TEST_ASSERT(merged.radius >= 4.0f);
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "BoundingSphere 测试" << std::endl;
    std::cout << "========================================" << std::endl;
    RUN_TEST(test_BasicSphere);
    RUN_TEST(test_ContainsPoint);
    RUN_TEST(test_IntersectSphere);
    RUN_TEST(test_Merge);
    std::cout << "\n总计测试: " << test_total << ", 通过: " << test_passed << ", 失败: " << test_failed << std::endl;
    return test_failed > 0 ? 1 : 0;
}
