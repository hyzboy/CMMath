/**
 * ShorelineData.h - 浅海区网格数据生成
 * 
 * 将等高线转换为带方向信息的海岸线，并生成浅海区三角网格数据
 * 用于游戏中的海浪特效渲染（配合 Vulkan 3D 渲染）
 * 
 * 功能：
 * - 将等高线转换为带方向的海岸线
 * - 生成浅海区网格顶点和索引
 * - 计算符号距离场、法线、UV坐标
 * - 支持导出为数组格式供 Vulkan 使用
 * 
 * 使用场景：
 * - 海浪特效渲染
 * - 海岸线可视化
 * - 水体模拟
 */

#pragma once

#include <hgl/math/VectorTypes.h>
#include <hgl/math/geometry/HeightMapContour.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

namespace hgl::math
{
    /**
     * @brief 海岸线线段
     * 
     * 表示海岸线的一个线段，包含方向和法线信息
     */
    struct ShorelineSegment
    {
        Vector2f start;              ///< 起点
        Vector2f end;                ///< 终点
        Vector2f normal;             ///< 法线（指向陆地）
        float length;                ///< 线段长度
        float accumulated_length;    ///< 累积长度（用于UV映射）

        ShorelineSegment()
            : start(0, 0), end(0, 0), normal(0, 0)
            , length(0), accumulated_length(0)
        {
        }
    };

    /**
     * @brief 海岸线轮廓
     * 
     * 由多个线段组成的完整海岸线，支持距离查询
     */
    struct ShorelineContour
    {
        std::vector<ShorelineSegment> segments;  ///< 线段列表
        float total_length;                      ///< 总长度
        bool is_closed;                          ///< 是否闭合
        Vector2f center;                         ///< 中心点

        ShorelineContour()
            : total_length(0), is_closed(false), center(0, 0)
        {
        }

        /**
         * @brief 查询点到海岸线的距离和相关信息
         * 
         * @param point 查询点
         * @param closest_point 输出最近点（可选）
         * @param distance_along_contour 输出沿轮廓的距离（可选）
         * @param normal 输出法线（可选）
         * @return 到海岸线的最短距离
         */
        float QueryDistance(const Vector2f& point,
                           Vector2f* closest_point = nullptr,
                           float* distance_along_contour = nullptr,
                           Vector2f* normal = nullptr) const
        {
            if (segments.empty())
                return std::numeric_limits<float>::max();

            float min_dist = std::numeric_limits<float>::max();
            Vector2f best_closest = point;
            float best_accumulated = 0;
            Vector2f best_normal(0, 1);

            for (const auto& seg : segments)
            {
                Vector2f ab = seg.end - seg.start;
                Vector2f ap = point - seg.start;
                
                float ab_len_sq = glm::dot(ab, ab);
                if (ab_len_sq < 1e-8f)
                {
                    float dist = glm::length(ap);
                    if (dist < min_dist)
                    {
                        min_dist = dist;
                        best_closest = seg.start;
                        best_accumulated = seg.accumulated_length;
                        best_normal = seg.normal;
                    }
                    continue;
                }

                float t = glm::dot(ap, ab) / ab_len_sq;
                t = glm::clamp(t, 0.0f, 1.0f);
                
                Vector2f closest = seg.start + t * ab;
                float dist = glm::length(point - closest);
                
                if (dist < min_dist)
                {
                    min_dist = dist;
                    best_closest = closest;
                    best_accumulated = seg.accumulated_length + t * seg.length;
                    best_normal = seg.normal;
                }
            }

            if (closest_point)
                *closest_point = best_closest;
            if (distance_along_contour)
                *distance_along_contour = best_accumulated;
            if (normal)
                *normal = best_normal;

            return min_dist;
        }
    };

    /**
     * @brief 浅海区顶点数据
     * 
     * 包含渲染海浪特效所需的所有顶点属性
     */
    struct ShallowWaterVertex
    {
        Vector2f position;           ///< 世界坐标
        float distance_to_shore;     ///< 到海岸线的符号距离（正=向海，负=向陆地）
        float contour_position;      ///< 沿海岸线的归一化位置 [0,1]（用于UV.x）
        float depth_normalized;      ///< 深度归一化 [0,1]（0=岸边，1=深海，用于UV.y）
        Vector2f shore_normal;       ///< 海岸线法线（指向陆地）
        int contour_id;              ///< 所属轮廓ID

        ShallowWaterVertex()
            : position(0, 0)
            , distance_to_shore(0)
            , contour_position(0)
            , depth_normalized(0)
            , shore_normal(0, 1)
            , contour_id(-1)
        {
        }
    };

    /**
     * @brief 浅海区网格配置
     * 
     * 控制网格生成的参数
     */
    struct ShallowWaterMeshConfig
    {
        float grid_spacing;          ///< 网格间距（像素单位）
        float max_wave_depth;        ///< 最大海浪深度（像素单位）
        bool generate_indices;       ///< 是否生成三角形索引

        ShallowWaterMeshConfig()
            : grid_spacing(2.0f)
            , max_wave_depth(50.0f)
            , generate_indices(true)
        {
        }
    };

    /**
     * @brief 浅海区网格数据
     * 
     * 包含顶点、索引和海岸线信息，可导出给 Vulkan 渲染器
     */
    struct ShallowWaterMeshData
    {
        std::vector<ShallowWaterVertex> vertices;           ///< 顶点数据
        std::vector<uint32_t> indices;                      ///< 三角形索引
        std::vector<ShorelineContour> shore_contours;       ///< 海岸线轮廓（浅水区边界）
        std::vector<ShorelineContour> deep_water_contours;  ///< 深水区轮廓

        /**
         * @brief 导出为数组格式供 Vulkan 使用
         * 
         * 顶点数据布局（每个顶点 8 个浮点数）：
         * - position.x, position.y
         * - distance_to_shore
         * - contour_position
         * - depth_normalized
         * - shore_normal.x, shore_normal.y
         * - contour_id (as float)
         * 
         * @param vertex_data 输出顶点数据数组指针
         * @param vertex_count 输出顶点数量
         * @param vertex_stride 顶点步长（浮点数个数）
         * @param index_data 输出索引数据数组指针
         * @param index_count 输出索引数量
         */
        void ExportToArrays(float** vertex_data, int* vertex_count, int vertex_stride,
                           uint32_t** index_data, int* index_count)
        {
            if (vertex_stride < 8)
                vertex_stride = 8;  // 最小 8 个浮点数

            *vertex_count = static_cast<int>(vertices.size());
            *index_count = static_cast<int>(indices.size());

            // 分配顶点数据
            if (*vertex_count > 0)
            {
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
                    
                    // 如果 stride 更大，填充 0
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
    };

    /**
     * @brief 浅海区数据提取器
     * 
     * 从等高线生成浅海区网格数据
     */
    class ShallowWaterDataExtractor
    {
    private:
        const HeightMapContourResult* contours_;
        ShallowWaterMeshConfig config_;

        /**
         * @brief 从等高线多边形构建海岸线轮廓
         */
        ShorelineContour BuildShorelineFromContour(const ContourPolygon& contour)
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

    public:
        /**
         * @brief 构造函数
         * 
         * @param contours 等高线提取结果
         * @param config 网格生成配置
         */
        ShallowWaterDataExtractor(const HeightMapContourResult* contours,
                                  const ShallowWaterMeshConfig& config)
            : contours_(contours)
            , config_(config)
        {
        }

        /**
         * @brief 仅提取海岸线数据（不生成网格）
         * 
         * @param shore_contours 输出海岸线轮廓
         * @param deep_contours 输出深水区轮廓
         */
        void ExtractShorelineOnly(std::vector<ShorelineContour>& shore_contours,
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

        /**
         * @brief 生成浅海区网格数据
         * 
         * @return 完整的网格数据，包括顶点、索引和海岸线信息
         */
        ShallowWaterMeshData Extract()
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
            
            // 由于这是一个复杂的算法，这里提供基本框架
            // 实际使用时可以根据需求扩展

            return mesh_data;
        }
    };

}//namespace hgl::math
