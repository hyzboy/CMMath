/**
 * test_collision_detector.cpp
 *
 * Test cases for CollisionDetector class
 * Tests various collision detection scenarios between different geometry types.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/queries/CollisionDetector.h>
#include <hgl/math/geometry/primitives/Sphere.h>
#include <hgl/math/geometry/primitives/Capsule.h>
#include <hgl/math/geometry/primitives/Cylinder.h>
#include <hgl/math/geometry/primitives/Cone.h>
#include <hgl/math/geometry/primitives/Torus.h>
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

#define ASSERT_NEAR(a, b, epsilon) \
    if (std::abs((a) - (b)) > (epsilon)) { \
        std::cerr << "FAILED: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ") at line " << __LINE__ << std::endl; \
        exit(1); \
    }

// ============================================================================
// Sphere-Sphere Collision Tests
// ============================================================================

void test_sphere_sphere_no_collision() {
    Sphere s1(Vector3f(0, 0, 0), 1.0f);
    Sphere s2(Vector3f(5, 0, 0), 1.0f);

    ASSERT_FALSE(CollisionDetector::Intersects(s1, s2));
}

void test_sphere_sphere_touching() {
    Sphere s1(Vector3f(0, 0, 0), 1.0f);
    Sphere s2(Vector3f(2, 0, 0), 1.0f);

    ASSERT_TRUE(CollisionDetector::Intersects(s1, s2));
}

void test_sphere_sphere_overlapping() {
    Sphere s1(Vector3f(0, 0, 0), 1.0f);
    Sphere s2(Vector3f(1, 0, 0), 1.0f);

    ASSERT_TRUE(CollisionDetector::Intersects(s1, s2));

    CollisionInfo info = CollisionDetector::TestCollision(s1, s2);
    ASSERT_TRUE(info.intersects);
    ASSERT_NEAR(info.penetration, 1.0f, 0.01f);
}

void test_sphere_sphere_contained() {
    Sphere s1(Vector3f(0, 0, 0), 2.0f);
    Sphere s2(Vector3f(0, 0, 0), 0.5f);

    ASSERT_TRUE(CollisionDetector::Intersects(s1, s2));
}

// ============================================================================
// Sphere-AABB Collision Tests
// ============================================================================

void test_sphere_aabb_no_collision() {
    Sphere sphere(Vector3f(5, 5, 5), 1.0f);
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    ASSERT_FALSE(CollisionDetector::Intersects(sphere, box));
}

void test_sphere_aabb_sphere_inside() {
    Sphere sphere(Vector3f(0.5f, 0.5f, 0.5f), 0.2f);
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    ASSERT_TRUE(CollisionDetector::Intersects(sphere, box));
}

void test_sphere_aabb_intersecting() {
    Sphere sphere(Vector3f(0, 0, 0), 1.5f);
    AABB box;
    box.SetMinMax(Vector3f(1, -0.5f, -0.5f), Vector3f(2, 0.5f, 0.5f));

    ASSERT_TRUE(CollisionDetector::Intersects(sphere, box));
}

void test_sphere_aabb_corner_collision() {
    Sphere sphere(Vector3f(2, 2, 2), 1.0f);
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    float cornerDist = sqrt(3.0f);  // Distance from (1,1,1) to (2,2,2)
    ASSERT_TRUE(cornerDist < 2.0f);  // Within sphere radius
    ASSERT_TRUE(CollisionDetector::Intersects(sphere, box));
}

// ============================================================================
// Sphere-Capsule Collision Tests
// ============================================================================

void test_sphere_capsule_no_collision() {
    Sphere sphere(Vector3f(5, 0, 0), 1.0f);
    Capsule capsule(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);

    ASSERT_FALSE(CollisionDetector::Intersects(sphere, capsule));
}

void test_sphere_capsule_colliding() {
    Sphere sphere(Vector3f(1, 0, 0), 0.8f);
    Capsule capsule(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);

    ASSERT_TRUE(CollisionDetector::Intersects(sphere, capsule));
}

void test_sphere_capsule_sphere_inside() {
    Sphere sphere(Vector3f(0, 0, 0), 0.2f);
    Capsule capsule(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 1.0f);

    ASSERT_TRUE(CollisionDetector::Intersects(sphere, capsule));
}

// ============================================================================
// Capsule-Capsule Collision Tests
// ============================================================================

void test_capsule_capsule_parallel_no_collision() {
    Capsule c1(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);
    Capsule c2(Vector3f(3, -1, 0), Vector3f(3, 1, 0), 0.5f);

    ASSERT_FALSE(CollisionDetector::Intersects(c1, c2));
}

void test_capsule_capsule_parallel_touching() {
    Capsule c1(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);
    Capsule c2(Vector3f(1, -1, 0), Vector3f(1, 1, 0), 0.5f);

    ASSERT_TRUE(CollisionDetector::Intersects(c1, c2));
}

void test_capsule_capsule_perpendicular_colliding() {
    Capsule c1(Vector3f(-1, 0, 0), Vector3f(1, 0, 0), 0.5f);
    Capsule c2(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);

    ASSERT_TRUE(CollisionDetector::Intersects(c1, c2));
}

void test_capsule_capsule_detailed_collision() {
    Capsule c1(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);
    Capsule c2(Vector3f(0.8f, -1, 0), Vector3f(0.8f, 1, 0), 0.5f);

    CollisionInfo info = CollisionDetector::TestCollision(c1, c2);
    ASSERT_TRUE(info.intersects);
    ASSERT_TRUE(info.penetration > 0.0f);
}

// ============================================================================
// Capsule-AABB Collision Tests
// ============================================================================

void test_capsule_aabb_no_collision() {
    Capsule capsule(Vector3f(5, 0, 0), Vector3f(5, 2, 0), 0.5f);
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    ASSERT_FALSE(CollisionDetector::Intersects(capsule, box));
}

void test_capsule_aabb_intersecting() {
    Capsule capsule(Vector3f(-1, 0.5f, 0.5f), Vector3f(2, 0.5f, 0.5f), 0.3f);
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    ASSERT_TRUE(CollisionDetector::Intersects(capsule, box));
}

// ============================================================================
// Cylinder Collision Tests
// ============================================================================

void test_cylinder_sphere_no_collision() {
    Cylinder cylinder(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 1.0f);
    Sphere sphere(Vector3f(5, 0, 0), 1.0f);

    ASSERT_FALSE(CollisionDetector::Intersects(cylinder, sphere));
}

void test_cylinder_sphere_colliding() {
    Cylinder cylinder(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 1.0f);
    Sphere sphere(Vector3f(1.5f, 0, 0), 1.0f);

    ASSERT_TRUE(CollisionDetector::Intersects(cylinder, sphere));
}

void test_cylinder_cylinder_collision() {
    Cylinder c1(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 1.0f);
    Cylinder c2(Vector3f(1.5f, 0, 0), Vector3f(0, 1, 0), 2.0f, 1.0f);

    ASSERT_TRUE(CollisionDetector::Intersects(c1, c2));
}

// ============================================================================
// Cone Collision Tests
// ============================================================================

void test_cone_sphere_no_collision() {
    Cone cone(Vector3f(0, 2, 0), Vector3f(0, -1, 0), 2.0f, 1.0f);
    Sphere sphere(Vector3f(5, 0, 0), 1.0f);

    ASSERT_FALSE(CollisionDetector::Intersects(cone, sphere));
}

void test_cone_sphere_colliding() {
    Cone cone(Vector3f(0, 2, 0), Vector3f(0, -1, 0), 2.0f, 1.0f);
    Sphere sphere(Vector3f(0, 1, 0), 0.5f);

    ASSERT_TRUE(CollisionDetector::Intersects(cone, sphere));
}

// ============================================================================
// Torus Collision Tests
// ============================================================================

void test_torus_sphere_no_collision() {
    Torus torus(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 0.5f);
    Sphere sphere(Vector3f(0, 0, 0), 0.3f);  // Inside hole

    ASSERT_FALSE(CollisionDetector::Intersects(torus, sphere));
}

void test_torus_sphere_colliding() {
    Torus torus(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 0.5f);
    Sphere sphere(Vector3f(2.3f, 0, 0), 0.5f);

    ASSERT_TRUE(CollisionDetector::Intersects(torus, sphere));
}

// ============================================================================
// Broad Phase Tests
// ============================================================================

void test_broad_phase_early_rejection() {
    Sphere s1(Vector3f(0, 0, 0), 1.0f);
    Sphere s2(Vector3f(100, 0, 0), 1.0f);

    // Broad phase should reject this early
    ASSERT_FALSE(CollisionDetector::IntersectsWithBroadPhase(s1, s2));
}

void test_broad_phase_passes_to_narrow() {
    Sphere s1(Vector3f(0, 0, 0), 1.0f);
    Sphere s2(Vector3f(1.5f, 0, 0), 1.0f);

    // Broad phase passes, narrow phase detects collision
    ASSERT_TRUE(CollisionDetector::IntersectsWithBroadPhase(s1, s2));
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "=== Collision Detector Test Suite ===" << std::endl << std::endl;

    std::cout << "--- Sphere-Sphere Collision Tests ---" << std::endl;
    TEST(sphere_sphere_no_collision);
    TEST(sphere_sphere_touching);
    TEST(sphere_sphere_overlapping);
    TEST(sphere_sphere_contained);

    std::cout << std::endl << "--- Sphere-AABB Collision Tests ---" << std::endl;
    TEST(sphere_aabb_no_collision);
    TEST(sphere_aabb_sphere_inside);
    TEST(sphere_aabb_intersecting);
    TEST(sphere_aabb_corner_collision);

    std::cout << std::endl << "--- Sphere-Capsule Collision Tests ---" << std::endl;
    TEST(sphere_capsule_no_collision);
    TEST(sphere_capsule_colliding);
    TEST(sphere_capsule_sphere_inside);

    std::cout << std::endl << "--- Capsule-Capsule Collision Tests ---" << std::endl;
    TEST(capsule_capsule_parallel_no_collision);
    TEST(capsule_capsule_parallel_touching);
    TEST(capsule_capsule_perpendicular_colliding);
    TEST(capsule_capsule_detailed_collision);

    std::cout << std::endl << "--- Capsule-AABB Collision Tests ---" << std::endl;
    TEST(capsule_aabb_no_collision);
    TEST(capsule_aabb_intersecting);

    std::cout << std::endl << "--- Cylinder Collision Tests ---" << std::endl;
    TEST(cylinder_sphere_no_collision);
    TEST(cylinder_sphere_colliding);
    TEST(cylinder_cylinder_collision);

    std::cout << std::endl << "--- Cone Collision Tests ---" << std::endl;
    TEST(cone_sphere_no_collision);
    TEST(cone_sphere_colliding);

    std::cout << std::endl << "--- Torus Collision Tests ---" << std::endl;
    TEST(torus_sphere_no_collision);
    TEST(torus_sphere_colliding);

    std::cout << std::endl << "--- Broad Phase Tests ---" << std::endl;
    TEST(broad_phase_early_rejection);
    TEST(broad_phase_passes_to_narrow);

    std::cout << std::endl << "=== All Tests Passed! ===" << std::endl;

    return 0;
}
