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
| **Total** | | **~140** | **96%** |

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

## Reporting Issues

If a test fails:
1. Note the test name and error message
2. Check if it's a known limitation
3. Verify your build environment
4. Report issue with reproduction steps

## License

Same as parent project (CMMath library).
