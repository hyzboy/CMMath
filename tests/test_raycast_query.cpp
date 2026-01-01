/**
 * test_raycast_query.cpp
 * 
 * Test cases for RaycastQuery class
 * Tests ray-geometry intersection for various shapes and scenarios.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/queries/RaycastQuery.h>
#include <hgl/math/geometry/primitives/Sphere.h>
#include <hgl/math/geometry/primitives/Capsule.h>
#include <hgl/math/geometry/primitives/Cylinder.h>
#include <hgl/math/geometry/Ray.h>
#include <hgl/math/geometry/Plane.h>
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
// Ray-Sphere Tests
// ============================================================================

void test_ray_sphere_miss() {
    Ray ray(Vector3f(0, 0, -5), Vector3f(0, 1, 0));  // Ray pointing up
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    
    ASSERT_FALSE(RaycastQuery::Intersects(ray, sphere));
}

void test_ray_sphere_hit_from_outside() {
    Ray ray(Vector3f(0, 0, -5), Vector3f(0, 0, 1));  // Ray pointing at sphere
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    
    float t;
    ASSERT_TRUE(RaycastQuery::Intersects(ray, sphere, t));
    ASSERT_NEAR(t, 4.0f, 0.01f);  // Hit at distance 4 (5 - 1)
}

void test_ray_sphere_hit_from_inside() {
    Ray ray(Vector3f(0, 0, 0), Vector3f(1, 0, 0));  // Ray from center
    Sphere sphere(Vector3f(0, 0, 0), 2.0f);
    
    float t;
    ASSERT_TRUE(RaycastQuery::Intersects(ray, sphere, t));
    ASSERT_NEAR(t, 2.0f, 0.01f);  // Exit at radius
}

void test_ray_sphere_detailed_hit() {
    Ray ray(Vector3f(-5, 0, 0), Vector3f(1, 0, 0));
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    
    RaycastHit hit = RaycastQuery::Test(ray, sphere);
    ASSERT_TRUE(hit.hit);
    ASSERT_NEAR(hit.distance, 4.0f, 0.01f);
    ASSERT_NEAR(hit.point.x, -1.0f, 0.01f);
    ASSERT_NEAR(hit.normal.x, -1.0f, 0.01f);
}

void test_ray_sphere_tangent() {
    Ray ray(Vector3f(-5, 1, 0), Vector3f(1, 0, 0));  // Ray at tangent height
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    
    float t;
    ASSERT_TRUE(RaycastQuery::Intersects(ray, sphere, t));
}

// ============================================================================
// Ray-Capsule Tests
// ============================================================================

void test_ray_capsule_miss() {
    Ray ray(Vector3f(0, 0, -5), Vector3f(0, 1, 0));
    Capsule capsule(Vector3f(-1, 0, 0), Vector3f(1, 0, 0), 0.5f);
    
    // Ray pointing away from capsule
    ASSERT_FALSE(RaycastQuery::Intersects(ray, capsule));
}

void test_ray_capsule_hit() {
    Ray ray(Vector3f(0, 0, -5), Vector3f(0, 0, 1));
    Capsule capsule(Vector3f(-1, 0, 0), Vector3f(1, 0, 0), 0.5f);
    
    ASSERT_TRUE(RaycastQuery::Intersects(ray, capsule));
}

void test_ray_capsule_detailed_hit() {
    Ray ray(Vector3f(0, 0, -5), Vector3f(0, 0, 1));
    Capsule capsule(Vector3f(-1, 0, 0), Vector3f(1, 0, 0), 0.5f);
    
    RaycastHit hit = RaycastQuery::Test(ray, capsule);
    ASSERT_TRUE(hit.hit);
    ASSERT_TRUE(hit.distance > 0.0f);
}

// ============================================================================
// Ray-AABB Tests
// ============================================================================

void test_ray_aabb_miss() {
    Ray ray(Vector3f(0, 0, -5), Vector3f(0, 1, 0));
    AABB box;
    box.SetMinMax(Vector3f(2, 2, 2), Vector3f(3, 3, 3));
    
    ASSERT_FALSE(RaycastQuery::Intersects(ray, box));
}

void test_ray_aabb_hit_center() {
    Ray ray(Vector3f(0.5f, 0.5f, -5), Vector3f(0, 0, 1));
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    
    float t;
    ASSERT_TRUE(RaycastQuery::Intersects(ray, box, t));
    ASSERT_NEAR(t, 5.0f, 0.01f);
}

void test_ray_aabb_hit_corner() {
    Ray ray(Vector3f(-5, -5, -5), Normalized(Vector3f(1, 1, 1)));
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    
    ASSERT_TRUE(RaycastQuery::Intersects(ray, box));
}

void test_ray_aabb_from_inside() {
    Ray ray(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(1, 0, 0));
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    
    float t;
    ASSERT_TRUE(RaycastQuery::Intersects(ray, box, t));
}

void test_ray_aabb_detailed_hit() {
    Ray ray(Vector3f(0.5f, 0.5f, -5), Vector3f(0, 0, 1));
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    
    RaycastHit hit = RaycastQuery::Test(ray, box);
    ASSERT_TRUE(hit.hit);
    ASSERT_NEAR(hit.distance, 5.0f, 0.01f);
    ASSERT_NEAR(hit.point.z, 0.0f, 0.01f);
}

// ============================================================================
// Ray-Plane Tests
// ============================================================================

void test_ray_plane_parallel_miss() {
    Ray ray(Vector3f(0, 1, 0), Vector3f(1, 0, 0));  // Parallel to plane
    Plane plane;
    plane.Set(Vector3f(0, 0, 0), Vector3f(0, 1, 0));  // XZ plane
    
    ASSERT_FALSE(RaycastQuery::Intersects(ray, plane));
}

void test_ray_plane_hit() {
    Ray ray(Vector3f(0, 5, 0), Vector3f(0, -1, 0));  // Ray pointing down
    Plane plane;
    plane.Set(Vector3f(0, 0, 0), Vector3f(0, 1, 0));  // XZ plane at y=0
    
    float t;
    ASSERT_TRUE(RaycastQuery::Intersects(ray, plane, t));
    ASSERT_NEAR(t, 5.0f, 0.01f);
}

void test_ray_plane_detailed_hit() {
    Ray ray(Vector3f(0, 5, 0), Vector3f(0, -1, 0));
    Plane plane;
    plane.Set(Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    
    RaycastHit hit = RaycastQuery::Test(ray, plane);
    ASSERT_TRUE(hit.hit);
    ASSERT_NEAR(hit.distance, 5.0f, 0.01f);
    ASSERT_NEAR(hit.point.y, 0.0f, 0.01f);
}

void test_ray_plane_behind_ray() {
    Ray ray(Vector3f(0, 5, 0), Vector3f(0, 1, 0));  // Ray pointing away
    Plane plane;
    plane.Set(Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    
    ASSERT_FALSE(RaycastQuery::Intersects(ray, plane));
}

// ============================================================================
// Ray-Triangle Tests
// ============================================================================

void test_ray_triangle_hit() {
    Ray ray(Vector3f(0.5f, 0.5f, -5), Vector3f(0, 0, 1));
    Vector3f v0(0, 0, 0);
    Vector3f v1(1, 0, 0);
    Vector3f v2(0, 1, 0);
    
    float t;
    ASSERT_TRUE(RaycastQuery::IntersectsTriangle(ray, v0, v1, v2, t));
    ASSERT_NEAR(t, 5.0f, 0.01f);
}

void test_ray_triangle_miss() {
    Ray ray(Vector3f(2, 2, -5), Vector3f(0, 0, 1));  // Ray outside triangle
    Vector3f v0(0, 0, 0);
    Vector3f v1(1, 0, 0);
    Vector3f v2(0, 1, 0);
    
    float t;
    ASSERT_FALSE(RaycastQuery::IntersectsTriangle(ray, v0, v1, v2, t));
}

void test_ray_triangle_edge_hit() {
    Ray ray(Vector3f(0.5f, 0, -5), Vector3f(0, 0, 1));  // Ray at edge
    Vector3f v0(0, 0, 0);
    Vector3f v1(1, 0, 0);
    Vector3f v2(0, 1, 0);
    
    float t;
    ASSERT_TRUE(RaycastQuery::IntersectsTriangle(ray, v0, v1, v2, t));
}

void test_ray_triangle_barycentric() {
    Ray ray(Vector3f(0.5f, 0.5f, -5), Vector3f(0, 0, 1));
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(0, 2, 0);
    
    float t, u, v;
    ASSERT_TRUE(RaycastQuery::IntersectsTriangle(ray, v0, v1, v2, t, u, v));
    
    // Check barycentric coordinates
    ASSERT_TRUE(u >= 0.0f && u <= 1.0f);
    ASSERT_TRUE(v >= 0.0f && v <= 1.0f);
    ASSERT_TRUE(u + v <= 1.0f);
}

// ============================================================================
// Edge Cases and Special Scenarios
// ============================================================================

void test_ray_origin_on_sphere_surface() {
    Ray ray(Vector3f(1, 0, 0), Vector3f(1, 0, 0));  // Origin on surface
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    
    float t;
    ASSERT_TRUE(RaycastQuery::Intersects(ray, sphere, t));
}

void test_ray_zero_direction() {
    // Edge case: ray with zero direction should not crash
    Ray ray(Vector3f(0, 0, 0), Vector3f(0, 0, 0));
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    
    // Behavior may vary, but should not crash
    float t;
    RaycastQuery::Intersects(ray, sphere, t);
}

void test_multiple_rays_same_sphere() {
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    
    // Test from different directions
    Ray ray1(Vector3f(-5, 0, 0), Vector3f(1, 0, 0));
    Ray ray2(Vector3f(0, -5, 0), Vector3f(0, 1, 0));
    Ray ray3(Vector3f(0, 0, -5), Vector3f(0, 0, 1));
    
    ASSERT_TRUE(RaycastQuery::Intersects(ray1, sphere));
    ASSERT_TRUE(RaycastQuery::Intersects(ray2, sphere));
    ASSERT_TRUE(RaycastQuery::Intersects(ray3, sphere));
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "=== Raycast Query Test Suite ===" << std::endl << std::endl;
    
    std::cout << "--- Ray-Sphere Tests ---" << std::endl;
    TEST(ray_sphere_miss);
    TEST(ray_sphere_hit_from_outside);
    TEST(ray_sphere_hit_from_inside);
    TEST(ray_sphere_detailed_hit);
    TEST(ray_sphere_tangent);
    
    std::cout << std::endl << "--- Ray-Capsule Tests ---" << std::endl;
    TEST(ray_capsule_miss);
    TEST(ray_capsule_hit);
    TEST(ray_capsule_detailed_hit);
    
    std::cout << std::endl << "--- Ray-AABB Tests ---" << std::endl;
    TEST(ray_aabb_miss);
    TEST(ray_aabb_hit_center);
    TEST(ray_aabb_hit_corner);
    TEST(ray_aabb_from_inside);
    TEST(ray_aabb_detailed_hit);
    
    std::cout << std::endl << "--- Ray-Plane Tests ---" << std::endl;
    TEST(ray_plane_parallel_miss);
    TEST(ray_plane_hit);
    TEST(ray_plane_detailed_hit);
    TEST(ray_plane_behind_ray);
    
    std::cout << std::endl << "--- Ray-Triangle Tests ---" << std::endl;
    TEST(ray_triangle_hit);
    TEST(ray_triangle_miss);
    TEST(ray_triangle_edge_hit);
    TEST(ray_triangle_barycentric);
    
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    TEST(ray_origin_on_sphere_surface);
    TEST(ray_zero_direction);
    TEST(multiple_rays_same_sphere);
    
    std::cout << std::endl << "=== All Tests Passed! ===" << std::endl;
    
    return 0;
}
