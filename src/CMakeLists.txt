##==================================================================================================
## Math 数学库
##==================================================================================================
SET(MATH_INCLUDE_PATH   ${CMMATH_ROOT_INCLUDE_PATH}/hgl/math)

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
                    )

SET(CMMATH_SOURCE   Math/LAtan.cpp
                    Math/LSinCos.cpp
                    Math/Matrix4f.cpp
                    Math/HalfFloat.cpp)

SOURCE_GROUP("Header Files" FILES ${CMMATH_HEADER})
SOURCE_GROUP("Source Files" FILES ${CMMATH_SOURCE})

add_cm_library(CMMath "CM" ${CMMATH_HEADER} ${CMMATH_SOURCE})
