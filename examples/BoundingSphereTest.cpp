#include<hgl/math/geometry/primitives/Sphere.h>
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

static bool SphereIntersects(const Sphere &a, const Sphere &b)
{
    const Vector3f diff = b.GetCenter() - a.GetCenter();
    const float dist_sq = glm::dot(diff, diff);
    const float sum_radius = a.GetRadius() + b.GetRadius();
    return dist_sq <= sum_radius * sum_radius;
}

static Sphere MergeSpheres(const Sphere &a, const Sphere &b)
{
    const Vector3f c1 = a.GetCenter();
    const Vector3f c2 = b.GetCenter();
    const float r1 = a.GetRadius();
    const float r2 = b.GetRadius();

    const Vector3f diff = c2 - c1;
    const float dist = glm::length(diff);

    // 如果一个完全包含另一个，返回较大的那个
    if (dist <= fabs(r1 - r2))
    {
        return (r1 >= r2) ? a : b;
    }

    const float new_radius = (dist + r1 + r2) * 0.5f;
    const Vector3f dir = (dist > 1e-6f) ? diff / dist : Vector3f(1.0f, 0.0f, 0.0f);
    const Vector3f new_center = c1 + dir * (new_radius - r1);

    return Sphere(new_center, new_radius);
}

void test_BasicSphere()
{
    std::cout << "验证基本球体构造..." << std::endl;
    Sphere s;
    TEST_ASSERT(IsNearlyEqual(s.GetCenter(), Vector3f{}));
    TEST_ASSERT(IsNearlyEqual(s.GetRadius(), 1.0f));

    Sphere s2(Vector3f{1.0f,2.0f,3.0f}, 5.0f);
    TEST_ASSERT(IsNearlyEqual(s2.GetCenter(), Vector3f{1.0f,2.0f,3.0f}));
    TEST_ASSERT(IsNearlyEqual(s2.GetRadius(), 5.0f));
}

void test_ContainsPoint()
{
    std::cout << "验证点包含关系..." << std::endl;
    Sphere s(Vector3f{0.0f,0.0f,0.0f}, 2.0f);
    TEST_ASSERT(s.ContainsPoint(Vector3f{0.0f,0.0f,0.0f}));
    TEST_ASSERT(s.ContainsPoint(Vector3f{1.0f,1.0f,1.0f}));
    TEST_ASSERT(!s.ContainsPoint(Vector3f{3.0f,0.0f,0.0f}));
}

void test_IntersectSphere()
{
    std::cout << "验证球体相交..." << std::endl;
    Sphere s1(Vector3f{0.0f,0.0f,0.0f}, 2.0f);
    Sphere s2(Vector3f{3.0f,0.0f,0.0f}, 2.0f);
    TEST_ASSERT(SphereIntersects(s1, s2));
    Sphere s3(Vector3f{5.0f,0.0f,0.0f}, 1.0f);
    TEST_ASSERT(!SphereIntersects(s1, s3));
}

void test_Merge()
{
    std::cout << "验证球体合并..." << std::endl;
    Sphere s1(Vector3f{0.0f,0.0f,0.0f}, 2.0f);
    Sphere s2(Vector3f{4.0f,0.0f,0.0f}, 2.0f);
    Sphere merged = MergeSpheres(s1, s2);
    TEST_ASSERT(merged.GetRadius() >= 4.0f);
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
