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
    
    std::cout << "  默认构造的AABB:" << std::endl;
    printVector3f("    Min", box.GetMin());
    printVector3f("    Max", box.GetMax());
    printVector3f("    Center", box.GetCenter());
    printVector3f("    Length", box.GetLength());
    
    if (!IsNearlyEqual(box.GetMin(), Vector3f(0,0,0))) {
        std::cerr << "  FAILED: 默认AABB最小点应为(0,0,0) at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (0, 0, 0)" << std::endl;
        std::cerr << "    实际: (" << box.GetMin().x << ", " << box.GetMin().y << ", " << box.GetMin().z << ")" << std::endl;
        return false;
    }
    
    if (!IsNearlyEqual(box.GetMax(), Vector3f(1,1,1))) {
        std::cerr << "  FAILED: 默认AABB最大点应为(1,1,1) at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (1, 1, 1)" << std::endl;
        std::cerr << "    实际: (" << box.GetMax().x << ", " << box.GetMax().y << ", " << box.GetMax().z << ")" << std::endl;
        return false;
    }
    
    if (!IsNearlyEqual(box.GetCenter(), Vector3f(0.5f,0.5f,0.5f))) {
        std::cerr << "  FAILED: 默认AABB中心应为(0.5,0.5,0.5) at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (0.5, 0.5, 0.5)" << std::endl;
        std::cerr << "    实际: (" << box.GetCenter().x << ", " << box.GetCenter().y << ", " << box.GetCenter().z << ")" << std::endl;
        return false;
    }
    
    if (!IsNearlyEqual(box.GetLength(), Vector3f(1,1,1))) {
        std::cerr << "  FAILED: 默认AABB长度应为(1,1,1) at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (1, 1, 1)" << std::endl;
        std::cerr << "    实际: (" << box.GetLength().x << ", " << box.GetLength().y << ", " << box.GetLength().z << ")" << std::endl;
        return false;
    }
    
    return true;
}

bool test_AABB_SetMinMax() {
    std::cout << "  测试SetMinMax方法..." << std::endl;
    
    AABB box;
    Vector3f min(-1, -2, -3);
    Vector3f max(4, 5, 6);
    box.SetMinMax(min, max);
    
    std::cout << "  设置后的AABB:" << std::endl;
    printVector3f("    Min", box.GetMin());
    printVector3f("    Max", box.GetMax());
    printVector3f("    Center", box.GetCenter());
    printVector3f("    Length", box.GetLength());
    
    if (!IsNearlyEqual(box.GetMin(), min)) {
        std::cerr << "  FAILED: AABB最小点设置错误 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (" << min.x << ", " << min.y << ", " << min.z << ")" << std::endl;
        std::cerr << "    实际: (" << box.GetMin().x << ", " << box.GetMin().y << ", " << box.GetMin().z << ")" << std::endl;
        return false;
    }
    
    if (!IsNearlyEqual(box.GetMax(), max)) {
        std::cerr << "  FAILED: AABB最大点设置错误 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (" << max.x << ", " << max.y << ", " << max.z << ")" << std::endl;
        std::cerr << "    实际: (" << box.GetMax().x << ", " << box.GetMax().y << ", " << box.GetMax().z << ")" << std::endl;
        return false;
    }
    
    Vector3f expectedCenter(1.5f, 1.5f, 1.5f);
    if (!IsNearlyEqual(box.GetCenter(), expectedCenter)) {
        std::cerr << "  FAILED: AABB中心计算错误 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (" << expectedCenter.x << ", " << expectedCenter.y << ", " << expectedCenter.z << ")" << std::endl;
        std::cerr << "    实际: (" << box.GetCenter().x << ", " << box.GetCenter().y << ", " << box.GetCenter().z << ")" << std::endl;
        return false;
    }
    
    Vector3f expectedLength(5, 7, 9);
    if (!IsNearlyEqual(box.GetLength(), expectedLength)) {
        std::cerr << "  FAILED: AABB长度计算错误 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (" << expectedLength.x << ", " << expectedLength.y << ", " << expectedLength.z << ")" << std::endl;
        std::cerr << "    实际: (" << box.GetLength().x << ", " << box.GetLength().y << ", " << box.GetLength().z << ")" << std::endl;
        return false;
    }
    
    return true;
}

bool test_AABB_SetCornerLength() {
    std::cout << "  测试SetCornerLength方法..." << std::endl;
    
    AABB box;
    Vector3f corner(1, 2, 3);
    Vector3f length(4, 5, 6);
    box.SetCornerLength(corner, length);
    
    std::cout << "  设置后的AABB:" << std::endl;
    printVector3f("    Corner", corner);
    printVector3f("    Length", length);
    printVector3f("    Min", box.GetMin());
    printVector3f("    Max", box.GetMax());
    
    if (!IsNearlyEqual(box.GetMin(), corner)) {
        std::cerr << "  FAILED: AABB顶角设置错误 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (" << corner.x << ", " << corner.y << ", " << corner.z << ")" << std::endl;
        std::cerr << "    实际: (" << box.GetMin().x << ", " << box.GetMin().y << ", " << box.GetMin().z << ")" << std::endl;
        return false;
    }
    
    Vector3f expectedMax(5, 7, 9);
    if (!IsNearlyEqual(box.GetMax(), expectedMax)) {
        std::cerr << "  FAILED: AABB从顶角+长度计算最大点错误 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (" << expectedMax.x << ", " << expectedMax.y << ", " << expectedMax.z << ")" << std::endl;
        std::cerr << "    实际: (" << box.GetMax().x << ", " << box.GetMax().y << ", " << box.GetMax().z << ")" << std::endl;
        return false;
    }
    
    if (!IsNearlyEqual(box.GetLength(), length)) {
        std::cerr << "  FAILED: AABB长度设置错误 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (" << length.x << ", " << length.y << ", " << length.z << ")" << std::endl;
        std::cerr << "    实际: (" << box.GetLength().x << ", " << box.GetLength().y << ", " << box.GetLength().z << ")" << std::endl;
        return false;
    }
    
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
    
    std::cout << "  输入点集:" << std::endl;
    for (int i = 0; i < 4; ++i) {
        std::cout << "    点" << i << ": (" << points[i*3] << ", " << points[i*3+1] << ", " << points[i*3+2] << ")" << std::endl;
    }
    
    AABB box;
    box.SetFromPoints(points, 4, 3);
    
    std::cout << "  计算得到的AABB:" << std::endl;
    printVector3f("    Min", box.GetMin());
    printVector3f("    Max", box.GetMax());
    
    Vector3f expectedMin(-2, -2, -3);
    if (!IsNearlyEqual(box.GetMin(), expectedMin)) {
        std::cerr << "  FAILED: 从点集计算的最小点错误 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (" << expectedMin.x << ", " << expectedMin.y << ", " << expectedMin.z << ")" << std::endl;
        std::cerr << "    实际: (" << box.GetMin().x << ", " << box.GetMin().y << ", " << box.GetMin().z << ")" << std::endl;
        return false;
    }
    
    Vector3f expectedMax(4, 5, 6);
    if (!IsNearlyEqual(box.GetMax(), expectedMax)) {
        std::cerr << "  FAILED: 从点集计算的最大点错误 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (" << expectedMax.x << ", " << expectedMax.y << ", " << expectedMax.z << ")" << std::endl;
        std::cerr << "    实际: (" << box.GetMax().x << ", " << box.GetMax().y << ", " << box.GetMax().z << ")" << std::endl;
        return false;
    }
    
    return true;
}

bool test_AABB_Clear() {
    std::cout << "  测试Clear方法..." << std::endl;
    
    AABB box;
    box.SetMinMax(Vector3f(-10, -10, -10), Vector3f(10, 10, 10));
    
    std::cout << "  Clear前:" << std::endl;
    printVector3f("    Min", box.GetMin());
    printVector3f("    Max", box.GetMax());
    std::cout << "    IsValid(): " << (box.IsValid() ? "true" : "false") << std::endl;
    std::cout << "    IsEmpty(): " << (box.IsEmpty() ? "true" : "false") << std::endl;
    
    box.Clear();
    
    std::cout << "  Clear后:" << std::endl;
    printVector3f("    Min", box.GetMin());
    printVector3f("    Max", box.GetMax());
    printVector3f("    Center", box.GetCenter());
    printVector3f("    Length", box.GetLength());
    std::cout << "    IsValid(): " << (box.IsValid() ? "true" : "false") << std::endl;
    std::cout << "    IsEmpty(): " << (box.IsEmpty() ? "true" : "false") << std::endl;
    
    // Clear后应该是无效状态（这是inverse bounding box的标准做法）
    if (box.IsValid()) {
        std::cerr << "  FAILED: Clear后AABB应为无效状态 at line " << __LINE__ << std::endl;
        std::cerr << "    IsValid() 返回: true (期望为 false)" << std::endl;
        return false;
    }
    
    // Clear后应该是空的
    if (!box.IsEmpty()) {
        std::cerr << "  FAILED: Clear后AABB应为空 at line " << __LINE__ << std::endl;
        std::cerr << "    IsEmpty() 返回: false (期望为 true)" << std::endl;
        return false;
    }
    
    // Center和Length应该是零向量
    Vector3f expectedZero(0, 0, 0);
    if (!IsNearlyEqual(box.GetCenter(), expectedZero)) {
        std::cerr << "  FAILED: Clear后中心应为零向量 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (0, 0, 0)" << std::endl;
        std::cerr << "    实际: (" << box.GetCenter().x << ", " << box.GetCenter().y << ", " << box.GetCenter().z << ")" << std::endl;
        return false;
    }
    
    if (!IsNearlyEqual(box.GetLength(), expectedZero)) {
        std::cerr << "  FAILED: Clear后长度应为零向量 at line " << __LINE__ << std::endl;
        std::cerr << "    期望: (0, 0, 0)" << std::endl;
        std::cerr << "    实际: (" << box.GetLength().x << ", " << box.GetLength().y << ", " << box.GetLength().z << ")" << std::endl;
        return false;
    }
    
    // 验证Clear后的AABB可以正确用于ExpandToInclude
    box.ExpandToInclude(Vector3f(5, 5, 5));
    if (!box.IsValid()) {
        std::cerr << "  FAILED: ExpandToInclude后AABB应变为有效 at line " << __LINE__ << std::endl;
        return false;
    }
    
    std::cout << "  ExpandToInclude(5,5,5)后:" << std::endl;
    printVector3f("    Min", box.GetMin());
    printVector3f("    Max", box.GetMax());
    
    return true;
}

bool test_AABB_IsEmpty() {
    std::cout << "  测试IsEmpty方法..." << std::endl;
    
    AABB empty_box;
    empty_box.Clear();
    
    std::cout << "  清空后的AABB:" << std::endl;
    printVector3f("    Min", empty_box.GetMin());
    printVector3f("    Max", empty_box.GetMax());
    std::cout << "    IsEmpty(): " << (empty_box.IsEmpty() ? "true" : "false") << std::endl;
    
    if (!empty_box.IsEmpty()) {
        std::cerr << "  FAILED: 空AABB应返回true at line " << __LINE__ << std::endl;
        std::cerr << "    IsEmpty() 返回: false (期望为 true)" << std::endl;
        return false;
    }
    
    AABB non_empty_box;
    non_empty_box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    
    std::cout << "  非空AABB:" << std::endl;
    printVector3f("    Min", non_empty_box.GetMin());
    printVector3f("    Max", non_empty_box.GetMax());
    std::cout << "    IsEmpty(): " << (non_empty_box.IsEmpty() ? "true" : "false") << std::endl;
    
    if (non_empty_box.IsEmpty()) {
        std::cerr << "  FAILED: 非空AABB应返回false at line " << __LINE__ << std::endl;
        std::cerr << "    IsEmpty() 返回: true (期望为 false)" << std::endl;
        return false;
    }
    
    // 测试静态工厂方法 Empty()
    AABB factory_empty = AABB::Empty();
    std::cout << "  AABB::Empty()创建的AABB:" << std::endl;
    std::cout << "    IsEmpty(): " << (factory_empty.IsEmpty() ? "true" : "false") << std::endl;
    std::cout << "    IsValid(): " << (factory_empty.IsValid() ? "true" : "false") << std::endl;
    
    if (!factory_empty.IsEmpty()) {
        std::cerr << "  FAILED: AABB::Empty()创建的AABB应为空 at line " << __LINE__ << std::endl;
        return false;
    }
    
    if (factory_empty.IsValid()) {
        std::cerr << "  FAILED: AABB::Empty()创建的AABB应为无效 at line " << __LINE__ << std::endl;
        return false;
    }
    
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
