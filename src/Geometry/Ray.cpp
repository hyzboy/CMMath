#include<hgl/math/geometry/Ray.h>
#include<hgl/math/geometry/Sphere.h>
#include<hgl/math/geometry/AABB.h>
#include<hgl/math/geometry/OBB.h>
#include<hgl/graph/CameraInfo.h>

namespace hgl::math
{
    /**
    * 求射线上距离指定点最近的点
    * @param point 指定点
    * @return 位于射线上的点
    */
    Vector3f Ray::ClosestPoint(const Vector3f &point)const
    {
        const float length=dot(direction,point-origin);

        if(length<=0)
            return origin;
        else
            return origin+direction*length;
    }
        
    void RayUnProjectZO(Vector3f &origin,Vector3f &direction,const Vector2i &win, const Matrix4f &Inverse, const Vector2u &viewport)
    {
        Vector4f near_point;
        Vector4f far_point;
        Vector4f tmp;

        tmp.x = float(win.x) / float(viewport.x);
        tmp.y = float(win.y) / float(viewport.y);
        tmp.x = tmp.x + tmp.x - 1.0;
        tmp.y = tmp.y + tmp.y - 1.0;

        tmp.z=0.0;
        tmp.w=1.0;

        near_point = Inverse * tmp;
        if(near_point.w != 0.0f)
            near_point/=near_point.w;

        tmp.z=1.0;

        far_point = Inverse * tmp;
        if(far_point.w != 0.0f)
            far_point/=far_point.w;

        //注意这里的远近点和我们的矩阵设置有关

        origin=near_point;
        direction=glm::normalize(far_point-near_point);
    }

    /**
    * 设置屏幕坐标产生拾取射线
    * @param mp 屏幕点坐标
    * @param camera_info 摄像机信息
    */
    void Ray::SetFromViewportPoint(const Vector2i &mp,const graph::CameraInfo *ci,const Vector2u &vp_size)
    {
        //新方案

        RayUnProjectZO(origin,direction,mp,ci->inverse_vp,vp_size);

        //由于near/far的xy一样，而near.z又是0。所以省去了direction=normalize(far-near)的计算

        //旧标准方案

        //Vector3f pos(mp.x,mp.y,0);
        //Vector4i vp(0,0,camera_info->viewport_resolution.x,camera_info->viewport_resolution.y);

        //origin      =glm::unProject(pos,camera_info->view,camera_info->projection,vp);        //射线最近点

        //pos.z=1.0f;

        //direction   =glm::unProject(pos,camera_info->view,camera_info->projection,vp);        //射线最远点
    }

    void Ray::ClosestPoint(Vector3f &point_on_ray,Vector3f &point_on_segment,const Vector3f &line_segment_start,const Vector3f &line_segment_end)const
    {
        Vector3f u = direction; // 射线方向，假定已归一化
        Vector3f v = line_segment_end - line_segment_start;
        Vector3f w = origin - line_segment_start;

        float a = dot(u, u); // = 1，如果u已归一化
        float b = dot(u, v);
        float c = dot(v, v);
        float d = dot(u, w);
        float e = dot(v, w);

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

        return length_squared(pointOnRay - pointOnSeg);
    }

    bool Ray::CrossSphere(const Sphere &s)const
    {
        const Vector3f  sphere_center=s.GetCenter();
        const float     sphere_radius=s.GetRadius();

        const Vector3f oc=origin-sphere_center;

        const float b=dot(oc,direction);
        const float c=dot(oc,oc)-sphere_radius*sphere_radius;

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

        const float b=dot(oc,direction);
        const float c=dot(oc,oc)-es_radius.x*es_radius.y*es_radius.z;

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

        const Vector3f normal=cross(a2-a1,a3-a1);

        float rad=dot(normal,direction);
            
        if(rad>=0)              //射线与三角形背对
        {
            if(!two_side)
                return(false);

            rad=-rad;
        }

        const float d=dot(normal,a1);

        const float t=(d-dot(normal,origin))/rad;

        if(t<0)return(false);            //射线与三角形不相交

        const Vector3f hit_point=origin+direction*t;

        const Vector3f v1=cross(a2-a1,hit_point-a1);
        const Vector3f v2=cross(a3-a2,hit_point-a2);
        const Vector3f v3=cross(a1-a3,hit_point-a3);

        if(dot(v1,v2)<0)return(false);
        if(dot(v2,v3)<0)return(false);
        if(dot(v3,v1)<0)return(false);

        return(true);
    }

    /**
    * 求指定面是否与射线交汇
    */
    bool Ray::CrossPlane(const Vector3f &v1,const Vector3f &v2,const Vector3f &v3,const Vector3f &v4,const bool two_side)const
    {
        const Vector3f normal=cross(v2-v1,v3-v1);

        float rad=dot(normal,direction);

        if(rad>=0)              //射线与三角形背对
        {
            if(!two_side)
                return(false);

            rad=-rad;
        }

        const float d=dot(normal,v1);

        const float t=(d-dot(normal,origin))/rad;

        if(t<0)return(false);            //射线与平面不相交

        return(true);
    }

    bool Ray::CrossCircle(const Vector3f &center,const Vector3f &normal,const float radius)const
    {
        // 计算射线与平面的交点
        float denom = dot(normal, direction);
        if(IsNearlyZero(denom)) return false; // 射线与平面平行

        float t = dot(normal, center - origin) / denom;
        if (t < 0) return false; // 平面在射线的反方向

        // 计算交点
        Vector3f hit_point = origin + t * direction;

        // 判断交点是否在圆内
        return length_squared(hit_point - center) <= radius * radius;
    }
}//namespace hgl::math
