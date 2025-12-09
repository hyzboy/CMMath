#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/MatrixTypes.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/math/geometry/BoundingSphere.h>

namespace hgl::math
{
    BoundingSphere ToBoundingSphere(const AABB &a);

    /**
     * Convert OBB to AABB
     * @param obb The oriented bounding box to convert
     * @return An axis-aligned bounding box that fully contains the OBB
     * @note This calculates the minimal AABB by using the OBB's axes and half extents
     */
    AABB ToAABB(const OBB &obb);

    /**
     * Convert AABB to axis-aligned OBB
     * @param aabb The axis-aligned bounding box to convert
     * @return An OBB with identity rotation that matches the AABB
     */
    OBB ToOBB(const AABB &aabb);

    /**
     * Convert AABB to transformed OBB
     * @param aabb The axis-aligned bounding box to convert
     * @param transform The transformation matrix to apply
     * @return An OBB transformed by the given matrix
     * @note The OBB's axes are extracted from the rotation part of the transform,
     *       and scaling is applied to the half extents
     */
    OBB ToOBB(const AABB &aabb, const math::Matrix4f &transform);

    struct BoundingVolumesData;

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

        bool IsEmpty()const
        {
            return aabb.IsEmpty() && obb.GetHalfExtend().x <= 0 && bsphere.radius <= 0;
        }

    public:

        void SetFromAABB(const AABB &box)
        {
            aabb = box;
            obb = ToOBB(box);
            bsphere.center = aabb.GetCenter();
            bsphere.radius = glm::length(aabb.GetMax() - bsphere.center);
        }

        void SetFromAABB(const math::Vector3f &min_v,const math::Vector3f &max_v)
        {
            AABB box;
            box.SetMinMax(min_v,max_v);
            SetFromAABB(box);
        }

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
         */
        bool ContainsPoint(const math::Vector3f &point) const
        {
            return aabb.ContainsPoint(point) || obb.ContainsPoint(point) || bsphere.ContainsPoint(point);
        }

        /**
         * 计算点到包围体的最近点(使用球体)
         */
        math::Vector3f ClosestPoint(const math::Vector3f &point) const
        {
            return bsphere.ClosestPoint(point);
        }

        /**
         * 计算点到包围体的距离(使用最保守的AABB)
         */
        float DistanceToPoint(const math::Vector3f &point) const
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
         * 检查是否完全包含另一个BoundingVolumes
         */
        bool Contains(const BoundingVolumes &other) const
        {
            return aabb.Contains(other.aabb) && bsphere.Contains(other.bsphere);
        }

        /**
         * 合并另一个BoundingVolumes
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
        void ExpandToInclude(const math::Vector3f &point)
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
         * 获取中心点(使用AABB中心)
         */
        const math::Vector3f &GetCenter() const
        {
            return aabb.GetCenter();
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
