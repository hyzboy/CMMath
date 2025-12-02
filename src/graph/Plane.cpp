#include<hgl/graph/Plane.h>
#include<hgl/math/Math.h>
#include<algorithm>

namespace hgl::graph
{
    float DistanceBetweenPointAndRectangle3D(const Vector3f &p,const Rectangle3D &rect)
    {
        Vector3f v=p-rect.center;

        // 计算点到平面的垂直距离
        float normal_dist=abs(dot(v,rect.normal));

        // 计算在矩形平面上的投影坐标
        float up_len=dot(v,rect.up);
        float right_len=dot(v,rect.right);

        // 矩形的范围应该是以中心为原点的 [-size/2, size/2]
        float half_height=rect.size.y*0.5f;
        float half_width=rect.size.x*0.5f;

        // 检查投影是否在矩形内
        if(up_len>=-half_height && up_len<=half_height
         && right_len>=-half_width && right_len<=half_width)
        {
            // 投影在矩形内,返回垂直距离
            return normal_dist;
        }

        // 投影在矩形外,需要找到最近的边界点
        float up_len_clamped=std::clamp(up_len,-half_height,half_height);
        float right_len_clamped=std::clamp(right_len,-half_width,half_width);

        Vector3f up_v=rect.up*up_len_clamped;
        Vector3f right_v=rect.right*right_len_clamped;

        Vector3f closest_point=rect.center+up_v+right_v;

        return length(p-closest_point);
    }
}//namespace hgl::graph
