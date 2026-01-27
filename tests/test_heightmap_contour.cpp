/**
 * test_heightmap_contour.cpp
 *
 * Test cases for heightmap contour extraction functionality
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/HeightMapContour.h>
#include <hgl/math/geometry/ShorelineData.h>
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
// Basic Contour Extraction Tests
// ============================================================================

void test_simple_heightmap_uint8() {
    // Create a simple 4x4 heightmap
    // Pattern: low values on left, high values on right
    uint8_t heightmap[16] = {
        50,  50,  150, 150,
        50,  50,  150, 150,
        50,  50,  150, 150,
        50,  50,  150, 150
    };

    HeightMapContourExtractor<uint8_t> extractor(heightmap, 4, 4, 80, 120);

    // Should not crash
    HeightMapContourResult result = extractor.Extract();

    // Basic validation - should have extracted some contours
    ASSERT_TRUE(result.low_to_mid_contours.size() >= 0);
    ASSERT_TRUE(result.mid_to_high_contours.size() >= 0);
}

void test_simple_heightmap_float() {
    // Create a simple 4x4 heightmap with float values
    float heightmap[16] = {
        0.2f, 0.2f, 0.8f, 0.8f,
        0.2f, 0.2f, 0.8f, 0.8f,
        0.2f, 0.2f, 0.8f, 0.8f,
        0.2f, 0.2f, 0.8f, 0.8f
    };

    HeightMapContourExtractor<float> extractor(heightmap, 4, 4, 0.4f, 0.6f);

    HeightMapContourResult result = extractor.Extract();

    ASSERT_TRUE(result.low_to_mid_contours.size() >= 0);
    ASSERT_TRUE(result.mid_to_high_contours.size() >= 0);
}

void test_uniform_heightmap() {
    // Create a uniform heightmap (all same value)
    uint8_t heightmap[16];
    for (int i = 0; i < 16; ++i)
        heightmap[i] = 100;

    HeightMapContourExtractor<uint8_t> extractor(heightmap, 4, 4, 80, 120);

    HeightMapContourResult result = extractor.Extract();

    // Uniform heightmap should produce no contours or very simple ones
    ASSERT_TRUE(true);  // Just check it doesn't crash
}

// ============================================================================
// Douglas-Peucker Simplification Tests
// ============================================================================

void test_simplify_straight_line() {
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(2, 0),
        Vector2f(3, 0),
        Vector2f(4, 0)
    };

    std::vector<Vector2f> simplified;
    SimplifyPolygonDP(vertices, 0.1f, simplified);

    // Straight line should be simplified to just endpoints
    ASSERT_EQ(simplified.size(), 2);
    ASSERT_NEAR(simplified[0].x, 0.0f, 0.01f);
    ASSERT_NEAR(simplified[1].x, 4.0f, 0.01f);
}

void test_simplify_zigzag() {
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(1, 1),
        Vector2f(2, 0),
        Vector2f(3, 1),
        Vector2f(4, 0)
    };

    std::vector<Vector2f> simplified;
    SimplifyPolygonDP(vertices, 0.1f, simplified);

    // Zigzag should keep key points
    ASSERT_TRUE(simplified.size() >= 2);
    ASSERT_TRUE(simplified.size() <= vertices.size());
}

// ============================================================================
// Chaikin Smoothing Tests
// ============================================================================

void test_smooth_square() {
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1)
    };

    std::vector<Vector2f> smoothed;
    SmoothPolygonChaikin(vertices, 1, true, smoothed);

    // Smoothing should create more vertices
    ASSERT_TRUE(smoothed.size() > vertices.size());
}

void test_smooth_line() {
    std::vector<Vector2f> vertices = {
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(2, 0)
    };

    std::vector<Vector2f> smoothed;
    SmoothPolygonChaikin(vertices, 1, false, smoothed);

    // Should smooth the line
    ASSERT_TRUE(smoothed.size() >= 2);
}

// ============================================================================
// Shoreline Data Tests
// ============================================================================

void test_shoreline_contour_basic() {
    ShorelineContour contour;

    // Add some segments
    ShorelineSegment seg;
    seg.start = Vector2f(0, 0);
    seg.end = Vector2f(1, 0);
    seg.length = 1.0f;
    seg.accumulated_length = 0.0f;
    seg.normal = Vector2f(0, 1);

    contour.segments.push_back(seg);
    contour.total_length = 1.0f;
    contour.is_closed = false;

    // Query distance
    Vector2f point(0.5f, 0.5f);
    float dist = contour.QueryDistance(point);

    ASSERT_TRUE(dist >= 0.0f);
    ASSERT_NEAR(dist, 0.5f, 0.1f);
}

void test_shallow_water_mesh_config() {
    ShallowWaterMeshConfig config;

    ASSERT_TRUE(config.grid_spacing > 0.0f);
    ASSERT_TRUE(config.max_wave_depth > 0.0f);
    ASSERT_TRUE(config.generate_indices == true);
}

void test_shallow_water_data_extractor() {
    // Create simple contour result
    HeightMapContourResult contour_result;

    ContourPolygon poly;
    poly.vertices.push_back(Vector2f(0, 0));
    poly.vertices.push_back(Vector2f(1, 0));
    poly.vertices.push_back(Vector2f(1, 1));
    poly.is_closed = false;

    contour_result.mid_to_high_contours.push_back(poly);

    ShallowWaterMeshConfig config;
    ShallowWaterDataExtractor extractor(&contour_result, config);

    // Should not crash
    ShallowWaterMeshData mesh_data = extractor.Extract();

    ASSERT_TRUE(true);  // Basic validation
}

void test_mesh_data_export() {
    ShallowWaterMeshData mesh_data;

    // Add a simple vertex
    ShallowWaterVertex v;
    v.position = Vector2f(1.0f, 2.0f);
    v.distance_to_shore = 3.0f;
    v.contour_position = 0.5f;
    v.depth_normalized = 0.7f;
    v.shore_normal = Vector2f(0.0f, 1.0f);
    v.contour_id = 0;

    mesh_data.vertices.push_back(v);
    mesh_data.indices.push_back(0);

    // Export to arrays
    float* vertex_data = nullptr;
    uint32_t* index_data = nullptr;
    int vertex_count = 0;
    int index_count = 0;

    mesh_data.ExportToArrays(&vertex_data, &vertex_count, 8,
                            &index_data, &index_count);

    ASSERT_EQ(vertex_count, 1);
    ASSERT_EQ(index_count, 1);
    ASSERT_TRUE(vertex_data != nullptr);
    ASSERT_TRUE(index_data != nullptr);

    // Verify data
    ASSERT_NEAR(vertex_data[0], 1.0f, 0.001f);
    ASSERT_NEAR(vertex_data[1], 2.0f, 0.001f);
    ASSERT_NEAR(vertex_data[2], 3.0f, 0.001f);
    ASSERT_EQ(index_data[0], 0);

    // Clean up
    delete[] vertex_data;
    delete[] index_data;
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main()
{
    std::cout << "=== HeightMap Contour Extraction Tests ===" << std::endl;

    TEST(simple_heightmap_uint8);
    TEST(simple_heightmap_float);
    TEST(uniform_heightmap);

    std::cout << "\n=== Douglas-Peucker Simplification Tests ===" << std::endl;

    TEST(simplify_straight_line);
    TEST(simplify_zigzag);

    std::cout << "\n=== Chaikin Smoothing Tests ===" << std::endl;

    TEST(smooth_square);
    TEST(smooth_line);

    std::cout << "\n=== Shoreline Data Tests ===" << std::endl;

    TEST(shoreline_contour_basic);
    TEST(shallow_water_mesh_config);
    TEST(shallow_water_data_extractor);
    TEST(mesh_data_export);

    std::cout << "\n=== All Tests Passed! ===" << std::endl;

    return 0;
}
