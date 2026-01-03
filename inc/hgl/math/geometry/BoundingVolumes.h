#pragma once
#include<numbers>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/BoundingSphere.h>
#include<hgl/math/geometry/primitives/Sphere.h>

namespace hgl::math
{
    struct Ray;
    struct Plane;

    BoundingSphere ToBoundingSphere(const AABB &a);
    AABB ToAABB(const OBB &obb);
    OBB ToOBB(const AABB &aabb);
    OBB ToOBB(const AABB &aabb, const Matrix4f &transform);

    /**
     * Convert OBB to BoundingSphere
     * @param obb The oriented bounding box to convert
     * @return A bounding sphere that fully contains the OBB
     */
    BoundingSphere ToBoundingSphere(const OBB &obb);

    /**
     * Convert BoundingSphere to AABB
     * @param sphere The bounding sphere to convert
     * @return An axis-aligned bounding box that fully contains the sphere
     */
    AABB ToAABB(const BoundingSphere &sphere);

    /**
     * Convert BoundingSphere to OBB
     * @param sphere The bounding sphere to convert
     * @return An oriented bounding box (axis-aligned) that fully contains the sphere
     */
    OBB ToOBB(const BoundingSphere &sphere);

    struct BoundingVolumesData;

    /**
     * 综合包围体 - 包含 AABB, OBB, 和 BoundingSphere
     * 提供多层次的碰撞检测：从快速但不精确(球体)到精确但较慢(OBB)
     */
    struct BoundingVolumes
    {
        AABB aabb;
        OBB obb;
        BoundingSphere bsphere;

    public:

        BoundingVolumes()
        {
            Clear();
        }

        void Clear()
        {
            aabb.Clear();
            obb.Clear();
            mem_zero(bsphere);
        }

        /**
         * 检查包围体是否为空
         * @note 只要任意一个包围体为空，就认为整个包围体为空
         */
        bool IsEmpty()const
        {
            return aabb.IsEmpty() || obb.IsEmpty() || bsphere.IsEmpty();
        }

    public: // 初始化方法

        /**
         * 从 AABB 初始化所有包围体
         * @note 会计算最优的包围球(半径为半对角线长度)
         */
        void SetFromAABB(const AABB &box)
        {
            aabb = box;
            obb = ToOBB(box);
            bsphere.center = aabb.GetCenter();
            bsphere.radius = glm::length((aabb.GetMax() - aabb.GetMin()) * 0.5f);
        }

        void SetFromAABB(const Vector3f &min_v,const Vector3f &max_v)
        {
            AABB box;
            box.SetMinMax(min_v,max_v);
            SetFromAABB(box);
        }

        /**
         * 从 OBB 初始化所有包围体
         */
        void SetFromOBB(const OBB &box)
        {
            obb = box;
            aabb = ToAABB(box);
            bsphere = ToBoundingSphere(box);
        }

        /**
         * 从 BoundingSphere 初始化所有包围体
         */
        void SetFromSphere(const BoundingSphere &sphere)
        {
            bsphere = sphere;
            aabb = ToAABB(sphere);
            obb = ToOBB(sphere);
        }

        /**
         * 从点集初始化所有包围体
         * @note 三个包围体独立计算，可能中心不一致
         */
        bool SetFromPoints(const float *pts,const uint32_t count,const uint32_t component_count)
        {
            if(!pts||count<=0)
            {
                Clear();
                return false;
            }

            aabb    .SetFromPoints(pts,count,component_count);
            obb     .SetFromPoints(pts,count,component_count);
            bsphere .SetFromPoints(pts,count,component_count);

            return true;
        }

    public: // 碰撞检测 - 点相关

        /**
         * 检查点是否在任一包围体内
         * @note 使用 OR 逻辑，只要在任一包围体内即返回 true
         * @note 如需严格检测，请使用 ContainsPointAll
         */
        bool ContainsPoint(const Vector3f &point) const
        {
            return aabb.ContainsPoint(point) || obb.ContainsPoint(point) || bsphere.ContainsPoint(point);
        }

        /**
         * 检查点是否在所有包围体内(严格检测)
         */
        bool ContainsPointAll(const Vector3f &point) const
        {
            return aabb.ContainsPoint(point) && obb.ContainsPoint(point) && bsphere.ContainsPoint(point);
        }

        /**
         * 计算点到包围体的最近点(使用 AABB，保证最快速度)
         */
        Vector3f ClosestPoint(const Vector3f &point) const
        {
            return aabb.ClosestPoint(point);
        }

        /**
         * 计算点到包围体的最近点(使用球体)
         */
        Vector3f ClosestPointSphere(const Vector3f &point) const
        {
            return bsphere.ClosestPoint(point);
        }

        /**
         * 计算点到包围体的距离(使用 AABB)
         */
        float DistanceToPoint(const Vector3f &point) const
        {
            return aabb.DistanceToPoint(point);
        }

    public: // 碰撞检测 - 包围体

        /**
         * 检查与另一个BoundingVolumes是否相交(快速球体测试)
         */
        bool IntersectsFast(const BoundingVolumes &other) const
        {
            return bsphere.Intersects(other.bsphere);
        }

        /**
         * 检查与另一个BoundingVolumes是否相交(精确AABB测试)
         */
        bool IntersectsAABB(const BoundingVolumes &other) const
        {
            return aabb.Intersects(other.aabb);
        }

        /**
         * 检查与另一个BoundingVolumes是否相交(精确OBB测试)
         */
        bool IntersectsOBB(const BoundingVolumes &other) const
        {
            return obb.Intersects(other.obb);
        }

        /**
         * 检查与另一个BoundingVolumes是否相交(层次测试:球体->AABB->OBB)
         */
        bool Intersects(const BoundingVolumes &other) const
        {
            if (!IntersectsFast(other))
                return false;
            if (!IntersectsAABB(other))
                return false;
            return IntersectsOBB(other);
        }

        /**
         * 检查与球体是否相交(使用 AABB 和球体进行测试)
         */
        bool IntersectsSphere(const Vector3f &sphere_center, float sphere_radius) const
        {
            return aabb.IntersectsSphere(sphere_center, sphere_radius);
        }

        /**
         * 检查与球体是否相交
         */
        bool IntersectsSphere(const BoundingSphere &sphere) const
        {
            return IntersectsSphere(sphere.center, sphere.radius);
        }

        /**
         * 检查是否完全包含另一个BoundingVolumes
         */
        bool Contains(const BoundingVolumes &other) const
        {
            return aabb.Contains(other.aabb) && bsphere.Contains(other.bsphere);
        }

        /**
         * 合并另一个BoundingVolumes
         * @note 独立合并三个包围体，可能导致它们不完全一致
         */
        void Merge(const BoundingVolumes &other)
        {
            aabb.Merge(other.aabb);
            obb.Merge(other.obb);
            bsphere.Merge(other.bsphere);
        }

    public: // 碰撞检测 - 其他几何体

        /**
         * 检查与射线是否相交(快速球体测试)
         */
        bool IntersectsRayFast(const Ray &ray) const
        {
            return bsphere.IntersectsRay(ray);
        }

        /**
         * 检查与射线是否相交(精确AABB测试)
         */
        bool IntersectsRay(const Ray &ray, float &distance) const
        {
            if (!IntersectsRayFast(ray))
                return false;
            return aabb.IntersectsRay(ray, distance);
        }

        bool IntersectsRay(const Ray &ray) const
        {
            float distance;
            return IntersectsRay(ray, distance);
        }

        /**
         * 检查与平面的关系
         */
        int ClassifyPlane(const Plane &plane) const
        {
            return aabb.ClassifyPlane(plane);
        }

        /**
         * 检查是否与平面相交
         */
        bool IntersectsPlane(const Plane &plane) const
        {
            return aabb.IntersectsPlane(plane) || bsphere.IntersectsPlane(plane);
        }

    public: // 工具函数

        /**
         * 扩展包围体以包含指定点
         */
        void ExpandToInclude(const Vector3f &point)
        {
            aabb.ExpandToInclude(point);
            obb.ExpandToInclude(point);
            bsphere.ExpandToInclude(point);
        }

        /**
         * 获取最大半径(球体半径)
         */
        float GetMaxRadius() const
        {
            return bsphere.GetRadius();
        }

        /**
         * 获取中心点(使用AABB中心，最常用且稳定)
         * @note 如需其他包围体的中心，请直接访问对应成员: obb.GetCenter() 或 bsphere.GetCenter()
         */
        const Vector3f &GetCenter() const
        {
            return aabb.GetCenter();
        }

        /**
         * 获取球体中心
         */
        const Vector3f &GetSphereCenter() const
        {
            return bsphere.GetCenter();
        }

        /**
         * 获取 OBB 中心
         */
        const Vector3f &GetOBBCenter() const
        {
            return obb.GetCenter();
        }

        /**
         * 获取表面积(使用 AABB)
         */
        float GetSurfaceArea() const
        {
            return aabb.GetSurfaceArea();
        }

        /**
         * 获取体积(使用 AABB)
         */
        float GetVolume() const
        {
            return aabb.GetVolume();
        }

        /**
         * 对包围体进行变换
         * @param transform 变换矩阵
         * @return 变换后的包围体
         */
        BoundingVolumes Transformed(const Matrix4f &transform) const
        {
            BoundingVolumes result;
            result.aabb = aabb.Transformed(transform);
            result.obb = obb.Transformed(transform);
            
            // Transform sphere center
            Vector3f new_center = Vector3f(transform * Vector4f(bsphere.center, 1.0f));
            
            // Calculate max scale to adjust radius
            const float s0 = glm::length(glm::vec3(transform[0]));
            const float s1 = glm::length(glm::vec3(transform[1]));
            const float s2 = glm::length(glm::vec3(transform[2]));
            const float max_scale = glm::max(glm::max(s0, s1), s2);
            
            result.bsphere.center = new_center;
            result.bsphere.radius = bsphere.radius * max_scale;
            
            return result;
        }

    public:

        void Pack(BoundingVolumesData *packed) const;
    };
    
#pragma pack(push,1)
    struct BoundingVolumesData
    {
        float aabbMin[3];
        float aabbMax[3];
        float obbCenter[3];
        float obbAxisX[3];
        float obbAxisY[3];
        float obbAxisZ[3];
        float obbHalfSize[3];
        float sphereCenter[3];
        float sphereRadius;

    public:

        void To(BoundingVolumes *bounds);
    };//struct BoundingVolumesData
#pragma pack(pop)
}//namespace hgl::math
