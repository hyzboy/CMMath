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
    OBB ToOBB(const AABB &aabb, const Matrix4f &transform);

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
            hgl_zero(bsphere);
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

        void SetFromAABB(const Vector3f &min_v,const Vector3f &max_v)
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
