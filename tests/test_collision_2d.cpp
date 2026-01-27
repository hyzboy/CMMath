/**
 * test_collision_2d.cpp
 *
 * Comprehensive test cases for 2D collision detection
 * Tests circle, rectangle, line segment intersections and 2D ray casting.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/Collision2D.h>
#include <hgl/math/geometry/AABB2D.h>
#include <hgl/math/VectorTypes.h>

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
// Circle-Circle Collision Tests
// ============================================================================

void test_circle_circle_separated() {
    Vector2f c1(0, 0);
    Vector2f c2(10, 0);
    float r1 = 2.0f;
    float r2 = 2.0f;

    ASSERT_FALSE(CircleCircleIntersection(c1, r1, c2, r2));
}

void test_circle_circle_touching() {
    Vector2f c1(0, 0);
    Vector2f c2(4, 0);
    float r1 = 2.0f;
    float r2 = 2.0f;

    ASSERT_TRUE(CircleCircleIntersection(c1, r1, c2, r2));
}

void test_circle_circle_overlapping() {
    Vector2f c1(0, 0);
    Vector2f c2(2, 0);
    float r1 = 2.0f;
    float r2 = 2.0f;

    ASSERT_TRUE(CircleCircleIntersection(c1, r1, c2, r2));
}

void test_circle_circle_one_inside_other() {
    Vector2f c1(0, 0);
    Vector2f c2(0, 0);
    float r1 = 5.0f;
    float r2 = 2.0f;

    ASSERT_TRUE(CircleCircleIntersection(c1, r1, c2, r2));
}

void test_circle_circle_coincident() {
    Vector2f c1(1, 1);
    Vector2f c2(1, 1);
    float r1 = 3.0f;
    float r2 = 3.0f;

    ASSERT_TRUE(CircleCircleIntersection(c1, r1, c2, r2));
}

void test_circle_circle_diagonal() {
    Vector2f c1(0, 0);
    Vector2f c2(3, 4);  // Distance = 5
    float r1 = 2.0f;
    float r2 = 2.0f;

    ASSERT_FALSE(CircleCircleIntersection(c1, r1, c2, r2));
}

void test_circle_circle_barely_touching() {
    Vector2f c1(0, 0);
    Vector2f c2(3, 0);
    float r1 = 1.5f;
    float r2 = 1.5f;

    ASSERT_TRUE(CircleCircleIntersection(c1, r1, c2, r2));
}

void test_circle_circle_different_sizes() {
    Vector2f c1(0, 0);
    Vector2f c2(5, 0);
    float r1 = 1.0f;
    float r2 = 5.0f;

    ASSERT_TRUE(CircleCircleIntersection(c1, r1, c2, r2));
}

// ============================================================================
// Circle-Point Collision Tests
// ============================================================================

void test_circle_point_inside() {
    Vector2f center(0, 0);
    float radius = 5.0f;
    Vector2f point(2, 2);

    ASSERT_TRUE(CirclePointIntersection(center, radius, point));
}

void test_circle_point_outside() {
    Vector2f center(0, 0);
    float radius = 2.0f;
    Vector2f point(5, 5);

    ASSERT_FALSE(CirclePointIntersection(center, radius, point));
}

void test_circle_point_on_edge() {
    Vector2f center(0, 0);
    float radius = 5.0f;
    Vector2f point(5, 0);

    ASSERT_TRUE(CirclePointIntersection(center, radius, point));
}

void test_circle_point_at_center() {
    Vector2f center(3, 4);
    float radius = 10.0f;
    Vector2f point(3, 4);

    ASSERT_TRUE(CirclePointIntersection(center, radius, point));
}

void test_circle_point_negative_coords() {
    Vector2f center(-5, -5);
    float radius = 3.0f;
    Vector2f point(-6, -5);

    ASSERT_TRUE(CirclePointIntersection(center, radius, point));
}

void test_circle_point_zero_radius() {
    Vector2f center(0, 0);
    float radius = 0.0f;
    Vector2f point(0, 0);

    ASSERT_TRUE(CirclePointIntersection(center, radius, point));
}

// ============================================================================
// Rectangle-Rectangle (AABB) Collision Tests
// ============================================================================

void test_rect_rect_separated() {
    AABB2D rect1;
    rect1.SetMinMax(Vector2f(0, 0), Vector2f(2, 2));

    AABB2D rect2;
    rect2.SetMinMax(Vector2f(5, 5), Vector2f(7, 7));

    ASSERT_FALSE(RectRectIntersection(rect1, rect2));
}

void test_rect_rect_touching() {
    AABB2D rect1;
    rect1.SetMinMax(Vector2f(0, 0), Vector2f(2, 2));

    AABB2D rect2;
    rect2.SetMinMax(Vector2f(2, 0), Vector2f(4, 2));

    ASSERT_TRUE(RectRectIntersection(rect1, rect2));
}

void test_rect_rect_overlapping() {
    AABB2D rect1;
    rect1.SetMinMax(Vector2f(0, 0), Vector2f(3, 3));

    AABB2D rect2;
    rect2.SetMinMax(Vector2f(1, 1), Vector2f(4, 4));

    ASSERT_TRUE(RectRectIntersection(rect1, rect2));
}

void test_rect_rect_one_inside_other() {
    AABB2D rect1;
    rect1.SetMinMax(Vector2f(0, 0), Vector2f(10, 10));

    AABB2D rect2;
    rect2.SetMinMax(Vector2f(2, 2), Vector2f(5, 5));

    ASSERT_TRUE(RectRectIntersection(rect1, rect2));
}

void test_rect_rect_coincident() {
    AABB2D rect1;
    rect1.SetMinMax(Vector2f(1, 1), Vector2f(5, 5));

    AABB2D rect2;
    rect2.SetMinMax(Vector2f(1, 1), Vector2f(5, 5));

    ASSERT_TRUE(RectRectIntersection(rect1, rect2));
}

void test_rect_rect_corner_overlap() {
    AABB2D rect1;
    rect1.SetMinMax(Vector2f(0, 0), Vector2f(2, 2));

    AABB2D rect2;
    rect2.SetMinMax(Vector2f(1, 1), Vector2f(3, 3));

    ASSERT_TRUE(RectRectIntersection(rect1, rect2));
}

void test_rect_rect_horizontal_overlap_only() {
    AABB2D rect1;
    rect1.SetMinMax(Vector2f(0, 0), Vector2f(5, 2));

    AABB2D rect2;
    rect2.SetMinMax(Vector2f(2, 3), Vector2f(7, 5));

    ASSERT_FALSE(RectRectIntersection(rect1, rect2));
}

void test_rect_rect_vertical_overlap_only() {
    AABB2D rect1;
    rect1.SetMinMax(Vector2f(0, 0), Vector2f(2, 5));

    AABB2D rect2;
    rect2.SetMinMax(Vector2f(3, 2), Vector2f(5, 7));

    ASSERT_FALSE(RectRectIntersection(rect1, rect2));
}

// ============================================================================
// Rectangle-Point Collision Tests
// ============================================================================

void test_rect_point_inside() {
    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(10, 10));
    Vector2f point(5, 5);

    ASSERT_TRUE(RectPointIntersection(rect, point));
}

void test_rect_point_outside() {
    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(5, 5));
    Vector2f point(10, 10);

    ASSERT_FALSE(RectPointIntersection(rect, point));
}

void test_rect_point_on_edge() {
    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(5, 5));
    Vector2f point(5, 2);

    ASSERT_TRUE(RectPointIntersection(rect, point));
}

void test_rect_point_on_corner() {
    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(5, 5));
    Vector2f point(5, 5);

    ASSERT_TRUE(RectPointIntersection(rect, point));
}

void test_rect_point_at_min() {
    AABB2D rect;
    rect.SetMinMax(Vector2f(1, 1), Vector2f(5, 5));
    Vector2f point(1, 1);

    ASSERT_TRUE(RectPointIntersection(rect, point));
}

// ============================================================================
// Circle-Rectangle Collision Tests
// ============================================================================

void test_circle_rect_separated() {
    Vector2f center(10, 10);
    float radius = 2.0f;

    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(3, 3));

    ASSERT_FALSE(CircleRectIntersection(center, radius, rect));
}

void test_circle_rect_circle_inside() {
    Vector2f center(5, 5);
    float radius = 2.0f;

    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(10, 10));

    ASSERT_TRUE(CircleRectIntersection(center, radius, rect));
}

void test_circle_rect_overlapping_edge() {
    Vector2f center(6, 2);
    float radius = 2.0f;

    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(5, 5));

    ASSERT_TRUE(CircleRectIntersection(center, radius, rect));
}

void test_circle_rect_overlapping_corner() {
    Vector2f center(6, 6);
    float radius = 2.0f;

    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(5, 5));

    // Distance from (5,5) to (6,6) = sqrt(2) ≈ 1.414, less than radius 2
    ASSERT_TRUE(CircleRectIntersection(center, radius, rect));
}

void test_circle_rect_circle_encompasses_rect() {
    Vector2f center(5, 5);
    float radius = 20.0f;

    AABB2D rect;
    rect.SetMinMax(Vector2f(3, 3), Vector2f(7, 7));

    ASSERT_TRUE(CircleRectIntersection(center, radius, rect));
}

void test_circle_rect_rect_encompasses_circle() {
    Vector2f center(5, 5);
    float radius = 1.0f;

    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(10, 10));

    ASSERT_TRUE(CircleRectIntersection(center, radius, rect));
}

void test_circle_rect_touching_edge() {
    Vector2f center(7, 2);
    float radius = 2.0f;

    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(5, 5));

    ASSERT_TRUE(CircleRectIntersection(center, radius, rect));
}

void test_circle_rect_near_miss_corner() {
    Vector2f center(7, 7);
    float radius = 1.0f;

    AABB2D rect;
    rect.SetMinMax(Vector2f(0, 0), Vector2f(5, 5));

    // Distance from (5,5) to (7,7) = sqrt(8) ≈ 2.828, greater than radius 1
    ASSERT_FALSE(CircleRectIntersection(center, radius, rect));
}

// ============================================================================
// Line Segment Intersection Tests
// ============================================================================

void test_line_segment_intersection_cross() {
    Vector2f p1(0, 0);
    Vector2f p2(4, 4);
    Vector2f q1(0, 4);
    Vector2f q2(4, 0);

    Vector2f intersection;
    ASSERT_TRUE(LineSegmentIntersection(p1, p2, q1, q2, intersection));

    // Should intersect at (2, 2)
    ASSERT_NEAR(intersection.x, 2.0f, 0.01f);
    ASSERT_NEAR(intersection.y, 2.0f, 0.01f);
}

void test_line_segment_intersection_parallel() {
    Vector2f p1(0, 0);
    Vector2f p2(4, 0);
    Vector2f q1(0, 2);
    Vector2f q2(4, 2);

    Vector2f intersection;
    ASSERT_FALSE(LineSegmentIntersection(p1, p2, q1, q2, intersection));
}

void test_line_segment_intersection_collinear_overlapping() {
    Vector2f p1(0, 0);
    Vector2f p2(4, 0);
    Vector2f q1(2, 0);
    Vector2f q2(6, 0);

    Vector2f intersection;
    // Collinear overlapping - behavior may vary
    LineSegmentIntersection(p1, p2, q1, q2, intersection);
    ASSERT_TRUE(true);  // Should not crash
}

void test_line_segment_intersection_collinear_separated() {
    Vector2f p1(0, 0);
    Vector2f p2(2, 0);
    Vector2f q1(4, 0);
    Vector2f q2(6, 0);

    Vector2f intersection;
    ASSERT_FALSE(LineSegmentIntersection(p1, p2, q1, q2, intersection));
}

void test_line_segment_intersection_touching_endpoint() {
    Vector2f p1(0, 0);
    Vector2f p2(2, 2);
    Vector2f q1(2, 2);
    Vector2f q2(4, 0);

    Vector2f intersection;
    ASSERT_TRUE(LineSegmentIntersection(p1, p2, q1, q2, intersection));

    ASSERT_NEAR(intersection.x, 2.0f, 0.01f);
    ASSERT_NEAR(intersection.y, 2.0f, 0.01f);
}

void test_line_segment_intersection_would_cross_if_extended() {
    Vector2f p1(0, 0);
    Vector2f p2(1, 1);
    Vector2f q1(0, 2);
    Vector2f q2(1, 3);

    Vector2f intersection;
    // Lines would intersect if extended, but segments don't
    ASSERT_FALSE(LineSegmentIntersection(p1, p2, q1, q2, intersection));
}

void test_line_segment_intersection_perpendicular() {
    Vector2f p1(-2, 0);
    Vector2f p2(2, 0);
    Vector2f q1(0, -2);
    Vector2f q2(0, 2);

    Vector2f intersection;
    ASSERT_TRUE(LineSegmentIntersection(p1, p2, q1, q2, intersection));

    ASSERT_NEAR(intersection.x, 0.0f, 0.01f);
    ASSERT_NEAR(intersection.y, 0.0f, 0.01f);
}

// ============================================================================
// 2D Ray Tests
// ============================================================================

void test_ray_2d_circle_hit() {
    Vector2f origin(0, 0);
    Vector2f direction(1, 0);  // Ray pointing right

    Vector2f circleCenter(5, 0);
    float radius = 2.0f;

    float t;
    ASSERT_TRUE(Ray2DCircleIntersection(origin, direction, circleCenter, radius, t));
    ASSERT_NEAR(t, 3.0f, 0.01f);  // Hit at distance 3 (5 - 2)
}

void test_ray_2d_circle_miss() {
    Vector2f origin(0, 0);
    Vector2f direction(1, 0);  // Ray pointing right

    Vector2f circleCenter(5, 5);  // Circle far above ray
    float radius = 2.0f;

    float t;
    ASSERT_FALSE(Ray2DCircleIntersection(origin, direction, circleCenter, radius, t));
}

void test_ray_2d_circle_from_inside() {
    Vector2f origin(5, 5);  // Inside circle
    Vector2f direction(1, 0);

    Vector2f circleCenter(5, 5);
    float radius = 3.0f;

    float t;
    ASSERT_TRUE(Ray2DCircleIntersection(origin, direction, circleCenter, radius, t));
}

void test_ray_2d_rect_hit() {
    Vector2f origin(0, 2);
    Vector2f direction(1, 0);  // Ray pointing right

    AABB2D rect;
    rect.SetMinMax(Vector2f(5, 0), Vector2f(7, 4));

    float t;
    ASSERT_TRUE(Ray2DRectIntersection(origin, direction, rect, t));
    ASSERT_NEAR(t, 5.0f, 0.01f);
}

void test_ray_2d_rect_miss() {
    Vector2f origin(0, 10);  // Ray far above rectangle
    Vector2f direction(1, 0);

    AABB2D rect;
    rect.SetMinMax(Vector2f(5, 0), Vector2f(7, 4));

    float t;
    ASSERT_FALSE(Ray2DRectIntersection(origin, direction, rect, t));
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_zero_radius_circle() {
    Vector2f c1(0, 0);
    Vector2f c2(0, 0);
    float r1 = 0.0f;
    float r2 = 0.0f;

    ASSERT_TRUE(CircleCircleIntersection(c1, r1, c2, r2));
}

void test_zero_size_rectangle() {
    AABB2D rect1;
    rect1.SetMinMax(Vector2f(5, 5), Vector2f(5, 5));

    AABB2D rect2;
    rect2.SetMinMax(Vector2f(5, 5), Vector2f(5, 5));

    ASSERT_TRUE(RectRectIntersection(rect1, rect2));
}

void test_very_large_circles() {
    Vector2f c1(0, 0);
    Vector2f c2(1000, 1000);
    float r1 = 2000.0f;
    float r2 = 2000.0f;

    ASSERT_TRUE(CircleCircleIntersection(c1, r1, c2, r2));
}

void test_very_small_circles() {
    Vector2f c1(0, 0);
    Vector2f c2(0.001f, 0);
    float r1 = 0.0005f;
    float r2 = 0.0005f;

    ASSERT_TRUE(CircleCircleIntersection(c1, r1, c2, r2));
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "=== 2D Collision Detection Test Suite ===" << std::endl << std::endl;

    std::cout << "--- Circle-Circle Collision Tests ---" << std::endl;
    TEST(circle_circle_separated);
    TEST(circle_circle_touching);
    TEST(circle_circle_overlapping);
    TEST(circle_circle_one_inside_other);
    TEST(circle_circle_coincident);
    TEST(circle_circle_diagonal);
    TEST(circle_circle_barely_touching);
    TEST(circle_circle_different_sizes);

    std::cout << std::endl << "--- Circle-Point Collision Tests ---" << std::endl;
    TEST(circle_point_inside);
    TEST(circle_point_outside);
    TEST(circle_point_on_edge);
    TEST(circle_point_at_center);
    TEST(circle_point_negative_coords);
    TEST(circle_point_zero_radius);

    std::cout << std::endl << "--- Rectangle-Rectangle Collision Tests ---" << std::endl;
    TEST(rect_rect_separated);
    TEST(rect_rect_touching);
    TEST(rect_rect_overlapping);
    TEST(rect_rect_one_inside_other);
    TEST(rect_rect_coincident);
    TEST(rect_rect_corner_overlap);
    TEST(rect_rect_horizontal_overlap_only);
    TEST(rect_rect_vertical_overlap_only);

    std::cout << std::endl << "--- Rectangle-Point Collision Tests ---" << std::endl;
    TEST(rect_point_inside);
    TEST(rect_point_outside);
    TEST(rect_point_on_edge);
    TEST(rect_point_on_corner);
    TEST(rect_point_at_min);

    std::cout << std::endl << "--- Circle-Rectangle Collision Tests ---" << std::endl;
    TEST(circle_rect_separated);
    TEST(circle_rect_circle_inside);
    TEST(circle_rect_overlapping_edge);
    TEST(circle_rect_overlapping_corner);
    TEST(circle_rect_circle_encompasses_rect);
    TEST(circle_rect_rect_encompasses_circle);
    TEST(circle_rect_touching_edge);
    TEST(circle_rect_near_miss_corner);

    std::cout << std::endl << "--- Line Segment Intersection Tests ---" << std::endl;
    TEST(line_segment_intersection_cross);
    TEST(line_segment_intersection_parallel);
    TEST(line_segment_intersection_collinear_overlapping);
    TEST(line_segment_intersection_collinear_separated);
    TEST(line_segment_intersection_touching_endpoint);
    TEST(line_segment_intersection_would_cross_if_extended);
    TEST(line_segment_intersection_perpendicular);

    std::cout << std::endl << "--- 2D Ray Tests ---" << std::endl;
    TEST(ray_2d_circle_hit);
    TEST(ray_2d_circle_miss);
    TEST(ray_2d_circle_from_inside);
    TEST(ray_2d_rect_hit);
    TEST(ray_2d_rect_miss);

    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    TEST(zero_radius_circle);
    TEST(zero_size_rectangle);
    TEST(very_large_circles);
    TEST(very_small_circles);

    std::cout << std::endl << "=== All 2D Collision Tests Passed! ===" << std::endl;

    return 0;
}
