/**
 * test_polygon_2d.cpp
 * 
 * 2D多边形三角剖分功能的测试用例
 * Tests for 2D polygon triangulation functionality
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <numbers>
#include <hgl/math/geometry/Polygon2D.h>
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

#define ASSERT_EQ(a, b) \
    if ((a) != (b)) { \
        std::cerr << "FAILED: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ") at line " << __LINE__ << std::endl; \
        exit(1); \
    }

// ============================================================================
// Polygon Area Tests
// ============================================================================

void test_polygon_area_triangle() {
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(4, 0),
        Vector2f(0, 3)
    };
    
    float area = Polygon2DArea(vertices);
    ASSERT_NEAR(area, 6.0f, 0.001f);  // Area = 1/2 * base * height = 1/2 * 4 * 3 = 6
}

void test_polygon_area_square() {
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(2, 2),
        Vector2f(0, 2)
    };
    
    float area = Polygon2DArea(vertices);
    ASSERT_NEAR(area, 4.0f, 0.001f);  // Area = 2 * 2 = 4
}

void test_polygon_area_pentagon() {
    // Regular pentagon with radius 1
    std::vector<Vector2f> vertices;
    const float radius = 1.0f;
    const int sides = 5;
    
    for (int i = 0; i < sides; ++i) {
        float angle = 2.0f * std::numbers::pi_v<float> * i / sides;
        vertices.push_back(Vector2f(radius * cos(angle), radius * sin(angle)));
    }
    
    float area = Polygon2DArea(vertices);
    // Regular pentagon area = (5 * r^2 * sin(72°)) / 2 ≈ 2.378
    ASSERT_NEAR(area, 2.378f, 0.01f);
}

// ============================================================================
// Winding Order Tests
// ============================================================================

void test_polygon_ccw_detection() {
    // Counter-clockwise square
    std::vector<Vector2f> ccw_vertices = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1)
    };
    
    ASSERT_TRUE(IsPolygon2DCCW(ccw_vertices));
    
    // Clockwise square
    std::vector<Vector2f> cw_vertices = {
        Vector2f(0, 0),
        Vector2f(0, 1),
        Vector2f(1, 1),
        Vector2f(1, 0)
    };
    
    ASSERT_FALSE(IsPolygon2DCCW(cw_vertices));
}

// ============================================================================
// Point in Triangle Tests
// ============================================================================

void test_point_in_triangle_inside() {
    Vector2f a(0, 0);
    Vector2f b(4, 0);
    Vector2f c(2, 3);
    Vector2f p(2, 1);  // Inside the triangle
    
    ASSERT_TRUE(IsPointInTriangle2D(a, b, c, p));
}

void test_point_in_triangle_outside() {
    Vector2f a(0, 0);
    Vector2f b(4, 0);
    Vector2f c(2, 3);
    Vector2f p(5, 5);  // Outside the triangle
    
    ASSERT_FALSE(IsPointInTriangle2D(a, b, c, p));
}

void test_point_in_triangle_on_edge() {
    Vector2f a(0, 0);
    Vector2f b(4, 0);
    Vector2f c(2, 3);
    Vector2f p(2, 0);  // On the edge AB
    
    ASSERT_TRUE(IsPointInTriangle2D(a, b, c, p));
}

void test_point_in_triangle_on_vertex() {
    Vector2f a(0, 0);
    Vector2f b(4, 0);
    Vector2f c(2, 3);
    
    ASSERT_TRUE(IsPointInTriangle2D(a, b, c, a));
    ASSERT_TRUE(IsPointInTriangle2D(a, b, c, b));
    ASSERT_TRUE(IsPointInTriangle2D(a, b, c, c));
}

// ============================================================================
// Triangulation Tests - Simple Polygons
// ============================================================================

void test_triangulate_triangle() {
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(0, 1)
    };
    
    std::vector<size_t> triangles;
    bool success = TriangulatePolygon2D(vertices, triangles);
    
    ASSERT_TRUE(success);
    ASSERT_EQ(triangles.size(), 3);  // One triangle = 3 indices
    ASSERT_EQ(triangles[0], 0);
    ASSERT_EQ(triangles[1], 1);
    ASSERT_EQ(triangles[2], 2);
}

void test_triangulate_square() {
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(2, 2),
        Vector2f(0, 2)
    };
    
    std::vector<size_t> triangles;
    bool success = TriangulatePolygon2D(vertices, triangles);
    
    ASSERT_TRUE(success);
    ASSERT_EQ(triangles.size(), 6);  // Two triangles = 6 indices
    
    // Verify total area equals square area
    float total_area = 0.0f;
    for (size_t i = 0; i < triangles.size(); i += 3) {
        Vector2f v0 = vertices[triangles[i]];
        Vector2f v1 = vertices[triangles[i + 1]];
        Vector2f v2 = vertices[triangles[i + 2]];
        total_area += TriangleArea(v0, v1, v2);
    }
    
    ASSERT_NEAR(total_area, 4.0f, 0.001f);
}

void test_triangulate_pentagon() {
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(3, 1.5f),
        Vector2f(1, 3),
        Vector2f(-1, 1.5f)
    };
    
    std::vector<size_t> triangles;
    bool success = TriangulatePolygon2D(vertices, triangles);
    
    ASSERT_TRUE(success);
    ASSERT_EQ(triangles.size(), 9);  // Three triangles = 9 indices (5 vertices - 2)
    
    // Verify total area equals pentagon area
    float polygon_area = Polygon2DArea(vertices);
    float total_triangle_area = 0.0f;
    
    for (size_t i = 0; i < triangles.size(); i += 3) {
        Vector2f v0 = vertices[triangles[i]];
        Vector2f v1 = vertices[triangles[i + 1]];
        Vector2f v2 = vertices[triangles[i + 2]];
        total_triangle_area += TriangleArea(v0, v1, v2);
    }
    
    ASSERT_NEAR(total_triangle_area, polygon_area, 0.01f);
}

void test_triangulate_hexagon() {
    std::vector<Vector2f> vertices = {
        Vector2f(1, 0),
        Vector2f(2, 0),
        Vector2f(3, 1),
        Vector2f(2, 2),
        Vector2f(1, 2),
        Vector2f(0, 1)
    };
    
    std::vector<size_t> triangles;
    bool success = TriangulatePolygon2D(vertices, triangles);
    
    ASSERT_TRUE(success);
    ASSERT_EQ(triangles.size(), 12);  // Four triangles = 12 indices (6 vertices - 2)
    
    // Verify areas match
    float polygon_area = Polygon2DArea(vertices);
    float total_triangle_area = 0.0f;
    
    for (size_t i = 0; i < triangles.size(); i += 3) {
        Vector2f v0 = vertices[triangles[i]];
        Vector2f v1 = vertices[triangles[i + 1]];
        Vector2f v2 = vertices[triangles[i + 2]];
        total_triangle_area += TriangleArea(v0, v1, v2);
    }
    
    ASSERT_NEAR(total_triangle_area, polygon_area, 0.01f);
}

// ============================================================================
// Triangulation Tests - Concave Polygons
// ============================================================================

void test_triangulate_concave_L_shape() {
    // L-shaped polygon (concave)
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(2, 1),
        Vector2f(1, 1),
        Vector2f(1, 2),
        Vector2f(0, 2)
    };
    
    std::vector<size_t> triangles;
    bool success = TriangulatePolygon2D(vertices, triangles);
    
    ASSERT_TRUE(success);
    ASSERT_EQ(triangles.size(), 12);  // Four triangles = 12 indices (6 vertices - 2)
    
    // Verify areas match
    float polygon_area = Polygon2DArea(vertices);
    float total_triangle_area = 0.0f;
    
    for (size_t i = 0; i < triangles.size(); i += 3) {
        Vector2f v0 = vertices[triangles[i]];
        Vector2f v1 = vertices[triangles[i + 1]];
        Vector2f v2 = vertices[triangles[i + 2]];
        total_triangle_area += TriangleArea(v0, v1, v2);
    }
    
    ASSERT_NEAR(total_triangle_area, polygon_area, 0.01f);
}

void test_triangulate_concave_star() {
    // Simple star shape (concave)
    std::vector<Vector2f> vertices = {
        Vector2f(0, 2),
        Vector2f(0.5f, 0.5f),
        Vector2f(2, 0),
        Vector2f(0.8f, 1.2f),
        Vector2f(1.5f, 2.5f),
        Vector2f(0, 1.5f)
    };
    
    std::vector<size_t> triangles;
    bool success = TriangulatePolygon2D(vertices, triangles);
    
    ASSERT_TRUE(success);
    ASSERT_EQ(triangles.size(), 12);  // Four triangles = 12 indices (6 vertices - 2)
}

// ============================================================================
// Triangulation Tests - Clockwise Input
// ============================================================================

void test_triangulate_clockwise_square() {
    // Clockwise square (should be automatically handled)
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(0, 2),
        Vector2f(2, 2),
        Vector2f(2, 0)
    };
    
    std::vector<size_t> triangles;
    bool success = TriangulatePolygon2D(vertices, triangles);
    
    ASSERT_TRUE(success);
    ASSERT_EQ(triangles.size(), 6);  // Two triangles = 6 indices
}

// ============================================================================
// Polygon2D Class Tests
// ============================================================================

void test_polygon2d_class_construction() {
    Polygon2Df polygon;
    ASSERT_EQ(polygon.GetVertexCount(), 0);
    
    polygon.AddVertex(Vector2f(0, 0));
    polygon.AddVertex(Vector2f(1, 0));
    polygon.AddVertex(Vector2f(1, 1));
    
    ASSERT_EQ(polygon.GetVertexCount(), 3);
}

void test_polygon2d_class_initializer_list() {
    Polygon2Df polygon({
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(2, 2),
        Vector2f(0, 2)
    });
    
    ASSERT_EQ(polygon.GetVertexCount(), 4);
    ASSERT_NEAR(polygon.Area(), 4.0f, 0.001f);
}

void test_polygon2d_class_triangulate_indices() {
    Polygon2Df polygon({
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(2, 2),
        Vector2f(0, 2)
    });
    
    std::vector<size_t> triangles;
    bool success = polygon.Triangulate(triangles);
    
    ASSERT_TRUE(success);
    ASSERT_EQ(triangles.size(), 6);
}

void test_polygon2d_class_triangulate_objects() {
    Polygon2Df polygon({
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(2, 2),
        Vector2f(0, 2)
    });
    
    std::vector<Triangle2f> triangles;
    bool success = polygon.Triangulate(triangles);
    
    ASSERT_TRUE(success);
    ASSERT_EQ(triangles.size(), 2);
    
    // Verify triangle areas sum to polygon area
    float total_area = triangles[0].Area() + triangles[1].Area();
    ASSERT_NEAR(total_area, 4.0f, 0.001f);
}

void test_polygon2d_class_reverse() {
    Polygon2Df polygon({
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(2, 2),
        Vector2f(0, 2)
    });
    
    bool was_ccw = polygon.IsCCW();
    polygon.Reverse();
    bool is_ccw = polygon.IsCCW();
    
    ASSERT_TRUE(was_ccw != is_ccw);
}

void test_polygon2d_class_vertex_access() {
    Polygon2Df polygon({
        Vector2f(0, 0),
        Vector2f(2, 0),
        Vector2f(2, 2)
    });
    
    ASSERT_NEAR(polygon[0].x, 0.0f, 0.001f);
    ASSERT_NEAR(polygon[1].x, 2.0f, 0.001f);
    ASSERT_NEAR(polygon[2].y, 2.0f, 0.001f);
    
    polygon[0] = Vector2f(1, 1);
    ASSERT_NEAR(polygon[0].x, 1.0f, 0.001f);
    ASSERT_NEAR(polygon[0].y, 1.0f, 0.001f);
}

// ============================================================================
// Edge Case Tests
// ============================================================================

void test_triangulate_empty_polygon() {
    std::vector<Vector2f> vertices;
    std::vector<size_t> triangles;
    
    bool success = TriangulatePolygon2D(vertices, triangles);
    ASSERT_FALSE(success);
}

void test_triangulate_two_vertices() {
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(1, 0)
    };
    
    std::vector<size_t> triangles;
    bool success = TriangulatePolygon2D(vertices, triangles);
    
    ASSERT_FALSE(success);
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main()
{
    std::cout << "===========================================" << std::endl;
    std::cout << "2D Polygon Triangulation Tests" << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "--- Polygon Area Tests ---" << std::endl;
    TEST(polygon_area_triangle);
    TEST(polygon_area_square);
    TEST(polygon_area_pentagon);
    std::cout << std::endl;

    std::cout << "--- Winding Order Tests ---" << std::endl;
    TEST(polygon_ccw_detection);
    std::cout << std::endl;

    std::cout << "--- Point in Triangle Tests ---" << std::endl;
    TEST(point_in_triangle_inside);
    TEST(point_in_triangle_outside);
    TEST(point_in_triangle_on_edge);
    TEST(point_in_triangle_on_vertex);
    std::cout << std::endl;

    std::cout << "--- Triangulation Tests - Simple Polygons ---" << std::endl;
    TEST(triangulate_triangle);
    TEST(triangulate_square);
    TEST(triangulate_pentagon);
    TEST(triangulate_hexagon);
    std::cout << std::endl;

    std::cout << "--- Triangulation Tests - Concave Polygons ---" << std::endl;
    TEST(triangulate_concave_L_shape);
    TEST(triangulate_concave_star);
    std::cout << std::endl;

    std::cout << "--- Triangulation Tests - Clockwise Input ---" << std::endl;
    TEST(triangulate_clockwise_square);
    std::cout << std::endl;

    std::cout << "--- Polygon2D Class Tests ---" << std::endl;
    TEST(polygon2d_class_construction);
    TEST(polygon2d_class_initializer_list);
    TEST(polygon2d_class_triangulate_indices);
    TEST(polygon2d_class_triangulate_objects);
    TEST(polygon2d_class_reverse);
    TEST(polygon2d_class_vertex_access);
    std::cout << std::endl;

    std::cout << "--- Edge Case Tests ---" << std::endl;
    TEST(triangulate_empty_polygon);
    TEST(triangulate_two_vertices);
    std::cout << std::endl;

    std::cout << "===========================================" << std::endl;
    std::cout << "All tests PASSED!" << std::endl;
    std::cout << "===========================================" << std::endl;

    return 0;
}
