/**
 * test_distance_query.cpp
 *
 * Test cases for DistanceQuery class
 * Tests distance calculations and closest point queries.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/queries/DistanceQuery.h>
#include <hgl/math/geometry/primitives/Sphere.h>
#include <hgl/math/geometry/primitives/Capsule.h>
#include <hgl/math/geometry/primitives/Cylinder.h>
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
// Point-to-Geometry Distance Tests
// ============================================================================

void test_point_to_sphere_outside() {
    Vector3f point(5, 0, 0);
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);

    float dist = DistanceQuery::Distance(point, sphere);
    ASSERT_NEAR(dist, 4.0f, 0.01f);
}

void test_point_to_sphere_inside() {
    Vector3f point(0.5f, 0, 0);
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);

    float dist = DistanceQuery::Distance(point, sphere);
    ASSERT_NEAR(dist, 0.0f, 0.01f);
}

void test_point_to_sphere_on_surface() {
    Vector3f point(1, 0, 0);
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);

    float dist = DistanceQuery::Distance(point, sphere);
    ASSERT_NEAR(dist, 0.0f, 0.01f);
}

void test_point_to_capsule() {
    Vector3f point(2, 0, 0);
    Capsule capsule(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);

    float dist = DistanceQuery::Distance(point, capsule);
    ASSERT_NEAR(dist, 1.5f, 0.01f);
}

void test_point_to_aabb_outside() {
    Vector3f point(3, 3, 3);
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    float dist = DistanceQuery::Distance(point, box);
    float expectedDist = sqrt(2.0f * 2.0f + 2.0f * 2.0f + 2.0f * 2.0f);
    ASSERT_NEAR(dist, expectedDist, 0.01f);
}

void test_point_to_aabb_inside() {
    Vector3f point(0.5f, 0.5f, 0.5f);
    AABB box;
    box.SetMinMax(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    float dist = DistanceQuery::Distance(point, box);
    ASSERT_NEAR(dist, 0.0f, 0.01f);
}

void test_point_to_cylinder() {
    Vector3f point(2, 0, 0);
    Cylinder cylinder(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 0.5f);

    float dist = DistanceQuery::Distance(point, cylinder);
    ASSERT_NEAR(dist, 1.5f, 0.01f);
}

// ============================================================================
// Sphere-to-Geometry Distance Tests
// ============================================================================

void test_sphere_sphere_separated() {
    Sphere s1(Vector3f(0, 0, 0), 1.0f);
    Sphere s2(Vector3f(5, 0, 0), 1.0f);

    float dist = DistanceQuery::Distance(s1, s2);
    ASSERT_NEAR(dist, 3.0f, 0.01f);
}

void test_sphere_sphere_touching() {
    Sphere s1(Vector3f(0, 0, 0), 1.0f);
    Sphere s2(Vector3f(2, 0, 0), 1.0f);

    float dist = DistanceQuery::Distance(s1, s2);
    ASSERT_NEAR(dist, 0.0f, 0.01f);
}

void test_sphere_sphere_overlapping() {
    Sphere s1(Vector3f(0, 0, 0), 1.0f);
    Sphere s2(Vector3f(1, 0, 0), 1.0f);

    float dist = DistanceQuery::Distance(s1, s2);
    ASSERT_NEAR(dist, 0.0f, 0.01f);  // Overlapping returns 0
}

void test_sphere_capsule_distance() {
    Sphere sphere(Vector3f(3, 0, 0), 0.5f);
    Capsule capsule(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);

    float dist = DistanceQuery::Distance(sphere, capsule);
    ASSERT_NEAR(dist, 2.0f, 0.01f);
}

// ============================================================================
// Capsule-to-Geometry Distance Tests
// ============================================================================

void test_capsule_capsule_parallel_separated() {
    Capsule c1(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);
    Capsule c2(Vector3f(3, -1, 0), Vector3f(3, 1, 0), 0.5f);

    float dist = DistanceQuery::Distance(c1, c2);
    ASSERT_NEAR(dist, 2.0f, 0.01f);
}

void test_capsule_capsule_perpendicular() {
    Capsule c1(Vector3f(-1, 0, 0), Vector3f(1, 0, 0), 0.3f);
    Capsule c2(Vector3f(0, 2, 0), Vector3f(0, 4, 0), 0.3f);

    float dist = DistanceQuery::Distance(c1, c2);
    ASSERT_NEAR(dist, 1.4f, 0.1f);
}

// ============================================================================
// Closest Point Helper Tests
// ============================================================================

void test_closest_point_on_line_segment() {
    Vector3f point(1, 1, 0);
    Vector3f segStart(0, 0, 0);
    Vector3f segEnd(2, 0, 0);

    Vector3f closest = DistanceQuery::ClosestPointOnLineSegment(point, segStart, segEnd);
    ASSERT_NEAR(closest.x, 1.0f, 0.01f);
    ASSERT_NEAR(closest.y, 0.0f, 0.01f);
    ASSERT_NEAR(closest.z, 0.0f, 0.01f);
}

void test_closest_point_on_line_segment_endpoint() {
    Vector3f point(-1, 0, 0);
    Vector3f segStart(0, 0, 0);
    Vector3f segEnd(2, 0, 0);

    Vector3f closest = DistanceQuery::ClosestPointOnLineSegment(point, segStart, segEnd);
    ASSERT_NEAR(closest.x, 0.0f, 0.01f);  // Clamped to start
}

void test_closest_points_on_segments_parallel() {
    Vector3f seg1Start(0, 0, 0);
    Vector3f seg1End(2, 0, 0);
    Vector3f seg2Start(0, 1, 0);
    Vector3f seg2End(2, 1, 0);

    ClosestPointsResult result = DistanceQuery::ClosestPointsOnLineSegments(
        seg1Start, seg1End, seg2Start, seg2End
    );

    ASSERT_NEAR(result.distance, 1.0f, 0.01f);
}

void test_closest_points_on_segments_perpendicular() {
    Vector3f seg1Start(-1, 0, 0);
    Vector3f seg1End(1, 0, 0);
    Vector3f seg2Start(0, -1, 0);
    Vector3f seg2End(0, 1, 0);

    ClosestPointsResult result = DistanceQuery::ClosestPointsOnLineSegments(
        seg1Start, seg1End, seg2Start, seg2End
    );

    ASSERT_NEAR(result.distance, 0.0f, 0.01f);  // They intersect at origin
    ASSERT_NEAR(result.pointOnA.x, 0.0f, 0.01f);
    ASSERT_NEAR(result.pointOnB.x, 0.0f, 0.01f);
}

void test_closest_points_on_segments_skew() {
    Vector3f seg1Start(0, 0, 0);
    Vector3f seg1End(1, 0, 0);
    Vector3f seg2Start(0.5f, 1, 1);
    Vector3f seg2End(0.5f, 2, 1);

    ClosestPointsResult result = DistanceQuery::ClosestPointsOnLineSegments(
        seg1Start, seg1End, seg2Start, seg2End
    );

    ASSERT_NEAR(result.pointOnA.x, 0.5f, 0.01f);
    ASSERT_NEAR(result.pointOnA.y, 0.0f, 0.01f);
    ASSERT_TRUE(result.distance > 0.0f);
}

// ============================================================================
// Closest Point Pairs Tests
// ============================================================================

void test_closest_points_sphere_capsule() {
    Sphere sphere(Vector3f(3, 0, 0), 0.5f);
    Capsule capsule(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);

    ClosestPointsResult result = DistanceQuery::ClosestPoints(sphere, capsule);

    ASSERT_TRUE(result.distance > 0.0f);
    ASSERT_NEAR(result.pointOnA.x, 2.5f, 0.01f);  // Point on sphere toward capsule
}

void test_closest_points_capsule_capsule() {
    Capsule c1(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.3f);
    Capsule c2(Vector3f(2, -1, 0), Vector3f(2, 1, 0), 0.3f);

    ClosestPointsResult result = DistanceQuery::ClosestPoints(c1, c2);

    ASSERT_NEAR(result.distance, 1.4f, 0.01f);
    ASSERT_NEAR(result.pointOnA.x, 0.3f, 0.01f);
    ASSERT_NEAR(result.pointOnB.x, 1.7f, 0.01f);
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_distance_zero_length_segment() {
    Vector3f point(1, 0, 0);
    Vector3f segStart(0, 0, 0);
    Vector3f segEnd(0, 0, 0);  // Zero-length segment

    Vector3f closest = DistanceQuery::ClosestPointOnLineSegment(point, segStart, segEnd);
    ASSERT_NEAR(closest.x, 0.0f, 0.01f);
}

void test_distance_coincident_spheres() {
    Sphere s1(Vector3f(0, 0, 0), 1.0f);
    Sphere s2(Vector3f(0, 0, 0), 1.0f);

    float dist = DistanceQuery::Distance(s1, s2);
    ASSERT_NEAR(dist, 0.0f, 0.01f);
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "=== Distance Query Test Suite ===" << std::endl << std::endl;

    std::cout << "--- Point-to-Geometry Distance Tests ---" << std::endl;
    TEST(point_to_sphere_outside);
    TEST(point_to_sphere_inside);
    TEST(point_to_sphere_on_surface);
    TEST(point_to_capsule);
    TEST(point_to_aabb_outside);
    TEST(point_to_aabb_inside);
    TEST(point_to_cylinder);

    std::cout << std::endl << "--- Sphere-to-Geometry Distance Tests ---" << std::endl;
    TEST(sphere_sphere_separated);
    TEST(sphere_sphere_touching);
    TEST(sphere_sphere_overlapping);
    TEST(sphere_capsule_distance);

    std::cout << std::endl << "--- Capsule-to-Geometry Distance Tests ---" << std::endl;
    TEST(capsule_capsule_parallel_separated);
    TEST(capsule_capsule_perpendicular);

    std::cout << std::endl << "--- Closest Point Helper Tests ---" << std::endl;
    TEST(closest_point_on_line_segment);
    TEST(closest_point_on_line_segment_endpoint);
    TEST(closest_points_on_segments_parallel);
    TEST(closest_points_on_segments_perpendicular);
    TEST(closest_points_on_segments_skew);

    std::cout << std::endl << "--- Closest Point Pairs Tests ---" << std::endl;
    TEST(closest_points_sphere_capsule);
    TEST(closest_points_capsule_capsule);

    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    TEST(distance_zero_length_segment);
    TEST(distance_coincident_spheres);

    std::cout << std::endl << "=== All Tests Passed! ===" << std::endl;

    return 0;
}
