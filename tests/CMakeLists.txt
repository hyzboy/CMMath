# Test Suite CMakeLists.txt for CMMath Geometry System

cmake_minimum_required(VERSION 3.28)

# Include parent configuration
include(../path_config.cmake)
CMMathSetup(${CMAKE_CURRENT_SOURCE_DIR}/..)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find GLM library
find_package(glm CONFIG)
if(NOT glm_FOUND)
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(GLM glm)
    endif()
endif()

# Test executables
set(TEST_EXECUTABLES
    test_geometry_primitives
    test_collision_detector
    test_raycast_query
    test_distance_query
    test_containment_query
    test_obb
    test_triangle
    test_frustum
    test_collision_2d
    test_line_segment
    test_hollow_cylinder
    test_polygon_2d
)

# Create test executables
foreach(test_name ${TEST_EXECUTABLES})
    add_executable(${test_name} ${test_name}.cpp)
    
    target_include_directories(${test_name} PRIVATE ${CMMATH_ROOT_INCLUDE_PATH})
    
    if(glm_FOUND)
        target_link_libraries(${test_name} PRIVATE glm::glm)
    elseif(GLM_FOUND)
        target_include_directories(${test_name} PRIVATE ${GLM_INCLUDE_DIRS})
    endif()
    
    # Link against CMMath library
    target_link_libraries(${test_name} PRIVATE CMMath)
endforeach()

# Add custom target to run all tests
add_custom_target(run_all_tests
    COMMAND echo "Running Geometry Primitives Tests..."
    COMMAND test_geometry_primitives
    COMMAND echo ""
    COMMAND echo "Running Collision Detector Tests..."
    COMMAND test_collision_detector
    COMMAND echo ""
    COMMAND echo "Running Raycast Query Tests..."
    COMMAND test_raycast_query
    COMMAND echo ""
    COMMAND echo "Running Distance Query Tests..."
    COMMAND test_distance_query
    COMMAND echo ""
    COMMAND echo "Running Containment Query Tests..."
    COMMAND test_containment_query
    COMMAND echo ""
    COMMAND echo "Running OBB Tests..."
    COMMAND test_obb
    COMMAND echo ""
    COMMAND echo "Running Triangle Tests..."
    COMMAND test_triangle
    COMMAND echo ""
    COMMAND echo "Running Frustum Tests..."
    COMMAND test_frustum
    COMMAND echo ""
    COMMAND echo "Running 2D Collision Tests..."
    COMMAND test_collision_2d
    COMMAND echo ""
    COMMAND echo "Running LineSegment Tests..."
    COMMAND test_line_segment
    COMMAND echo ""
    COMMAND echo "Running HollowCylinder Tests..."
    COMMAND test_hollow_cylinder
    COMMAND echo ""
    COMMAND echo "Running 2D Polygon Tests..."
    COMMAND test_polygon_2d
    DEPENDS ${TEST_EXECUTABLES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Print test information
message(STATUS "Configured ${CMAKE_CURRENT_LIST_FILE}")
message(STATUS "Test executables: ${TEST_EXECUTABLES}")
message(STATUS "Run 'make run_all_tests' to execute all tests")
