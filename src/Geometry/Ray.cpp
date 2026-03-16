#include<hgl/math/geometry/Ray.h>
#include<hgl/math/geometry/primitives/Sphere.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/graph/CameraInfo.h>
#include<cmath>

namespace hgl::math
{
    /**
    * 求射线上距离指定点最近的点
    * @param point 指定点
    * @return 位于射线上的点
    */
    Vector3f Ray::ClosestPoint(const Vector3f &point)const
    {
        const float length=Dot(direction,point-origin);

        if(length<=0)
            return origin;
        else
            return origin+direction*length;
    }

    /**
     * 屏幕坐标反投影生成射线 (Vulkan Z-up, 深度范围[0,1])
     *
     * @param origin 输出：射线起点（近平面上的点）
     * @param direction 输出：射线方向（归一化）
     * @param win 屏幕坐标 (左上角为原点)
     * @param Inverse 逆VP矩阵 (inverse(projection * view))
     * @param viewport 视口大小
     */
    void RayUnProjectZO(Vector3f &origin,Vector3f &direction,const Vector2i &win, const Matrix4f &Inverse, const Vector2u &viewport, bool reversed_z)
    {
        Vector4f near_point;
        Vector4f far_point;
        Vector4f tmp;

        // 屏幕坐标 -> NDC [-1, 1]
        tmp.x = float(win.x) / float(viewport.x);
        tmp.y = float(win.y) / float(viewport.y);
        tmp.x = tmp.x + tmp.x - 1.0;  // [0,1] -> [-1,1]
        tmp.y = tmp.y + tmp.y - 1.0;  // [0,1] -> [-1,1] (Vulkan: y向下)

        // 近平面: 标准 ZO → NDC.z=0, Reversed-Z → NDC.z=1
        tmp.z = reversed_z ? 1.0f : 0.0f;
        tmp.w=1.0;

        near_point = Inverse * tmp;
        if(near_point.w != 0.0f)
            near_point/=near_point.w;

        // 远平面: 标准 ZO → NDC.z=1, Reversed-Z → NDC.z=0
        tmp.z = reversed_z ? 0.0f : 1.0f;

        far_point = Inverse * tmp;
        if(std::fabs(far_point.w) > 1e-7f)
            far_point/=far_point.w;

        // 生成射线：从近平面点指向远平面点
        origin=near_point;

        // 无限远平面（Reversed-Z + Infinite Far）时 far_point.w ≈ 0，
        // 此时 far_point.xyz 是方向向量而非世界坐标点，直接用作射线方向。
        if(std::fabs(far_point.w) <= 1e-7f)
            direction=glm::normalize(Vector3f(far_point));
        else
            direction=glm::normalize(Vector3f(far_point-near_point));
    }

    /**
     * 从屏幕坐标生成拾取射线 (Vulkan Z-up)
     *
     * @param mp 屏幕点坐标 (左上角为原点)
     * @param ci 摄像机信息
     * @param vp_size 视口大小
     *
     * 生成的射线：
     * - origin: 近平面上对应屏幕点的世界坐标
     * - direction: 从摄像机指向该点的方向（Z-up系统中沿+Y方向）
     */
    void Ray::SetFromViewportPoint(const Vector2i &mp,const graph::CameraInfo *ci,const Vector2u &vp_size)
    {
        // 按相机的深度约定选择 NDC near/far z 值
        RayUnProjectZO(origin,direction,mp,ci->inverse_vp,vp_size,ci->use_reversed_z != 0);

        // 注意：不使用glm::unProject，因为它假设OpenGL的深度范围[-1,1]和Y-up坐标系
    }

    void Ray::ClosestPoint(Vector3f &point_on_ray,Vector3f &point_on_segment,const Vector3f &line_segment_start,const Vector3f &line_segment_end)const
    {
        Vector3f u = direction; // 射线方向，假定已归一化
        Vector3f v = line_segment_end - line_segment_start;
        Vector3f w = origin - line_segment_start;

        float a = Dot(u, u); // = 1，如果u已归一化
        float b = Dot(u, v);
        float c = Dot(v, v);
        float d = Dot(u, w);
        float e = Dot(v, w);

        float D = a * c - b * b;
        float sc, tc;

        if (D < 1e-8f)
        {
            // 射线和线段几乎平行
            sc = 0.0f;
            tc = (b > c ? d / b : e / c);
        }
        else
        {
            sc = (b * e - c * d) / D;
            tc = (a * e - b * d) / D;
        }

        // 限制参数范围
        if (sc < 0.0f) sc = 0.0f; // 射线参数不能小于0
        if (tc < 0.0f) tc = 0.0f;
        if (tc > 1.0f) tc = 1.0f;

        point_on_ray = origin + u * sc;
        point_on_segment = line_segment_start + v * tc;
    }

    /**
    * 求线段上离射线最近的点
    */
    Vector3f Ray::ClosestPoint(const Vector3f &start,const Vector3f &end)const
    {
        Vector3f pointOnRay;
        Vector3f pointOnSeg;

        ClosestPoint(pointOnRay,pointOnSeg,start,end);

        return pointOnRay+(pointOnSeg-pointOnRay)*0.5f; // 返回两点的中点
    }

    /**
    * 求射线与指定线段的距离的平方(Github Copilot)
    */
    const float Ray::ToLineSegmentDistanceSquared(const Vector3f &start,const Vector3f &end)const
    {
        Vector3f pointOnRay;
        Vector3f pointOnSeg;

        ClosestPoint(pointOnRay,pointOnSeg,start,end);

        return LengthSquared(pointOnRay - pointOnSeg);
    }

    bool Ray::CrossSphere(const Sphere &s)const
    {
        const Vector3f  sphere_center=s.GetCenter();
        const float     sphere_radius=s.GetRadius();

        const Vector3f oc=origin-sphere_center;

        const float b=Dot(oc,direction);
        const float c=Dot(oc,oc)-sphere_radius*sphere_radius;

        if(c>0&&b>0)return(false);

        const float discr=b*b-c;

        if(discr<0)return(false);

        return(true);
    }

    bool Ray::CrossEllipseSphere(const EllipseSphere &es)const
    {
        const Vector3f es_center=es.GetCenter();
        const Vector3f es_radius=es.GetRadius();

        const Vector3f oc=origin-es_center;

        const float b=Dot(oc,direction);
        const float c=Dot(oc,oc)-es_radius.x*es_radius.y*es_radius.z;

        if(c>0&&b>0)return(false);

        const float discr=b*b-c;

        if(discr<0)return(false);

        return(true);
    }

    /**
    * 指当前射线是否与指定三角形相交(此函数由Github Copilot生成，未经测试)
    */
    bool Ray::CrossTriangle(const Triangle3f &tri,bool two_side)const
    {
        const Vector3f a1=tri[0];
        const Vector3f a2=tri[1];
        const Vector3f a3=tri[2];

        const Vector3f normal=Cross(a2-a1,a3-a1);

        float rad=Dot(normal,direction);

        if(rad>=0)              //射线与三角形背对
        {
            if(!two_side)
                return(false);

            rad=-rad;
        }

        const float d=Dot(normal,a1);

        const float t=(d-Dot(normal,origin))/rad;

        if(t<0)return(false);            //射线与三角形不相交

        const Vector3f hit_point=origin+direction*t;

        const Vector3f v1=Cross(a2-a1,hit_point-a1);
        const Vector3f v2=Cross(a3-a2,hit_point-a2);
        const Vector3f v3=Cross(a1-a3,hit_point-a3);

        if(Dot(v1,v2)<0)return(false);
        if(Dot(v2,v3)<0)return(false);
        if(Dot(v3,v1)<0)return(false);

        return(true);
    }

    /**
    * 求指定面是否与射线交汇
    */
    bool Ray::CrossPlane(const Vector3f &v1,const Vector3f &v2,const Vector3f &v3,const Vector3f &v4,const bool two_side)const
    {
        const Vector3f normal=Cross(v2-v1,v3-v1);

        float rad=Dot(normal,direction);

        if(rad>=0)              //射线与三角形背对
        {
            if(!two_side)
                return(false);

            rad=-rad;
        }

        const float d=Dot(normal,v1);

        const float t=(d-Dot(normal,origin))/rad;

        if(t<0)return(false);            //射线与平面不相交

        return(true);
    }

    bool Ray::CrossCircle(const Vector3f &center,const Vector3f &normal,const float radius)const
    {
        // 计算射线与平面的交点
        float denom = Dot(normal, direction);
        if(IsNearlyZero(denom)) return false; // 射线与平面平行

        float t = Dot(normal, center - origin) / denom;
        if (t < 0) return false; // 平面在射线的反方向

        // 计算交点
        Vector3f hit_point = origin + t * direction;

        // 判断交点是否在圆内
        return LengthSquared(hit_point - center) <= radius * radius;
    }
}//namespace hgl::math
