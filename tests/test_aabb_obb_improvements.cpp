// 测试 AABB 和 OBB 改进的示例代码
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<iostream>

using namespace hgl::math;

void TestAABBClear()
{
    std::cout << "=== 测试 AABB::Clear() 修复 ===" << std::endl;

    AABB aabb;
    aabb.SetMinMax(Vector3f(-1, -1, -1), Vector3f(1, 1, 1));

    std::cout << "设置前 IsValid: " << (aabb.IsValid() ? "true" : "false") << std::endl;
    std::cout << "设置前 IsEmpty: " << (aabb.IsEmpty() ? "true" : "false") << std::endl;

    aabb.Clear();

    std::cout << "Clear 后 IsValid: " << (aabb.IsValid() ? "true" : "false") << std::endl;
    std::cout << "Clear 后 IsEmpty: " << (aabb.IsEmpty() ? "true" : "false") << std::endl;
    std::cout << "期望: IsValid=false, IsEmpty=true" << std::endl;
    std::cout << std::endl;
}

void TestOBBClear()
{
    std::cout << "=== 测试 OBB::Clear() 修复 ===" << std::endl;

    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    std::cout << "设置前 IsValid: " << (obb.IsValid() ? "true" : "false") << std::endl;
    std::cout << "设置前 IsEmpty: " << (obb.IsEmpty() ? "true" : "false") << std::endl;

    obb.Clear();

    std::cout << "Clear 后 IsValid: " << (obb.IsValid() ? "true" : "false") << std::endl;
    std::cout << "Clear 后 IsEmpty: " << (obb.IsEmpty() ? "true" : "false") << std::endl;
    std::cout << "期望: IsValid=false, IsEmpty=true" << std::endl;
    std::cout << std::endl;
}

void TestAABBFactoryMethods()
{
    std::cout << "=== 测试 AABB 工厂方法 ===" << std::endl;

    // FromTwoPoints
    AABB aabb1 = AABB::FromTwoPoints(Vector3f(1, 2, 3), Vector3f(-1, -2, -3));
    std::cout << "FromTwoPoints(1,2,3), (-1,-2,-3):" << std::endl;
    std::cout << "  Min: (" << aabb1.GetMin().x << ", " << aabb1.GetMin().y << ", " << aabb1.GetMin().z << ")" << std::endl;
    std::cout << "  Max: (" << aabb1.GetMax().x << ", " << aabb1.GetMax().y << ", " << aabb1.GetMax().z << ")" << std::endl;
    std::cout << "  期望: Min(-1,-2,-3) Max(1,2,3)" << std::endl;

    // FromCenterAndSize
    AABB aabb2 = AABB::FromCenterAndSize(Vector3f(0, 0, 0), Vector3f(2, 4, 6));
    std::cout << "FromCenterAndSize(0,0,0), (2,4,6):" << std::endl;
    std::cout << "  Min: (" << aabb2.GetMin().x << ", " << aabb2.GetMin().y << ", " << aabb2.GetMin().z << ")" << std::endl;
    std::cout << "  Max: (" << aabb2.GetMax().x << ", " << aabb2.GetMax().y << ", " << aabb2.GetMax().z << ")" << std::endl;
    std::cout << "  期望: Min(-1,-2,-3) Max(1,2,3)" << std::endl;

    // Empty
    AABB aabb3 = AABB::Empty();
    std::cout << "Empty() IsValid: " << (aabb3.IsValid() ? "true" : "false") << std::endl;
    std::cout << "Empty() IsEmpty: " << (aabb3.IsEmpty() ? "true" : "false") << std::endl;
    std::cout << std::endl;
}

void TestOBBFactoryMethods()
{
    std::cout << "=== 测试 OBB 工厂方法 ===" << std::endl;

    AABB aabb;
    aabb.SetMinMax(Vector3f(-1, -2, -3), Vector3f(1, 2, 3));

    OBB obb = OBB::FromAABB(aabb);

    std::cout << "FromAABB(AABB(-1,-2,-3, 1,2,3)):" << std::endl;
    std::cout << "  Center: (" << obb.GetCenter().x << ", " << obb.GetCenter().y << ", " << obb.GetCenter().z << ")" << std::endl;
    std::cout << "  HalfExtend: (" << obb.GetHalfExtend().x << ", " << obb.GetHalfExtend().y << ", " << obb.GetHalfExtend().z << ")" << std::endl;
    std::cout << "  期望: Center(0,0,0) HalfExtend(1,2,3)" << std::endl;
    std::cout << std::endl;
}

void TestAABBScaleAndOffset()
{
    std::cout << "=== 测试 AABB 缩放和偏移方法 ===" << std::endl;

    AABB aabb;
    aabb.SetMinMax(Vector3f(-1, -1, -1), Vector3f(1, 1, 1));

    // Scaled
    AABB scaled = aabb.Scaled(2.0f);
    std::cout << "原始 AABB Center: (" << aabb.GetCenter().x << ", " << aabb.GetCenter().y << ", " << aabb.GetCenter().z << ")" << std::endl;
    std::cout << "原始 AABB Size: (" << aabb.GetSize().x << ", " << aabb.GetSize().y << ", " << aabb.GetSize().z << ")" << std::endl;
    std::cout << "Scaled(2.0) Size: (" << scaled.GetSize().x << ", " << scaled.GetSize().y << ", " << scaled.GetSize().z << ")" << std::endl;
    std::cout << "期望 Size: (4, 4, 4)" << std::endl;

    // Offset
    AABB offset = aabb.Offset(Vector3f(10, 20, 30));
    std::cout << "Offset(10,20,30) Center: (" << offset.GetCenter().x << ", " << offset.GetCenter().y << ", " << offset.GetCenter().z << ")" << std::endl;
    std::cout << "期望 Center: (10, 20, 30)" << std::endl;

    // Expanded
    AABB expanded = aabb.Expanded(1.0f);
    std::cout << "Expanded(1.0) Min: (" << expanded.GetMin().x << ", " << expanded.GetMin().y << ", " << expanded.GetMin().z << ")" << std::endl;
    std::cout << "Expanded(1.0) Max: (" << expanded.GetMax().x << ", " << expanded.GetMax().y << ", " << expanded.GetMax().z << ")" << std::endl;
    std::cout << "期望: Min(-2,-2,-2) Max(2,2,2)" << std::endl;

    // Translate
    AABB translated = aabb;
    translated.Translate(Vector3f(5, 5, 5));
    std::cout << "Translate(5,5,5) Center: (" << translated.GetCenter().x << ", " << translated.GetCenter().y << ", " << translated.GetCenter().z << ")" << std::endl;
    std::cout << "期望 Center: (5, 5, 5)" << std::endl;
    std::cout << std::endl;
}

void TestAABBGetters()
{
    std::cout << "=== 测试 AABB 新增的 Getter 方法 ===" << std::endl;

    AABB aabb;
    aabb.SetMinMax(Vector3f(-1, -2, -3), Vector3f(1, 2, 3));

    std::cout << "AABB(-1,-2,-3, 1,2,3):" << std::endl;
    std::cout << "  GetSize: (" << aabb.GetSize().x << ", " << aabb.GetSize().y << ", " << aabb.GetSize().z << ")" << std::endl;
    std::cout << "  GetExtent: (" << aabb.GetExtent().x << ", " << aabb.GetExtent().y << ", " << aabb.GetExtent().z << ")" << std::endl;
    std::cout << "  GetDiagonal: " << aabb.GetDiagonal() << std::endl;
    std::cout << "  GetSurfaceArea: " << aabb.GetSurfaceArea() << std::endl;
    std::cout << "  GetVolume: " << aabb.GetVolume() << std::endl;
    std::cout << "  期望: Size(2,4,6) Extent(1,2,3)" << std::endl;
    std::cout << std::endl;
}

void TestOBBGetters()
{
    std::cout << "=== 测试 OBB 新增的 Getter 方法 ===" << std::endl;

    OBB obb;
    obb.Set(Vector3f(0, 0, 0), Vector3f(1, 2, 3));

    std::cout << "OBB HalfExtend(1,2,3):" << std::endl;
    std::cout << "  GetSize: (" << obb.GetSize().x << ", " << obb.GetSize().y << ", " << obb.GetSize().z << ")" << std::endl;
    std::cout << "  GetDiagonal: " << obb.GetDiagonal() << std::endl;
    std::cout << "  GetSurfaceArea: " << obb.GetSurfaceArea() << std::endl;
    std::cout << "  GetVolume: " << obb.GetVolume() << std::endl;
    std::cout << "  期望: Size(2,4,6)" << std::endl;
    std::cout << std::endl;
}

void TestOBBImprovedDistance()
{
    std::cout << "=== 测试 OBB::Distance() 改进 ===" << std::endl;

    OBB obb1;
    obb1.Set(Vector3f(0, 0, 0), Vector3f(1, 1, 1));

    OBB obb2;
    obb2.Set(Vector3f(5, 0, 0), Vector3f(1, 1, 1));

    float dist = obb1.Distance(obb2);
    std::cout << "OBB1 center(0,0,0) half(1,1,1)" << std::endl;
    std::cout << "OBB2 center(5,0,0) half(1,1,1)" << std::endl;
    std::cout << "Distance: " << dist << std::endl;
    std::cout << "期望: 约 3.0（中心距离 5 - 两个半径 2）" << std::endl;
    std::cout << std::endl;
}

int main()
{
    std::cout << "AABB 和 OBB 改进测试" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << std::endl;

    TestAABBClear();
    TestOBBClear();
    TestAABBFactoryMethods();
    TestOBBFactoryMethods();
    TestAABBScaleAndOffset();
    TestAABBGetters();
    TestOBBGetters();
    TestOBBImprovedDistance();

    std::cout << "=====================================" << std::endl;
    std::cout << "所有测试完成！" << std::endl;

    return 0;
}
