// 测试 BoundingVolumes 改进的示例代码
#include<hgl/math/geometry/BoundingVolumes.h>
#include<iostream>

using namespace hgl::math;

void TestIsEmpty()
{
    std::cout << "=== 测试 IsEmpty 修复 ===" << std::endl;

    BoundingVolumes bv;
    std::cout << "空的 BoundingVolumes IsEmpty: " << (bv.IsEmpty() ? "true" : "false") << std::endl;

    AABB box;
    box.SetMinMax(Vector3f(0,0,0), Vector3f(1,1,1));
    bv.SetFromAABB(box);
    std::cout << "设置后 IsEmpty: " << (bv.IsEmpty() ? "true" : "false") << std::endl;
    std::cout << std::endl;
}

void TestSetFromAABB()
{
    std::cout << "=== 测试 SetFromAABB 球体半径修复 ===" << std::endl;

    AABB box;
    box.SetMinMax(Vector3f(-1,-1,-1), Vector3f(1,1,1));

    BoundingVolumes bv;
    bv.SetFromAABB(box);

    std::cout << "AABB: min(-1,-1,-1) max(1,1,1)" << std::endl;
    std::cout << "BoundingSphere center: ("
              << bv.bsphere.center.x << ", "
              << bv.bsphere.center.y << ", "
              << bv.bsphere.center.z << ")" << std::endl;
    std::cout << "BoundingSphere radius: " << bv.bsphere.radius << std::endl;
    std::cout << "期望半径(半对角线): " << glm::length(Vector3f(1,1,1)) << std::endl;
    std::cout << std::endl;
}

void TestSetFromOBB()
{
    std::cout << "=== 测试新增的 SetFromOBB 方法 ===" << std::endl;

    OBB obb;
    obb.Set(Vector3f(0,0,0), Vector3f(2,3,4));

    BoundingVolumes bv;
    bv.SetFromOBB(obb);

    std::cout << "OBB center: (0,0,0) half_extents: (2,3,4)" << std::endl;
    std::cout << "生成的 AABB min: ("
              << bv.aabb.GetMin().x << ", "
              << bv.aabb.GetMin().y << ", "
              << bv.aabb.GetMin().z << ")" << std::endl;
    std::cout << "生成的 AABB max: ("
              << bv.aabb.GetMax().x << ", "
              << bv.aabb.GetMax().y << ", "
              << bv.aabb.GetMax().z << ")" << std::endl;
    std::cout << std::endl;
}

void TestSetFromSphere()
{
    std::cout << "=== 测试新增的 SetFromSphere 方法 ===" << std::endl;

    BoundingSphere sphere;
    sphere.center = Vector3f(1,2,3);
    sphere.radius = 5.0f;

    BoundingVolumes bv;
    bv.SetFromSphere(sphere);

    std::cout << "Sphere center: (1,2,3) radius: 5" << std::endl;
    std::cout << "生成的 AABB min: ("
              << bv.aabb.GetMin().x << ", "
              << bv.aabb.GetMin().y << ", "
              << bv.aabb.GetMin().z << ")" << std::endl;
    std::cout << "生成的 AABB max: ("
              << bv.aabb.GetMax().x << ", "
              << bv.aabb.GetMax().y << ", "
              << bv.aabb.GetMax().z << ")" << std::endl;
    std::cout << "期望: min(-4,-3,-2) max(6,7,8)" << std::endl;
    std::cout << std::endl;
}

void TestTransformed()
{
    std::cout << "=== 测试新增的 Transformed 方法 ===" << std::endl;

    AABB box;
    box.SetMinMax(Vector3f(-1,-1,-1), Vector3f(1,1,1));

    BoundingVolumes bv;
    bv.SetFromAABB(box);

    // 平移变换
    Matrix4f transform = glm::translate(Matrix4f(1.0f), Vector3f(10, 20, 30));

    BoundingVolumes transformed = bv.Transformed(transform);

    std::cout << "原始 AABB center: ("
              << bv.aabb.GetCenter().x << ", "
              << bv.aabb.GetCenter().y << ", "
              << bv.aabb.GetCenter().z << ")" << std::endl;
    std::cout << "变换后 AABB center: ("
              << transformed.aabb.GetCenter().x << ", "
              << transformed.aabb.GetCenter().y << ", "
              << transformed.aabb.GetCenter().z << ")" << std::endl;
    std::cout << "期望 center: (10, 20, 30)" << std::endl;
    std::cout << std::endl;
}

void TestIntersectsSphere()
{
    std::cout << "=== 测试新增的 IntersectsSphere 方法 ===" << std::endl;

    AABB box;
    box.SetMinMax(Vector3f(-1,-1,-1), Vector3f(1,1,1));

    BoundingVolumes bv;
    bv.SetFromAABB(box);

    // 相交的球体
    bool intersects1 = bv.IntersectsSphere(Vector3f(2, 0, 0), 1.5f);
    std::cout << "球体(2,0,0,r=1.5)与包围体相交: " << (intersects1 ? "true" : "false") << std::endl;

    // 不相交的球体
    bool intersects2 = bv.IntersectsSphere(Vector3f(10, 0, 0), 1.0f);
    std::cout << "球体(10,0,0,r=1.0)与包围体相交: " << (intersects2 ? "true" : "false") << std::endl;
    std::cout << std::endl;
}

void TestContainsPointAll()
{
    std::cout << "=== 测试新增的 ContainsPointAll 方法 ===" << std::endl;

    AABB box;
    box.SetMinMax(Vector3f(-1,-1,-1), Vector3f(1,1,1));

    BoundingVolumes bv;
    bv.SetFromAABB(box);

    Vector3f point1(0, 0, 0);  // 中心点
    Vector3f point2(2, 0, 0);  // 外部点

    std::cout << "点(0,0,0) ContainsPoint: " << (bv.ContainsPoint(point1) ? "true" : "false") << std::endl;
    std::cout << "点(0,0,0) ContainsPointAll: " << (bv.ContainsPointAll(point1) ? "true" : "false") << std::endl;
    std::cout << "点(2,0,0) ContainsPoint: " << (bv.ContainsPoint(point2) ? "true" : "false") << std::endl;
    std::cout << "点(2,0,0) ContainsPointAll: " << (bv.ContainsPointAll(point2) ? "true" : "false") << std::endl;
    std::cout << std::endl;
}

void TestGetters()
{
    std::cout << "=== 测试新增的 Getter 方法 ===" << std::endl;

    AABB box;
    box.SetMinMax(Vector3f(-1,-1,-1), Vector3f(1,1,1));

    BoundingVolumes bv;
    bv.SetFromAABB(box);

    std::cout << "GetCenter (AABB): ("
              << bv.GetCenter().x << ", "
              << bv.GetCenter().y << ", "
              << bv.GetCenter().z << ")" << std::endl;
    std::cout << "GetSphereCenter: ("
              << bv.GetSphereCenter().x << ", "
              << bv.GetSphereCenter().y << ", "
              << bv.GetSphereCenter().z << ")" << std::endl;
    std::cout << "GetOBBCenter: ("
              << bv.GetOBBCenter().x << ", "
              << bv.GetOBBCenter().y << ", "
              << bv.GetOBBCenter().z << ")" << std::endl;
    std::cout << "GetMaxRadius: " << bv.GetMaxRadius() << std::endl;
    std::cout << "GetSurfaceArea: " << bv.GetSurfaceArea() << std::endl;
    std::cout << "GetVolume: " << bv.GetVolume() << std::endl;
    std::cout << std::endl;
}

int main()
{
    std::cout << "BoundingVolumes 改进测试" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << std::endl;

    TestIsEmpty();
    TestSetFromAABB();
    TestSetFromOBB();
    TestSetFromSphere();
    TestTransformed();
    TestIntersectsSphere();
    TestContainsPointAll();
    TestGetters();

    std::cout << "=====================================" << std::endl;
    std::cout << "所有测试完成！" << std::endl;

    return 0;
}
