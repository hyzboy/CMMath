# CMMath Examples
# Lightweight wrapper for cm_example_project with module-specific settings

macro(cm_example_project sub_folder project_name)
    cm_example_project_base(
        PROJECT_NAME ${project_name}
        FOLDER_PATH "Examples/CMMath/${sub_folder}"
        SOURCES ${ARGN}
        PRIVATE_LIBRARIES CMMath
    )
    # Set C++20 standard for all CMMath examples
    set_property(TARGET ${project_name} PROPERTY CXX_STANDARD 20)
endmacro()

####################################################################################################

cm_example_project("" HalfFloatTest         HalfFloatTest.cpp)
cm_example_project("" AlphaBlendTest        AlphaBlendTest.cpp)

cm_example_project("" OutputEpsilon         OutputEpsilon.cpp)
cm_example_project("" TransformBenchmark    TransformBenchmark.cpp)
cm_example_project("" TransformTest         TransformTest.cpp)
cm_example_project("" RotateMatrix          RotateMatrix.cpp)

# Geometry Test Suite - 拆分为多个独立测试
cm_example_project("" AABBTest              AABBTest.cpp)
cm_example_project("" OBBTest               OBBTest.cpp)
cm_example_project("" BoundingSphereTest    BoundingSphereTest.cpp)
cm_example_project("" BoundingVolumesTest   BoundingVolumesTest.cpp)
cm_example_project("" FrustumTest           FrustumTest.cpp)
cm_example_project("" SphereTest            SphereTest.cpp)
cm_example_project("" RayTest               RayTest.cpp)
cm_example_project("" LineSegmentTest       LineSegmentTest.cpp)
cm_example_project("" PlaneTest             PlaneTest.cpp)
cm_example_project("" TriangleTest          TriangleTest.cpp)
#cm_example_project("" NormalCompressTest    NormalCompressTest.cpp)
cm_example_project("" AngleTest             AngleTest.cpp)
cm_example_project("" QuaternionTest        QuaternionTest.cpp)
cm_example_project("" GeometryTest          GeometryTest.cpp)
