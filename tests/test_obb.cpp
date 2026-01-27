/**
 * test_obb.cpp
 *
 * Comprehensive test cases for OBB (Oriented Bounding Box) class
 * Tests OBB construction, ray intersection, collision detection, and containment.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/OBB.h>
#include <hgl/math/geometry/Ray.h>
#include <hgl/math/geometry/AABB.h>
#include <hgl/math/geometry/primitives/Sphere.h>

using namespace hgl::math;

#define TEST(name) \
    std::cout << "Testing " << #name << "... "; \
    test_##name(); \
    std::cout << "PASSED" << std::endl;

#define ASSERT_TRUE(expr) \
    if (!(expr)) { \
        std::cerr << "FAILED: " << #expr << " at line " << __LINE__ << std::endl; \
        exit(1); \
    }

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

#define ASSERT_NEAR(a, b, epsilon) \
    if (std::abs((a) - (b)) > (epsilon)) { \
        std::cerr << "FAILED: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ") at line " << __LINE__ << std::endl; \
        exit(1); \
    }

// ============================================================================
// OBB Construction Tests
// ============================================================================

void test_obb_construction_default() {
    OBB obb;
    // Should construct without crashing
    ASSERT_TRUE(true);
}

void test_obb_construction_from_center() {
    OBB obb;
    Vector3f center(1, 2, 3);
    Vector3f halfExtent(0.5f, 1.0f, 1.5f);

    obb.Set(center, halfExtent);

    ASSERT_NEAR(obb.GetCenter().x, center.x, 0.001f);
    ASSERT_NEAR(obb.GetCenter().y, center.y, 0.001f);
    ASSERT_NEAR(obb.GetCenter().z, center.z, 0.001f);
}

void test_obb_construction_with_rotation() {
    OBB obb;
    Vector3f center(0, 0, 0);
    Vector3f axis0(1, 0, 0);
    Vector3f axis1(0, 1, 0);
    Vector3f axis2(0, 0, 1);
    Vector3f halfExtent(1, 2, 3);

    obb.Set(center, axis0, axis1, axis2, halfExtent);

    // Verify center
    ASSERT_NEAR(obb.GetCenter().x, 0.0f, 0.001f);
    ASSERT_NEAR(obb.GetCenter().y, 0.0f, 0.001f);
    ASSERT_NEAR(obb.GetCenter().z, 0.0f, 0.001f);
}

void test_obb_construction_45_degree_rotation() {
    OBB obb;
    Vector3f center(0, 0, 0);

    // 45 degree rotation around Z axis
    float cos45 = std::cos(pi / 4.0f);
    float sin45 = std::sin(pi / 4.0f);
    Vector3f axis0(cos45, sin45, 0);
    Vector3f axis1(-sin45, cos45, 0);
    Vector3f axis2(0, 0, 1);
    Vector3f halfExtent(1, 1, 1);

    obb.Set(center, axis0, axis1, axis2, halfExtent);

    // Should construct successfully
    ASSERT_TRUE(true);
}

// ============================================================================
// OBB Ray Intersection Tests
// ============================================================================

void test_ray_obb_aligned_hit() {
    OBB obb;
    Vector3f center(0, 0, 0);
    Vector3f halfExtent(1, 1, 1);
    obb.Set(center, halfExtent);

    Ray ray(Vector3f(0, 0, -5), Vector3f(0, 0, 1));

    float t;
    ASSERT_TRUE(obb.Intersects(ray, t));
    ASSERT_NEAR(t, 4.0f, 0.01f);
}

void test_ray_obb_aligned_miss() {
    OBB obb;
    Vector3f center(0, 0, 0);
    Vector3f halfExtent(1, 1, 1);
    obb.Set(center, halfExtent);

    Ray ray(Vector3f(5, 0, 0), Vector3f(0, 1, 0));  // Ray parallel to box, pointing away

    float t;
    ASSERT_FALSE(obb.Intersects(ray, t));
}

void test_ray_obb_rotated_hit() {
    OBB obb;
    Vector3f center(0, 0, 0);

    // 45 degree rotation around Z
    float cos45 = std::cos(pi / 4.0f);
    float sin45 = std::sin(pi / 4.0f);
    Vector3f axis0(cos45, sin45, 0);
    Vector3f axis1(-sin45, cos45, 0);
    Vector3f axis2(0, 0, 1);
    Vector3f halfExtent(1, 0.5f, 0.5f);

    obb.Set(center, axis0, axis1, axis2, halfExtent);

    // Ray through center
    Ray ray(Vector3f(0, 0, -5), Vector3f(0, 0, 1));

    float t;
    ASSERT_TRUE(obb.Intersects(ray, t));
}

void test_ray_obb_from_inside() {
    OBB obb;
    Vector3f center(0, 0, 0);
    Vector3f halfExtent(2, 2, 2);
    obb.Set(center, halfExtent);

    Ray ray(Vector3f(0, 0, 0), Vector3f(1, 0, 0));  // From inside

    float t;
    ASSERT_TRUE(obb.Intersects(ray, t));
}

void test_ray_obb_edge_hit() {
    OBB obb;
    Vector3f center(0, 0, 0);
    Vector3f halfExtent(1, 1, 1);
    obb.Set(center, halfExtent);

    // Ray along edge
    Ray ray(Vector3f(-1, -1, -5), Vector3f(0, 0, 1));

    float t;
    ASSERT_TRUE(obb.Intersects(ray, t));
}

void test_ray_obb_corner_near_miss() {
    OBB obb;
    Vector3f center(0, 0, 0);
    Vector3f halfExtent(1, 1, 1);
    obb.Set(center, halfExtent);

    // Ray just past corner
    Ray ray(Vector3f(-1.1f, -1.1f, -5), Vector3f(0, 0, 1));

    float t;
    ASSERT_FALSE(obb.Intersects(ray, t));
}

// ============================================================================
// OBB-OBB Collision Tests
// ============================================================================

void test_obb_obb_aligned_no_collision() {
    OBB obb1, obb2;
    obb1.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    obb2.Set(Vector3f(5, 0, 0), Vector3f(1, 1, 1));

    ASSERT_FALSE(obb1.Intersects(obb2));
}

void test_obb_obb_aligned_touching() {
    OBB obb1, obb2;
    obb1.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    obb2.Set(Vector3f(2, 0, 0), Vector3f(1, 1, 1));

    ASSERT_TRUE(obb1.Intersects(obb2));
}

void test_obb_obb_aligned_overlapping() {
    OBB obb1, obb2;
    obb1.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    obb2.Set(Vector3f(1, 0, 0), Vector3f(1, 1, 1));

    ASSERT_TRUE(obb1.Intersects(obb2));
}

void test_obb_obb_one_inside_other() {
    OBB obb1, obb2;
    obb1.Set(Vector3f(0, 0, 0), Vector3f(3, 3, 3));
    obb2.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    ASSERT_TRUE(obb1.Intersects(obb2));
}

void test_obb_obb_rotated_collision() {
    OBB obb1, obb2;

    // First OBB aligned
    obb1.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    // Second OBB rotated 45 degrees around Z
    float cos45 = std::cos(pi / 4.0f);
    float sin45 = std::sin(pi / 4.0f);
    Vector3f axis0(cos45, sin45, 0);
    Vector3f axis1(-sin45, cos45, 0);
    Vector3f axis2(0, 0, 1);

    obb2.Set(Vector3f(1.5f, 0, 0), axis0, axis1, axis2, Vector3f(1, 1, 1));

    ASSERT_TRUE(obb1.Intersects(obb2));
}

void test_obb_obb_perpendicular_collision() {
    OBB obb1, obb2;

    // First OBB aligned
    obb1.Set(Vector3f(0, 0, 0), Vector3f(2, 0.5f, 0.5f));

    // Second OBB rotated 90 degrees around Z
    Vector3f axis0(0, 1, 0);
    Vector3f axis1(-1, 0, 0);
    Vector3f axis2(0, 0, 1);

    obb2.Set(Vector3f(0, 0, 0), axis0, axis1, axis2, Vector3f(2, 0.5f, 0.5f));

    ASSERT_TRUE(obb1.Intersects(obb2));
}

// ============================================================================
// OBB-Sphere Collision Tests
// ============================================================================

void test_obb_sphere_no_collision() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    Sphere sphere(Vector3f(5, 0, 0), 1.0f);

    ASSERT_FALSE(obb.Intersects(sphere));
}

void test_obb_sphere_collision_center() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(2, 2, 2));

    Sphere sphere(Vector3f(0, 0, 0), 1.0f);

    ASSERT_TRUE(obb.Intersects(sphere));
}

void test_obb_sphere_collision_edge() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    Sphere sphere(Vector3f(1.5f, 0, 0), 0.7f);

    ASSERT_TRUE(obb.Intersects(sphere));
}

void test_obb_sphere_collision_corner() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    // Sphere near corner
    Sphere sphere(Vector3f(1.5f, 1.5f, 1.5f), 1.0f);

    ASSERT_TRUE(obb.Intersects(sphere));
}

void test_obb_sphere_sphere_inside() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(5, 5, 5));

    Sphere sphere(Vector3f(0, 0, 0), 1.0f);

    ASSERT_TRUE(obb.Intersects(sphere));
}

// ============================================================================
// OBB Containment Tests
// ============================================================================

void test_obb_contains_point_center() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    ASSERT_TRUE(obb.ContainsPoint(Vector3f(0, 0, 0)));
}

void test_obb_contains_point_inside() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(2, 2, 2));

    ASSERT_TRUE(obb.ContainsPoint(Vector3f(1, 1, 1)));
}

void test_obb_contains_point_on_surface() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    ASSERT_TRUE(obb.ContainsPoint(Vector3f(1, 0, 0)));
}

void test_obb_contains_point_outside() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    ASSERT_FALSE(obb.ContainsPoint(Vector3f(2, 0, 0)));
}

void test_obb_rotated_contains_point() {
    OBB obb;

    // 45 degree rotation around Z
    float cos45 = std::cos(pi / 4.0f);
    float sin45 = std::sin(pi / 4.0f);
    Vector3f axis0(cos45, sin45, 0);
    Vector3f axis1(-sin45, cos45, 0);
    Vector3f axis2(0, 0, 1);

    obb.Set(Vector3f(0, 0, 0), axis0, axis1, axis2, Vector3f(1, 1, 1));

    // Point at center should be inside
    ASSERT_TRUE(obb.ContainsPoint(Vector3f(0, 0, 0)));
}

// ============================================================================
// OBB to AABB Conversion Tests
// ============================================================================

void test_obb_to_aabb_aligned() {
    OBB obb;
    obb.Set(Vector3f(1, 2, 3), Vector3f(0.5f, 1.0f, 1.5f));

    AABB aabb = obb.ToAABB();

    Vector3f min = aabb.GetMin();
    Vector3f max = aabb.GetMax();

    ASSERT_NEAR(min.x, 0.5f, 0.01f);
    ASSERT_NEAR(min.y, 1.0f, 0.01f);
    ASSERT_NEAR(min.z, 1.5f, 0.01f);

    ASSERT_NEAR(max.x, 1.5f, 0.01f);
    ASSERT_NEAR(max.y, 3.0f, 0.01f);
    ASSERT_NEAR(max.z, 4.5f, 0.01f);
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_obb_zero_size() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(0, 0, 0));

    // Should not crash
    ASSERT_TRUE(obb.ContainsPoint(Vector3f(0, 0, 0)));
}

void test_obb_very_large() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(1000, 1000, 1000));

    ASSERT_TRUE(obb.ContainsPoint(Vector3f(500, 500, 500)));
    ASSERT_FALSE(obb.ContainsPoint(Vector3f(1500, 0, 0)));
}

void test_obb_very_small() {
    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(0.001f, 0.001f, 0.001f));

    ASSERT_TRUE(obb.ContainsPoint(Vector3f(0, 0, 0)));
    ASSERT_FALSE(obb.ContainsPoint(Vector3f(0.01f, 0, 0)));
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "=== OBB (Oriented Bounding Box) Test Suite ===" << std::endl << std::endl;

    std::cout << "--- OBB Construction Tests ---" << std::endl;
    TEST(obb_construction_default);
    TEST(obb_construction_from_center);
    TEST(obb_construction_with_rotation);
    TEST(obb_construction_45_degree_rotation);

    std::cout << std::endl << "--- OBB Ray Intersection Tests ---" << std::endl;
    TEST(ray_obb_aligned_hit);
    TEST(ray_obb_aligned_miss);
    TEST(ray_obb_rotated_hit);
    TEST(ray_obb_from_inside);
    TEST(ray_obb_edge_hit);
    TEST(ray_obb_corner_near_miss);

    std::cout << std::endl << "--- OBB-OBB Collision Tests ---" << std::endl;
    TEST(obb_obb_aligned_no_collision);
    TEST(obb_obb_aligned_touching);
    TEST(obb_obb_aligned_overlapping);
    TEST(obb_obb_one_inside_other);
    TEST(obb_obb_rotated_collision);
    TEST(obb_obb_perpendicular_collision);

    std::cout << std::endl << "--- OBB-Sphere Collision Tests ---" << std::endl;
    TEST(obb_sphere_no_collision);
    TEST(obb_sphere_collision_center);
    TEST(obb_sphere_collision_edge);
    TEST(obb_sphere_collision_corner);
    TEST(obb_sphere_sphere_inside);

    std::cout << std::endl << "--- OBB Containment Tests ---" << std::endl;
    TEST(obb_contains_point_center);
    TEST(obb_contains_point_inside);
    TEST(obb_contains_point_on_surface);
    TEST(obb_contains_point_outside);
    TEST(obb_rotated_contains_point);

    std::cout << std::endl << "--- OBB to AABB Conversion Tests ---" << std::endl;
    TEST(obb_to_aabb_aligned);

    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    TEST(obb_zero_size);
    TEST(obb_very_large);
    TEST(obb_very_small);

    std::cout << std::endl << "=== All OBB Tests Passed! ===" << std::endl;

    return 0;
}
