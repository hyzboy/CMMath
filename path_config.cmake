macro(CMMathSetup CMMATH_ROOT_PATH)
    message("CMMath root path: " ${CMMATH_ROOT_PATH})

    set(CMMATH_ROOT_INCLUDE_PATH ${CMMATH_ROOT_PATH}/inc)
    set(CMMATH_ROOT_SOURCE_PATH ${CMMATH_ROOT_PATH}/src)

    include_directories(${CMMATH_ROOT_INCLUDE_PATH})
endmacro()
