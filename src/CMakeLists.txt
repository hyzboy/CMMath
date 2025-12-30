##==================================================================================================
## Math 数学库
##==================================================================================================
SET(MATH_INCLUDE_PATH   ${CMMATH_ROOT_INCLUDE_PATH}/hgl/math)
SET(MATH_GEOMETRY_INCLUDE_PATH  ${MATH_INCLUDE_PATH}/geometry)
SET(MATH_SPATIAL_INCLUDE_PATH  ${MATH_INCLUDE_PATH}/spatial)
SET(MATH_ANIMATION_INCLUDE_PATH  ${MATH_INCLUDE_PATH}/animation)

SET(CMMATH_HEADER   ${MATH_INCLUDE_PATH}/bvec.h
                    ${MATH_INCLUDE_PATH}/FastTriangle.h
                    ${MATH_INCLUDE_PATH}/Area.h
                    ${MATH_INCLUDE_PATH}/MathConstants.h
                    ${MATH_INCLUDE_PATH}/TrigConstants.h
                    ${MATH_INCLUDE_PATH}/MathUtils.h
                    ${MATH_INCLUDE_PATH}/MatrixTypes.h
                    ${MATH_INCLUDE_PATH}/MatrixOperations.h
                    ${MATH_INCLUDE_PATH}/Quaternion.h
                    ${MATH_INCLUDE_PATH}/Projection.h
                    ${MATH_INCLUDE_PATH}/Vector.h
                    ${MATH_INCLUDE_PATH}/VectorTypes.h
                    ${MATH_INCLUDE_PATH}/VectorOperations.h
                    ${MATH_INCLUDE_PATH}/VectorUtils.h
                    ${MATH_INCLUDE_PATH}/Sum.h
                    ${MATH_INCLUDE_PATH}/PhysicsConst.h
                    ${MATH_INCLUDE_PATH}/HalfFloat.h
                    ${MATH_INCLUDE_PATH}/LerpType.h
                    ${MATH_INCLUDE_PATH}/Lerp1D.h
                    ${MATH_INCLUDE_PATH}/Lerp2D.h
                    ${MATH_INCLUDE_PATH}/Lerp3D.h
                    ${MATH_INCLUDE_PATH}/NormalData.h
                    ${MATH_INCLUDE_PATH}/Noise.h
                    ${MATH_INCLUDE_PATH}/SIMD.h
                    ${MATH_INCLUDE_PATH}/Curve.h
                    ${MATH_INCLUDE_PATH}/Easing.h
                    ${MATH_INCLUDE_PATH}/Transform2D.h
                    ${MATH_INCLUDE_PATH}/Random.h
                    ${MATH_INCLUDE_PATH}/Color.h
                    )

SET(CMMATH_GEOMETRY_HEADER  ${MATH_GEOMETRY_INCLUDE_PATH}/Plane.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Sphere.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/LineSegment.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Capsule.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Cylinder.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Torus.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/HollowCylinder.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Cone.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Triangle.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/BoundingSphere.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/AABB.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/OBB.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/BoundingVolumes.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Ray.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Frustum.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Coordinate.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/AABB2D.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Collision2D.h
                            )

SET(CMMATH_SPATIAL_HEADER  ${MATH_SPATIAL_INCLUDE_PATH}/QuadTree.h
                          ${MATH_SPATIAL_INCLUDE_PATH}/Octree.h
                          ${MATH_SPATIAL_INCLUDE_PATH}/BVH.h
                          )

SET(CMMATH_ANIMATION_HEADER  ${MATH_ANIMATION_INCLUDE_PATH}/DualQuaternion.h
                            ${MATH_ANIMATION_INCLUDE_PATH}/SkeletonMath.h
                            )

SET(CMMATH_SOURCE   Math/LAtan.cpp
                    Math/LSinCos.cpp
                    Math/Matrix4f.cpp
                    Math/HalfFloat.cpp
                    Math/PerlinNoise.cpp
                    Math/SimplexNoise.cpp
                    Math/SIMD_SSE.cpp
                    Math/SIMD_AVX.cpp
                    Math/Curve.cpp
                    Math/DualQuaternion.cpp
                    Math/SkeletonMath.cpp)
                    Math/Random.cpp
                    Math/Color.cpp)

SET(CMMATH_GEOMETRY_SOURCE  Geometry/Plane.cpp
                            Geometry/AABB.cpp
                            Geometry/OBB.cpp
                            Geometry/OBB_SetFromPoints_AVX2.cpp
                            Geometry/BoundingSphere.cpp
                            Geometry/BoundingVolumes.cpp
                            Geometry/Cylinder.cpp
                            Geometry/Cone.cpp
                            Geometry/Ray.cpp
                            Geometry/Frustum.cpp
                            )

SET(CMMATH_SPATIAL_SOURCE  Spatial/QuadTree.cpp
                          Spatial/Octree.cpp
                          Spatial/BVH.cpp
                          )

SET(CM_CAMERA_INFO_SOURCE ${CMMATH_ROOT_INCLUDE_PATH}/hgl/graph/CameraInfo.h)

# Organized SOURCE_GROUPs for Visual Studio

# Core math headers
SOURCE_GROUP("Math\\Core" FILES
    ${MATH_INCLUDE_PATH}/MathConstants.h
    ${MATH_INCLUDE_PATH}/TrigConstants.h
    ${MATH_INCLUDE_PATH}/MathUtils.h
    ${MATH_INCLUDE_PATH}/Projection.h
    ${MATH_INCLUDE_PATH}/PhysicsConst.h
    ${MATH_INCLUDE_PATH}/Sum.h
)

# Types and basic utilities
SOURCE_GROUP("Math\\Types" FILES
    ${MATH_INCLUDE_PATH}/MatrixTypes.h
    ${MATH_INCLUDE_PATH}/VectorTypes.h
    ${MATH_INCLUDE_PATH}/bvec.h
    ${MATH_INCLUDE_PATH}/Vector.h
)

# Linear algebra and operations
SOURCE_GROUP("Math\\Linear Algebra" FILES
    ${MATH_INCLUDE_PATH}/MatrixOperations.h
    ${MATH_INCLUDE_PATH}/Quaternion.h
)

# Vector utilities and helpers
SOURCE_GROUP("Math\\Vectors" FILES
    ${MATH_INCLUDE_PATH}/VectorOperations.h
    ${MATH_INCLUDE_PATH}/VectorUtils.h
    ${MATH_INCLUDE_PATH}/NormalData.h
)

# Numeric utilities
SOURCE_GROUP("Math\\Numeric" FILES
    ${MATH_INCLUDE_PATH}/HalfFloat.h
    ${MATH_INCLUDE_PATH}/LerpType.h
    ${MATH_INCLUDE_PATH}/Lerp1D.h
    ${MATH_INCLUDE_PATH}/Lerp2D.h
    ${MATH_INCLUDE_PATH}/Lerp3D.h
)

# Special utilities and algorithms
SOURCE_GROUP("Math\\Algorithms" FILES
    ${MATH_INCLUDE_PATH}/FastTriangle.h
    ${MATH_INCLUDE_PATH}/Area.h
)

# Noise functions
SOURCE_GROUP("Math\\Noise" FILES
    ${MATH_INCLUDE_PATH}/Noise.h
)

# SIMD optimizations
SOURCE_GROUP("Math\\SIMD" FILES
    ${MATH_INCLUDE_PATH}/SIMD.h
)

# Curve system
SOURCE_GROUP("Math\\Curves" FILES
    ${MATH_INCLUDE_PATH}/Curve.h
# Game development utilities
SOURCE_GROUP("Math\\Game" FILES
    ${MATH_INCLUDE_PATH}/Easing.h
    ${MATH_INCLUDE_PATH}/Transform2D.h
    ${MATH_INCLUDE_PATH}/Random.h
    ${MATH_INCLUDE_PATH}/Color.h
)

# Geometry headers grouped by function
SOURCE_GROUP("Geometry\\Primitives" FILES
    ${MATH_GEOMETRY_INCLUDE_PATH}/Plane.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/Sphere.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/Triangle.h
)

SOURCE_GROUP("Geometry\\Curves" FILES
    ${MATH_GEOMETRY_INCLUDE_PATH}/Torus.h
)

SOURCE_GROUP("Geometry\\Solids" FILES
    ${MATH_GEOMETRY_INCLUDE_PATH}/Capsule.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/Cylinder.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/HollowCylinder.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/Cone.h
)

SOURCE_GROUP("Geometry\\Bounding" FILES
    ${MATH_GEOMETRY_INCLUDE_PATH}/BoundingSphere.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/AABB.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/OBB.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/BoundingVolumes.h
)

SOURCE_GROUP("Geometry\\Query" FILES
    ${MATH_GEOMETRY_INCLUDE_PATH}/LineSegment.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/Ray.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/Frustum.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/Coordinate.h
)

# Spatial partitioning headers
SOURCE_GROUP("Spatial" FILES
    ${MATH_SPATIAL_INCLUDE_PATH}/QuadTree.h
    ${MATH_SPATIAL_INCLUDE_PATH}/Octree.h
    ${MATH_SPATIAL_INCLUDE_PATH}/BVH.h
)

# Animation math headers
SOURCE_GROUP("Animation" FILES
    ${MATH_ANIMATION_INCLUDE_PATH}/DualQuaternion.h
    ${MATH_ANIMATION_INCLUDE_PATH}/SkeletonMath.h
SOURCE_GROUP("Geometry\\2D" FILES
    ${MATH_GEOMETRY_INCLUDE_PATH}/AABB2D.h
    ${MATH_GEOMETRY_INCLUDE_PATH}/Collision2D.h
)

# Camera related header
SOURCE_GROUP("Graph\\Camera" FILES ${CM_CAMERA_INFO_SOURCE})

# Source files: core math
SOURCE_GROUP("Source\\Math" FILES
    Math/LAtan.cpp
    Math/LSinCos.cpp
    Math/Matrix4f.cpp
    Math/HalfFloat.cpp
)

SOURCE_GROUP("Source\\Math\\Noise" FILES
    Math/PerlinNoise.cpp
    Math/SimplexNoise.cpp
)

SOURCE_GROUP("Source\\Math\\SIMD" FILES
    Math/SIMD_SSE.cpp
    Math/SIMD_AVX.cpp
)

SOURCE_GROUP("Source\\Math\\Curves" FILES
    Math/Curve.cpp
)

SOURCE_GROUP("Source\\Math\\Animation" FILES
    Math/DualQuaternion.cpp
    Math/SkeletonMath.cpp
)

# Source files: geometry categorized
SOURCE_GROUP("Source\\Geometry\\Primitives" FILES
    Geometry/Plane.cpp
)

SOURCE_GROUP("Source\\Geometry\\Bounding" FILES
    Geometry/AABB.cpp
    Geometry/OBB.cpp
    Geometry/BoundingSphere.cpp
    Geometry/BoundingVolumes.cpp
)

SOURCE_GROUP("Source\\Geometry\\Utilities" FILES
    Geometry/OBB_SetFromPoints_AVX2.cpp
    Geometry/Cylinder.cpp
    Geometry/Cone.cpp
    Geometry/Ray.cpp
    Geometry/Frustum.cpp
)

SOURCE_GROUP("Source\\Spatial" FILES
    Spatial/QuadTree.cpp
    Spatial/Octree.cpp
    Spatial/BVH.cpp
)

# Fallback groups used by the original add_cm_library invocation
SOURCE_GROUP("Header Files" FILES ${CMMATH_HEADER} ${CM_CAMERA_INFO_SOURCE})
SOURCE_GROUP("Source Files" FILES ${CMMATH_SOURCE})
SOURCE_GROUP("Geometry\\Header Files" FILES ${CMMATH_GEOMETRY_HEADER})
SOURCE_GROUP("Geometry\\Source Files" FILES ${CMMATH_GEOMETRY_SOURCE})
SOURCE_GROUP("Spatial\\Header Files" FILES ${CMMATH_SPATIAL_HEADER})
SOURCE_GROUP("Spatial\\Source Files" FILES ${CMMATH_SPATIAL_SOURCE})
SOURCE_GROUP("Animation\\Header Files" FILES ${CMMATH_ANIMATION_HEADER})
SOURCE_GROUP("Animation\\Source Files" FILES Math/DualQuaternion.cpp Math/SkeletonMath.cpp)

add_cm_library(CMMath "CM" ${CMMATH_HEADER} ${CMMATH_SOURCE} ${CMMATH_GEOMETRY_HEADER} ${CMMATH_GEOMETRY_SOURCE} ${CMMATH_SPATIAL_HEADER} ${CMMATH_SPATIAL_SOURCE} ${CMMATH_ANIMATION_HEADER} ${CM_CAMERA_INFO_SOURCE})

# SIMD 编译选项配置
# 为 SIMD 源文件设置特定的编译标志以启用相应的指令集
if(MSVC)
    # Microsoft Visual C++ 编译器
    set_source_files_properties(Math/SIMD_SSE.cpp PROPERTIES COMPILE_FLAGS "/arch:SSE2")
    set_source_files_properties(Math/SIMD_AVX.cpp PROPERTIES COMPILE_FLAGS "/arch:AVX2")
else()
    # GCC/Clang 编译器
    set_source_files_properties(Math/SIMD_SSE.cpp PROPERTIES COMPILE_FLAGS "-msse4.1")
    set_source_files_properties(Math/SIMD_AVX.cpp PROPERTIES COMPILE_FLAGS "-mavx2 -mfma")
endif()

# Create CMMath library
find_package(glm CONFIG)
if(NOT glm_FOUND)
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(GLM glm)
    endif()
endif()

add_library(CMMath STATIC ${CMMATH_HEADER} ${CMMATH_SOURCE} ${CMMATH_GEOMETRY_HEADER} ${CMMATH_GEOMETRY_SOURCE} ${CM_CAMERA_INFO_SOURCE})
target_include_directories(CMMath PUBLIC ${CMMATH_ROOT_INCLUDE_PATH})

if(glm_FOUND)
    target_link_libraries(CMMath PUBLIC glm::glm)
elseif(GLM_FOUND)
    target_include_directories(CMMath PUBLIC ${GLM_INCLUDE_DIRS})
endif()
