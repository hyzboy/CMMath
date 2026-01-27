/**
 * test_containment_query.cpp
 *
 * Test cases for ContainmentQuery class
 * Tests point and geometry containment checks.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/queries/ContainmentQuery.h>
#include <hgl/math/geometry/primitives/Sphere.h>
#include <hgl/math/geometry/primitives/Capsule.h>
#include <hgl/math/geometry/AABB.h>

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

// ============================================================================
// Point Containment Tests
// ============================================================================

void test_sphere_contains_point_inside() {
    Sphere sphere(Vector3f(0, 0, 0), 2.0f);
    Vector3f point(1, 0, 0);

    ASSERT_TRUE(ContainmentQuery::Contains(sphere, point));
}

void test_sphere_contains_point_outside() {
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    Vector3f point(3, 0, 0);

    ASSERT_FALSE(ContainmentQuery::Contains(sphere, point));
}

void test_sphere_contains_point_on_surface() {
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    Vector3f point(1, 0, 0);

    ASSERT_TRUE(ContainmentQuery::Contains(sphere, point));
}

void test_capsule_contains_point_inside() {
    Capsule capsule(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);
    Vector3f point(0, 0, 0);

    ASSERT_TRUE(ContainmentQuery::Contains(capsule, point));
}

void test_capsule_contains_point_outside() {
    Capsule capsule(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);
    Vector3f point(2, 0, 0);

    ASSERT_FALSE(ContainmentQuery::Contains(capsule, point));
}

void test_aabb_contains_point_inside() {
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));
    Vector3f point(1, 1, 1);

    ASSERT_TRUE(ContainmentQuery::Contains(box, point));
}

void test_aabb_contains_point_outside() {
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    Vector3f point(3, 3, 3);

    ASSERT_FALSE(ContainmentQuery::Contains(box, point));
}

void test_aabb_contains_point_on_edge() {
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    Vector3f point(0, 0.5f, 0.5f);

    ASSERT_TRUE(ContainmentQuery::Contains(box, point));
}

// ============================================================================
// Sphere Containment Tests
// ============================================================================

void test_sphere_contains_smaller_sphere() {
    Sphere container(Vector3f(0, 0, 0), 3.0f);
    Sphere contained(Vector3f(0, 0, 0), 1.0f);

    ASSERT_TRUE(ContainmentQuery::Contains(container, contained));
}

void test_sphere_contains_sphere_at_edge() {
    Sphere container(Vector3f(0, 0, 0), 3.0f);
    Sphere contained(Vector3f(2, 0, 0), 1.0f);

    ASSERT_TRUE(ContainmentQuery::Contains(container, contained));
}

void test_sphere_does_not_contain_larger_sphere() {
    Sphere container(Vector3f(0, 0, 0), 1.0f);
    Sphere contained(Vector3f(0, 0, 0), 2.0f);

    ASSERT_FALSE(ContainmentQuery::Contains(container, contained));
}

void test_sphere_does_not_contain_overlapping_sphere() {
    Sphere container(Vector3f(0, 0, 0), 2.0f);
    Sphere contained(Vector3f(2, 0, 0), 1.0f);

    ASSERT_FALSE(ContainmentQuery::Contains(container, contained));
}

void test_aabb_contains_sphere_inside() {
    AABB container;
    container.SetMinMax(Vector3f(0, 0, 0), Vector3f(4, 4, 4));
    Sphere sphere(Vector3f(2, 2, 2), 1.0f);

    ASSERT_TRUE(ContainmentQuery::Contains(container, sphere));
}

void test_aabb_does_not_contain_sphere_too_large() {
    AABB container;
    container.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));
    Sphere sphere(Vector3f(1, 1, 1), 2.0f);

    ASSERT_FALSE(ContainmentQuery::Contains(container, sphere));
}

void test_aabb_does_not_contain_sphere_protruding() {
    AABB container;
    container.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));
    Sphere sphere(Vector3f(1.5f, 1.5f, 1.5f), 1.0f);

    ASSERT_FALSE(ContainmentQuery::Contains(container, sphere));
}

// ============================================================================
// AABB Containment Tests
// ============================================================================

void test_aabb_contains_smaller_aabb() {
    AABB container;
    container.SetMinMax(Vector3f(0, 0, 0), Vector3f(4, 4, 4));

    AABB contained;
    contained.SetMinMax(Vector3f(1, 1, 1), Vector3f(3, 3, 3));

    ASSERT_TRUE(ContainmentQuery::Contains(container, contained));
}

void test_aabb_contains_aabb_at_corner() {
    AABB container;
    container.SetMinMax(Vector3f(0, 0, 0), Vector3f(4, 4, 4));

    AABB contained;
    contained.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));

    ASSERT_TRUE(ContainmentQuery::Contains(container, contained));
}

void test_aabb_does_not_contain_larger_aabb() {
    AABB container;
    container.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));

    AABB contained;
    contained.SetMinMax(Vector3f(0, 0, 0), Vector3f(3, 3, 3));

    ASSERT_FALSE(ContainmentQuery::Contains(container, contained));
}

void test_aabb_does_not_contain_overlapping_aabb() {
    AABB container;
    container.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));

    AABB contained;
    contained.SetMinMax(Vector3f(1, 1, 1), Vector3f(3, 3, 3));

    ASSERT_FALSE(ContainmentQuery::Contains(container, contained));
}

void test_aabb_does_not_contain_separate_aabb() {
    AABB container;
    container.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));

    AABB contained;
    contained.SetMinMax(Vector3f(5, 5, 5), Vector3f(7, 7, 7));

    ASSERT_FALSE(ContainmentQuery::Contains(container, contained));
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_sphere_contains_coincident_sphere() {
    Sphere container(Vector3f(0, 0, 0), 2.0f);
    Sphere contained(Vector3f(0, 0, 0), 2.0f);

    ASSERT_TRUE(ContainmentQuery::Contains(container, contained));
}

void test_aabb_contains_coincident_aabb() {
    AABB container;
    container.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));

    AABB contained;
    contained.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));

    ASSERT_TRUE(ContainmentQuery::Contains(container, contained));
}

void test_sphere_contains_point_at_center() {
    Sphere sphere(Vector3f(1, 2, 3), 5.0f);
    Vector3f point(1, 2, 3);

    ASSERT_TRUE(ContainmentQuery::Contains(sphere, point));
}

void test_aabb_contains_point_at_min_corner() {
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));
    Vector3f point(0, 0, 0);

    ASSERT_TRUE(ContainmentQuery::Contains(box, point));
}

void test_aabb_contains_point_at_max_corner() {
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));
    Vector3f point(2, 2, 2);

    ASSERT_TRUE(ContainmentQuery::Contains(box, point));
}

void test_sphere_contains_zero_radius_sphere() {
    Sphere container(Vector3f(0, 0, 0), 2.0f);
    Sphere contained(Vector3f(0, 0, 0), 0.0f);

    ASSERT_TRUE(ContainmentQuery::Contains(container, contained));
}

void test_aabb_contains_zero_size_aabb() {
    AABB container;
    container.SetMinMax(Vector3f(0, 0, 0), Vector3f(2, 2, 2));

    AABB contained;
    contained.SetMinMax(Vector3f(1, 1, 1), Vector3f(1, 1, 1));

    ASSERT_TRUE(ContainmentQuery::Contains(container, contained));
}

// ============================================================================
// Multiple Containment Checks
// ============================================================================

void test_nested_containment() {
    // Test nested containment: sphere1 contains sphere2 contains sphere3
    Sphere s1(Vector3f(0, 0, 0), 5.0f);
    Sphere s2(Vector3f(0, 0, 0), 3.0f);
    Sphere s3(Vector3f(0, 0, 0), 1.0f);

    ASSERT_TRUE(ContainmentQuery::Contains(s1, s2));
    ASSERT_TRUE(ContainmentQuery::Contains(s2, s3));
    ASSERT_TRUE(ContainmentQuery::Contains(s1, s3));
}

void test_multiple_points_in_sphere() {
    Sphere sphere(Vector3f(0, 0, 0), 2.0f);

    ASSERT_TRUE(ContainmentQuery::Contains(sphere, Vector3f(0, 0, 0)));
    ASSERT_TRUE(ContainmentQuery::Contains(sphere, Vector3f(1, 0, 0)));
    ASSERT_TRUE(ContainmentQuery::Contains(sphere, Vector3f(0, 1, 0)));
    ASSERT_TRUE(ContainmentQuery::Contains(sphere, Vector3f(0, 0, 1)));
    ASSERT_FALSE(ContainmentQuery::Contains(sphere, Vector3f(3, 0, 0)));
    ASSERT_FALSE(ContainmentQuery::Contains(sphere, Vector3f(0, 3, 0)));
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "=== Containment Query Test Suite ===" << std::endl << std::endl;

    std::cout << "--- Point Containment Tests ---" << std::endl;
    TEST(sphere_contains_point_inside);
    TEST(sphere_contains_point_outside);
    TEST(sphere_contains_point_on_surface);
    TEST(capsule_contains_point_inside);
    TEST(capsule_contains_point_outside);
    TEST(aabb_contains_point_inside);
    TEST(aabb_contains_point_outside);
    TEST(aabb_contains_point_on_edge);

    std::cout << std::endl << "--- Sphere Containment Tests ---" << std::endl;
    TEST(sphere_contains_smaller_sphere);
    TEST(sphere_contains_sphere_at_edge);
    TEST(sphere_does_not_contain_larger_sphere);
    TEST(sphere_does_not_contain_overlapping_sphere);
    TEST(aabb_contains_sphere_inside);
    TEST(aabb_does_not_contain_sphere_too_large);
    TEST(aabb_does_not_contain_sphere_protruding);

    std::cout << std::endl << "--- AABB Containment Tests ---" << std::endl;
    TEST(aabb_contains_smaller_aabb);
    TEST(aabb_contains_aabb_at_corner);
    TEST(aabb_does_not_contain_larger_aabb);
    TEST(aabb_does_not_contain_overlapping_aabb);
    TEST(aabb_does_not_contain_separate_aabb);

    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    TEST(sphere_contains_coincident_sphere);
    TEST(aabb_contains_coincident_aabb);
    TEST(sphere_contains_point_at_center);
    TEST(aabb_contains_point_at_min_corner);
    TEST(aabb_contains_point_at_max_corner);
    TEST(sphere_contains_zero_radius_sphere);
    TEST(aabb_contains_zero_size_aabb);

    std::cout << std::endl << "--- Multiple Containment Checks ---" << std::endl;
    TEST(nested_containment);
    TEST(multiple_points_in_sphere);

    std::cout << std::endl << "=== All Tests Passed! ===" << std::endl;

    return 0;
}
