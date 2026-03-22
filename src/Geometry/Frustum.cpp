#include<hgl/math/geometry/Frustum.h>

namespace hgl::math
{
    /**
     * 从 MVP 矩阵提取视锥体六个平面。
     *
     * 当前矩阵约定：
     * - 视图矩阵等价于 glm::lookAtRH，摄像机朝向为 -Z
     * - 投影矩阵使用 Vulkan 深度范围 [0,1]
     * - 平面提取应基于裁剪空间约束：
     *   x ∈ [-w, w], y ∈ [-w, w], z ∈ [0, w]
     */
    void GetFrustumPlanes(FrustumPlanes &planes,const math::Matrix4f &mvp)
    {

        planes[size_t(Frustum::Side::Left   )].x = mvp[0].w + mvp[0].x;
        planes[size_t(Frustum::Side::Left   )].y = mvp[1].w + mvp[1].x;
        planes[size_t(Frustum::Side::Left   )].z = mvp[2].w + mvp[2].x;
        planes[size_t(Frustum::Side::Left   )].w = mvp[3].w + mvp[3].x;

        planes[size_t(Frustum::Side::Right  )].x = mvp[0].w - mvp[0].x;
        planes[size_t(Frustum::Side::Right  )].y = mvp[1].w - mvp[1].x;
        planes[size_t(Frustum::Side::Right  )].z = mvp[2].w - mvp[2].x;
        planes[size_t(Frustum::Side::Right  )].w = mvp[3].w - mvp[3].x;

        planes[size_t(Frustum::Side::Top    )].x = mvp[0].w - mvp[0].y;
        planes[size_t(Frustum::Side::Top    )].y = mvp[1].w - mvp[1].y;
        planes[size_t(Frustum::Side::Top    )].z = mvp[2].w - mvp[2].y;
        planes[size_t(Frustum::Side::Top    )].w = mvp[3].w - mvp[3].y;

        planes[size_t(Frustum::Side::Bottom )].x = mvp[0].w + mvp[0].y;
        planes[size_t(Frustum::Side::Bottom )].y = mvp[1].w + mvp[1].y;
        planes[size_t(Frustum::Side::Bottom )].z = mvp[2].w + mvp[2].y;
        planes[size_t(Frustum::Side::Bottom )].w = mvp[3].w + mvp[3].y;

        planes[size_t(Frustum::Side::Front  )].x = mvp[0].z;
        planes[size_t(Frustum::Side::Front  )].y = mvp[1].z;
        planes[size_t(Frustum::Side::Front  )].z = mvp[2].z;
        planes[size_t(Frustum::Side::Front  )].w = mvp[3].z;

        planes[size_t(Frustum::Side::Back   )].x = mvp[0].w - mvp[0].z;
        planes[size_t(Frustum::Side::Back   )].y = mvp[1].w - mvp[1].z;
        planes[size_t(Frustum::Side::Back   )].z = mvp[2].w - mvp[2].z;
        planes[size_t(Frustum::Side::Back   )].w = mvp[3].w - mvp[3].z;

        for(int i=0;i<6;i++)
        {
            float len=sqrtf(planes[i].x * planes[i].x + planes[i].y * planes[i].y + planes[i].z * planes[i].z);

            if(len > 0.0f)
                planes[i] /= len;
        }
    }

    void Frustum::SetMatrix(const math::Matrix4f &mvp)
    {
        FrustumPlanes planes;

        GetFrustumPlanes(planes,mvp);   // 从矩阵提取六个平面

        for(int i=0;i<6;i++)pl[i].Set(planes[i]);         // 设置平面
    }

    Frustum::Scope Frustum::PointIn(const Vector3f &p) const
    {
        Frustum::Scope result = Frustum::Scope::INSIDE;

        for(int i=0; i < 6; i++)
            if (pl[i].Distance(p) < 0)
                return Frustum::Scope::OUTSIDE;

        return(result);
    }

    Frustum::Scope Frustum::SphereIn(const Vector3f &p, float radius) const
    {
        Frustum::Scope result = Frustum::Scope::INSIDE;
        float distance;

        for(int i=0; i < 6; i++)
        {
            distance = pl[i].Distance(p);

            if (distance < -radius)
                return Frustum::Scope::OUTSIDE;
            else
            if (distance < radius)
                result = Frustum::Scope::INTERSECT;
        }

        return(result);
    }

    Frustum::Scope Frustum::BoxIn(const AABB &b) const
    {
        Frustum::Scope result = Frustum::Scope::INSIDE;

        for(int i=0; i < 6; i++)
        {
            if (pl[i].Distance(b.GetVertexP(pl[i].normal)) < 0)
                return Frustum::Scope::OUTSIDE;
            else
            if (pl[i].Distance(b.GetVertexN(pl[i].normal)) < 0)
                result = Frustum::Scope::INTERSECT;
        }

        return(result);
    }
}//namespace hgl::math
