/**
 * test_frustum.cpp
 *
 * Frustum tests aligned with current engine conventions:
 * - world space: Z-up
 * - view space: LookAt RH (camera forward maps to -Z in view)
 * - clip depth: Vulkan [0,1]
 */

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <hgl/math/geometry/Frustum.h>
#include <hgl/math/Projection.h>

using namespace hgl::math;

#define TEST(name) \
    std::cout << "Testing " << #name << "... "; \
    test_##name(); \
    std::cout << "PASSED" << std::endl;

#define ASSERT_TRUE(expr) \
    if (!(expr)) { \
        std::cerr << "FAILED: " << #expr << " at line " << __LINE__ << std::endl; \
        std::exit(1); \
    }

#define ASSERT_FALSE(expr) ASSERT_TRUE(!(expr))

namespace
{
    constexpr float kPi = 3.14159265358979323846f;

    Matrix4f MakeViewProj(const float aspect = 1.0f,
                          const float near_z = 1.0f,
                          const float far_z = 100.0f)
    {
        const Matrix4f projection = PerspectiveMatrix(60.0f, aspect, near_z, far_z);
        const Matrix4f view = LookAtMatrix(Vector3f(0.0f, -10.0f, 0.0f),
                                           Vector3f(0.0f,   0.0f, 0.0f),
                                           AxisVector::Z);
        return projection * view;
    }
}

void test_frustum_planes_extraction_finite()
{
    FrustumPlanes planes;
    GetFrustumPlanes(planes, MakeViewProj(16.0f / 9.0f, 0.1f, 500.0f));

    for (int i = 0; i < 6; ++i)
    {
        ASSERT_TRUE(std::isfinite(planes[i].x));
        ASSERT_TRUE(std::isfinite(planes[i].y));
        ASSERT_TRUE(std::isfinite(planes[i].z));
        ASSERT_TRUE(std::isfinite(planes[i].w));
    }
}

void test_point_in_front_is_inside()
{
    Frustum fr;
    fr.SetMatrix(MakeViewProj());

    ASSERT_TRUE(fr.PointIn(Vector3f(0.0f, 0.0f, 0.0f)) != Frustum::Scope::OUTSIDE);
}

void test_point_behind_camera_is_outside()
{
    Frustum fr;
    fr.SetMatrix(MakeViewProj());

    ASSERT_TRUE(fr.PointIn(Vector3f(0.0f, -20.0f, 0.0f)) == Frustum::Scope::OUTSIDE);
}

void test_sphere_inside()
{
    Frustum fr;
    fr.SetMatrix(MakeViewProj());

    ASSERT_TRUE(fr.SphereIn(Vector3f(0.0f, 0.0f, 0.0f), 1.0f) != Frustum::Scope::OUTSIDE);
}

void test_sphere_outside_left_right_top_bottom()
{
    Frustum fr;
    fr.SetMatrix(MakeViewProj());

    ASSERT_TRUE(fr.SphereIn(Vector3f(-100.0f, 0.0f,   0.0f), 1.0f) == Frustum::Scope::OUTSIDE);
    ASSERT_TRUE(fr.SphereIn(Vector3f( 100.0f, 0.0f,   0.0f), 1.0f) == Frustum::Scope::OUTSIDE);
    ASSERT_TRUE(fr.SphereIn(Vector3f(   0.0f, 0.0f, 100.0f), 1.0f) == Frustum::Scope::OUTSIDE);
    ASSERT_TRUE(fr.SphereIn(Vector3f(   0.0f, 0.0f,-100.0f), 1.0f) == Frustum::Scope::OUTSIDE);
}

void test_sphere_outside_near_far()
{
    Frustum fr;
    fr.SetMatrix(MakeViewProj(1.0f, 1.0f, 100.0f));

    // Near plane is about 1 unit ahead of eye at y=-10, so y=-9.5 is before near.
    ASSERT_TRUE(fr.SphereIn(Vector3f(0.0f, -9.5f, 0.0f), 0.1f) == Frustum::Scope::OUTSIDE);

    // Far plane roughly near y=90 in this setup.
    ASSERT_TRUE(fr.SphereIn(Vector3f(0.0f, 150.0f, 0.0f), 1.0f) == Frustum::Scope::OUTSIDE);
}

void test_aabb_inside_and_outside()
{
    Frustum fr;
    fr.SetMatrix(MakeViewProj());

    AABB inside_box;
    inside_box.SetMinMax(Vector3f(-1.0f, -1.0f, -1.0f), Vector3f(1.0f, 1.0f, 1.0f));

    AABB outside_box;
    outside_box.SetMinMax(Vector3f(50.0f, -20.0f, 50.0f), Vector3f(52.0f, -18.0f, 52.0f));

    ASSERT_TRUE(fr.BoxIn(inside_box) != Frustum::Scope::OUTSIDE);
    ASSERT_TRUE(fr.BoxIn(outside_box) == Frustum::Scope::OUTSIDE);
}

void test_orthographic_frustum_basic()
{
    Frustum fr;

    const Matrix4f projection = OrthoMatrix(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    const Matrix4f view = LookAtMatrix(Vector3f(0.0f, -10.0f, 0.0f),
                                       Vector3f(0.0f,   0.0f, 0.0f),
                                       AxisVector::Z);
    fr.SetMatrix(projection * view);

    ASSERT_TRUE(fr.SphereIn(Vector3f(0.0f, 0.0f, 0.0f), 1.0f) != Frustum::Scope::OUTSIDE);
    ASSERT_TRUE(fr.PointIn(Vector3f(30.0f, 0.0f, 0.0f)) == Frustum::Scope::OUTSIDE);
}

void test_extreme_fov_stability()
{
    Frustum fr_narrow;
    Frustum fr_wide;

    const Matrix4f view = LookAtMatrix(Vector3f(0.0f, -10.0f, 0.0f),
                                       Vector3f(0.0f,   0.0f, 0.0f),
                                       AxisVector::Z);

    fr_narrow.SetMatrix(PerspectiveMatrix(1.0f, 1.0f, 1.0f, 100.0f) * view);
    fr_wide.SetMatrix(PerspectiveMatrix(170.0f, 1.0f, 1.0f, 100.0f) * view);

    ASSERT_TRUE(fr_narrow.SphereIn(Vector3f(0.0f, 0.0f, 0.0f), 0.1f) != Frustum::Scope::OUTSIDE);
    ASSERT_TRUE(fr_wide.SphereIn(Vector3f(8.0f, 0.0f, 0.0f), 1.0f) != Frustum::Scope::OUTSIDE);
}

int main()
{
    std::cout << "=== Frustum Test Suite (Z-up world) ===" << std::endl << std::endl;

    TEST(frustum_planes_extraction_finite);
    TEST(point_in_front_is_inside);
    TEST(point_behind_camera_is_outside);
    TEST(sphere_inside);
    TEST(sphere_outside_left_right_top_bottom);
    TEST(sphere_outside_near_far);
    TEST(aabb_inside_and_outside);
    TEST(orthographic_frustum_basic);
    TEST(extreme_fov_stability);

    std::cout << std::endl << "=== All Frustum Tests Passed! ===" << std::endl;
    return 0;
}
