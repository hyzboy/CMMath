#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/Ray.h>
#include<hgl/math/geometry/Triangle.h>
#include<hgl/math/geometry/Plane.h>
#include<hgl/math/VectorOperations.h>
#include<glm/glm.hpp>
#include<glm/ext/matrix_transform.hpp>
#include<iostream>
#include<cassert>
#include<cmath>
#include<vector>

using namespace hgl::math;

// ============================================================================
// 测试辅助宏和函数
// ============================================================================

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "  FAILED: " << message << " at line " << __LINE__ << std::endl; \
            return false; \
        } \
    } while(0)

#define RUN_TEST(test_func) \
    do { \
        std::cout << "\nRunning " << #test_func << "..." << std::endl; \
        if (test_func()) { \
            std::cout << "  ✓ PASSED" << std::endl; \
            passed++; \
        } else { \
            std::cout << "  ✗ FAILED" << std::endl; \
            failed++; \
        } \
        total++; \
    } while(0)

void printVector3f(const char* name, const Vector3f& v) {
    std::cout << "    " << name << ": (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
}

void printAABB(const char* name, const AABB& box) {
    std::cout << "    " << name << ":" << std::endl;
    printVector3f("  Min", box.GetMin());
    printVector3f("  Max", box.GetMax());
    printVector3f("  Center", box.GetCenter());
    printVector3f("  Length", box.GetLength());
}

// ============================================================================
// 基本构造和设置测试
// ============================================================================

bool test_AABB_DefaultConstruction() {
    std::cout << "  测试默认构造函数..." << std::endl;
    
    AABB box;
    TEST_ASSERT(IsNearlyEqual(box.GetMin(), Vector3f(0,0,0)), "默认AABB最小点应为(0,0,0)");
    TEST_ASSERT(IsNearlyEqual(box.GetMax(), Vector3f(1,1,1)), "默认AABB最大点应为(1,1,1)");
    TEST_ASSERT(IsNearlyEqual(box.GetCenter(), Vector3f(0.5f,0.5f,0.5f)), "默认AABB中心应为(0.5,0.5,0.5)");
    TEST_ASSERT(IsNearlyEqual(box.GetLength(), Vector3f(1,1,1)), "默认AABB长度应为(1,1,1)");
    
    return true;
}

bool test_AABB_SetMinMax() {
    std::cout << "  测试SetMinMax方法..." << std::endl;
    
    AABB box;
    Vector3f min(-1, -2, -3);
    Vector3f max(4, 5, 6);
    box.SetMinMax(min, max);
    
    TEST_ASSERT(IsNearlyEqual(box.GetMin(), min), "AABB最小点设置错误");
    TEST_ASSERT(IsNearlyEqual(box.GetMax(), max), "AABB最大点设置错误");
    TEST_ASSERT(IsNearlyEqual(box.GetCenter(), Vector3f(1.5f, 1.5f, 1.5f)), "AABB中心计算错误");
    TEST_ASSERT(IsNearlyEqual(box.GetLength(), Vector3f(5, 7, 9)), "AABB长度计算错误");
    
    return true;
}

bool test_AABB_SetCornerLength() {
    std::cout << "  测试SetCornerLength方法..." << std::endl;
    
    AABB box;
    Vector3f corner(1, 2, 3);
    Vector3f length(4, 5, 6);
    box.SetCornerLength(corner, length);
    
    TEST_ASSERT(IsNearlyEqual(box.GetMin(), corner), "AABB顶角设置错误");
    TEST_ASSERT(IsNearlyEqual(box.GetMax(), Vector3f(5, 7, 9)), "AABB从顶角+长度计算最大点错误");
    TEST_ASSERT(IsNearlyEqual(box.GetLength(), length), "AABB长度设置错误");
    
    return true;
}

bool test_AABB_SetFromPoints() {
    std::cout << "  测试SetFromPoints方法..." << std::endl;
    
    float points[] = {
        -1.0f, -2.0f, -3.0f,
        4.0f, 5.0f, 6.0f,
        2.0f, 1.0f, 0.0f,
        -2.0f, 3.0f, 4.0f
    };
    
    AABB box;
    box.SetFromPoints(points, 4, 3);
    
    TEST_ASSERT(IsNearlyEqual(box.GetMin(), Vector3f(-2, -2, -3)), "从点集计算的最小点错误");
    TEST_ASSERT(IsNearlyEqual(box.GetMax(), Vector3f(4, 5, 6)), "从点集计算的最大点错误");
    
    return true;
}

bool test_AABB_Clear() {
    std::cout << "  测试Clear方法..." << std::endl;
    
    AABB box;
    box.SetMinMax(Vector3f(-10, -10, -10), Vector3f(10, 10, 10));
    box.Clear();
    
    TEST_ASSERT(IsNearlyEqual(box.GetMin(), Vector3f(0,0,0)), "Clear后最小点应为零向量");
    TEST_ASSERT(IsNearlyEqual(box.GetMax(), Vector3f(0,0,0)), "Clear后最大点应为零向量");
    TEST_ASSERT(box.IsEmpty(), "Clear后AABB应为空");
    
    return true;
}

bool test_AABB_IsEmpty() {
    std::cout << "  测试IsEmpty方法..." << std::endl;
    
    AABB empty_box;
    empty_box.Clear();
    TEST_ASSERT(empty_box.IsEmpty(), "空AABB应返回true");
    
    AABB non_empty_box;
    non_empty_box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    TEST_ASSERT(!non_empty_box.IsEmpty(), "非空AABB应返回false");
    
    return true;
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "AABB (轴对齐包围盒) 基础测试" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    int total = 0;
    int passed = 0;
    int failed = 0;
    
    RUN_TEST(test_AABB_DefaultConstruction);
    RUN_TEST(test_AABB_SetMinMax);
    RUN_TEST(test_AABB_SetCornerLength);
    RUN_TEST(test_AABB_SetFromPoints);
    RUN_TEST(test_AABB_Clear);
    RUN_TEST(test_AABB_IsEmpty);
    
    std::cout << "\n总计测试: " << total << ", 通过: " << passed << ", 失败: " << failed << std::endl;
    return (failed == 0) ? 0 : 1;
}
