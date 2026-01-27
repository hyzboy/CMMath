#include<hgl/math/Angle.h>
#include<iostream>
#include<iomanip>
#include<cmath>

using namespace hgl::math;
using namespace hgl::math::angle_literals;

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

bool nearly_equal(double a, double b, double epsilon = 1e-10) {
    return std::abs(a - b) < epsilon;
}

void test_BasicConstruction()
{
    std::cout << "验证角度的基本构造和类型转换..." << std::endl;
    Angle a1;
    TEST_ASSERT(a1.AsRadians() == 0.0);
    TEST_ASSERT(a1.AsDegrees() == 0.0);
    Angle a2{Radians(std::numbers::pi_v<float>)};
    TEST_ASSERT(nearly_equal(a2.AsRadians(), std::numbers::pi_v<float>));
    TEST_ASSERT(nearly_equal(a2.AsDegrees(), 180.0));
    Angle a3{Degrees(90.0)};
    TEST_ASSERT(nearly_equal(a3.AsRadians(), std::numbers::pi_v<float> / 2.0));
    TEST_ASSERT(nearly_equal(a3.AsDegrees(), 90.0));
    std::cout << "  π rad = " << a2.AsDegrees() << "°" << std::endl;
    std::cout << "  90° = " << a3.AsRadians() << " rad" << std::endl;
}

void test_StaticFactoryMethods()
{
    std::cout << "验证静态工厂方法..." << std::endl;
    Angle from_rad = Angle::FromRadians(std::numbers::pi_v<float> / 4.0);
    TEST_ASSERT(nearly_equal(from_rad.AsDegrees(), 45.0));
    Angle from_deg = Angle::FromDegrees(180.0);
    TEST_ASSERT(nearly_equal(from_deg.AsRadians(), std::numbers::pi_v<float>));
}

void test_Addition()
{
    std::cout << "验证角度加法..." << std::endl;
    Angle a1 = Angle::FromDegrees(30.0);
    Angle a2 = Angle::FromDegrees(60.0);
    Angle sum = a1 + a2;
    TEST_ASSERT(nearly_equal(sum.AsDegrees(), 90.0));
    Angle a3 = Angle::FromDegrees(45.0);
    a3 += Angle::FromDegrees(45.0);
    TEST_ASSERT(nearly_equal(a3.AsDegrees(), 90.0));
}

void test_Subtraction()
{
    std::cout << "验证角度减法..." << std::endl;
    Angle a1 = Angle::FromDegrees(90.0);
    Angle a2 = Angle::FromDegrees(30.0);
    Angle diff = a1 - a2;
    TEST_ASSERT(nearly_equal(diff.AsDegrees(), 60.0));
    Angle a3 = Angle::FromDegrees(180.0);
    a3 -= Angle::FromDegrees(90.0);
    TEST_ASSERT(nearly_equal(a3.AsDegrees(), 90.0));
}

void test_ScalarMultiplication()
{
    std::cout << "验证角度标量乘法..." << std::endl;
    Angle a1 = Angle::FromDegrees(30.0);
    Angle doubled = a1 * 2.0;
    Angle tripled = 3.0 * a1;
    TEST_ASSERT(nearly_equal(doubled.AsDegrees(), 60.0));
    TEST_ASSERT(nearly_equal(tripled.AsDegrees(), 90.0));
    Angle a2 = Angle::FromDegrees(45.0);
    a2 *= 2.0;
    TEST_ASSERT(nearly_equal(a2.AsDegrees(), 90.0));
}

void test_ScalarDivision()
{
    std::cout << "验证角度标量除法..." << std::endl;
    Angle a1 = Angle::FromDegrees(180.0);
    Angle halved = a1 / 2.0;
    TEST_ASSERT(nearly_equal(halved.AsDegrees(), 90.0));
    Angle a2 = Angle::FromDegrees(360.0);
    a2 /= 4.0;
    TEST_ASSERT(nearly_equal(a2.AsDegrees(), 90.0));
}

void test_ComparisonOperators()
{
    std::cout << "验证角度比较运算..." << std::endl;
    Angle a1 = Angle::FromDegrees(30.0);
    Angle a2 = Angle::FromDegrees(30.0);
    Angle a3 = Angle::FromDegrees(60.0);
    TEST_ASSERT(a1 == a2);
    TEST_ASSERT(a1 != a3);
    TEST_ASSERT(a1 < a3);
    TEST_ASSERT(a3 > a1);
    TEST_ASSERT(a1 <= a2);
    TEST_ASSERT(a1 <= a3);
    TEST_ASSERT(a3 >= a1);
    TEST_ASSERT(a2 >= a1);
}

void test_Literals()
{
    std::cout << "验证用户定义字面量..." << std::endl;
    auto angle_rad1 = 3.14159_rad;
    TEST_ASSERT(nearly_equal(angle_rad1.AsRadians(), 3.14159, 1e-5));
    TEST_ASSERT(nearly_equal(angle_rad1.AsDegrees(), 180.0, 0.01));
    auto angle_rad2 = 1_rad;
    TEST_ASSERT(nearly_equal(angle_rad2.AsRadians(), 1.0));
    auto angle_deg1 = 90.0_deg;
    TEST_ASSERT(nearly_equal(angle_deg1.AsDegrees(), 90.0));
    TEST_ASSERT(nearly_equal(angle_deg1.AsRadians(), std::numbers::pi_v<float> / 2.0));
}

void test_CommonAngles()
{
    std::cout << "验证常用角度值的精度..." << std::endl;
    struct TestCase { double degrees; double radians; const char* name; };
    TestCase cases[] = {
        {0.0,   0.0,              "0°"},
        {30.0,  std::numbers::pi_v<float> / 6.0,     "30°"},
        {45.0,  std::numbers::pi_v<float> / 4.0,     "45°"},
        {60.0,  std::numbers::pi_v<float> / 3.0,     "60°"},
        {90.0,  std::numbers::pi_v<float> / 2.0,     "90°"},
        {120.0, 2.0 * std::numbers::pi_v<float> / 3.0, "120°"},
        {135.0, 3.0 * std::numbers::pi_v<float> / 4.0, "135°"},
        {180.0, std::numbers::pi_v<float>,           "180°"},
        {270.0, 3.0 * std::numbers::pi_v<float> / 2.0, "270°"},
        {360.0, 2.0 * std::numbers::pi_v<float>,     "360°"}
    };
    for (const auto& tc : cases) {
        Angle from_deg = Angle::FromDegrees(tc.degrees);
        Angle from_rad = Angle::FromRadians(tc.radians);
        TEST_ASSERT(nearly_equal(from_deg.AsRadians(), tc.radians));
        TEST_ASSERT(nearly_equal(from_rad.AsDegrees(), tc.degrees));
    }
}

void test_NegativeAngles()
{
    std::cout << "验证负角度..." << std::endl;
    Angle neg_deg = Angle::FromDegrees(-90.0);
    TEST_ASSERT(nearly_equal(neg_deg.AsDegrees(), -90.0));
    TEST_ASSERT(nearly_equal(neg_deg.AsRadians(), -std::numbers::pi_v<float> / 2.0));
    Angle neg_rad = Angle::FromRadians(-std::numbers::pi_v<float>);
    TEST_ASSERT(nearly_equal(neg_rad.AsRadians(), -std::numbers::pi_v<float>));
    TEST_ASSERT(nearly_equal(neg_rad.AsDegrees(), -180.0));
    Angle a1 = Angle::FromDegrees(30.0);
    Angle a2 = Angle::FromDegrees(-30.0);
    Angle sum = a1 + a2;
    TEST_ASSERT(nearly_equal(sum.AsDegrees(), 0.0));
}

void test_LargeAngles()
{
    std::cout << "验证大角度（超过360°）..." << std::endl;
    Angle large1 = Angle::FromDegrees(720.0);
    TEST_ASSERT(nearly_equal(large1.AsDegrees(), 720.0));
    TEST_ASSERT(nearly_equal(large1.AsRadians(), 4.0 * std::numbers::pi_v<float>));
    Angle large2 = Angle::FromDegrees(450.0);
    TEST_ASSERT(nearly_equal(large2.AsDegrees(), 450.0));
    Angle a1 = Angle::FromDegrees(270.0);
    Angle a2 = Angle::FromDegrees(180.0);
    Angle sum = a1 + a2;
    TEST_ASSERT(nearly_equal(sum.AsDegrees(), 450.0));
}

void test_MixedOperations()
{
    std::cout << "验证混合运算..." << std::endl;
    auto angle1 = 45.0_deg;
    auto angle2 = Angle::FromRadians(std::numbers::pi_v<float> / 4.0);
    auto sum = angle1 + angle2;
    TEST_ASSERT(nearly_equal(sum.AsDegrees(), 90.0));
    auto result = (90.0_deg + 45.0_deg) * 2.0 - 180.0_deg;
    TEST_ASSERT(nearly_equal(result.AsDegrees(), 90.0));
    auto chained = Angle::FromDegrees(0.0);
    chained += 30.0_deg;
    chained += 30.0_deg;
    chained += 30.0_deg;
    TEST_ASSERT(nearly_equal(chained.AsDegrees(), 90.0));
}

void test_ConstexprSupport()
{
    std::cout << "验证 constexpr 编译期计算..." << std::endl;
    constexpr Angle right_angle = Angle::FromDegrees(90.0);
    constexpr double rad_value = right_angle.AsRadians();
    constexpr double deg_value = right_angle.AsDegrees();
    TEST_ASSERT(nearly_equal(deg_value, 90.0));
    TEST_ASSERT(nearly_equal(rad_value, std::numbers::pi_v<float> / 2.0));
    constexpr Angle sum = right_angle + right_angle;
    constexpr double sum_deg = sum.AsDegrees();
    TEST_ASSERT(nearly_equal(sum_deg, 180.0));
}

void test_Precision()
{
    std::cout << "验证转换精度..." << std::endl;
    double original_deg = 123.456789;
    Angle a = Angle::FromDegrees(original_deg);
    double converted_deg = a.AsDegrees();
    TEST_ASSERT(nearly_equal(original_deg, converted_deg, 1e-9));
    double original_rad = 2.1234567890;
    Angle b = Angle::FromRadians(original_rad);
    double converted_rad = b.AsRadians();
    TEST_ASSERT(nearly_equal(original_rad, converted_rad, 1e-12));
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Angle 类型安全角度测试" << std::endl;
    std::cout << "========================================" << std::endl;
    RUN_TEST(test_BasicConstruction);
    RUN_TEST(test_StaticFactoryMethods);
    RUN_TEST(test_Addition);
    RUN_TEST(test_Subtraction);
    RUN_TEST(test_ScalarMultiplication);
    RUN_TEST(test_ScalarDivision);
    RUN_TEST(test_ComparisonOperators);
    RUN_TEST(test_Literals);
    RUN_TEST(test_CommonAngles);
    RUN_TEST(test_NegativeAngles);
    RUN_TEST(test_LargeAngles);
    RUN_TEST(test_MixedOperations);
    RUN_TEST(test_ConstexprSupport);
    RUN_TEST(test_Precision);
    std::cout << "\n========================================" << std::endl;
    std::cout << "测试总结" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "总计测试: " << test_total << std::endl;
    std::cout << "通过: " << test_passed << " ✓" << std::endl;
    std::cout << "失败: " << test_failed << " ✗" << std::endl;
    return test_failed > 0 ? 1 : 0;
}
