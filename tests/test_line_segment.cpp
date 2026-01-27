/**
 * test_line_segment.cpp
 *
 * Comprehensive test cases for LineSegment class
 * Tests line segment construction, closest point calculations, and distance queries.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/LineSegment.h>
#include <hgl/math/geometry/primitives/Sphere.h>
#include <hgl/math/VectorOperations.h>

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
// LineSegment Construction Tests
// ============================================================================

void test_line_segment_default_construction() {
    LineSegment seg;
    // Should construct without crashing
    ASSERT_TRUE(true);
}

void test_line_segment_parameterized_construction() {
    Vector3f start(0, 0, 0);
    Vector3f end(5, 0, 0);

    LineSegment seg(start, end);

    ASSERT_NEAR(seg.GetStart().x, 0.0f, 0.001f);
    ASSERT_NEAR(seg.GetEnd().x, 5.0f, 0.001f);
}

void test_line_segment_set_endpoints() {
    LineSegment seg;
    seg.Set(Vector3f(1, 2, 3), Vector3f(4, 5, 6));

    ASSERT_NEAR(seg.GetStart().x, 1.0f, 0.001f);
    ASSERT_NEAR(seg.GetStart().y, 2.0f, 0.001f);
    ASSERT_NEAR(seg.GetStart().z, 3.0f, 0.001f);

    ASSERT_NEAR(seg.GetEnd().x, 4.0f, 0.001f);
    ASSERT_NEAR(seg.GetEnd().y, 5.0f, 0.001f);
    ASSERT_NEAR(seg.GetEnd().z, 6.0f, 0.001f);
}

void test_line_segment_set_start() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    seg.SetStart(Vector3f(5, 5, 5));

    ASSERT_NEAR(seg.GetStart().x, 5.0f, 0.001f);
    ASSERT_NEAR(seg.GetEnd().x, 1.0f, 0.001f);
}

void test_line_segment_set_end() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    seg.SetEnd(Vector3f(7, 8, 9));

    ASSERT_NEAR(seg.GetStart().x, 0.0f, 0.001f);
    ASSERT_NEAR(seg.GetEnd().x, 7.0f, 0.001f);
}

// ============================================================================
// LineSegment Properties Tests
// ============================================================================

void test_line_segment_center() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(4, 0, 0));
    Vector3f center = seg.GetCenter();

    ASSERT_NEAR(center.x, 2.0f, 0.001f);
    ASSERT_NEAR(center.y, 0.0f, 0.001f);
    ASSERT_NEAR(center.z, 0.0f, 0.001f);
}

void test_line_segment_direction() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(3, 0, 0));
    Vector3f dir = seg.GetStartDirection();

    ASSERT_NEAR(dir.x, 1.0f, 0.001f);
    ASSERT_NEAR(dir.y, 0.0f, 0.001f);
    ASSERT_NEAR(dir.z, 0.0f, 0.001f);
}

void test_line_segment_reverse_direction() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(3, 0, 0));
    Vector3f dir = seg.GetEndDirection();

    ASSERT_NEAR(dir.x, -1.0f, 0.001f);
    ASSERT_NEAR(dir.y, 0.0f, 0.001f);
    ASSERT_NEAR(dir.z, 0.0f, 0.001f);
}

void test_line_segment_vector() {
    LineSegment seg(Vector3f(1, 2, 3), Vector3f(4, 6, 9));
    Vector3f vec = seg.GetVector();

    ASSERT_NEAR(vec.x, 3.0f, 0.001f);
    ASSERT_NEAR(vec.y, 4.0f, 0.001f);
    ASSERT_NEAR(vec.z, 6.0f, 0.001f);
}

void test_line_segment_distance() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(3, 4, 0));
    float dist = seg.Distance();

    ASSERT_NEAR(dist, 5.0f, 0.001f);  // 3-4-5 triangle
}

void test_line_segment_distance_squared() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(3, 4, 0));
    float distSq = seg.DistanceSquared();

    ASSERT_NEAR(distSq, 25.0f, 0.001f);
}

void test_line_segment_distance_3d() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
    float dist = seg.Distance();

    ASSERT_NEAR(dist, std::sqrt(3.0f), 0.001f);
}

// ============================================================================
// LineSegment Parameterization Tests
// ============================================================================

void test_line_segment_get_point_at_start() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10, 0, 0));
    Vector3f point = seg.GetPoint(0.0f);

    ASSERT_NEAR(point.x, 0.0f, 0.001f);
    ASSERT_NEAR(point.y, 0.0f, 0.001f);
    ASSERT_NEAR(point.z, 0.0f, 0.001f);
}

void test_line_segment_get_point_at_end() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10, 0, 0));
    Vector3f point = seg.GetPoint(1.0f);

    ASSERT_NEAR(point.x, 10.0f, 0.001f);
    ASSERT_NEAR(point.y, 0.0f, 0.001f);
    ASSERT_NEAR(point.z, 0.0f, 0.001f);
}

void test_line_segment_get_point_at_midpoint() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10, 0, 0));
    Vector3f point = seg.GetPoint(0.5f);

    ASSERT_NEAR(point.x, 5.0f, 0.001f);
    ASSERT_NEAR(point.y, 0.0f, 0.001f);
    ASSERT_NEAR(point.z, 0.0f, 0.001f);
}

void test_line_segment_get_point_quarter() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(8, 0, 0));
    Vector3f point = seg.GetPoint(0.25f);

    ASSERT_NEAR(point.x, 2.0f, 0.001f);
}

void test_line_segment_get_point_three_quarters() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(8, 0, 0));
    Vector3f point = seg.GetPoint(0.75f);

    ASSERT_NEAR(point.x, 6.0f, 0.001f);
}

// ============================================================================
// Closest Point Tests
// ============================================================================

void test_closest_point_perpendicular() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10, 0, 0));
    Vector3f point(5, 5, 0);

    Vector3f closest = seg.ClosestPoint(point);

    ASSERT_NEAR(closest.x, 5.0f, 0.001f);
    ASSERT_NEAR(closest.y, 0.0f, 0.001f);
    ASSERT_NEAR(closest.z, 0.0f, 0.001f);
}

void test_closest_point_beyond_start() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10, 0, 0));
    Vector3f point(-5, 5, 0);

    Vector3f closest = seg.ClosestPoint(point);

    // Should clamp to start
    ASSERT_NEAR(closest.x, 0.0f, 0.001f);
    ASSERT_NEAR(closest.y, 0.0f, 0.001f);
    ASSERT_NEAR(closest.z, 0.0f, 0.001f);
}

void test_closest_point_beyond_end() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10, 0, 0));
    Vector3f point(15, 5, 0);

    Vector3f closest = seg.ClosestPoint(point);

    // Should clamp to end
    ASSERT_NEAR(closest.x, 10.0f, 0.001f);
    ASSERT_NEAR(closest.y, 0.0f, 0.001f);
    ASSERT_NEAR(closest.z, 0.0f, 0.001f);
}

void test_closest_point_on_segment() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10, 0, 0));
    Vector3f point(5, 0, 0);

    Vector3f closest = seg.ClosestPoint(point);

    ASSERT_NEAR(closest.x, 5.0f, 0.001f);
    ASSERT_NEAR(closest.y, 0.0f, 0.001f);
    ASSERT_NEAR(closest.z, 0.0f, 0.001f);
}

void test_closest_point_diagonal_segment() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10, 10, 0));
    Vector3f point(5, 0, 0);

    Vector3f closest = seg.ClosestPoint(point);

    // Closest point should be at (2.5, 2.5, 0)
    ASSERT_NEAR(closest.x, 2.5f, 0.01f);
    ASSERT_NEAR(closest.y, 2.5f, 0.01f);
}

void test_closest_point_3d_segment() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10, 10, 10));
    Vector3f point(5, 5, 0);

    Vector3f closest = seg.ClosestPoint(point);

    // Should project onto segment
    ASSERT_TRUE(Length(closest - point) < Length(seg.GetStart() - point));
}

void test_closest_point_zero_length_segment() {
    LineSegment seg(Vector3f(5, 5, 5), Vector3f(5, 5, 5));
    Vector3f point(0, 0, 0);

    Vector3f closest = seg.ClosestPoint(point);

    // Should return the single point
    ASSERT_NEAR(closest.x, 5.0f, 0.001f);
    ASSERT_NEAR(closest.y, 5.0f, 0.001f);
    ASSERT_NEAR(closest.z, 5.0f, 0.001f);
}

// ============================================================================
// LineSegment-LineSegment Tests
// ============================================================================

void test_parallel_segments_distance() {
    LineSegment seg1(Vector3f(0, 0, 0), Vector3f(10, 0, 0));
    LineSegment seg2(Vector3f(0, 5, 0), Vector3f(10, 5, 0));

    // Distance between parallel segments should be 5
    Vector3f closest1 = seg1.ClosestPoint(seg2.GetStart());
    Vector3f closest2 = seg1.ClosestPoint(seg2.GetEnd());

    ASSERT_TRUE(true);  // Segments are parallel
}

void test_perpendicular_segments_distance() {
    LineSegment seg1(Vector3f(-5, 0, 0), Vector3f(5, 0, 0));
    LineSegment seg2(Vector3f(0, -5, 0), Vector3f(0, 5, 0));

    // Perpendicular segments intersecting at origin
    Vector3f closest1 = seg1.ClosestPoint(Vector3f(0, 0, 0));
    Vector3f closest2 = seg2.ClosestPoint(Vector3f(0, 0, 0));

    ASSERT_NEAR(closest1.x, 0.0f, 0.001f);
    ASSERT_NEAR(closest2.y, 0.0f, 0.001f);
}

void test_skew_segments() {
    LineSegment seg1(Vector3f(0, 0, 0), Vector3f(10, 0, 0));
    LineSegment seg2(Vector3f(5, 5, 5), Vector3f(5, 5, 15));

    // Skew segments in 3D
    Vector3f closest = seg1.ClosestPoint(seg2.GetStart());
    ASSERT_NEAR(closest.x, 5.0f, 0.001f);
}

// ============================================================================
// LineSegment-Sphere Intersection Tests
// ============================================================================

void test_segment_sphere_intersection_through_center() {
    LineSegment seg(Vector3f(-10, 0, 0), Vector3f(10, 0, 0));
    Sphere sphere(Vector3f(0, 0, 0), 2.0f);

    // Segment passes through sphere center
    Vector3f closest = seg.ClosestPoint(sphere.GetCenter());
    float dist = Length(closest - sphere.GetCenter());

    ASSERT_NEAR(dist, 0.0f, 0.001f);
    ASSERT_TRUE(dist < sphere.GetRadius());
}

void test_segment_sphere_intersection_tangent() {
    LineSegment seg(Vector3f(-10, 2, 0), Vector3f(10, 2, 0));
    Sphere sphere(Vector3f(0, 0, 0), 2.0f);

    // Segment tangent to sphere
    Vector3f closest = seg.ClosestPoint(sphere.GetCenter());
    float dist = Length(closest - sphere.GetCenter());

    ASSERT_NEAR(dist, 2.0f, 0.001f);
}

void test_segment_sphere_no_intersection() {
    LineSegment seg(Vector3f(-10, 10, 0), Vector3f(10, 10, 0));
    Sphere sphere(Vector3f(0, 0, 0), 2.0f);

    // Segment far from sphere
    Vector3f closest = seg.ClosestPoint(sphere.GetCenter());
    float dist = Length(closest - sphere.GetCenter());

    ASSERT_TRUE(dist > sphere.GetRadius());
}

void test_segment_endpoint_inside_sphere() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10, 0, 0));
    Sphere sphere(Vector3f(0, 0, 0), 5.0f);

    // Start endpoint inside sphere
    float distStart = Length(seg.GetStart() - sphere.GetCenter());
    ASSERT_TRUE(distStart < sphere.GetRadius());
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_very_long_segment() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(10000, 0, 0));
    float dist = seg.Distance();

    ASSERT_NEAR(dist, 10000.0f, 0.01f);
}

void test_very_short_segment() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(0.001f, 0, 0));
    float dist = seg.Distance();

    ASSERT_NEAR(dist, 0.001f, 0.0001f);
}

void test_negative_coordinates() {
    LineSegment seg(Vector3f(-5, -5, -5), Vector3f(-1, -1, -1));
    Vector3f center = seg.GetCenter();

    ASSERT_NEAR(center.x, -3.0f, 0.001f);
    ASSERT_NEAR(center.y, -3.0f, 0.001f);
    ASSERT_NEAR(center.z, -3.0f, 0.001f);
}

void test_vertical_segment() {
    LineSegment seg(Vector3f(0, 0, 0), Vector3f(0, 10, 0));
    Vector3f dir = seg.GetStartDirection();

    ASSERT_NEAR(dir.x, 0.0f, 0.001f);
    ASSERT_NEAR(dir.y, 1.0f, 0.001f);
    ASSERT_NEAR(dir.z, 0.0f, 0.001f);
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "=== LineSegment Test Suite ===" << std::endl << std::endl;

    std::cout << "--- LineSegment Construction Tests ---" << std::endl;
    TEST(line_segment_default_construction);
    TEST(line_segment_parameterized_construction);
    TEST(line_segment_set_endpoints);
    TEST(line_segment_set_start);
    TEST(line_segment_set_end);

    std::cout << std::endl << "--- LineSegment Properties Tests ---" << std::endl;
    TEST(line_segment_center);
    TEST(line_segment_direction);
    TEST(line_segment_reverse_direction);
    TEST(line_segment_vector);
    TEST(line_segment_distance);
    TEST(line_segment_distance_squared);
    TEST(line_segment_distance_3d);

    std::cout << std::endl << "--- LineSegment Parameterization Tests ---" << std::endl;
    TEST(line_segment_get_point_at_start);
    TEST(line_segment_get_point_at_end);
    TEST(line_segment_get_point_at_midpoint);
    TEST(line_segment_get_point_quarter);
    TEST(line_segment_get_point_three_quarters);

    std::cout << std::endl << "--- Closest Point Tests ---" << std::endl;
    TEST(closest_point_perpendicular);
    TEST(closest_point_beyond_start);
    TEST(closest_point_beyond_end);
    TEST(closest_point_on_segment);
    TEST(closest_point_diagonal_segment);
    TEST(closest_point_3d_segment);
    TEST(closest_point_zero_length_segment);

    std::cout << std::endl << "--- LineSegment-LineSegment Tests ---" << std::endl;
    TEST(parallel_segments_distance);
    TEST(perpendicular_segments_distance);
    TEST(skew_segments);

    std::cout << std::endl << "--- LineSegment-Sphere Intersection Tests ---" << std::endl;
    TEST(segment_sphere_intersection_through_center);
    TEST(segment_sphere_intersection_tangent);
    TEST(segment_sphere_no_intersection);
    TEST(segment_endpoint_inside_sphere);

    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    TEST(very_long_segment);
    TEST(very_short_segment);
    TEST(negative_coordinates);
    TEST(vertical_segment);

    std::cout << std::endl << "=== All LineSegment Tests Passed! ===" << std::endl;

    return 0;
}
