/**
 * test_geometry_primitives.cpp
 * 
 * Test cases for geometry primitive classes (Sphere, Capsule, Cylinder, Cone, Torus)
 * Tests basic properties, containment, closest points, and distances.
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <hgl/math/geometry/primitives/Sphere.h>
#include <hgl/math/geometry/primitives/Capsule.h>
#include <hgl/math/geometry/primitives/Cylinder.h>
#include <hgl/math/geometry/primitives/Cone.h>
#include <hgl/math/geometry/primitives/Torus.h>

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
// Sphere Tests
// ============================================================================

void test_sphere_construction() {
    Sphere s1;
    ASSERT_NEAR(s1.GetRadius(), 1.0f, 0.001f);
    
    Sphere s2(Vector3f(1, 2, 3), 2.5f);
    ASSERT_NEAR(s2.GetCenter().x, 1.0f, 0.001f);
    ASSERT_NEAR(s2.GetCenter().y, 2.0f, 0.001f);
    ASSERT_NEAR(s2.GetCenter().z, 3.0f, 0.001f);
    ASSERT_NEAR(s2.GetRadius(), 2.5f, 0.001f);
}

void test_sphere_volume_surface_area() {
    Sphere s(Vector3f(0, 0, 0), 1.0f);
    float volume = s.GetVolume();
    float expectedVolume = (4.0f / 3.0f) * pi * 1.0f * 1.0f * 1.0f;
    ASSERT_NEAR(volume, expectedVolume, 0.001f);
    
    float surfaceArea = s.GetSurfaceArea();
    float expectedArea = 4.0f * pi * 1.0f * 1.0f;
    ASSERT_NEAR(surfaceArea, expectedArea, 0.001f);
}

void test_sphere_contains_point() {
    Sphere s(Vector3f(0, 0, 0), 1.0f);
    
    // Point at center
    ASSERT_TRUE(s.ContainsPoint(Vector3f(0, 0, 0)));
    
    // Point on surface
    ASSERT_TRUE(s.ContainsPoint(Vector3f(1, 0, 0)));
    ASSERT_TRUE(s.ContainsPoint(Vector3f(0, 1, 0)));
    
    // Point inside
    ASSERT_TRUE(s.ContainsPoint(Vector3f(0.5f, 0, 0)));
    
    // Point outside
    ASSERT_FALSE(s.ContainsPoint(Vector3f(2, 0, 0)));
    ASSERT_FALSE(s.ContainsPoint(Vector3f(1, 1, 0)));
}

void test_sphere_closest_point() {
    Sphere s(Vector3f(0, 0, 0), 1.0f);
    
    // Point inside - should return same point
    Vector3f inside(0.5f, 0, 0);
    Vector3f closest = s.ClosestPoint(inside);
    ASSERT_NEAR(closest.x, inside.x, 0.001f);
    
    // Point outside - should return point on surface
    Vector3f outside(3, 0, 0);
    closest = s.ClosestPoint(outside);
    ASSERT_NEAR(closest.x, 1.0f, 0.001f);
    ASSERT_NEAR(closest.y, 0.0f, 0.001f);
    ASSERT_NEAR(closest.z, 0.0f, 0.001f);
}

void test_sphere_distance_to_point() {
    Sphere s(Vector3f(0, 0, 0), 1.0f);
    
    // Point inside - distance should be 0
    ASSERT_NEAR(s.DistanceToPoint(Vector3f(0, 0, 0)), 0.0f, 0.001f);
    ASSERT_NEAR(s.DistanceToPoint(Vector3f(0.5f, 0, 0)), 0.0f, 0.001f);
    
    // Point outside
    ASSERT_NEAR(s.DistanceToPoint(Vector3f(3, 0, 0)), 2.0f, 0.001f);
    ASSERT_NEAR(s.DistanceToPoint(Vector3f(0, 5, 0)), 4.0f, 0.001f);
}

void test_sphere_bounding_box() {
    Sphere s(Vector3f(1, 2, 3), 2.0f);
    AABB box = s.GetBoundingBox();
    
    Vector3f min = box.GetMin();
    Vector3f max = box.GetMax();
    
    ASSERT_NEAR(min.x, -1.0f, 0.001f);
    ASSERT_NEAR(min.y, 0.0f, 0.001f);
    ASSERT_NEAR(min.z, 1.0f, 0.001f);
    
    ASSERT_NEAR(max.x, 3.0f, 0.001f);
    ASSERT_NEAR(max.y, 4.0f, 0.001f);
    ASSERT_NEAR(max.z, 5.0f, 0.001f);
}

// ============================================================================
// Capsule Tests
// ============================================================================

void test_capsule_construction() {
    Capsule c1;
    ASSERT_NEAR(c1.GetRadius(), 0.5f, 0.001f);
    
    Capsule c2(Vector3f(0, 0, 0), Vector3f(0, 2, 0), 1.0f);
    ASSERT_NEAR(c2.GetCylinderHeight(), 2.0f, 0.001f);
    ASSERT_NEAR(c2.GetTotalLength(), 4.0f, 0.001f);
    ASSERT_NEAR(c2.GetRadius(), 1.0f, 0.001f);
}

void test_capsule_volume_surface_area() {
    Capsule c(Vector3f(0, 0, 0), Vector3f(0, 2, 0), 1.0f);
    float h = 2.0f;
    float r = 1.0f;
    
    float volume = c.GetVolume();
    float expectedVolume = pi * r * r * h + (4.0f / 3.0f) * pi * r * r * r;
    ASSERT_NEAR(volume, expectedVolume, 0.001f);
    
    float surfaceArea = c.GetSurfaceArea();
    float expectedArea = 2.0f * pi * r * h + 4.0f * pi * r * r;
    ASSERT_NEAR(surfaceArea, expectedArea, 0.001f);
}

void test_capsule_contains_point() {
    Capsule c(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);
    
    // Point at center
    ASSERT_TRUE(c.ContainsPoint(Vector3f(0, 0, 0)));
    
    // Point on cylindrical surface
    ASSERT_TRUE(c.ContainsPoint(Vector3f(0.5f, 0, 0)));
    
    // Point on hemispherical end
    ASSERT_TRUE(c.ContainsPoint(Vector3f(0, 1.5f, 0)));
    
    // Point outside
    ASSERT_FALSE(c.ContainsPoint(Vector3f(1, 0, 0)));
    ASSERT_FALSE(c.ContainsPoint(Vector3f(0, 3, 0)));
}

void test_capsule_closest_point() {
    Capsule c(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);
    
    // Point outside, near cylinder
    Vector3f closest = c.ClosestPoint(Vector3f(2, 0, 0));
    ASSERT_NEAR(closest.x, 0.5f, 0.001f);
    ASSERT_NEAR(closest.y, 0.0f, 0.001f);
}

void test_capsule_distance_to_point() {
    Capsule c(Vector3f(0, -1, 0), Vector3f(0, 1, 0), 0.5f);
    
    // Point inside
    ASSERT_NEAR(c.DistanceToPoint(Vector3f(0, 0, 0)), 0.0f, 0.001f);
    
    // Point outside
    float dist = c.DistanceToPoint(Vector3f(2, 0, 0));
    ASSERT_NEAR(dist, 1.5f, 0.01f);
}

// ============================================================================
// Cylinder Tests
// ============================================================================

void test_cylinder_construction() {
    Cylinder cyl;
    ASSERT_NEAR(cyl.GetHeight(), 1.0f, 0.001f);
    ASSERT_NEAR(cyl.GetRadius(), 0.5f, 0.001f);
    
    Cylinder cyl2(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 1.0f);
    ASSERT_NEAR(cyl2.GetHeight(), 2.0f, 0.001f);
    ASSERT_NEAR(cyl2.GetRadius(), 1.0f, 0.001f);
}

void test_cylinder_volume_surface_area() {
    Cylinder cyl(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 1.0f);
    
    float volume = cyl.GetVolume();
    float expectedVolume = pi * 1.0f * 1.0f * 2.0f;
    ASSERT_NEAR(volume, expectedVolume, 0.001f);
    
    float surfaceArea = cyl.GetSurfaceArea();
    float expectedArea = 2.0f * pi * 1.0f * (1.0f + 2.0f);
    ASSERT_NEAR(surfaceArea, expectedArea, 0.001f);
}

void test_cylinder_contains_point() {
    Cylinder cyl(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 1.0f);
    
    // Point at center
    ASSERT_TRUE(cyl.ContainsPoint(Vector3f(0, 0, 0)));
    
    // Point on lateral surface
    ASSERT_TRUE(cyl.ContainsPoint(Vector3f(1, 0, 0)));
    
    // Point on top cap
    ASSERT_TRUE(cyl.ContainsPoint(Vector3f(0.5f, 1, 0)));
    
    // Point outside (too far from axis)
    ASSERT_FALSE(cyl.ContainsPoint(Vector3f(2, 0, 0)));
    
    // Point outside (beyond height)
    ASSERT_FALSE(cyl.ContainsPoint(Vector3f(0, 2, 0)));
}

// ============================================================================
// Cone Tests
// ============================================================================

void test_cone_construction() {
    Cone cone;
    ASSERT_NEAR(cone.GetHeight(), 1.0f, 0.001f);
    ASSERT_NEAR(cone.GetBaseRadius(), 0.5f, 0.001f);
    
    Cone cone2(Vector3f(0, 2, 0), Vector3f(0, -1, 0), 2.0f, 1.0f);
    ASSERT_NEAR(cone2.GetHeight(), 2.0f, 0.001f);
    ASSERT_NEAR(cone2.GetBaseRadius(), 1.0f, 0.001f);
}

void test_cone_volume_surface_area() {
    Cone cone(Vector3f(0, 1, 0), Vector3f(0, -1, 0), 3.0f, 2.0f);
    
    float volume = cone.GetVolume();
    float expectedVolume = (1.0f / 3.0f) * pi * 2.0f * 2.0f * 3.0f;
    ASSERT_NEAR(volume, expectedVolume, 0.001f);
}

void test_cone_contains_point() {
    Cone cone(Vector3f(0, 2, 0), Vector3f(0, -1, 0), 2.0f, 1.0f);
    
    // Point at apex
    ASSERT_TRUE(cone.ContainsPoint(Vector3f(0, 2, 0)));
    
    // Point inside cone
    ASSERT_TRUE(cone.ContainsPoint(Vector3f(0, 1, 0)));
    
    // Point on base
    ASSERT_TRUE(cone.ContainsPoint(Vector3f(0.5f, 0, 0)));
    
    // Point outside (beyond base)
    ASSERT_FALSE(cone.ContainsPoint(Vector3f(0, -1, 0)));
    
    // Point outside (too far from axis)
    ASSERT_FALSE(cone.ContainsPoint(Vector3f(2, 1, 0)));
}

void test_cone_radius_at_height() {
    Cone cone(Vector3f(0, 2, 0), Vector3f(0, -1, 0), 2.0f, 2.0f);
    
    // At apex (height 0)
    ASSERT_NEAR(cone.GetRadiusAtHeight(0.0f), 0.0f, 0.001f);
    
    // At base (height 2)
    ASSERT_NEAR(cone.GetRadiusAtHeight(2.0f), 2.0f, 0.001f);
    
    // At midpoint
    ASSERT_NEAR(cone.GetRadiusAtHeight(1.0f), 1.0f, 0.001f);
}

// ============================================================================
// Torus Tests
// ============================================================================

void test_torus_construction() {
    Torus torus;
    ASSERT_NEAR(torus.GetMajorRadius(), 1.0f, 0.001f);
    ASSERT_NEAR(torus.GetMinorRadius(), 0.3f, 0.001f);
    
    Torus torus2(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 0.5f);
    ASSERT_NEAR(torus2.GetMajorRadius(), 2.0f, 0.001f);
    ASSERT_NEAR(torus2.GetMinorRadius(), 0.5f, 0.001f);
}

void test_torus_volume_surface_area() {
    Torus torus(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 0.5f);
    
    float volume = torus.GetVolume();
    float expectedVolume = 2.0f * pi * pi * 2.0f * 0.5f * 0.5f;
    ASSERT_NEAR(volume, expectedVolume, 0.001f);
    
    float surfaceArea = torus.GetSurfaceArea();
    float expectedArea = 4.0f * pi * pi * 2.0f * 0.5f;
    ASSERT_NEAR(surfaceArea, expectedArea, 0.001f);
}

void test_torus_contains_point() {
    Torus torus(Vector3f(0, 0, 0), Vector3f(0, 1, 0), 2.0f, 0.5f);
    
    // Point on major circle at tube surface
    ASSERT_TRUE(torus.ContainsPoint(Vector3f(2.5f, 0, 0)));
    
    // Point inside torus
    ASSERT_TRUE(torus.ContainsPoint(Vector3f(2.2f, 0, 0)));
    
    // Point at center (hole)
    ASSERT_FALSE(torus.ContainsPoint(Vector3f(0, 0, 0)));
    
    // Point outside
    ASSERT_FALSE(torus.ContainsPoint(Vector3f(5, 0, 0)));
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main() {
    std::cout << "=== Geometry Primitives Test Suite ===" << std::endl << std::endl;
    
    std::cout << "--- Sphere Tests ---" << std::endl;
    TEST(sphere_construction);
    TEST(sphere_volume_surface_area);
    TEST(sphere_contains_point);
    TEST(sphere_closest_point);
    TEST(sphere_distance_to_point);
    TEST(sphere_bounding_box);
    
    std::cout << std::endl << "--- Capsule Tests ---" << std::endl;
    TEST(capsule_construction);
    TEST(capsule_volume_surface_area);
    TEST(capsule_contains_point);
    TEST(capsule_closest_point);
    TEST(capsule_distance_to_point);
    
    std::cout << std::endl << "--- Cylinder Tests ---" << std::endl;
    TEST(cylinder_construction);
    TEST(cylinder_volume_surface_area);
    TEST(cylinder_contains_point);
    
    std::cout << std::endl << "--- Cone Tests ---" << std::endl;
    TEST(cone_construction);
    TEST(cone_volume_surface_area);
    TEST(cone_contains_point);
    TEST(cone_radius_at_height);
    
    std::cout << std::endl << "--- Torus Tests ---" << std::endl;
    TEST(torus_construction);
    TEST(torus_volume_surface_area);
    TEST(torus_contains_point);
    
    std::cout << std::endl << "=== All Tests Passed! ===" << std::endl;
    
    return 0;
}
