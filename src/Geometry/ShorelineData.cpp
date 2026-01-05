#include <hgl/math/geometry/ShorelineData.h>
#include <glm/glm.hpp>

namespace hgl::math
{
    void ShallowWaterMeshData::ExportToArrays(float** vertex_data, int* vertex_count, int vertex_stride,
                           uint32_t** index_data, int* index_count)
    {
        if (vertex_stride < 8)
            vertex_stride = 8;  // 最小 8 个浮点数

        *vertex_count = static_cast<int>(vertices.size());
        *index_count = static_cast<int>(indices.size());

        // 分配顶点数据
        if (*vertex_count > 0)
        {
            // 检查整数溢出
            if (*vertex_count > std::numeric_limits<int>::max() / vertex_stride)
            {
                *vertex_data = nullptr;
                *vertex_count = 0;
                *index_data = nullptr;
                *index_count = 0;
                return;
            }

            *vertex_data = new float[*vertex_count * vertex_stride];
            float* vptr = *vertex_data;

            for (const auto& v : vertices)
            {
                vptr[0] = v.position.x;
                vptr[1] = v.position.y;
                vptr[2] = v.distance_to_shore;
                vptr[3] = v.contour_position;
                vptr[4] = v.depth_normalized;
                vptr[5] = v.shore_normal.x;
                vptr[6] = v.shore_normal.y;
                vptr[7] = static_cast<float>(v.contour_id);

                for (int i = 8; i < vertex_stride; ++i)
                    vptr[i] = 0.0f;

                vptr += vertex_stride;
            }
        }
        else
        {
            *vertex_data = nullptr;
        }

        // 分配索引数据
        if (*index_count > 0)
        {
            *index_data = new uint32_t[*index_count];
            std::copy(indices.begin(), indices.end(), *index_data);
        }
        else
        {
            *index_data = nullptr;
        }
    }

    ShorelineContour ShallowWaterDataExtractor::BuildShorelineFromContour(const ContourPolygon& contour)
    {
        ShorelineContour shoreline;
        shoreline.is_closed = contour.is_closed;

        if (contour.vertices.size() < 2)
            return shoreline;

        // 计算中心点
        Vector2f sum(0, 0);
        for (const auto& v : contour.vertices)
            sum += v;
        shoreline.center = sum / static_cast<float>(contour.vertices.size());

        // 构建线段
        float accumulated = 0;
        for (size_t i = 0; i < contour.vertices.size() - 1; ++i)
        {
            ShorelineSegment seg;
            seg.start = contour.vertices[i];
            seg.end = contour.vertices[i + 1];
            seg.length = glm::length(seg.end - seg.start);
            seg.accumulated_length = accumulated;

            // 计算法线（指向陆地，即向右旋转90度）
            Vector2f dir = glm::normalize(seg.end - seg.start);
            seg.normal = Vector2f(dir.y, -dir.x);

            shoreline.segments.push_back(seg);
            accumulated += seg.length;
        }

        // 闭合轮廓的最后一条边
        if (contour.is_closed && contour.vertices.size() > 2)
        {
            ShorelineSegment seg;
            seg.start = contour.vertices.back();
            seg.end = contour.vertices.front();
            seg.length = glm::length(seg.end - seg.start);
            seg.accumulated_length = accumulated;

            Vector2f dir = glm::normalize(seg.end - seg.start);
            seg.normal = Vector2f(dir.y, -dir.x);

            shoreline.segments.push_back(seg);
            accumulated += seg.length;
        }

        shoreline.total_length = accumulated;
        return shoreline;
    }

    void ShallowWaterDataExtractor::ExtractShorelineOnly(std::vector<ShorelineContour>& shore_contours,
                                  std::vector<ShorelineContour>& deep_contours)
    {
        shore_contours.clear();
        deep_contours.clear();

        if (!contours_)
            return;

        // 转换高阈值等高线为海岸线（浅水区/陆地边界）
        for (const auto& contour : contours_->mid_to_high_contours)
        {
            shore_contours.push_back(BuildShorelineFromContour(contour));
        }

        // 转换低阈值等高线为深水区边界
        for (const auto& contour : contours_->low_to_mid_contours)
        {
            deep_contours.push_back(BuildShorelineFromContour(contour));
        }
    }

    ShallowWaterMeshData ShallowWaterDataExtractor::Extract()
    {
        ShallowWaterMeshData mesh_data;

        if (!contours_)
            return mesh_data;

        // 提取海岸线
        ExtractShorelineOnly(mesh_data.shore_contours, mesh_data.deep_water_contours);

        // 简化实现：这里返回空的网格数据
        // 完整实现需要：
        // 1. 生成规则网格
        // 2. 计算每个顶点到海岸线的距离
        // 3. 生成三角形索引
        // 4. 计算UV和法线信息

        return mesh_data;
    }

}//namespace hgl::math
