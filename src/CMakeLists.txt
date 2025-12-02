SET(MATH_INCLUDE_PATH ${CMMATH_ROOT_INCLUDE_PATH}/hgl/math)

file(GLOB MATH_HEADER_FILES ${MATH_INCLUDE_PATH}/*.h)

SET(MATH_SOURCE_FILES  Math/HalfFloat.cpp
                        Math/LAtan.cpp
                        Math/LSinCos.cpp
                        Math/Matrix4f.cpp
                        Math/Transform.cpp)

SET(GRAPH_INCLUDE_PATH ${CMMATH_ROOT_INCLUDE_PATH}/hgl/graph)

SET(GRAPH_BOUNDING_HEADER_FILES ${GRAPH_INCLUDE_PATH}/AABB.h
                                 ${GRAPH_INCLUDE_PATH}/OBB.h
                                 ${GRAPH_INCLUDE_PATH}/BoundingSphere.h
                                 ${GRAPH_INCLUDE_PATH}/BoundingVolumes.h)

SET(GRAPH_BOUNDING_SOURCE_FILES  graph/Bounding/AABB.cpp
                                 graph/Bounding/OBB.cpp
                                 graph/Bounding/OBB_SetFromPoints_AVX2.cpp
                                 graph/Bounding/BoundingSphere.cpp
                                 graph/Bounding/BoundingVolumes.cpp)

SET(GRAPH_CORE_HEADER_FILES  ${GRAPH_INCLUDE_PATH}/Capsule.h
                             ${GRAPH_INCLUDE_PATH}/Coordinate.h
                             ${GRAPH_INCLUDE_PATH}/Frustum.h
                             ${GRAPH_INCLUDE_PATH}/LineSegment.h
                             ${GRAPH_INCLUDE_PATH}/Plane.h
                             ${GRAPH_INCLUDE_PATH}/Ray.h
                             ${GRAPH_INCLUDE_PATH}/Sphere.h
                             ${GRAPH_INCLUDE_PATH}/TRS.h
                             ${GRAPH_INCLUDE_PATH}/Triangle.h)

SET(GRAPH_CORE_SOURCE_FILES  graph/Coordinate.cpp
                             graph/Ray.cpp
                             graph/Plane.cpp
                             graph/Frustum.cpp
                             graph/TRS.cpp)

SET(GRAPH_DISTANCE_SOURCE_FILES graph/DistanceField/DistanceField2D.cpp
                                graph/DistanceField/DistanceField3D.cpp)

SOURCE_GROUP("Math\\Header" FILES ${MATH_HEADER_FILES})
SOURCE_GROUP("Math\\Source" FILES ${MATH_SOURCE_FILES})
SOURCE_GROUP("Math\\Graph\\Bounding" FILES ${GRAPH_BOUNDING_HEADER_FILES} ${GRAPH_BOUNDING_SOURCE_FILES})
SOURCE_GROUP("Math\\Graph" FILES ${GRAPH_CORE_HEADER_FILES} ${GRAPH_CORE_SOURCE_FILES} ${GRAPH_DISTANCE_SOURCE_FILES})

add_cm_library(CMMath "CM" ${MATH_HEADER_FILES}
                        ${MATH_SOURCE_FILES}
                        ${GRAPH_BOUNDING_HEADER_FILES}
                        ${GRAPH_BOUNDING_SOURCE_FILES}
                        ${GRAPH_CORE_HEADER_FILES}
                        ${GRAPH_CORE_SOURCE_FILES}
                        ${GRAPH_DISTANCE_SOURCE_FILES})

target_link_libraries(CMMath PRIVATE CMTypeCore)
