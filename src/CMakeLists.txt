##==================================================================================================
## Math 数学库
##==================================================================================================
SET(MATH_INCLUDE_PATH   ${CMMATH_ROOT_INCLUDE_PATH}/hgl/math)
SET(MATH_GEOMETRY_INCLUDE_PATH  ${MATH_INCLUDE_PATH}/geometry)

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
                    )

SET(CMMATH_GEOMETRY_HEADER  ${MATH_GEOMETRY_INCLUDE_PATH}/Plane.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Sphere.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/LineSegment.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Capsule.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Triangle.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/BoundingSphere.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/AABB.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/OBB.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/BoundingVolumes.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Ray.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Frustum.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/Coordinate.h
                            ${MATH_GEOMETRY_INCLUDE_PATH}/TRS.h
                            )

SET(CMMATH_SOURCE   Math/LAtan.cpp
                    Math/LSinCos.cpp
                    Math/Matrix4f.cpp
                    Math/HalfFloat.cpp)

SET(CMMATH_GEOMETRY_SOURCE  Geometry/Plane.cpp
                            Geometry/AABB.cpp
                            Geometry/OBB.cpp
                            Geometry/OBB_SetFromPoints_AVX2.cpp
                            Geometry/BoundingSphere.cpp
                            Geometry/BoundingVolumes.cpp
                            Geometry/Ray.cpp
                            Geometry/Frustum.cpp
                            Geometry/TRS.cpp)

SET(CM_CAMERA_INFO_SOURCE ${CMMATH_ROOT_INCLUDE_PATH}/hgl/graph/CameraInfo.h)

SOURCE_GROUP("Header Files" FILES ${CMMATH_HEADER} ${CM_CAMERA_INFO_SOURCE})
SOURCE_GROUP("Source Files" FILES ${CMMATH_SOURCE})
SOURCE_GROUP("Geometry\\Header Files" FILES ${CMMATH_GEOMETRY_HEADER})
SOURCE_GROUP("Geometry\\Source Files" FILES ${CMMATH_GEOMETRY_SOURCE})

add_cm_library(CMMath "CM" ${CMMATH_HEADER} ${CMMATH_SOURCE} ${CMMATH_GEOMETRY_HEADER} ${CMMATH_GEOMETRY_SOURCE} ${CM_CAMERA_INFO_SOURCE})
