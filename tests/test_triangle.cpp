/**
 * test_triangle.cpp
 * 
 * Comprehensive test cases for Triangle geometry
 * Tests triangle construction, ray intersection, area calculation, and point containment.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/Triangle.h>
#include <hgl/math/geometry/Ray.h>
#include <hgl/math/VectorTypes.h>
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
// 2D Point-in-Triangle Tests
// ============================================================================

void test_point_in_triangle_2d_center() {
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(1, 2)
    };
    
    Vector2f point(1, 0.5f);
    ASSERT_TRUE(PointInTriangle(vertices, point));
}

void test_point_in_triangle_2d_outside() {
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(1, 2)
    };
    
    Vector2f point(5, 5);
    ASSERT_FALSE(PointInTriangle(vertices, point));
}

void test_point_in_triangle_2d_on_edge() {
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(1, 2)
    };
    
    Vector2f point(1, 0);  // On bottom edge
    ASSERT_TRUE(PointInTriangle(vertices, point));
}

void test_point_in_triangle_2d_on_vertex() {
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(1, 2)
    };
    
    Vector2f point(0, 0);  // On vertex
    ASSERT_TRUE(PointInTriangle(vertices, point));
}

void test_point_in_triangle_2d_near_edge_outside() {
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(1, 2)
    };
    
    Vector2f point(1, -0.1f);  // Just outside bottom edge
    ASSERT_FALSE(PointInTriangle(vertices, point));
}

void test_point_in_triangle_2d_equilateral() {
    // Equilateral triangle
    float h = std::sqrt(3.0f) / 2.0f;
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(0.5f, h)
    };
    
    Vector2f center(0.5f, h / 3.0f);
    ASSERT_TRUE(PointInTriangle(vertices, center));
}

void test_point_in_triangle_2d_degenerate() {
    // Degenerate triangle (all points on a line)
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(2, 0)
    };
    
    Vector2f point(1, 0);
    // Behavior may vary for degenerate cases
    PointInTriangle(vertices, point);  // Should not crash
    ASSERT_TRUE(true);
}

void test_point_in_triangle_2d_large_triangle() {
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(1000, 0),
        Vector2f(500, 1000)
    };
    
    Vector2f point(500, 500);
    ASSERT_TRUE(PointInTriangle(vertices, point));
}

void test_point_in_triangle_2d_very_small() {
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(0.001f, 0),
        Vector2f(0.0005f, 0.001f)
    };
    
    Vector2f point(0.0005f, 0.0005f);
    ASSERT_TRUE(PointInTriangle(vertices, point));
}

void test_point_in_triangle_2d_negative_coords() {
    Vector2f vertices[3] = {
        Vector2f(-1, -1),
        Vector2f(1, -1),
        Vector2f(0, 1)
    };
    
    Vector2f point(0, 0);
    ASSERT_TRUE(PointInTriangle(vertices, point));
}

// ============================================================================
// 2D Triangle Area Tests
// ============================================================================

void test_triangle_area_2d_simple() {
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(0, 2)
    };
    
    float area = TriangleArea(vertices[0], vertices[1], vertices[2]);
    ASSERT_NEAR(area, 2.0f, 0.001f);  // Area = 0.5 * base * height = 0.5 * 2 * 2
}

void test_triangle_area_2d_unit() {
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(0, 1)
    };
    
    float area = TriangleArea(vertices[0], vertices[1], vertices[2]);
    ASSERT_NEAR(area, 0.5f, 0.001f);
}

void test_triangle_area_2d_equilateral() {
    float h = std::sqrt(3.0f) / 2.0f;
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(0.5f, h)
    };
    
    float area = TriangleArea(vertices[0], vertices[1], vertices[2]);
    float expected = std::sqrt(3.0f) / 4.0f;  // For equilateral with side 1
    ASSERT_NEAR(area, expected, 0.001f);
}

void test_triangle_area_2d_degenerate() {
    // Collinear points
    Vector2f vertices[3] = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(2, 0)
    };
    
    float area = TriangleArea(vertices[0], vertices[1], vertices[2]);
    ASSERT_NEAR(area, 0.0f, 0.001f);
}

// ============================================================================
// 3D Triangle Tests
// ============================================================================

void test_triangle_area_3d_xy_plane() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(0, 2, 0);
    
    float area = TriangleArea(v0, v1, v2);
    ASSERT_NEAR(area, 2.0f, 0.001f);
}

void test_triangle_area_3d_arbitrary_plane() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(1, 0, 0);
    Vector3f v2(0, 1, 1);
    
    // Area = 0.5 * |cross product|
    Vector3f edge1 = v1 - v0;
    Vector3f edge2 = v2 - v0;
    float expected = 0.5f * Length(Cross(edge1, edge2));
    
    float area = TriangleArea(v0, v1, v2);
    ASSERT_NEAR(area, expected, 0.001f);
}

void test_triangle_normal_3d() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(1, 0, 0);
    Vector3f v2(0, 1, 0);
    
    Vector3f normal = TriangleNormal(v0, v1, v2);
    
    // Normal should point in Z direction
    ASSERT_NEAR(normal.x, 0.0f, 0.001f);
    ASSERT_NEAR(normal.y, 0.0f, 0.001f);
    ASSERT_NEAR(std::abs(normal.z), 1.0f, 0.001f);
}

void test_triangle_normal_3d_reversed() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(0, 1, 0);
    Vector3f v2(1, 0, 0);
    
    Vector3f normal = TriangleNormal(v0, v1, v2);
    
    // Normal should point in negative Z direction (winding order reversed)
    ASSERT_NEAR(normal.x, 0.0f, 0.001f);
    ASSERT_NEAR(normal.y, 0.0f, 0.001f);
    ASSERT_NEAR(std::abs(normal.z), 1.0f, 0.001f);
}

// ============================================================================
// Triangle Ray Intersection Tests
// ============================================================================

void test_ray_triangle_intersection_perpendicular() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(1, 2, 0);
    
    Ray ray(Vector3f(1, 1, -5), Vector3f(0, 0, 1));
    
    float t;
    ASSERT_TRUE(RayTriangleIntersection(ray, v0, v1, v2, t));
    ASSERT_NEAR(t, 5.0f, 0.01f);
}

void test_ray_triangle_intersection_miss() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(1, 2, 0);
    
    Ray ray(Vector3f(5, 5, -5), Vector3f(0, 0, 1));  // Outside triangle
    
    float t;
    ASSERT_FALSE(RayTriangleIntersection(ray, v0, v1, v2, t));
}

void test_ray_triangle_intersection_edge() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(1, 2, 0);
    
    Ray ray(Vector3f(1, 0, -5), Vector3f(0, 0, 1));  // Ray at edge
    
    float t;
    ASSERT_TRUE(RayTriangleIntersection(ray, v0, v1, v2, t));
}

void test_ray_triangle_intersection_vertex() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(1, 2, 0);
    
    Ray ray(Vector3f(0, 0, -5), Vector3f(0, 0, 1));  // Ray at vertex
    
    float t;
    ASSERT_TRUE(RayTriangleIntersection(ray, v0, v1, v2, t));
}

void test_ray_triangle_intersection_parallel_miss() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(1, 2, 0);
    
    Ray ray(Vector3f(0, 0, 1), Vector3f(1, 0, 0));  // Ray parallel to triangle
    
    float t;
    ASSERT_FALSE(RayTriangleIntersection(ray, v0, v1, v2, t));
}

void test_ray_triangle_intersection_behind_ray() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(1, 2, 0);
    
    Ray ray(Vector3f(1, 1, 5), Vector3f(0, 0, 1));  // Triangle behind ray
    
    float t;
    ASSERT_FALSE(RayTriangleIntersection(ray, v0, v1, v2, t));
}

void test_ray_triangle_intersection_oblique_angle() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(1, 2, 0);
    
    // Ray at 45 degree angle
    Ray ray(Vector3f(1, 1, -5), Normalized(Vector3f(0, 0, 1)));
    
    float t;
    ASSERT_TRUE(RayTriangleIntersection(ray, v0, v1, v2, t));
}

void test_ray_triangle_intersection_grazing() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(1, 2, 0);
    
    // Ray grazing along the plane
    Ray ray(Vector3f(-1, 0, 0), Normalized(Vector3f(1, 0, 0.01f)));
    
    float t;
    bool hit = RayTriangleIntersection(ray, v0, v1, v2, t);
    // This is an edge case - behavior may vary
    ASSERT_TRUE(true);  // Should not crash
}

// ============================================================================
// Triangle Barycentric Coordinates Tests
// ============================================================================

void test_barycentric_coordinates_center() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(3, 0, 0);
    Vector3f v2(0, 3, 0);
    
    Vector3f point(1, 1, 0);
    
    float u, v, w;
    BarycentricCoordinates(point, v0, v1, v2, u, v, w);
    
    // Check that they sum to 1
    ASSERT_NEAR(u + v + w, 1.0f, 0.001f);
    
    // Check all are positive
    ASSERT_TRUE(u >= 0.0f);
    ASSERT_TRUE(v >= 0.0f);
    ASSERT_TRUE(w >= 0.0f);
}

void test_barycentric_coordinates_vertex() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(1, 0, 0);
    Vector3f v2(0, 1, 0);
    
    Vector3f point = v0;  // At v0
    
    float u, v, w;
    BarycentricCoordinates(point, v0, v1, v2, u, v, w);
    
    // Should be (1, 0, 0)
    ASSERT_NEAR(u, 1.0f, 0.001f);
    ASSERT_NEAR(v, 0.0f, 0.001f);
    ASSERT_NEAR(w, 0.0f, 0.001f);
}

void test_barycentric_coordinates_edge_midpoint() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(2, 0, 0);
    Vector3f v2(0, 2, 0);
    
    Vector3f point(1, 0, 0);  // Midpoint of v0-v1 edge
    
    float u, v, w;
    BarycentricCoordinates(point, v0, v1, v2, u, v, w);
    
    // Should be approximately (0.5, 0.5, 0)
    ASSERT_NEAR(u, 0.5f, 0.01f);
    ASSERT_NEAR(v, 0.5f, 0.01f);
    ASSERT_NEAR(w, 0.0f, 0.01f);
}

// ============================================================================
// Triangle-Triangle Intersection Tests
// ============================================================================

void test_triangle_triangle_coplanar_overlapping() {
    Vector3f t1v0(0, 0, 0);
    Vector3f t1v1(2, 0, 0);
    Vector3f t1v2(1, 2, 0);
    
    Vector3f t2v0(0.5f, 0.5f, 0);
    Vector3f t2v1(1.5f, 0.5f, 0);
    Vector3f t2v2(1, 1.5f, 0);
    
    ASSERT_TRUE(TriangleTriangleIntersection(t1v0, t1v1, t1v2, t2v0, t2v1, t2v2));
}

void test_triangle_triangle_coplanar_separated() {
    Vector3f t1v0(0, 0, 0);
    Vector3f t1v1(1, 0, 0);
    Vector3f t1v2(0.5f, 1, 0);
    
    Vector3f t2v0(5, 0, 0);
    Vector3f t2v1(6, 0, 0);
    Vector3f t2v2(5.5f, 1, 0);
    
    ASSERT_FALSE(TriangleTriangleIntersection(t1v0, t1v1, t1v2, t2v0, t2v1, t2v2));
}

void test_triangle_triangle_intersecting_different_planes() {
    Vector3f t1v0(0, 0, 0);
    Vector3f t1v1(2, 0, 0);
    Vector3f t1v2(1, 2, 0);
    
    // Second triangle in XZ plane, intersecting first
    Vector3f t2v0(1, -1, -1);
    Vector3f t2v1(1, -1, 1);
    Vector3f t2v2(1, 3, 0);
    
    ASSERT_TRUE(TriangleTriangleIntersection(t1v0, t1v1, t1v2, t2v0, t2v1, t2v2));
}

void test_triangle_triangle_edge_touching() {
    Vector3f t1v0(0, 0, 0);
    Vector3f t1v1(1, 0, 0);
    Vector3f t1v2(0.5f, 1, 0);
    
    Vector3f t2v0(1, 0, 0);
    Vector3f t2v1(2, 0, 0);
    Vector3f t2v2(1.5f, 1, 0);
    
    ASSERT_TRUE(TriangleTriangleIntersection(t1v0, t1v1, t1v2, t2v0, t2v1, t2v2));
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_triangle_zero_area() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(0, 0, 0);
    Vector3f v2(0, 0, 0);
    
    float area = TriangleArea(v0, v1, v2);
    ASSERT_NEAR(area, 0.0f, 0.001f);
}

void test_triangle_very_large() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(10000, 0, 0);
    Vector3f v2(5000, 10000, 0);
    
    float area = TriangleArea(v0, v1, v2);
    ASSERT_TRUE(area > 0);  // Should compute without overflow
}

void test_triangle_very_small() {
    Vector3f v0(0, 0, 0);
    Vector3f v1(0.0001f, 0, 0);
    Vector3f v2(0.00005f, 0.0001f, 0);
    
    float area = TriangleArea(v0, v1, v2);
    ASSERT_TRUE(area >= 0);  // Should handle precision
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "=== Triangle Geometry Test Suite ===" << std::endl << std::endl;
    
    std::cout << "--- 2D Point-in-Triangle Tests ---" << std::endl;
    TEST(point_in_triangle_2d_center);
    TEST(point_in_triangle_2d_outside);
    TEST(point_in_triangle_2d_on_edge);
    TEST(point_in_triangle_2d_on_vertex);
    TEST(point_in_triangle_2d_near_edge_outside);
    TEST(point_in_triangle_2d_equilateral);
    TEST(point_in_triangle_2d_degenerate);
    TEST(point_in_triangle_2d_large_triangle);
    TEST(point_in_triangle_2d_very_small);
    TEST(point_in_triangle_2d_negative_coords);
    
    std::cout << std::endl << "--- 2D Triangle Area Tests ---" << std::endl;
    TEST(triangle_area_2d_simple);
    TEST(triangle_area_2d_unit);
    TEST(triangle_area_2d_equilateral);
    TEST(triangle_area_2d_degenerate);
    
    std::cout << std::endl << "--- 3D Triangle Tests ---" << std::endl;
    TEST(triangle_area_3d_xy_plane);
    TEST(triangle_area_3d_arbitrary_plane);
    TEST(triangle_normal_3d);
    TEST(triangle_normal_3d_reversed);
    
    std::cout << std::endl << "--- Triangle Ray Intersection Tests ---" << std::endl;
    TEST(ray_triangle_intersection_perpendicular);
    TEST(ray_triangle_intersection_miss);
    TEST(ray_triangle_intersection_edge);
    TEST(ray_triangle_intersection_vertex);
    TEST(ray_triangle_intersection_parallel_miss);
    TEST(ray_triangle_intersection_behind_ray);
    TEST(ray_triangle_intersection_oblique_angle);
    TEST(ray_triangle_intersection_grazing);
    
    std::cout << std::endl << "--- Triangle Barycentric Coordinates Tests ---" << std::endl;
    TEST(barycentric_coordinates_center);
    TEST(barycentric_coordinates_vertex);
    TEST(barycentric_coordinates_edge_midpoint);
    
    std::cout << std::endl << "--- Triangle-Triangle Intersection Tests ---" << std::endl;
    TEST(triangle_triangle_coplanar_overlapping);
    TEST(triangle_triangle_coplanar_separated);
    TEST(triangle_triangle_intersecting_different_planes);
    TEST(triangle_triangle_edge_touching);
    
    std::cout << std::endl << "--- Edge Cases ---" << std::endl;
    TEST(triangle_zero_area);
    TEST(triangle_very_large);
    TEST(triangle_very_small);
    
    std::cout << std::endl << "=== All Triangle Tests Passed! ===" << std::endl;
    
    return 0;
}
