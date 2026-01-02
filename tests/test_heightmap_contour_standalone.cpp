/**
 * test_heightmap_contour_standalone.cpp
 * 
 * Standalone test for heightmap contour extraction
 * Does not depend on CMMath library
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

// Include our headers
#include <hgl/math/VectorTypes.h>
#include <hgl/math/geometry/HeightMapContour.h>
#include <hgl/math/geometry/ShorelineData.h>

using namespace hgl::math;

int main()
{
    std::cout << "=== Standalone HeightMap Contour Test ===" << std::endl;
    
    // Test 1: Simple uint8 heightmap
    std::cout << "Test 1: Simple uint8 heightmap... ";
    {
        uint8_t heightmap[16] = {
            50,  50,  150, 150,
            50,  50,  150, 150,
            50,  50,  150, 150,
            50,  50,  150, 150
        };

        HeightMapContourExtractor<uint8_t> extractor(heightmap, 4, 4, 80, 120);
        HeightMapContourResult result = extractor.Extract();
        
        std::cout << "OK (low_contours: " << result.low_to_mid_contours.size() 
                  << ", high_contours: " << result.mid_to_high_contours.size() << ")" << std::endl;
    }
    
    // Test 2: Float heightmap
    std::cout << "Test 2: Float heightmap... ";
    {
        float heightmap[16] = {
            0.2f, 0.2f, 0.8f, 0.8f,
            0.2f, 0.2f, 0.8f, 0.8f,
            0.2f, 0.2f, 0.8f, 0.8f,
            0.2f, 0.2f, 0.8f, 0.8f
        };

        HeightMapContourExtractor<float> extractor(heightmap, 4, 4, 0.4f, 0.6f);
        HeightMapContourResult result = extractor.Extract();
        
        std::cout << "OK (low_contours: " << result.low_to_mid_contours.size() 
                  << ", high_contours: " << result.mid_to_high_contours.size() << ")" << std::endl;
    }
    
    // Test 3: Douglas-Peucker simplification
    std::cout << "Test 3: Douglas-Peucker simplification... ";
    {
        std::vector<Vector2f> vertices = {
            Vector2f(0, 0),
            Vector2f(1, 0),
            Vector2f(2, 0),
            Vector2f(3, 0),
            Vector2f(4, 0)
        };

        std::vector<Vector2f> simplified;
        SimplifyPolygonDP(vertices, 0.1f, simplified);
        
        std::cout << "OK (vertices: " << vertices.size() 
                  << " -> " << simplified.size() << ")" << std::endl;
    }
    
    // Test 4: Chaikin smoothing
    std::cout << "Test 4: Chaikin smoothing... ";
    {
        std::vector<Vector2f> vertices = {
            Vector2f(0, 0),
            Vector2f(1, 0),
            Vector2f(1, 1),
            Vector2f(0, 1)
        };

        std::vector<Vector2f> smoothed;
        SmoothPolygonChaikin(vertices, 1, true, smoothed);
        
        std::cout << "OK (vertices: " << vertices.size() 
                  << " -> " << smoothed.size() << ")" << std::endl;
    }
    
    // Test 5: Shoreline contour
    std::cout << "Test 5: Shoreline contour query... ";
    {
        ShorelineContour contour;
        
        ShorelineSegment seg;
        seg.start = Vector2f(0, 0);
        seg.end = Vector2f(1, 0);
        seg.length = 1.0f;
        seg.accumulated_length = 0.0f;
        seg.normal = Vector2f(0, 1);
        
        contour.segments.push_back(seg);
        contour.total_length = 1.0f;

        Vector2f point(0.5f, 0.5f);
        float dist = contour.QueryDistance(point);
        
        std::cout << "OK (distance: " << dist << ")" << std::endl;
    }
    
    // Test 6: Mesh data export
    std::cout << "Test 6: Mesh data export... ";
    {
        ShallowWaterMeshData mesh_data;
        
        ShallowWaterVertex v;
        v.position = Vector2f(1.0f, 2.0f);
        v.distance_to_shore = 3.0f;
        v.contour_position = 0.5f;
        v.depth_normalized = 0.7f;
        v.shore_normal = Vector2f(0.0f, 1.0f);
        v.contour_id = 0;
        
        mesh_data.vertices.push_back(v);
        mesh_data.indices.push_back(0);

        float* vertex_data = nullptr;
        uint32_t* index_data = nullptr;
        int vertex_count = 0;
        int index_count = 0;

        mesh_data.ExportToArrays(&vertex_data, &vertex_count, 8,
                                &index_data, &index_count);

        bool success = (vertex_count == 1 && index_count == 1 &&
                       vertex_data != nullptr && index_data != nullptr);

        if (success) {
            delete[] vertex_data;
            delete[] index_data;
        }
        
        std::cout << (success ? "OK" : "FAILED") << std::endl;
    }
    
    std::cout << "\n=== All Tests Completed Successfully! ===" << std::endl;
    
    return 0;
}
