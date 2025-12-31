##==================================================================================================
## CMMath - Game Mathematics Library
## 数学库 CMake 配置文件
##==================================================================================================

# Base include paths
set(CMMATH_MATH_INCLUDE_PATH        ${CMMATH_ROOT_INCLUDE_PATH}/hgl/math)
set(CMMATH_GEOMETRY_INCLUDE_PATH    ${CMMATH_MATH_INCLUDE_PATH}/geometry)
set(CMMATH_ANIMATION_INCLUDE_PATH   ${CMMATH_MATH_INCLUDE_PATH}/animation)
set(CMMATH_TRANSFORM_INCLUDE_PATH   ${CMMATH_MATH_INCLUDE_PATH}/transform)
set(CMMATH_GRAPH_INCLUDE_PATH       ${CMMATH_ROOT_INCLUDE_PATH}/hgl/graph)

# Note: Spatial module currently has build issues and is excluded from the build
# set(CMMATH_SPATIAL_INCLUDE_PATH   ${CMMATH_MATH_INCLUDE_PATH}/spatial)

##==================================================================================================
## Math Module - Header Files
##==================================================================================================

# Math Core: Constants and fundamental utilities
set(CMMATH_MATH_CORE_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/MathConstants.h
    ${CMMATH_MATH_INCLUDE_PATH}/TrigConstants.h
    ${CMMATH_MATH_INCLUDE_PATH}/MathUtils.h
    ${CMMATH_MATH_INCLUDE_PATH}/PhysicsConst.h
    ${CMMATH_MATH_INCLUDE_PATH}/Projection.h
    ${CMMATH_MATH_INCLUDE_PATH}/Sum.h
)

# Math Vectors: Vector types and operations
set(CMMATH_MATH_VECTORS_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/Vector.h
    ${CMMATH_MATH_INCLUDE_PATH}/VectorTypes.h
    ${CMMATH_MATH_INCLUDE_PATH}/VectorOperations.h
    ${CMMATH_MATH_INCLUDE_PATH}/VectorUtils.h
    ${CMMATH_MATH_INCLUDE_PATH}/NormalData.h
    ${CMMATH_MATH_INCLUDE_PATH}/bvec.h
)

# Math Linear Algebra: Matrices and quaternions
set(CMMATH_MATH_LINALG_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/MatrixTypes.h
    ${CMMATH_MATH_INCLUDE_PATH}/MatrixOperations.h
    ${CMMATH_MATH_INCLUDE_PATH}/Quaternion.h
)

# Math Numeric: Numeric types and interpolation
set(CMMATH_MATH_NUMERIC_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/HalfFloat.h
    ${CMMATH_MATH_INCLUDE_PATH}/LerpType.h
    ${CMMATH_MATH_INCLUDE_PATH}/Lerp1D.h
    ${CMMATH_MATH_INCLUDE_PATH}/Lerp2D.h
    ${CMMATH_MATH_INCLUDE_PATH}/Lerp3D.h
)

# Math Algorithms: Specialized algorithms
set(CMMATH_MATH_ALGORITHMS_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/FastTriangle.h
    ${CMMATH_MATH_INCLUDE_PATH}/Area.h
)

# Math Noise: Noise generation functions
set(CMMATH_MATH_NOISE_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/Noise.h
)

# Math Curves: Curve and easing systems
set(CMMATH_MATH_CURVES_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/Curve.h
)

# Math SIMD: SIMD optimizations
set(CMMATH_MATH_SIMD_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/SIMD.h
)

# Math Game: Game development specific utilities
set(CMMATH_MATH_GAME_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/Easing.h
    ${CMMATH_MATH_INCLUDE_PATH}/Transform2D.h
    ${CMMATH_MATH_INCLUDE_PATH}/Random.h
    ${CMMATH_MATH_INCLUDE_PATH}/Color.h
    ${CMMATH_MATH_INCLUDE_PATH}/Luminance.h
)

# Combined Math headers list
set(CMMATH_MATH_HEADERS
    ${CMMATH_MATH_CORE_HEADERS}
    ${CMMATH_MATH_VECTORS_HEADERS}
    ${CMMATH_MATH_LINALG_HEADERS}
    ${CMMATH_MATH_NUMERIC_HEADERS}
    ${CMMATH_MATH_ALGORITHMS_HEADERS}
    ${CMMATH_MATH_NOISE_HEADERS}
    ${CMMATH_MATH_CURVES_HEADERS}
    ${CMMATH_MATH_SIMD_HEADERS}
    ${CMMATH_MATH_GAME_HEADERS}
)

##==================================================================================================
## Geometry Module - Header Files
##==================================================================================================

# Geometry Primitives: Basic geometric shapes
set(CMMATH_GEOMETRY_PRIMITIVES_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Plane.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Sphere.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Triangle.h
)

# Geometry Solids: 3D solid shapes
set(CMMATH_GEOMETRY_SOLIDS_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Capsule.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Cylinder.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/HollowCylinder.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Cone.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Torus.h
)

# Geometry Bounding: Bounding volumes
set(CMMATH_GEOMETRY_BOUNDING_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/BoundingSphere.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/AABB.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/OBB.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/BoundingVolumes.h
)

# Geometry Query: Query and intersection shapes
set(CMMATH_GEOMETRY_QUERY_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Ray.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/LineSegment.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Frustum.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Coordinate.h
)

# Geometry 2D: 2D geometry utilities
set(CMMATH_GEOMETRY_2D_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/AABB2D.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Collision2D.h
)

# Geometry Utils: Utility functions
set(CMMATH_GEOMETRY_UTILS_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/GeometryUtils.h
)

# Combined Geometry headers list
set(CMMATH_GEOMETRY_HEADERS
    ${CMMATH_GEOMETRY_PRIMITIVES_HEADERS}
    ${CMMATH_GEOMETRY_SOLIDS_HEADERS}
    ${CMMATH_GEOMETRY_BOUNDING_HEADERS}
    ${CMMATH_GEOMETRY_QUERY_HEADERS}
    ${CMMATH_GEOMETRY_2D_HEADERS}
    ${CMMATH_GEOMETRY_UTILS_HEADERS}
)

##==================================================================================================
## Animation Module - Header Files
##==================================================================================================

set(CMMATH_ANIMATION_HEADERS
    ${CMMATH_ANIMATION_INCLUDE_PATH}/DualQuaternion.h
    ${CMMATH_ANIMATION_INCLUDE_PATH}/SkeletonMath.h
)

##==================================================================================================
## Transform Module - Header Files
##==================================================================================================

set(CMMATH_TRANSFORM_HEADERS
    ${CMMATH_TRANSFORM_INCLUDE_PATH}/Transform.h
    ${CMMATH_TRANSFORM_INCLUDE_PATH}/TransformAction.h
)

##==================================================================================================
## Graph Module - Header Files
##==================================================================================================

set(CMMATH_GRAPH_HEADERS
    ${CMMATH_GRAPH_INCLUDE_PATH}/CameraInfo.h
)

##==================================================================================================
## Math Module - Source Files
##==================================================================================================

# Math Core sources
set(CMMATH_MATH_CORE_SOURCES
    Math/LAtan.cpp
    Math/LSinCos.cpp
    Math/Matrix4f.cpp
    Math/HalfFloat.cpp
)

# Math Noise sources
set(CMMATH_MATH_NOISE_SOURCES
    Math/PerlinNoise.cpp
    Math/SimplexNoise.cpp
)

# Math Curves sources
set(CMMATH_MATH_CURVES_SOURCES
    Math/Curve.cpp
)

# Math Game sources
set(CMMATH_MATH_GAME_SOURCES
    Math/Random.cpp
    Math/Color.cpp
    Math/Luminance.cpp
)

# Combined Math sources list
set(CMMATH_MATH_SOURCES
    ${CMMATH_MATH_CORE_SOURCES}
    ${CMMATH_MATH_NOISE_SOURCES}
    ${CMMATH_MATH_CURVES_SOURCES}
    ${CMMATH_MATH_GAME_SOURCES}
)

##==================================================================================================
## Geometry Module - Source Files
##==================================================================================================

# Geometry Primitives sources
set(CMMATH_GEOMETRY_PRIMITIVES_SOURCES
    Geometry/Plane.cpp
)

# Geometry Bounding sources
set(CMMATH_GEOMETRY_BOUNDING_SOURCES
    Geometry/AABB.cpp
    Geometry/OBB.cpp
    Geometry/OBB_SetFromPoints_AVX2.cpp
    Geometry/BoundingSphere.cpp
    Geometry/BoundingVolumes.cpp
)

# Geometry Query sources
set(CMMATH_GEOMETRY_QUERY_SOURCES
    Geometry/Ray.cpp
    Geometry/Frustum.cpp
)

# Geometry Solids sources
set(CMMATH_GEOMETRY_SOLIDS_SOURCES
    Geometry/Cylinder.cpp
    Geometry/Cone.cpp
)

# Geometry Utils sources
set(CMMATH_GEOMETRY_UTILS_SOURCES
    Geometry/GeometryUtils.cpp
)

# Combined Geometry sources list
set(CMMATH_GEOMETRY_SOURCES
    ${CMMATH_GEOMETRY_PRIMITIVES_SOURCES}
    ${CMMATH_GEOMETRY_BOUNDING_SOURCES}
    ${CMMATH_GEOMETRY_QUERY_SOURCES}
    ${CMMATH_GEOMETRY_SOLIDS_SOURCES}
    ${CMMATH_GEOMETRY_UTILS_SOURCES}
)

##==================================================================================================
## Animation Module - Source Files
##==================================================================================================

set(CMMATH_ANIMATION_SOURCES
    Math/DualQuaternion.cpp
    Math/SkeletonMath.cpp
)

##==================================================================================================
## Transform Module - Source Files
##==================================================================================================

set(CMMATH_TRANSFORM_SOURCES
    Transform/Transform.cpp
)

##==================================================================================================
## Visual Studio Source Groups Organization
##==================================================================================================

# Math module headers organized by category
source_group("Math\\Core" FILES ${CMMATH_MATH_CORE_HEADERS})
source_group("Math\\Vectors" FILES ${CMMATH_MATH_VECTORS_HEADERS})
source_group("Math\\Linear Algebra" FILES ${CMMATH_MATH_LINALG_HEADERS})
source_group("Math\\Numeric" FILES ${CMMATH_MATH_NUMERIC_HEADERS})
source_group("Math\\Algorithms" FILES ${CMMATH_MATH_ALGORITHMS_HEADERS})
source_group("Math\\Noise" FILES ${CMMATH_MATH_NOISE_HEADERS})
source_group("Math\\Curves" FILES ${CMMATH_MATH_CURVES_HEADERS})
source_group("Math\\SIMD" FILES ${CMMATH_MATH_SIMD_HEADERS})
source_group("Math\\Game" FILES ${CMMATH_MATH_GAME_HEADERS})

# Math module sources organized by category
source_group("Source\\Math\\Core" FILES ${CMMATH_MATH_CORE_SOURCES})
source_group("Source\\Math\\Noise" FILES ${CMMATH_MATH_NOISE_SOURCES})
source_group("Source\\Math\\Curves" FILES ${CMMATH_MATH_CURVES_SOURCES})
source_group("Source\\Math\\Game" FILES ${CMMATH_MATH_GAME_SOURCES})

# Geometry module headers organized by category
source_group("Geometry\\Primitives" FILES ${CMMATH_GEOMETRY_PRIMITIVES_HEADERS})
source_group("Geometry\\Solids" FILES ${CMMATH_GEOMETRY_SOLIDS_HEADERS})
source_group("Geometry\\Bounding" FILES ${CMMATH_GEOMETRY_BOUNDING_HEADERS})
source_group("Geometry\\Query" FILES ${CMMATH_GEOMETRY_QUERY_HEADERS})
source_group("Geometry\\2D" FILES ${CMMATH_GEOMETRY_2D_HEADERS})
source_group("Geometry\\Utils" FILES ${CMMATH_GEOMETRY_UTILS_HEADERS})

# Geometry module sources organized by category
source_group("Source\\Geometry\\Primitives" FILES ${CMMATH_GEOMETRY_PRIMITIVES_SOURCES})
source_group("Source\\Geometry\\Bounding" FILES ${CMMATH_GEOMETRY_BOUNDING_SOURCES})
source_group("Source\\Geometry\\Query" FILES ${CMMATH_GEOMETRY_QUERY_SOURCES})
source_group("Source\\Geometry\\Solids" FILES ${CMMATH_GEOMETRY_SOLIDS_SOURCES})
source_group("Source\\Geometry\\Utils" FILES ${CMMATH_GEOMETRY_UTILS_SOURCES})

# Animation module
source_group("Animation" FILES ${CMMATH_ANIMATION_HEADERS})
source_group("Source\\Animation" FILES ${CMMATH_ANIMATION_SOURCES})

# Transform module
source_group("Transform" FILES ${CMMATH_TRANSFORM_HEADERS})
source_group("Source\\Transform" FILES ${CMMATH_TRANSFORM_SOURCES})

# Graph module (CameraInfo)
source_group("Graph\\Camera" FILES ${CMMATH_GRAPH_HEADERS})

# Note: Spatial module is currently excluded due to build issues
# When fixed, add: source_group("Spatial" FILES ${CMMATH_SPATIAL_HEADERS})

##==================================================================================================
## Library Creation
##==================================================================================================

# Create CMMath library with all modules
add_cm_library(CMMath "CM"
    ${CMMATH_MATH_HEADERS}
    ${CMMATH_MATH_SOURCES}
    ${CMMATH_GEOMETRY_HEADERS}
    ${CMMATH_GEOMETRY_SOURCES}
    ${CMMATH_ANIMATION_HEADERS}
    ${CMMATH_ANIMATION_SOURCES}
    ${CMMATH_TRANSFORM_HEADERS}
    ${CMMATH_TRANSFORM_SOURCES}
    ${CMMATH_GRAPH_HEADERS}
)

##==================================================================================================
## Dependencies and Include Paths
##==================================================================================================

# Find GLM library (required dependency)
find_package(glm CONFIG)
if(NOT glm_FOUND)
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(GLM glm)
    endif()
endif()

# Set include directories
target_include_directories(CMMath PUBLIC ${CMMATH_ROOT_INCLUDE_PATH})

# Link GLM library
if(glm_FOUND)
    target_link_libraries(CMMath PUBLIC glm::glm)
elseif(GLM_FOUND)
    target_include_directories(CMMath PUBLIC ${GLM_INCLUDE_DIRS})
endif()
