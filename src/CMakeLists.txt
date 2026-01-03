##==================================================================================================
## CMMath - Game Mathematics Library
## 数学库 CMake 配置文件
##==================================================================================================

##==================================================================================================
## Include Path Definitions
##==================================================================================================

set(CMMATH_MATH_INCLUDE_PATH        ${CMMATH_ROOT_INCLUDE_PATH}/hgl/math)
set(CMMATH_GEOMETRY_INCLUDE_PATH    ${CMMATH_MATH_INCLUDE_PATH}/geometry)
set(CMMATH_ANIMATION_INCLUDE_PATH   ${CMMATH_MATH_INCLUDE_PATH}/animation)
set(CMMATH_TRANSFORM_INCLUDE_PATH   ${CMMATH_MATH_INCLUDE_PATH}/transform)
set(CMMATH_GRAPH_INCLUDE_PATH       ${CMMATH_ROOT_INCLUDE_PATH}/hgl/graph)

##==================================================================================================
## Math Module - Headers
##==================================================================================================

# Core: Constants and fundamental utilities
set(CMMATH_MATH_CORE_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/TrigConstants.h
    ${CMMATH_MATH_INCLUDE_PATH}/MathUtils.h
    ${CMMATH_MATH_INCLUDE_PATH}/PhysicsCalculations.h
    ${CMMATH_MATH_INCLUDE_PATH}/Projection.h
    ${CMMATH_MATH_INCLUDE_PATH}/Sum.h
    ${CMMATH_MATH_INCLUDE_PATH}/Clamp.h
)

# Vectors: Vector types and operations
set(CMMATH_MATH_VECTORS_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/Vector.h
    ${CMMATH_MATH_INCLUDE_PATH}/VectorTypes.h
    ${CMMATH_MATH_INCLUDE_PATH}/VectorOperations.h
    ${CMMATH_MATH_INCLUDE_PATH}/VectorUtils.h
    ${CMMATH_MATH_INCLUDE_PATH}/NormalData.h
    ${CMMATH_MATH_INCLUDE_PATH}/bvec.h
)

# Linear Algebra: Matrices and quaternions
set(CMMATH_MATH_LINALG_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/MatrixTypes.h
    ${CMMATH_MATH_INCLUDE_PATH}/MatrixOperations.h
    ${CMMATH_MATH_INCLUDE_PATH}/Quaternion.h
)

# Numeric: Numeric types and interpolation
set(CMMATH_MATH_NUMERIC_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/HalfFloat.h
    ${CMMATH_MATH_INCLUDE_PATH}/LerpType.h
    ${CMMATH_MATH_INCLUDE_PATH}/Lerp1D.h
    ${CMMATH_MATH_INCLUDE_PATH}/Lerp2D.h
    ${CMMATH_MATH_INCLUDE_PATH}/Lerp3D.h
    ${CMMATH_MATH_INCLUDE_PATH}/VectorLerp.h
    ${CMMATH_MATH_INCLUDE_PATH}/Gradient.h
)

# Algorithms: Specialized algorithms
set(CMMATH_MATH_ALGORITHMS_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/FastTriangle.h
    ${CMMATH_MATH_INCLUDE_PATH}/Area.h
)

# Noise: Noise generation
set(CMMATH_MATH_NOISE_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/Noise.h
)

# Curves: Curve and easing systems
set(CMMATH_MATH_CURVES_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/Curve.h
)

# SIMD: SIMD optimizations
set(CMMATH_MATH_SIMD_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/SIMD.h
)

# Game: Game development utilities
set(CMMATH_MATH_GAME_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/Easing.h
    ${CMMATH_MATH_INCLUDE_PATH}/Transform2D.h
    ${CMMATH_MATH_INCLUDE_PATH}/Random.h
    ${CMMATH_MATH_INCLUDE_PATH}/Color.h
    ${CMMATH_MATH_INCLUDE_PATH}/Luminance.h
)

##==================================================================================================
## Math Module - Sources
##==================================================================================================

# Core sources
set(CMMATH_MATH_CORE_SOURCES
    Math/LAtan.cpp
    Math/LSinCos.cpp
    Math/Matrix4f.cpp
    Math/HalfFloat.cpp
)

# Noise sources
set(CMMATH_MATH_NOISE_SOURCES
    Math/PerlinNoise.cpp
    Math/SimplexNoise.cpp
)

# Curves sources
set(CMMATH_MATH_CURVES_SOURCES
    Math/Curve.cpp
)

# Game sources
set(CMMATH_MATH_GAME_SOURCES
    Math/Random.cpp
    Math/Color.cpp
    Math/Luminance.cpp
)

##==================================================================================================
## Geometry Module - Headers
##==================================================================================================

# Primitives: Basic geometric shapes
set(CMMATH_GEOMETRY_PRIMITIVES_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Plane.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Triangle.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/primitives/GeometryBase.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/primitives/Sphere.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/primitives/Capsule.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/primitives/Cylinder.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/primitives/Cone.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/primitives/Torus.h
)

# Solids: Complex 3D shapes
set(CMMATH_GEOMETRY_SOLIDS_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/HollowCylinder.h
)

# Bounding: Bounding volumes
set(CMMATH_GEOMETRY_BOUNDING_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/AABB.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/OBB.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/BoundingSphere.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/BoundingVolumes.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/BoundingVolumesDataStorage.h
)

# Query: Intersection and query shapes
set(CMMATH_GEOMETRY_QUERY_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Ray.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/LineSegment.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Frustum.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Coordinate.h
)

# 2D: 2D geometry
set(CMMATH_GEOMETRY_2D_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/AABB2D.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Collision2D.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/Polygon2D.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/HeightMapContour.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/ShorelineData.h
)

# Queries: Collision detection and query algorithms
set(CMMATH_GEOMETRY_QUERIES_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/queries/CollisionDetector.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/queries/RaycastQuery.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/queries/DistanceQuery.h
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/queries/ContainmentQuery.h
)

# Utils: Utility functions
set(CMMATH_GEOMETRY_UTILS_HEADERS
    ${CMMATH_GEOMETRY_INCLUDE_PATH}/GeometryUtils.h
)

# Unified: Convenience entry points
set(CMMATH_GEOMETRY_UNIFIED_HEADERS
    ${CMMATH_MATH_INCLUDE_PATH}/Geometry.h
    ${CMMATH_MATH_INCLUDE_PATH}/GeometryCore.h
    ${CMMATH_MATH_INCLUDE_PATH}/GeometryExtended.h
    ${CMMATH_MATH_INCLUDE_PATH}/GeometryQueries.h
)

##==================================================================================================
## Geometry Module - Sources
##==================================================================================================

# Primitives sources
set(CMMATH_GEOMETRY_PRIMITIVES_SOURCES
    Geometry/Plane.cpp
)

# Bounding sources
set(CMMATH_GEOMETRY_BOUNDING_SOURCES
    Geometry/AABB.cpp
    Geometry/OBB.cpp
    Geometry/OBB_SetFromPoints_AVX2.cpp
    Geometry/BoundingSphere.cpp
    Geometry/BoundingVolumes.cpp
)

# 2D sources
set(CMMATH_GEOMETRY_2D_SOURCES
    Geometry/HeightMapContour.cpp
    Geometry/ShorelineData.cpp
)

# Query sources
set(CMMATH_GEOMETRY_QUERY_SOURCES
    Geometry/Ray.cpp
    Geometry/Frustum.cpp
)

# Queries sources
set(CMMATH_GEOMETRY_QUERIES_SOURCES
    Geometry/queries/CollisionDetector.cpp
    Geometry/queries/RaycastQuery.cpp
    Geometry/queries/DistanceQuery.cpp
    Geometry/queries/ContainmentQuery.cpp
)

# Utils sources
set(CMMATH_GEOMETRY_UTILS_SOURCES
    Geometry/GeometryUtils.cpp
)

##==================================================================================================
## Animation Module
##==================================================================================================

set(CMMATH_ANIMATION_HEADERS
    ${CMMATH_ANIMATION_INCLUDE_PATH}/DualQuaternion.h
    ${CMMATH_ANIMATION_INCLUDE_PATH}/SkeletonMath.h
)

set(CMMATH_ANIMATION_SOURCES
    Math/DualQuaternion.cpp
    Math/SkeletonMath.cpp
)

##==================================================================================================
## Transform Module
##==================================================================================================

set(CMMATH_TRANSFORM_HEADERS
    ${CMMATH_TRANSFORM_INCLUDE_PATH}/Transform.h
    ${CMMATH_TRANSFORM_INCLUDE_PATH}/TransformAction.h
)

set(CMMATH_TRANSFORM_SOURCES
    Transform/Transform.cpp
)

##==================================================================================================
## Graph Module
##==================================================================================================

set(CMMATH_GRAPH_HEADERS
    ${CMMATH_GRAPH_INCLUDE_PATH}/CameraInfo.h
)

##==================================================================================================
## Aggregate Header and Source Lists
##==================================================================================================

# Math module aggregate
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

set(CMMATH_MATH_SOURCES
    ${CMMATH_MATH_CORE_SOURCES}
    ${CMMATH_MATH_NOISE_SOURCES}
    ${CMMATH_MATH_CURVES_SOURCES}
    ${CMMATH_MATH_GAME_SOURCES}
)

# Geometry module aggregate
set(CMMATH_GEOMETRY_HEADERS
    ${CMMATH_GEOMETRY_PRIMITIVES_HEADERS}
    ${CMMATH_GEOMETRY_SOLIDS_HEADERS}
    ${CMMATH_GEOMETRY_BOUNDING_HEADERS}
    ${CMMATH_GEOMETRY_QUERY_HEADERS}
    ${CMMATH_GEOMETRY_2D_HEADERS}
    ${CMMATH_GEOMETRY_QUERIES_HEADERS}
    ${CMMATH_GEOMETRY_UTILS_HEADERS}
    ${CMMATH_GEOMETRY_UNIFIED_HEADERS}
)

set(CMMATH_GEOMETRY_SOURCES
    ${CMMATH_GEOMETRY_PRIMITIVES_SOURCES}
    ${CMMATH_GEOMETRY_BOUNDING_SOURCES}
    ${CMMATH_GEOMETRY_QUERY_SOURCES}
    ${CMMATH_GEOMETRY_QUERIES_SOURCES}
    ${CMMATH_GEOMETRY_UTILS_SOURCES}
    ${CMMATH_GEOMETRY_2D_SOURCES}
)

##==================================================================================================
## Visual Studio Source Groups
##==================================================================================================

# Math module groups
source_group("Math\\Core"               FILES ${CMMATH_MATH_CORE_HEADERS}       ${CMMATH_MATH_CORE_SOURCES})
source_group("Math\\Vectors"            FILES ${CMMATH_MATH_VECTORS_HEADERS})
source_group("Math\\Linear Algebra"     FILES ${CMMATH_MATH_LINALG_HEADERS})
source_group("Math\\Numeric"            FILES ${CMMATH_MATH_NUMERIC_HEADERS})
source_group("Math\\Algorithms"         FILES ${CMMATH_MATH_ALGORITHMS_HEADERS})
source_group("Math\\Noise"              FILES ${CMMATH_MATH_NOISE_HEADERS}      ${CMMATH_MATH_NOISE_SOURCES})
source_group("Math\\Curves"             FILES ${CMMATH_MATH_CURVES_HEADERS}     ${CMMATH_MATH_CURVES_SOURCES})
source_group("Math\\SIMD"               FILES ${CMMATH_MATH_SIMD_HEADERS})
source_group("Math\\Game"               FILES ${CMMATH_MATH_GAME_HEADERS}       ${CMMATH_MATH_GAME_SOURCES})

# Geometry module groups
source_group("Geometry\\Primitives"     FILES ${CMMATH_GEOMETRY_PRIMITIVES_HEADERS}     ${CMMATH_GEOMETRY_PRIMITIVES_SOURCES})
source_group("Geometry\\Solids"         FILES ${CMMATH_GEOMETRY_SOLIDS_HEADERS})
source_group("Geometry\\Bounding"       FILES ${CMMATH_GEOMETRY_BOUNDING_HEADERS}       ${CMMATH_GEOMETRY_BOUNDING_SOURCES})
source_group("Geometry\\Query"          FILES ${CMMATH_GEOMETRY_QUERY_HEADERS}          ${CMMATH_GEOMETRY_QUERY_SOURCES})
source_group("Geometry\\2D"             FILES ${CMMATH_GEOMETRY_2D_HEADERS}             ${CMMATH_GEOMETRY_2D_SOURCES})
source_group("Geometry\\Queries"        FILES ${CMMATH_GEOMETRY_QUERIES_HEADERS}        ${CMMATH_GEOMETRY_QUERIES_SOURCES})
source_group("Geometry\\Utils"          FILES ${CMMATH_GEOMETRY_UTILS_HEADERS}          ${CMMATH_GEOMETRY_UTILS_SOURCES})
source_group("Geometry\\Unified"        FILES ${CMMATH_GEOMETRY_UNIFIED_HEADERS})

# Animation module group
source_group("Animation"                FILES ${CMMATH_ANIMATION_HEADERS}       ${CMMATH_ANIMATION_SOURCES})

# Transform module group
source_group("Transform"                FILES ${CMMATH_TRANSFORM_HEADERS}       ${CMMATH_TRANSFORM_SOURCES})

# Graph module group
source_group("Graph\\Camera"            FILES ${CMMATH_GRAPH_HEADERS})

##==================================================================================================
## Library Target
##==================================================================================================

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
## Target Configuration
##==================================================================================================

target_include_directories(CMMath PUBLIC ${CMMATH_ROOT_INCLUDE_PATH})

