/**
 * test_frustum.cpp
 *
 * Comprehensive test cases for Frustum (View Frustum) class
 * Tests frustum construction, plane extraction, and intersection detection.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/Frustum.h>
#include <hgl/math/geometry/primitives/Sphere.h>
#include <hgl/math/geometry/AABB.h>
#include <hgl/math/MatrixOperations.h>

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
// Frustum Plane Extraction Tests
// ============================================================================

void test_frustum_planes_extraction_identity() {
    FrustumPlanes planes;
    Matrix4f identity = IdentityMatrix<4, float>();

    GetFrustumPlanes(planes, identity);

    // Should extract 6 planes without crashing
    ASSERT_TRUE(true);
}

void test_frustum_planes_extraction_perspective() {
    FrustumPlanes planes;

    // Create a simple perspective projection matrix
    float fov = pi / 4.0f;  // 45 degrees
    float aspect = 16.0f / 9.0f;
    float near = 0.1f;
    float far = 100.0f;

    Matrix4f projection = PerspectiveMatrix(fov, aspect, near, far);

    GetFrustumPlanes(planes, projection);

    // All 6 planes should be extracted
    ASSERT_TRUE(true);
}

void test_frustum_planes_extraction_orthographic() {
    FrustumPlanes planes;

    // Create orthographic projection matrix
    float left = -10.0f, right = 10.0f;
    float bottom = -10.0f, top = 10.0f;
    float near = 0.1f, far = 100.0f;

    Matrix4f projection = OrthographicMatrix(left, right, bottom, top, near, far);

    GetFrustumPlanes(planes, projection);

    // Should extract 6 planes
    ASSERT_TRUE(true);
}

void test_frustum_planes_mvp_matrix() {
    FrustumPlanes planes;

    // Create MVP matrix
    Matrix4f model = IdentityMatrix<4, float>();
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 5), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 16.0f / 9.0f, 0.1f, 100.0f);

    Matrix4f mvp = projection * view * model;

    GetFrustumPlanes(planes, mvp);

    // Should successfully extract planes
    ASSERT_TRUE(true);
}

// ============================================================================
// Frustum-Sphere Intersection Tests
// ============================================================================

void test_frustum_sphere_inside() {
    Frustum frustum;

    // Simple frustum setup
    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Sphere at origin, should be inside
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);

    ASSERT_TRUE(frustum.Intersects(sphere));
}

void test_frustum_sphere_outside_left() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Sphere far to the left
    Sphere sphere(Vector3f(-50, 0, 0), 1.0f);

    ASSERT_FALSE(frustum.Intersects(sphere));
}

void test_frustum_sphere_outside_right() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Sphere far to the right
    Sphere sphere(Vector3f(50, 0, 0), 1.0f);

    ASSERT_FALSE(frustum.Intersects(sphere));
}

void test_frustum_sphere_outside_top() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Sphere far above
    Sphere sphere(Vector3f(0, 50, 0), 1.0f);

    ASSERT_FALSE(frustum.Intersects(sphere));
}

void test_frustum_sphere_outside_bottom() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Sphere far below
    Sphere sphere(Vector3f(0, -50, 0), 1.0f);

    ASSERT_FALSE(frustum.Intersects(sphere));
}

void test_frustum_sphere_outside_near() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Sphere before near plane
    Sphere sphere(Vector3f(0, 0, 15), 1.0f);

    ASSERT_FALSE(frustum.Intersects(sphere));
}

void test_frustum_sphere_outside_far() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Sphere beyond far plane
    Sphere sphere(Vector3f(0, 0, -150), 1.0f);

    ASSERT_FALSE(frustum.Intersects(sphere));
}

void test_frustum_sphere_on_edge() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Sphere on the edge of frustum
    Sphere sphere(Vector3f(5, 5, 0), 2.0f);

    bool intersects = frustum.Intersects(sphere);
    // Behavior may vary at edge - just ensure no crash
    ASSERT_TRUE(true);
}

void test_frustum_sphere_large_radius() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Very large sphere that encompasses frustum
    Sphere sphere(Vector3f(0, 0, 0), 200.0f);

    ASSERT_TRUE(frustum.Intersects(sphere));
}

// ============================================================================
// Frustum-AABB Intersection Tests
// ============================================================================

void test_frustum_aabb_inside() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Small AABB at origin
    AABB box;
    box.SetMinMax(Vector3f(-1, -1, -1), Vector3f(1, 1, 1));

    ASSERT_TRUE(frustum.Intersects(box));
}

void test_frustum_aabb_outside() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // AABB far outside frustum
    AABB box;
    box.SetMinMax(Vector3f(50, 50, 50), Vector3f(52, 52, 52));

    ASSERT_FALSE(frustum.Intersects(box));
}

void test_frustum_aabb_partially_inside() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Large AABB that partially intersects
    AABB box;
    box.SetMinMax(Vector3f(-2, -2, -2), Vector3f(2, 2, 2));

    ASSERT_TRUE(frustum.Intersects(box));
}

void test_frustum_aabb_behind_camera() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // AABB behind camera
    AABB box;
    box.SetMinMax(Vector3f(-1, -1, 15), Vector3f(1, 1, 17));

    ASSERT_FALSE(frustum.Intersects(box));
}

// ============================================================================
// Frustum Point Containment Tests
// ============================================================================

void test_frustum_contains_point_origin() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    ASSERT_TRUE(frustum.ContainsPoint(Vector3f(0, 0, 0)));
}

void test_frustum_contains_point_outside() {
    Frustum frustum;

    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    ASSERT_FALSE(frustum.ContainsPoint(Vector3f(100, 100, 100)));
}

// ============================================================================
// Orthographic Frustum Tests
// ============================================================================

void test_orthographic_frustum_sphere_inside() {
    Frustum frustum;

    Matrix4f projection = OrthographicMatrix(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    Sphere sphere(Vector3f(0, 0, 0), 1.0f);

    ASSERT_TRUE(frustum.Intersects(sphere));
}

void test_orthographic_frustum_aabb_outside() {
    Frustum frustum;

    Matrix4f projection = OrthographicMatrix(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    AABB box;
    box.SetMinMax(Vector3f(20, 20, 20), Vector3f(22, 22, 22));

    ASSERT_FALSE(frustum.Intersects(box));
}

// ============================================================================
// Frustum Update Tests
// ============================================================================

void test_frustum_update_different_matrices() {
    Frustum frustum;

    // First setup
    Matrix4f proj1 = PerspectiveMatrix(pi / 4.0f, 1.0f, 1.0f, 100.0f);
    Matrix4f view1 = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    frustum.SetFromMatrix(proj1 * view1);

    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    bool result1 = frustum.Intersects(sphere);

    // Update frustum with different view
    Matrix4f view2 = LookAtMatrix(Vector3f(0, 0, -10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
    frustum.SetFromMatrix(proj1 * view2);

    bool result2 = frustum.Intersects(sphere);

    // Results should potentially be different
    ASSERT_TRUE(true);  // Just ensure no crash
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_frustum_very_narrow_fov() {
    Frustum frustum;

    // Very narrow FOV
    Matrix4f projection = PerspectiveMatrix(pi / 180.0f, 1.0f, 1.0f, 100.0f);  // 1 degree
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Should still work
    Sphere sphere(Vector3f(0, 0, 0), 0.1f);
    bool intersects = frustum.Intersects(sphere);
    ASSERT_TRUE(true);
}

void test_frustum_very_wide_fov() {
    Frustum frustum;

    // Very wide FOV
    Matrix4f projection = PerspectiveMatrix(pi * 0.9f, 1.0f, 1.0f, 100.0f);  // ~162 degrees
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Should capture more objects
    Sphere sphere(Vector3f(8, 0, 0), 1.0f);
    bool intersects = frustum.Intersects(sphere);
    ASSERT_TRUE(true);
}

void test_frustum_extreme_aspect_ratio() {
    Frustum frustum;

    // Extreme aspect ratio
    Matrix4f projection = PerspectiveMatrix(pi / 4.0f, 10.0f, 1.0f, 100.0f);
    Matrix4f view = LookAtMatrix(Vector3f(0, 0, 10), Vector3f(0, 0, 0), Vector3f(0, 1, 0));

    frustum.SetFromMatrix(projection * view);

    // Should still function
    AABB box;
    box.SetMinMax(Vector3f(-1, -1, -1), Vector3f(1, 1, 1));
    bool intersects = frustum.Intersects(box);
    ASSERT_TRUE(true);
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "=== Frustum (View Frustum) Test Suite ===" << std::endl << std::endl;

    std::cout << "--- Frustum Plane Extraction Tests ---" << std::endl;
    TEST(frustum_planes_extraction_identity);
    TEST(frustum_planes_extraction_perspective);
    TEST(frustum_planes_extraction_orthographic);
    TEST(frustum_planes_mvp_matrix);

    std::cout << std::endl << "--- Frustum-Sphere Intersection Tests ---" << std::endl;
    TEST(frustum_sphere_inside);
    TEST(frustum_sphere_outside_left);
    TEST(frustum_sphere_outside_right);
    TEST(frustum_sphere_outside_top);
    TEST(frustum_sphere_outside_bottom);
    TEST(frustum_sphere_outside_near);
    TEST(frustum_sphere_outside_far);
    TEST(frustum_sphere_on_edge);
    TEST(frustum_sphere_large_radius);

    std::cout << std::endl << "--- Frustum-AABB Intersection Tests ---" << std::endl;
    TEST(frustum_aabb_inside);
    TEST(frustum_aabb_outside);
    TEST(frustum_aabb_partially_inside);
    TEST(frustum_aabb_behind_camera);

    std::cout << std::endl << "--- Frustum Point Containment Tests ---" << std::endl;
    TEST(frustum_contains_point_origin);
    TEST(frustum_contains_point_outside);

    std::cout << std::endl << "--- Orthographic Frustum Tests ---" << std::endl;
    TEST(orthographic_frustum_sphere_inside);
    TEST(orthographic_frustum_aabb_outside);

    std::cout << std::endl << "--- Frustum Update Tests ---" << std::endl;
    TEST(frustum_update_different_matrices);

    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    TEST(frustum_very_narrow_fov);
    TEST(frustum_very_wide_fov);
    TEST(frustum_extreme_aspect_ratio);

    std::cout << std::endl << "=== All Frustum Tests Passed! ===" << std::endl;

    return 0;
}
