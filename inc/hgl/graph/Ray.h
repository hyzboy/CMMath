#pragma once

#include<hgl/graph/LineSegment.h>
#include<hgl/graph/Triangle.h>

namespace hgl::graph
{
    struct CameraInfo;

    class Sphere;
    class EllipseSphere;
    class AABB;
    class OBB;

    /**
    * 射线类
    */
    struct Ray
    {
        Vector3f origin;
        Vector3f direction;

    public:

        Ray()
        {
            origin=Vector3f(0.0f,0.0f,0.0f);
            direction=Vector3f(0.0f,0.0f,0.0f);
        }

        Ray(const Vector3f &o,const Vector3f &d)
        {
            origin=o;
            direction=d;
        }

        void SetFromViewportPoint(const Vector2i &,const CameraInfo *,const Vector2u &viewport_size);

        /**
        * 取得与原点指定距离的点
        */
        Vector3f GetLengthPoint(const float len)const
        {
            return origin+direction*(len/length_2d(direction));
        }

        /**
        * 取射线与指定点的夹角cos值
        */
        float GetAngleCos(const Vector3f &point)const
        {
            return dot(direction,normalize(point-origin));
        }

        /**
        * 取射线与指定点的夹角
        */
        float GetAngle(const Vector3f &point)const
        {
            return acos(GetAngleCos(point))*180.0f/HGL_PI;
        }

    public: //距离计算

        /**
        * 求射线上距离指定点最近的点
        * @param point 指定点
        * @return 位于射线上的点
        */
        Vector3f ClosestPoint(const Vector3f &point)const;

        void ClosestPoint(Vector3f &point_on_ray,Vector3f &point_on_segment,const Vector3f &line_segment_start,const Vector3f &line_segment_end)const;

        Vector3f ClosestPoint(const Vector3f &,const Vector3f &)const;                                                  ///<求指定线段上离射线最近的点
        Vector3f ClosestPoint(const LineSegment &ls)const{return ClosestPoint(ls.GetStart(),ls.GetEnd());}              ///<求指定线段上离射线最近的点

        /**
        * 求指定点与射线的距离的平方
        * @param point 指定点
        * @return 该点与射线的距离
        */
        float ToPointDistanceSquared(const Vector3f &point)const{return length_squared(point,ClosestPoint(point));}

        /**
        * 求指定点与射线的距离
        * @param point 指定点
        * @return 该点与射线的距离
        */
        float ToPointDistance(const Vector3f &point)const{return sqrt(ToPointDistanceSquared(point));}

        const float ToLineSegmentDistanceSquared(const Vector3f &,const Vector3f &)const;
        const float ToLineSegmentDistance(const Vector3f &start,const Vector3f &end)const{return(sqrt(ToLineSegmentDistanceSquared(start,end)));}

        const float ToLineSegmentDistanceSquared(const LineSegment &ls)const{return ToLineSegmentDistanceSquared(ls.GetStart(),ls.GetEnd());}
        const float ToLineSegmentDistance(const LineSegment &ls)const{return(sqrt(ToLineSegmentDistanceSquared(ls)));}

    public: //交叉判断

        bool CrossSphere(const Sphere &)const;                                                      ///<求射线是否与指定球体相交
        bool CrossEllipseSphere(const EllipseSphere &)const;                                        ///<求射线是否与指定椭球体相交

        bool CrossTriangle(const Triangle3f &,bool two_side=true)const;                                                ///<指当前射线是否与指定三角形相交
        bool CrossPlane(const Vector3f &,const Vector3f &,const Vector3f &,const Vector3f &,bool two_side=true)const;  ///<求指定面是否与射线交汇

        bool CrossCircle(const Vector3f &center,const Vector3f &normal,const float radius)const; ///<求射线是否与指定圆相交
    };//struct Ray
}//namespace hgl::graph
