// BoundingVolumesDataStorage 测试和使用示例
#include<hgl/math/geometry/BoundingVolumesDataStorage.h>
#include<iostream>
#include<chrono>

using namespace hgl::math;

void TestBasicOperations()
{
    std::cout << "=== 测试基本操作 ===" << std::endl;

    BoundingVolumesDataStorage storage(10);

    // 创建测试数据
    BoundingVolumes bv1;
    bv1.SetFromAABB(Vector3f(-1, -1, -1), Vector3f(1, 1, 1));

    BoundingVolumes bv2;
    bv2.SetFromAABB(Vector3f(5, 5, 5), Vector3f(7, 7, 7));

    BoundingVolumes bv3;
    bv3.SetFromSphere(BoundingSphere(Vector3f(0, 10, 0), 3.0f));

    // 添加数据
    size_t idx1 = storage.Add(bv1);
    size_t idx2 = storage.Add(bv2);
    size_t idx3 = storage.Add(bv3);

    std::cout << "Added 3 bounding volumes" << std::endl;
    std::cout << "Count: " << storage.GetCount() << std::endl;
    std::cout << "Capacity: " << storage.GetCapacity() << std::endl;

    // 验证读取
    BoundingVolumes read_bv;
    if (storage.Get(idx1, read_bv))
    {
        std::cout << "BV[0] AABB center: ("
                  << read_bv.aabb.GetCenter().x << ", "
                  << read_bv.aabb.GetCenter().y << ", "
                  << read_bv.aabb.GetCenter().z << ")" << std::endl;
    }

    // 验证一致性
    std::cout << "Data consistency: " << (storage.ValidateConsistency() ? "OK" : "FAILED") << std::endl;
    std::cout << std::endl;
}

void TestBatchOperations()
{
    std::cout << "=== 测试批量操作 ===" << std::endl;

    BoundingVolumesDataStorage storage(100);

    // 批量添加
    std::vector<BoundingVolumes> batch;
    for (int i = 0; i < 50; i++)
    {
        BoundingVolumes bv;
        bv.SetFromAABB(Vector3f(i * 2.0f, 0, 0), Vector3f(i * 2.0f + 1, 1, 1));
        batch.push_back(bv);
    }

    storage.AddBatch(batch.data(), batch.size());

    std::cout << "Added " << batch.size() << " bounding volumes" << std::endl;
    std::cout << "Count: " << storage.GetCount() << std::endl;

    // 批量平移
    storage.TranslateAll(Vector3f(0, 10, 0));
    std::cout << "Translated all by (0, 10, 0)" << std::endl;

    // 验证平移
    BoundingVolumes first_bv;
    storage.Get(0, first_bv);
    std::cout << "First BV center after translation: ("
              << first_bv.aabb.GetCenter().x << ", "
              << first_bv.aabb.GetCenter().y << ", "
              << first_bv.aabb.GetCenter().z << ")" << std::endl;

    // 批量缩放
    storage.ScaleAll(2.0f);
    std::cout << "Scaled all by 2.0" << std::endl;

    storage.Get(0, first_bv);
    std::cout << "First BV size after scaling: ("
              << first_bv.aabb.GetLength().x << ", "
              << first_bv.aabb.GetLength().y << ", "
              << first_bv.aabb.GetLength().z << ")" << std::endl;

    std::cout << std::endl;
}

void TestCollisionDetection()
{
    std::cout << "=== 测试碰撞检测 ===" << std::endl;

    BoundingVolumesDataStorage storage(100);

    // 创建网格布局的包围体
    for (int x = 0; x < 10; x++)
    {
        for (int z = 0; z < 10; z++)
        {
            BoundingVolumes bv;
            Vector3f min_v(x * 2.0f, 0, z * 2.0f);
            Vector3f max_v(x * 2.0f + 1, 1, z * 2.0f + 1);
            bv.SetFromAABB(min_v, max_v);
            storage.Add(bv);
        }
    }

    std::cout << "Created 10x10 grid of bounding volumes" << std::endl;

    // 点相交测试
    Vector3f test_point(5.5f, 0.5f, 5.5f);
    std::vector<size_t> intersecting_indices;

    storage.FindIntersectingPoint(test_point, intersecting_indices);
    std::cout << "Point (" << test_point.x << ", " << test_point.y << ", " << test_point.z
              << ") intersects " << intersecting_indices.size() << " volumes" << std::endl;

    // 球体相交测试
    Vector3f sphere_center(10.0f, 0.5f, 10.0f);
    float sphere_radius = 5.0f;

    storage.FindIntersectingSphere(sphere_center, sphere_radius, intersecting_indices);
    std::cout << "Sphere (center=" << sphere_center.x << "," << sphere_center.y << "," << sphere_center.z
              << ", radius=" << sphere_radius << ") intersects "
              << intersecting_indices.size() << " volumes" << std::endl;

    // AABB 相交测试
    AABB test_aabb = AABB::FromCenterAndSize(Vector3f(10, 0.5f, 10), Vector3f(6, 2, 6));

    storage.FindIntersectingAABB(test_aabb, intersecting_indices);
    std::cout << "AABB intersects " << intersecting_indices.size() << " volumes" << std::endl;

    std::cout << std::endl;
}

void TestTransformation()
{
    std::cout << "=== 测试矩阵变换 ===" << std::endl;

    BoundingVolumesDataStorage storage(10);

    // 添加测试数据
    for (int i = 0; i < 5; i++)
    {
        BoundingVolumes bv;
        bv.SetFromAABB(Vector3f(i * 2.0f, 0, 0), Vector3f(i * 2.0f + 1, 1, 1));
        storage.Add(bv);
    }

    std::cout << "Added 5 bounding volumes" << std::endl;

    // 打印变换前的第一个 BV
    BoundingVolumes bv_before;
    storage.Get(0, bv_before);
    std::cout << "Before transform - First BV center: ("
              << bv_before.aabb.GetCenter().x << ", "
              << bv_before.aabb.GetCenter().y << ", "
              << bv_before.aabb.GetCenter().z << ")" << std::endl;

    // 创建变换矩阵（平移+旋转+缩放）
    Matrix4f transform = glm::translate(Matrix4f(1.0f), Vector3f(10, 5, 0));
    transform = glm::rotate(transform, glm::radians(45.0f), Vector3f(0, 1, 0));
    transform = glm::scale(transform, Vector3f(2.0f, 2.0f, 2.0f));

    // 应用变换
    storage.TransformAll(transform);

    // 打印变换后的第一个 BV
    BoundingVolumes bv_after;
    storage.Get(0, bv_after);
    std::cout << "After transform - First BV center: ("
              << bv_after.aabb.GetCenter().x << ", "
              << bv_after.aabb.GetCenter().y << ", "
              << bv_after.aabb.GetCenter().z << ")" << std::endl;
    std::cout << "After transform - First BV size: ("
              << bv_after.aabb.GetLength().x << ", "
              << bv_after.aabb.GetLength().y << ", "
              << bv_after.aabb.GetLength().z << ")" << std::endl;

    std::cout << std::endl;
}

void TestPerformance()
{
    std::cout << "=== 性能测试 ===" << std::endl;

    const size_t NUM_VOLUMES = 10000;
    BoundingVolumesDataStorage storage(NUM_VOLUMES);

    // 测试添加性能
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < NUM_VOLUMES; i++)
    {
        BoundingVolumes bv;
        float x = (i % 100) * 2.0f;
        float z = (i / 100) * 2.0f;
        bv.SetFromAABB(Vector3f(x, 0, z), Vector3f(x + 1, 1, z + 1));
        storage.Add(bv);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Added " << NUM_VOLUMES << " volumes in "
              << duration.count() / 1000.0 << " ms" << std::endl;

    // 测试批量平移性能
    start = std::chrono::high_resolution_clock::now();
    storage.TranslateAll(Vector3f(10, 10, 10));
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Translated " << NUM_VOLUMES << " volumes in "
              << duration.count() / 1000.0 << " ms" << std::endl;

    // 测试碰撞检测性能
    start = std::chrono::high_resolution_clock::now();

    std::vector<size_t> intersecting_indices;
    Vector3f sphere_center(100, 10, 100);
    float sphere_radius = 50.0f;
    storage.FindIntersectingSphere(sphere_center, sphere_radius, intersecting_indices);

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Collision test against " << NUM_VOLUMES << " volumes in "
              << duration.count() / 1000.0 << " ms" << std::endl;
    std::cout << "Found " << intersecting_indices.size() << " intersecting volumes" << std::endl;

    // 打印内存使用
    storage.PrintStats();

    std::cout << std::endl;
}

void TestSwapAndRemove()
{
    std::cout << "=== 测试交换和移除 ===" << std::endl;

    BoundingVolumesDataStorage storage(10);

    // 添加测试数据
    for (int i = 0; i < 5; i++)
    {
        BoundingVolumes bv;
        bv.SetFromAABB(Vector3f(i * 2.0f, 0, 0), Vector3f(i * 2.0f + 1, 1, 1));
        storage.Add(bv);
    }

    std::cout << "Initial count: " << storage.GetCount() << std::endl;

    // 打印第 0 和第 2 个的中心
    BoundingVolumes bv0, bv2;
    storage.Get(0, bv0);
    storage.Get(2, bv2);
    std::cout << "BV[0] center: (" << bv0.aabb.GetCenter().x << ", ...)" << std::endl;
    std::cout << "BV[2] center: (" << bv2.aabb.GetCenter().x << ", ...)" << std::endl;

    // 交换
    storage.Swap(0, 2);
    std::cout << "Swapped BV[0] and BV[2]" << std::endl;

    storage.Get(0, bv0);
    storage.Get(2, bv2);
    std::cout << "After swap - BV[0] center: (" << bv0.aabb.GetCenter().x << ", ...)" << std::endl;
    std::cout << "After swap - BV[2] center: (" << bv2.aabb.GetCenter().x << ", ...)" << std::endl;

    // 移除
    storage.RemoveSwap(1);
    std::cout << "Removed BV[1] using swap-remove" << std::endl;
    std::cout << "Count after removal: " << storage.GetCount() << std::endl;

    // 验证一致性
    std::cout << "Consistency after operations: "
              << (storage.ValidateConsistency() ? "OK" : "FAILED") << std::endl;

    std::cout << std::endl;
}

void CompareSOAvsAOS()
{
    std::cout << "=== SOA vs AOS 内存布局对比 ===" << std::endl;

    const size_t NUM = 1000;

    // AOS 方式（传统）
    std::vector<BoundingVolumes> aos_storage;
    aos_storage.reserve(NUM);

    // SOA 方式（新）
    BoundingVolumesDataStorage soa_storage(NUM);

    // 添加数据
    for (size_t i = 0; i < NUM; i++)
    {
        BoundingVolumes bv;
        bv.SetFromAABB(Vector3f(i * 2.0f, 0, 0), Vector3f(i * 2.0f + 1, 1, 1));

        aos_storage.push_back(bv);
        soa_storage.Add(bv);
    }

    // 内存使用对比
    size_t aos_memory = aos_storage.capacity() * sizeof(BoundingVolumes);
    size_t soa_memory = soa_storage.GetMemoryUsage();

    std::cout << "AOS Memory: " << aos_memory / 1024.0f << " KB" << std::endl;
    std::cout << "SOA Memory: " << soa_memory / 1024.0f << " KB" << std::endl;
    std::cout << "Memory difference: " << (int)(soa_memory - aos_memory) / 1024.0f << " KB" << std::endl;

    // 性能对比：批量访问球体中心
    auto start = std::chrono::high_resolution_clock::now();

    Vector3f sum_aos(0);
    for (const auto& bv : aos_storage)
    {
        sum_aos += bv.bsphere.GetCenter();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto aos_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();

    Vector3f sum_soa(0);
    const Vector3f* centers = soa_storage.GetSphereCenters();
    for (size_t i = 0; i < NUM; i++)
    {
        sum_soa += centers[i];
    }

    end = std::chrono::high_resolution_clock::now();
    auto soa_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "AOS access time: " << aos_duration.count() / 1000.0 << " μs" << std::endl;
    std::cout << "SOA access time: " << soa_duration.count() / 1000.0 << " μs" << std::endl;
    std::cout << "Speedup: " << (float)aos_duration.count() / soa_duration.count() << "x" << std::endl;

    std::cout << std::endl;
}

int main()
{
    std::cout << "BoundingVolumesDataStorage 测试程序" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << std::endl;

    TestBasicOperations();
    TestBatchOperations();
    TestCollisionDetection();
    TestTransformation();
    TestSwapAndRemove();
    TestPerformance();
    CompareSOAvsAOS();

    std::cout << "=====================================" << std::endl;
    std::cout << "所有测试完成！" << std::endl;

    return 0;
}
