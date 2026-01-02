/**
 * HeightMapContour.h - 高度图等高线提取
 * 
 * 使用 Marching Squares 算法从单通道高度图提取等高线
 * 用于游戏中的地形分析、海岸线检测等场景
 * 
 * 功能：
 * - 提取指定高度阈值的等高线
 * - 支持多种数据类型（uint8, uint16, uint32, float）
 * - 线性插值实现亚像素精度
 * - 支持多边形简化（Douglas-Peucker 算法）
 * - 支持轮廓平滑（Chaikin 算法）
 * 
 * 使用场景：
 * - 从高度图提取海岸线
 * - 地形等高线可视化
 * - 区域划分和分析
 */

#pragma once

#include <hgl/math/VectorTypes.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <functional>

namespace hgl::math
{
    /**
     * @brief 等高线多边形
     * 
     * 表示一条等高线，由一系列顶点组成
     * 可以是闭合的（环形）或开放的（线段）
     */
    struct ContourPolygon
    {
        std::vector<Vector2f> vertices;  ///< 顶点列表
        bool is_closed;                  ///< 是否为闭合轮廓

        ContourPolygon() : is_closed(false) {}
    };

    /**
     * @brief 等高线提取结果
     * 
     * 包含两个高度阈值对应的等高线集合：
     * - low_to_mid_contours: 深水区与浅水区的边界
     * - mid_to_high_contours: 浅水区与陆地的边界
     */
    struct HeightMapContourResult
    {
        std::vector<ContourPolygon> low_to_mid_contours;   ///< 低阈值等高线（深水区边界）
        std::vector<ContourPolygon> mid_to_high_contours;  ///< 高阈值等高线（陆地边界）
    };

    /**
     * @brief Douglas-Peucker 多边形简化算法
     * 
     * 使用垂直距离判断法简化多边形，减少顶点数量同时保持形状特征
     * 
     * @param vertices 输入顶点列表
     * @param epsilon 简化容差（像素单位），越大简化越激进
     * @param out_vertices 输出简化后的顶点列表
     */
    inline void SimplifyPolygonDP(const std::vector<Vector2f>& vertices, 
                                   float epsilon, 
                                   std::vector<Vector2f>& out_vertices)
    {
        if (vertices.size() < 3)
        {
            out_vertices = vertices;
            return;
        }

        // 点到线段的距离平方
        auto pointLineDistSq = [](const Vector2f& p, const Vector2f& a, const Vector2f& b) -> float
        {
            Vector2f ab = b - a;
            Vector2f ap = p - a;
            float ab_len_sq = glm::dot(ab, ab);
            
            if (ab_len_sq < 1e-8f)
                return glm::dot(ap, ap);
            
            float t = glm::dot(ap, ab) / ab_len_sq;
            t = glm::clamp(t, 0.0f, 1.0f);
            Vector2f projection = a + t * ab;
            Vector2f diff = p - projection;
            return glm::dot(diff, diff);
        };

        // 递归简化
        std::function<void(int, int, std::vector<bool>&)> dpRecursive;
        dpRecursive = [&](int start, int end, std::vector<bool>& keep) -> void
        {
            if (end - start < 2)
                return;

            float max_dist_sq = 0.0f;
            int max_index = start;

            for (int i = start + 1; i < end; ++i)
            {
                float dist_sq = pointLineDistSq(vertices[i], vertices[start], vertices[end]);
                if (dist_sq > max_dist_sq)
                {
                    max_dist_sq = dist_sq;
                    max_index = i;
                }
            }

            if (max_dist_sq > epsilon * epsilon)
            {
                keep[max_index] = true;
                dpRecursive(start, max_index, keep);
                dpRecursive(max_index, end, keep);
            }
        };

        std::vector<bool> keep(vertices.size(), false);
        keep[0] = true;
        keep[vertices.size() - 1] = true;

        dpRecursive(0, vertices.size() - 1, keep);

        out_vertices.clear();
        out_vertices.reserve(vertices.size());
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            if (keep[i])
                out_vertices.push_back(vertices[i]);
        }
    }

    /**
     * @brief Chaikin 曲线平滑算法
     * 
     * 通过迭代细分和平滑使多边形轮廓更加圆润
     * 
     * @param vertices 输入顶点列表
     * @param iterations 迭代次数（1-3 通常足够）
     * @param is_closed 是否为闭合曲线
     * @param out_vertices 输出平滑后的顶点列表
     */
    inline void SmoothPolygonChaikin(const std::vector<Vector2f>& vertices,
                                      int iterations,
                                      bool is_closed,
                                      std::vector<Vector2f>& out_vertices)
    {
        if (vertices.size() < 2 || iterations <= 0)
        {
            out_vertices = vertices;
            return;
        }

        std::vector<Vector2f> current = vertices;
        std::vector<Vector2f> next;

        for (int iter = 0; iter < iterations; ++iter)
        {
            next.clear();
            size_t n = current.size();

            for (size_t i = 0; i < n - 1; ++i)
            {
                Vector2f p0 = current[i];
                Vector2f p1 = current[i + 1];
                
                // 在每条边上生成两个新点（1/4 和 3/4 位置）
                next.push_back(p0 * 0.75f + p1 * 0.25f);
                next.push_back(p0 * 0.25f + p1 * 0.75f);
            }

            // 处理闭合曲线的最后一条边
            if (is_closed && n > 2)
            {
                Vector2f p0 = current[n - 1];
                Vector2f p1 = current[0];
                next.push_back(p0 * 0.75f + p1 * 0.25f);
                next.push_back(p0 * 0.25f + p1 * 0.75f);
            }

            current = next;
        }

        out_vertices = current;
    }

    /**
     * @brief 高度图等高线提取器
     * 
     * 使用 Marching Squares 算法从高度图提取等高线
     * 支持多种数据类型，并提供亚像素精度的线性插值
     * 
     * @tparam T 高度数据类型（uint8, uint16, uint32, float）
     */
    template<typename T>
    class HeightMapContourExtractor
    {
    private:
        const T* data_;          ///< 高度图数据
        int width_;              ///< 图像宽度
        int height_;             ///< 图像高度
        T low_threshold_;        ///< 低阈值（深水区/浅水区边界）
        T high_threshold_;       ///< 高阈值（浅水区/陆地边界）

        /**
         * @brief 计算 Marching Squares 的格子配置索引
         * 
         * @param x 格子左下角 x 坐标
         * @param y 格子左下角 y 坐标
         * @param threshold 高度阈值
         * @return 格子配置索引（0-15）
         */
        int GetCellConfig(int x, int y, T threshold) const
        {
            if (x < 0 || x >= width_ - 1 || y < 0 || y >= height_ - 1)
                return 0;

            int config = 0;
            
            // 按逆时针顺序检查四个角点：左下、右下、右上、左上
            if (data_[y * width_ + x] >= threshold)                    config |= 1;  // 左下
            if (data_[y * width_ + (x + 1)] >= threshold)              config |= 2;  // 右下
            if (data_[(y + 1) * width_ + (x + 1)] >= threshold)        config |= 4;  // 右上
            if (data_[(y + 1) * width_ + x] >= threshold)              config |= 8;  // 左上

            return config;
        }

        /**
         * @brief 使用线性插值计算等高线与格子边的交点
         * 
         * @param x 格子左下角 x 坐标
         * @param y 格子左下角 y 坐标
         * @param edge 边索引（0=下边, 1=右边, 2=上边, 3=左边）
         * @param threshold 高度阈值
         * @return 交点坐标（亚像素精度）
         */
        Vector2f InterpolateEdge(int x, int y, int edge, T threshold) const
        {
            float fx = static_cast<float>(x);
            float fy = static_cast<float>(y);
            float t = 0.5f;  // 默认中点

            switch (edge)
            {
            case 0: // 下边（y, 从 x 到 x+1）
            {
                T v0 = data_[y * width_ + x];
                T v1 = data_[y * width_ + (x + 1)];
                if (v1 != v0)
                    t = static_cast<float>(threshold - v0) / static_cast<float>(v1 - v0);
                return Vector2f(fx + glm::clamp(t, 0.0f, 1.0f), fy);
            }
            case 1: // 右边（x+1, 从 y 到 y+1）
            {
                T v0 = data_[y * width_ + (x + 1)];
                T v1 = data_[(y + 1) * width_ + (x + 1)];
                if (v1 != v0)
                    t = static_cast<float>(threshold - v0) / static_cast<float>(v1 - v0);
                return Vector2f(fx + 1.0f, fy + glm::clamp(t, 0.0f, 1.0f));
            }
            case 2: // 上边（y+1, 从 x+1 到 x）
            {
                T v0 = data_[(y + 1) * width_ + (x + 1)];
                T v1 = data_[(y + 1) * width_ + x];
                if (v1 != v0)
                    t = static_cast<float>(threshold - v0) / static_cast<float>(v1 - v0);
                return Vector2f(fx + 1.0f - glm::clamp(t, 0.0f, 1.0f), fy + 1.0f);
            }
            case 3: // 左边（x, 从 y+1 到 y）
            {
                T v0 = data_[(y + 1) * width_ + x];
                T v1 = data_[y * width_ + x];
                if (v1 != v0)
                    t = static_cast<float>(threshold - v0) / static_cast<float>(v1 - v0);
                return Vector2f(fx, fy + 1.0f - glm::clamp(t, 0.0f, 1.0f));
            }
            default:
                return Vector2f(fx + 0.5f, fy + 0.5f);
            }
        }

    public:
        /**
         * @brief 构造函数
         * 
         * @param data 高度图数据指针
         * @param width 图像宽度
         * @param height 图像高度
         * @param low_threshold 低阈值（深水区/浅水区边界）
         * @param high_threshold 高阈值（浅水区/陆地边界）
         */
        HeightMapContourExtractor(const T* data, int width, int height, 
                                  T low_threshold, T high_threshold)
            : data_(data)
            , width_(width)
            , height_(height)
            , low_threshold_(low_threshold)
            , high_threshold_(high_threshold)
        {
        }

        /**
         * @brief 提取单个阈值的等高线
         * 
         * @param threshold 高度阈值
         * @param contours 输出等高线列表
         */
        void ExtractSingleContour(T threshold, std::vector<ContourPolygon>& contours)
        {
            contours.clear();

            // Marching Squares 边查找表
            // 每个配置对应的边列表（-1 表示结束）
            static const int edgeTable[16][5] = {
                {-1, -1, -1, -1, -1}, // 0: 无边
                {0, 3, -1, -1, -1},   // 1: 左下
                {0, 1, -1, -1, -1},   // 2: 右下
                {1, 3, -1, -1, -1},   // 3: 左下+右下
                {1, 2, -1, -1, -1},   // 4: 右上
                {0, 1, 2, 3, -1},     // 5: 左下+右上（模糊情况）
                {0, 2, -1, -1, -1},   // 6: 右下+右上
                {2, 3, -1, -1, -1},   // 7: 左下+右下+右上
                {2, 3, -1, -1, -1},   // 8: 左上
                {0, 2, -1, -1, -1},   // 9: 左下+左上
                {0, 1, 2, 3, -1},     // 10: 右下+左上（模糊情况）
                {1, 2, -1, -1, -1},   // 11: 左下+右下+左上
                {1, 3, -1, -1, -1},   // 12: 右上+左上
                {0, 1, -1, -1, -1},   // 13: 左下+右上+左上
                {0, 3, -1, -1, -1},   // 14: 右下+右上+左上
                {-1, -1, -1, -1, -1}  // 15: 全部高于阈值
            };

            // 遍历所有格子，提取线段
            std::vector<std::pair<Vector2f, Vector2f>> segments;
            
            for (int y = 0; y < height_ - 1; ++y)
            {
                for (int x = 0; x < width_ - 1; ++x)
                {
                    int config = GetCellConfig(x, y, threshold);
                    
                    const int* edges = edgeTable[config];
                    for (int i = 0; edges[i] != -1; i += 2)
                    {
                        if (edges[i + 1] == -1)
                            break;

                        Vector2f p0 = InterpolateEdge(x, y, edges[i], threshold);
                        Vector2f p1 = InterpolateEdge(x, y, edges[i + 1], threshold);
                        segments.push_back({p0, p1});
                    }
                }
            }

            // 将线段连接成轮廓
            // 简化实现：直接将每个线段作为一个轮廓
            // 在实际应用中，可以实现更复杂的线段连接算法
            for (const auto& seg : segments)
            {
                ContourPolygon poly;
                poly.vertices.push_back(seg.first);
                poly.vertices.push_back(seg.second);
                poly.is_closed = false;
                contours.push_back(poly);
            }
        }

        /**
         * @brief 提取两个阈值的等高线
         * 
         * @param smooth 是否平滑轮廓
         * @param simplify_epsilon 简化容差（0 表示不简化）
         * @return 等高线提取结果
         */
        HeightMapContourResult Extract(bool smooth = false, float simplify_epsilon = 0.0f)
        {
            HeightMapContourResult result;

            // 提取低阈值等高线（深水区/浅水区边界）
            ExtractSingleContour(low_threshold_, result.low_to_mid_contours);

            // 提取高阈值等高线（浅水区/陆地边界）
            ExtractSingleContour(high_threshold_, result.mid_to_high_contours);

            // 应用简化和平滑
            if (simplify_epsilon > 0.0f || smooth)
            {
                auto processContours = [&](std::vector<ContourPolygon>& contours)
                {
                    for (auto& contour : contours)
                    {
                        // 简化
                        if (simplify_epsilon > 0.0f && contour.vertices.size() > 2)
                        {
                            std::vector<Vector2f> simplified;
                            SimplifyPolygonDP(contour.vertices, simplify_epsilon, simplified);
                            contour.vertices = simplified;
                        }

                        // 平滑
                        if (smooth && contour.vertices.size() > 2)
                        {
                            std::vector<Vector2f> smoothed;
                            SmoothPolygonChaikin(contour.vertices, 2, contour.is_closed, smoothed);
                            contour.vertices = smoothed;
                        }
                    }
                };

                processContours(result.low_to_mid_contours);
                processContours(result.mid_to_high_contours);
            }

            return result;
        }
    };

}//namespace hgl::math
