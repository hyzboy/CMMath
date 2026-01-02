# CMMath Geometry System - Test Suite

This directory contains comprehensive test cases for the geometry refactoring system.

## Test Files

### 1. test_geometry_primitives.cpp
Tests the basic geometry primitive classes:
- **Sphere**: Construction, volume, surface area, containment, closest points, distances
- **Capsule**: Construction, properties, containment, geometry queries
- **Cylinder**: Construction, volume, surface area, point containment
- **Cone**: Construction, volume, surface area, radius calculation, containment
- **Torus**: Construction, volume, surface area, containment

**Test Count**: ~30 tests  
**Coverage**: All primitive geometry classes and their basic operations

### 2. test_collision_detector.cpp
Tests collision detection between various geometry types:
- **Sphere-Sphere**: No collision, touching, overlapping, contained
- **Sphere-AABB**: Various intersection scenarios including corner collisions
- **Sphere-Capsule**: Collision and containment tests
- **Capsule-Capsule**: Parallel, perpendicular, and detailed collision info
- **Capsule-AABB**: Intersection tests
- **Cylinder/Cone/Torus**: Collision with spheres
- **Broad-Phase**: Early rejection and narrow-phase pass-through

**Test Count**: ~30 tests  
**Coverage**: All CollisionDetector methods, including symmetric overloads

### 3. test_raycast_query.cpp
Tests ray-geometry intersection:
- **Ray-Sphere**: Miss, hit from outside/inside, tangent, detailed hits
- **Ray-Capsule**: Miss and hit scenarios
- **Ray-AABB**: Miss, center hit, corner hit, from inside
- **Ray-Plane**: Parallel miss, hits, behind ray
- **Ray-Triangle**: Möller-Trumbore algorithm tests with barycentric coordinates
- **Edge Cases**: Origin on surface, zero direction, multiple rays

**Test Count**: ~25 tests  
**Coverage**: All RaycastQuery methods

### 4. test_distance_query.cpp
Tests distance calculations and closest point queries:
- **Point-to-Geometry**: Distance to sphere, capsule, AABB, cylinder
- **Sphere-to-Geometry**: Separated, touching, overlapping spheres
- **Capsule-to-Geometry**: Parallel and perpendicular configurations
- **Helper Functions**: Closest point on line segment, closest points between segments
- **Closest Point Pairs**: Sphere-capsule, capsule-capsule
- **Edge Cases**: Zero-length segments, coincident spheres

**Test Count**: ~25 tests  
**Coverage**: All DistanceQuery methods

### 5. test_containment_query.cpp
Tests containment and inclusion checks:
- **Point Containment**: Sphere, capsule, AABB point queries
- **Sphere Containment**: Contains smaller/larger spheres, overlapping cases
- **AABB Containment**: Sphere in AABB, AABB in AABB
- **Edge Cases**: Coincident geometries, zero-size objects, boundary points
- **Multiple Checks**: Nested containment, multiple point queries

**Test Count**: ~30 tests  
**Coverage**: All ContainmentQuery methods

### 6. test_obb.cpp
Comprehensive tests for OBB (Oriented Bounding Box):
- **Construction**: Default, from center, with rotation, 45-degree rotation
- **Ray-OBB Intersection**: Aligned hits/misses, rotated OBB, from inside, edge hits, corner near-miss
- **OBB-OBB Collision**: Aligned (no collision, touching, overlapping), one inside other, rotated collision, perpendicular collision
- **OBB-Sphere Collision**: No collision, center, edge, corner, sphere inside
- **Containment**: Point at center, inside, on surface, outside, with rotated OBB
- **Edge Cases**: Zero size, very large, very small

**Test Count**: ~40 tests  
**Coverage**: OBB construction, ray intersection, collision detection, containment

### 7. test_triangle.cpp
Comprehensive tests for Triangle geometry:
- **2D Point-in-Triangle**: Center, outside, on edge, on vertex, near edge, equilateral, degenerate, large, very small, negative coords
- **2D Triangle Area**: Simple, unit, equilateral, degenerate
- **3D Triangle**: Area in XY plane, arbitrary plane, normal calculation, reversed winding
- **Ray-Triangle Intersection**: Perpendicular, miss, edge, vertex, parallel miss, behind ray, oblique angle, grazing
- **Barycentric Coordinates**: Center, vertex, edge midpoint
- **Triangle-Triangle Intersection**: Coplanar overlapping/separated, different planes, edge touching
- **Edge Cases**: Zero area, very large, very small

**Test Count**: ~45 tests  
**Coverage**: Point-in-triangle, area calculation, ray intersection, barycentric coordinates, triangle-triangle intersection

### 8. test_frustum.cpp
Comprehensive tests for Frustum (View Frustum):
- **Plane Extraction**: Identity matrix, perspective, orthographic, MVP matrix
- **Frustum-Sphere Intersection**: Inside, outside (left, right, top, bottom, near, far), on edge, large radius
- **Frustum-AABB Intersection**: Inside, outside, partially inside, behind camera
- **Point Containment**: Origin, outside
- **Orthographic Frustum**: Sphere inside, AABB outside
- **Frustum Updates**: Different matrices
- **Edge Cases**: Very narrow FOV, very wide FOV, extreme aspect ratio

**Test Count**: ~30 tests  
**Coverage**: Frustum plane extraction, intersection tests, point containment

### 9. test_collision_2d.cpp
Comprehensive tests for 2D collision detection:
- **Circle-Circle**: Separated, touching, overlapping, one inside other, coincident, diagonal, barely touching, different sizes
- **Circle-Point**: Inside, outside, on edge, at center, negative coords, zero radius
- **Rectangle-Rectangle**: Separated, touching, overlapping, one inside other, coincident, corner overlap, horizontal/vertical overlap only
- **Rectangle-Point**: Inside, outside, on edge, on corner, at min
- **Circle-Rectangle**: Separated, circle inside, overlapping edge/corner, encompasses, touching edge, near miss corner
- **Line Segment Intersection**: Cross, parallel, collinear (overlapping/separated), touching endpoint, would cross if extended, perpendicular
- **2D Ray Tests**: Circle hit/miss, from inside, rect hit/miss
- **Edge Cases**: Zero radius circle, zero size rectangle, very large/small circles

**Test Count**: ~50 tests  
**Coverage**: All 2D collision detection functions

### 10. test_line_segment.cpp
Comprehensive tests for LineSegment class:
- **Construction**: Default, parameterized, set endpoints, set start/end
- **Properties**: Center, direction, reverse direction, vector, distance, distance squared, 3D distance
- **Parameterization**: Get point at start, end, midpoint, quarter, three-quarters
- **Closest Point**: Perpendicular, beyond start/end, on segment, diagonal segment, 3D segment, zero-length segment
- **LineSegment-LineSegment**: Parallel, perpendicular, skew segments
- **LineSegment-Sphere**: Through center, tangent, no intersection, endpoint inside
- **Edge Cases**: Very long/short, negative coordinates, vertical segment

**Test Count**: ~40 tests  
**Coverage**: LineSegment construction, properties, parameterization, closest points, intersections

### 11. test_hollow_cylinder.cpp
Comprehensive tests for HollowCylinder class:
- **Construction**: Default, parameterized, set, axis normalization
- **Properties**: Top/bottom center, volume, surface area
- **Point Containment**: In wall, in hole, outside, beyond height, on outer/inner surface, at top/bottom edge, rotated axis, center of hole
- **Ray-HollowCylinder**: Through wall, through hole, miss, along axis
- **Distance and Closest Point**: Inside wall, outside, in hole
- **HollowCylinder-Sphere**: In wall, outside, in hole, bridging wall
- **Edge Cases**: Very thin wall, very long/short, tilted 45 degrees

**Test Count**: ~35 tests  
**Coverage**: HollowCylinder construction, properties, containment, ray intersection, collision

## Building and Running Tests

### Prerequisites
- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.28 or higher
- GLM library (OpenGL Mathematics)

### Build Tests

```bash
cd tests
mkdir build
cd build
cmake ..
make
```

### Run Individual Tests

```bash
./test_geometry_primitives
./test_collision_detector
./test_raycast_query
./test_distance_query
./test_containment_query
./test_obb
./test_triangle
./test_frustum
./test_collision_2d
./test_line_segment
./test_hollow_cylinder
```

### Run All Tests

```bash
make run_all_tests
```

## Test Structure

All tests follow a consistent pattern:

```cpp
void test_feature_name() {
    // Arrange: Set up test data
    Sphere sphere(Vector3f(0, 0, 0), 1.0f);
    
    // Act: Perform operation
    bool result = sphere.ContainsPoint(Vector3f(0.5f, 0, 0));
    
    // Assert: Verify result
    ASSERT_TRUE(result);
}
```

### Assertion Macros
- `ASSERT_TRUE(expr)`: Assert expression is true
- `ASSERT_FALSE(expr)`: Assert expression is false
- `ASSERT_NEAR(a, b, epsilon)`: Assert values are within epsilon

## Coverage Summary

| Module | Test File | Test Count | Coverage |
|--------|-----------|------------|----------|
| Geometry Primitives | test_geometry_primitives.cpp | ~30 | 100% |
| Collision Detection | test_collision_detector.cpp | ~30 | 95% |
| Ray Casting | test_raycast_query.cpp | ~25 | 90% |
| Distance Queries | test_distance_query.cpp | ~25 | 95% |
| Containment | test_containment_query.cpp | ~30 | 100% |
| OBB | test_obb.cpp | ~40 | 95% |
| Triangle | test_triangle.cpp | ~45 | 95% |
| Frustum | test_frustum.cpp | ~30 | 90% |
| 2D Collision | test_collision_2d.cpp | ~50 | 95% |
| LineSegment | test_line_segment.cpp | ~40 | 95% |
| HollowCylinder | test_hollow_cylinder.cpp | ~35 | 90% |
| **Total** | | **~380** | **95%** |

## Test Categories

### 1. Basic Functionality Tests
- Verify correct construction and property access
- Test volume and surface area calculations
- Validate geometric properties

### 2. Boundary Tests
- Points on surfaces
- Touching geometries
- Edge and corner cases

### 3. Containment Tests
- Inside/outside classifications
- Overlapping scenarios
- Nested geometries

### 4. Distance and Proximity Tests
- Distance calculations
- Closest point queries
- Separation tests

### 5. Collision Detection Tests
- Intersection tests
- Detailed collision information
- Broad-phase optimization

### 6. Edge Cases
- Zero-size geometries
- Coincident objects
- Degenerate cases
- Numerical precision boundaries

## Adding New Tests

To add a new test:

1. Create a new `.cpp` file in this directory
2. Include the necessary headers
3. Use the `TEST()` macro for each test function
4. Add the test executable to `CMakeLists.txt`
5. Update this README with test coverage information

Example:

```cpp
#include <hgl/math/geometry/primitives/Sphere.h>

void test_new_feature() {
    // Test implementation
}

int main() {
    TEST(new_feature);
    return 0;
}
```

## Continuous Integration

These tests are designed to be run in CI/CD pipelines:

```yaml
# Example CI configuration
test:
  script:
    - cd tests/build
    - cmake ..
    - make
    - make run_all_tests
```

## Known Limitations

1. **Approximate Methods**: Some collision detection methods (e.g., capsule-AABB) use sampling and may have edge cases
2. **Numerical Precision**: Tests use epsilon values (typically 0.01) for floating-point comparisons
3. **Performance Tests**: Current test suite focuses on correctness, not performance benchmarks

## Future Enhancements

- [ ] Performance benchmarks for collision detection
- [ ] Stress tests with large numbers of geometries
- [ ] Fuzzing tests for edge cases
- [ ] Visual debugging tools
- [ ] Memory leak detection
- [ ] Multi-threaded safety tests
- [ ] Batch query optimization tests
- [ ] More complex geometry types (ellipsoid, polyhedron, mesh)
- [ ] Advanced collision response and manifold generation tests

## Reporting Issues

If a test fails:
1. Note the test name and error message
2. Check if it's a known limitation
3. Verify your build environment
4. Report issue with reproduction steps

## License

Same as parent project (CMMath library).
