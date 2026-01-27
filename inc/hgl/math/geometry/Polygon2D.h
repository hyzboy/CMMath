/**
 * Polygon2D.h - 2D多边形及三角剖分功能
 *
 * 提供2D多边形的表示和三角剖分（Triangulation）功能。
 * 三角剖分是将多边形分解为多个三角形的过程，广泛应用于：
 * - 2D图形渲染和填充
 * - 碰撞检测优化
 * - 纹理映射
 * - 物理模拟
 * - UI界面渲染
 *
 * 算法说明：
 * 使用耳切法（Ear Clipping）进行三角剖分。
 * - 适用于简单多边形（无自相交）
 * - 时间复杂度：O(n²)，n为顶点数
 * - 稳定可靠，实现简单
 *
 * 参考资料：
 * - "Computational Geometry: Algorithms and Applications" (de Berg et al.)
 * - "Real-Time Collision Detection" (Christer Ericson)
 */

#pragma once

#include <hgl/math/VectorTypes.h>
#include <hgl/math/geometry/Triangle.h>
#include <vector>
#include <cmath>
#include <limits>

namespace hgl::math
{
    /**
     * @brief 计算2D多边形的有向面积
     *
     * 使用Shoelace公式（鞋带公式）计算多边形的有向面积。
     * - 正值：逆时针方向（CCW）
     * - 负值：顺时针方向（CW）
     * - 绝对值即为多边形面积
     *
     * @param vertices 多边形顶点数组
     * @return 有向面积（可为负）
     */
    template<typename T>
    inline T Polygon2DSignedArea(const std::vector<glm::vec<2, T>>& vertices)
    {
        if (vertices.size() < 3)
            return T(0);

        T area = T(0);
        const size_t n = vertices.size();

        for (size_t i = 0; i < n; ++i)
        {
            const size_t j = (i + 1) % n;
            area += vertices[i].x * vertices[j].y;
            area -= vertices[j].x * vertices[i].y;
        }

        return area * T(0.5);
    }

    /**
     * @brief 计算2D多边形的面积（绝对值）
     *
     * @param vertices 多边形顶点数组
     * @return 多边形面积（非负）
     */
    template<typename T>
    inline T Polygon2DArea(const std::vector<glm::vec<2, T>>& vertices)
    {
        return std::abs(Polygon2DSignedArea(vertices));
    }

    /**
     * @brief 判断多边形顶点是否按逆时针顺序排列
     *
     * @param vertices 多边形顶点数组
     * @return true表示逆时针，false表示顺时针
     */
    template<typename T>
    inline bool IsPolygon2DCCW(const std::vector<glm::vec<2, T>>& vertices)
    {
        return Polygon2DSignedArea(vertices) > T(0);
    }

    /**
     * @brief 判断点是否在三角形内（2D）
     *
     * 使用叉积法判断点是否在三角形内部。
     * 检查点是否在三条边的同一侧。
     *
     * @param a 三角形顶点A
     * @param b 三角形顶点B
     * @param c 三角形顶点C
     * @param p 待测试的点
     * @return true表示点在三角形内或边上
     */
    template<typename T>
    inline bool IsPointInTriangle2D(const glm::vec<2, T>& a,
                                    const glm::vec<2, T>& b,
                                    const glm::vec<2, T>& c,
                                    const glm::vec<2, T>& p)
    {
        // 计算叉积判断点的位置
        auto sign = [](const glm::vec<2, T>& p1, const glm::vec<2, T>& p2, const glm::vec<2, T>& p3) -> T
        {
            return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
        };

        T d1 = sign(p, a, b);
        T d2 = sign(p, b, c);
        T d3 = sign(p, c, a);

        bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(has_neg && has_pos);
    }

    /**
     * @brief 判断三角形是否为"耳朵"
     *
     * 耳朵的定义：
     * 1. 三角形的三个顶点在多边形上连续
     * 2. 三角形完全在多边形内部
     * 3. 三角形内部不包含其他多边形顶点
     *
     * @param vertices 多边形所有顶点
     * @param i 三角形中间顶点的索引
     * @param indices 当前剩余顶点的索引数组
     * @return true表示是耳朵
     */
    template<typename T>
    inline bool IsEar(const std::vector<glm::vec<2, T>>& vertices,
                     size_t i,
                     const std::vector<size_t>& indices)
    {
        const size_t n = indices.size();
        if (n < 3)
            return false;

        // 获取三角形的三个顶点
        const size_t prev = (i + n - 1) % n;
        const size_t next = (i + 1) % n;

        const glm::vec<2, T>& a = vertices[indices[prev]];
        const glm::vec<2, T>& b = vertices[indices[i]];
        const glm::vec<2, T>& c = vertices[indices[next]];

        // 检查三角形的方向（必须与多边形方向一致）
        T cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        if (cross <= T(0))  // 不是凸顶点
            return false;

        // 检查是否有其他顶点在三角形内
        for (size_t j = 0; j < n; ++j)
        {
            if (j == prev || j == i || j == next)
                continue;

            const glm::vec<2, T>& p = vertices[indices[j]];
            if (IsPointInTriangle2D(a, b, c, p))
                return false;
        }

        return true;
    }

    /**
     * @brief 判断三角形是否为"耳朵"（带边长约束版本）
     *
     * 耳朵的定义：
     * 1. 三角形的三个顶点在多边形上连续
     * 2. 三角形完全在多边形内部
     * 3. 三角形内部不包含其他多边形顶点
     * 4. 三角形的所有边长满足指定的最小和最大长度约束
     *
     * @param vertices 多边形所有顶点
     * @param i 三角形中间顶点的索引
     * @param indices 当前剩余顶点的索引数组
     * @param min_edge_length 最小边长约束（默认1.0）
     * @param max_edge_length 最大边长约束（默认无限大）
     * @return true表示是耳朵
     */
    template<typename T>
    inline bool IsEar(const std::vector<glm::vec<2, T>>& vertices,
                     size_t i,
                     const std::vector<size_t>& indices,
                     T min_edge_length,
                     T max_edge_length)
    {
        const size_t n = indices.size();
        if (n < 3)
            return false;

        // 获取三角形的三个顶点
        const size_t prev = (i + n - 1) % n;
        const size_t next = (i + 1) % n;

        const glm::vec<2, T>& a = vertices[indices[prev]];
        const glm::vec<2, T>& b = vertices[indices[i]];
        const glm::vec<2, T>& c = vertices[indices[next]];

        // 检查三角形的方向（必须与多边形方向一致）
        T cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        if (cross <= T(0))  // 不是凸顶点
            return false;

        // 检查边长约束
        T edge1_len = glm::length(b - a);
        T edge2_len = glm::length(c - b);
        T edge3_len = glm::length(a - c);

        // 所有边长必须在指定范围内
        if (edge1_len < min_edge_length || edge1_len > max_edge_length ||
            edge2_len < min_edge_length || edge2_len > max_edge_length ||
            edge3_len < min_edge_length || edge3_len > max_edge_length)
        {
            return false;
        }

        // 检查是否有其他顶点在三角形内
        for (size_t j = 0; j < n; ++j)
        {
            if (j == prev || j == i || j == next)
                continue;

            const glm::vec<2, T>& p = vertices[indices[j]];
            if (IsPointInTriangle2D(a, b, c, p))
                return false;
        }

        return true;
    }

    /**
     * @brief 2D多边形三角剖分（Ear Clipping算法）
     *
     * 将简单多边形（无自相交）分解为多个三角形。
     *
     * 算法流程：
     * 1. 确保顶点按逆时针顺序排列
     * 2. 寻找一个"耳朵"（满足特定条件的三角形）
     * 3. 切除该耳朵，添加到输出三角形列表
     * 4. 重复步骤2-3，直到只剩下一个三角形
     *
     * 注意事项：
     * - 输入多边形必须是简单多边形（边不能相交）
     * - 顶点数必须 >= 3
     * - 算法会自动处理顶点顺序
     *
     * @param vertices 多边形顶点数组（至少3个顶点）
     * @param out_triangles 输出三角形索引数组，每3个索引构成一个三角形
     * @return true表示三角剖分成功，false表示失败（通常因为输入无效）
     */
    template<typename T>
    inline bool TriangulatePolygon2D(const std::vector<glm::vec<2, T>>& vertices,
                                    std::vector<size_t>& out_triangles)
    {
        out_triangles.clear();

        const size_t n = vertices.size();
        if (n < 3)
            return false;

        // 如果只有3个顶点，直接返回
        if (n == 3)
        {
            out_triangles = {0, 1, 2};
            return true;
        }

        // 创建顶点索引数组
        std::vector<size_t> indices(n);
        for (size_t i = 0; i < n; ++i)
            indices[i] = i;

        // 确保顶点按逆时针顺序排列
        std::vector<glm::vec<2, T>> ordered_vertices = vertices;
        if (!IsPolygon2DCCW(ordered_vertices))
        {
            // 如果是顺时针，反转顶点顺序
            std::reverse(ordered_vertices.begin(), ordered_vertices.end());
            for (size_t i = 0; i < n; ++i)
                indices[i] = n - 1 - i;
        }

        // 耳切法主循环
        size_t remaining = n;
        size_t iterations = 0;
        // 最坏情况下需要 O(n²) 次迭代才能找到所有耳朵
        // 设置上限防止无限循环（通常因为输入多边形无效）
        const size_t max_iterations = n * n;

        while (remaining > 3 && iterations < max_iterations)
        {
            bool ear_found = false;

            for (size_t i = 0; i < remaining; ++i)
            {
                if (IsEar(ordered_vertices, i, indices))
                {
                    // 找到耳朵，添加三角形
                    const size_t prev = (i + remaining - 1) % remaining;
                    const size_t next = (i + 1) % remaining;

                    // 添加三角形索引（使用原始顶点索引）
                    out_triangles.push_back(indices[prev]);
                    out_triangles.push_back(indices[i]);
                    out_triangles.push_back(indices[next]);

                    // 移除当前顶点
                    indices.erase(indices.begin() + i);
                    remaining--;
                    ear_found = true;
                    break;
                }
            }

            if (!ear_found)
            {
                // 没有找到耳朵，可能输入有问题
                return false;
            }

            iterations++;
        }

        // 添加最后剩余的三角形
        if (remaining == 3)
        {
            out_triangles.push_back(indices[0]);
            out_triangles.push_back(indices[1]);
            out_triangles.push_back(indices[2]);
        }

        return true;
    }

    /**
     * @brief 2D多边形三角剖分（带边长约束的Ear Clipping算法）
     *
     * 将简单多边形（无自相交）分解为多个三角形，同时满足边长约束。
     *
     * 算法流程：
     * 1. 确保顶点按逆时针顺序排列
     * 2. 寻找一个"耳朵"（满足特定条件且边长在范围内的三角形）
     * 3. 切除该耳朵，添加到输出三角形列表
     * 4. 重复步骤2-3，直到只剩下一个三角形
     *
     * 边长约束说明：
     * - 生成的所有三角形的每条边长度必须在 [min_edge_length, max_edge_length] 范围内
     * - min_edge_length 默认为1.0（最小1像素点）
     * - max_edge_length 默认为无限大（std::numeric_limits<T>::max()）
     * - 如果无法找到满足边长约束的三角剖分，函数返回false
     *
     * 注意事项：
     * - 输入多边形必须是简单多边形（边不能相交）
     * - 顶点数必须 >= 3
     * - 算法会自动处理顶点顺序
     * - 过于严格的边长约束可能导致无法完成三角剖分
     *
     * @param vertices 多边形顶点数组（至少3个顶点）
     * @param out_triangles 输出三角形索引数组，每3个索引构成一个三角形
     * @param min_edge_length 三角形最小边长约束（默认1.0）
     * @param max_edge_length 三角形最大边长约束（默认无限大）
     * @return true表示三角剖分成功，false表示失败（通常因为输入无效或无法满足边长约束）
     */
    template<typename T>
    inline bool TriangulatePolygon2D(const std::vector<glm::vec<2, T>>& vertices,
                                    std::vector<size_t>& out_triangles,
                                    T min_edge_length,
                                    T max_edge_length)
    {
        out_triangles.clear();

        const size_t n = vertices.size();
        if (n < 3)
            return false;

        // 如果只有3个顶点，检查边长约束后直接返回
        if (n == 3)
        {
            const glm::vec<2, T>& a = vertices[0];
            const glm::vec<2, T>& b = vertices[1];
            const glm::vec<2, T>& c = vertices[2];

            T edge1 = glm::length(b - a);
            T edge2 = glm::length(c - b);
            T edge3 = glm::length(a - c);

            // 检查所有边是否满足长度约束
            if (edge1 >= min_edge_length && edge1 <= max_edge_length &&
                edge2 >= min_edge_length && edge2 <= max_edge_length &&
                edge3 >= min_edge_length && edge3 <= max_edge_length)
            {
                out_triangles = {0, 1, 2};
                return true;
            }
            return false;  // 不满足边长约束
        }

        // 创建顶点索引数组
        std::vector<size_t> indices(n);
        for (size_t i = 0; i < n; ++i)
            indices[i] = i;

        // 确保顶点按逆时针顺序排列
        std::vector<glm::vec<2, T>> ordered_vertices = vertices;
        if (!IsPolygon2DCCW(ordered_vertices))
        {
            // 如果是顺时针，反转顶点顺序
            std::reverse(ordered_vertices.begin(), ordered_vertices.end());
            for (size_t i = 0; i < n; ++i)
                indices[i] = n - 1 - i;
        }

        // 耳切法主循环
        size_t remaining = n;
        size_t iterations = 0;
        // 最坏情况下需要 O(n²) 次迭代才能找到所有耳朵
        // 设置上限防止无限循环（通常因为输入多边形无效或边长约束过严）
        const size_t max_iterations = n * n;

        while (remaining > 3 && iterations < max_iterations)
        {
            bool ear_found = false;

            for (size_t i = 0; i < remaining; ++i)
            {
                if (IsEar(ordered_vertices, i, indices, min_edge_length, max_edge_length))
                {
                    // 找到满足边长约束的耳朵，添加三角形
                    const size_t prev = (i + remaining - 1) % remaining;
                    const size_t next = (i + 1) % remaining;

                    // 添加三角形索引（使用原始顶点索引）
                    out_triangles.push_back(indices[prev]);
                    out_triangles.push_back(indices[i]);
                    out_triangles.push_back(indices[next]);

                    // 移除当前顶点
                    indices.erase(indices.begin() + i);
                    remaining--;
                    ear_found = true;
                    break;
                }
            }

            if (!ear_found)
            {
                // 没有找到满足边长约束的耳朵
                return false;
            }

            iterations++;
        }

        // 添加最后剩余的三角形（同样需要检查边长约束）
        if (remaining == 3)
        {
            const glm::vec<2, T>& a = ordered_vertices[indices[0]];
            const glm::vec<2, T>& b = ordered_vertices[indices[1]];
            const glm::vec<2, T>& c = ordered_vertices[indices[2]];

            T edge1 = glm::length(b - a);
            T edge2 = glm::length(c - b);
            T edge3 = glm::length(a - c);

            // 检查最后一个三角形是否满足边长约束
            if (edge1 >= min_edge_length && edge1 <= max_edge_length &&
                edge2 >= min_edge_length && edge2 <= max_edge_length &&
                edge3 >= min_edge_length && edge3 <= max_edge_length)
            {
                out_triangles.push_back(indices[0]);
                out_triangles.push_back(indices[1]);
                out_triangles.push_back(indices[2]);
                return true;
            }
            return false;  // 最后一个三角形不满足边长约束
        }

        return true;
    }

    /**
     * @brief 2D多边形类
     *
     * 表示2D平面上的多边形，支持三角剖分功能。
     *
     * 主要功能：
     * - 顶点管理：添加、删除、获取顶点
     * - 几何计算：面积、周长
     * - 三角剖分：将多边形分解为三角形
     * - 拓扑验证：检查多边形是否有效
     *
     * @tparam T 坐标类型（float, double等）
     */
    template<typename T>
    class Polygon2D
    {
    private:
        std::vector<glm::vec<2, T>> vertices_;

    public:
        /**
         * @brief 默认构造函数
         */
        Polygon2D() = default;

        /**
         * @brief 从顶点数组构造多边形
         */
        explicit Polygon2D(const std::vector<glm::vec<2, T>>& vertices)
            : vertices_(vertices)
        {
        }

        /**
         * @brief 从顶点列表构造多边形
         */
        Polygon2D(std::initializer_list<glm::vec<2, T>> vertices)
            : vertices_(vertices)
        {
        }

        /**
         * @brief 添加顶点
         */
        void AddVertex(const glm::vec<2, T>& vertex)
        {
            vertices_.push_back(vertex);
        }

        /**
         * @brief 清空所有顶点
         */
        void Clear()
        {
            vertices_.clear();
        }

        /**
         * @brief 获取顶点数量
         */
        size_t GetVertexCount() const
        {
            return vertices_.size();
        }

        /**
         * @brief 获取顶点数组
         */
        const std::vector<glm::vec<2, T>>& GetVertices() const
        {
            return vertices_;
        }

        /**
         * @brief 设置顶点数组
         */
        void SetVertices(const std::vector<glm::vec<2, T>>& vertices)
        {
            vertices_ = vertices;
        }

        /**
         * @brief 获取指定索引的顶点
         */
        const glm::vec<2, T>& GetVertex(size_t index) const
        {
            return vertices_[index];
        }

        /**
         * @brief 设置指定索引的顶点
         */
        void SetVertex(size_t index, const glm::vec<2, T>& vertex)
        {
            if (index < vertices_.size())
                vertices_[index] = vertex;
        }

        /**
         * @brief 计算多边形面积
         */
        T Area() const
        {
            return Polygon2DArea(vertices_);
        }

        /**
         * @brief 判断顶点顺序是否为逆时针
         */
        bool IsCCW() const
        {
            return IsPolygon2DCCW(vertices_);
        }

        /**
         * @brief 反转顶点顺序
         */
        void Reverse()
        {
            std::reverse(vertices_.begin(), vertices_.end());
        }

        /**
         * @brief 对多边形进行三角剖分
         *
         * 将多边形分解为多个三角形。
         *
         * @param out_triangles 输出三角形索引数组，每3个索引构成一个三角形
         * @return true表示成功，false表示失败
         */
        bool Triangulate(std::vector<size_t>& out_triangles) const
        {
            return TriangulatePolygon2D(vertices_, out_triangles);
        }

        /**
         * @brief 对多边形进行三角剖分，返回Triangle2对象数组
         *
         * @param out_triangles 输出Triangle2对象数组
         * @return true表示成功，false表示失败
         */
        bool Triangulate(std::vector<Triangle2<T>>& out_triangles) const
        {
            std::vector<size_t> indices;
            if (!TriangulatePolygon2D(vertices_, indices))
                return false;

            out_triangles.clear();
            out_triangles.reserve(indices.size() / 3);

            for (size_t i = 0; i < indices.size(); i += 3)
            {
                out_triangles.emplace_back(
                    vertices_[indices[i]],
                    vertices_[indices[i + 1]],
                    vertices_[indices[i + 2]]
                );
            }

            return true;
        }

        /**
         * @brief 对多边形进行三角剖分（带边长约束版本）
         *
         * 将多边形分解为多个三角形，同时满足边长约束。
         * 所有生成的三角形的边长必须在指定范围内。
         *
         * @param out_triangles 输出三角形索引数组，每3个索引构成一个三角形
         * @param min_edge_length 三角形最小边长约束（默认1.0，最小1像素点）
         * @param max_edge_length 三角形最大边长约束（默认无限大）
         * @return true表示成功，false表示失败（输入无效或无法满足边长约束）
         */
        bool Triangulate(std::vector<size_t>& out_triangles,
                        T min_edge_length,
                        T max_edge_length = std::numeric_limits<T>::max()) const
        {
            return TriangulatePolygon2D(vertices_, out_triangles, min_edge_length, max_edge_length);
        }

        /**
         * @brief 对多边形进行三角剖分，返回Triangle2对象数组（带边长约束版本）
         *
         * 将多边形分解为多个三角形，同时满足边长约束。
         * 所有生成的三角形的边长必须在指定范围内。
         *
         * @param out_triangles 输出Triangle2对象数组
         * @param min_edge_length 三角形最小边长约束（默认1.0，最小1像素点）
         * @param max_edge_length 三角形最大边长约束（默认无限大）
         * @return true表示成功，false表示失败（输入无效或无法满足边长约束）
         */
        bool Triangulate(std::vector<Triangle2<T>>& out_triangles,
                        T min_edge_length,
                        T max_edge_length = std::numeric_limits<T>::max()) const
        {
            std::vector<size_t> indices;
            if (!TriangulatePolygon2D(vertices_, indices, min_edge_length, max_edge_length))
                return false;

            out_triangles.clear();
            out_triangles.reserve(indices.size() / 3);

            for (size_t i = 0; i < indices.size(); i += 3)
            {
                out_triangles.emplace_back(
                    vertices_[indices[i]],
                    vertices_[indices[i + 1]],
                    vertices_[indices[i + 2]]
                );
            }

            return true;
        }

        /**
         * @brief 获取顶点（支持数组访问）
         */
        glm::vec<2, T>& operator[](size_t index)
        {
            return vertices_[index];
        }

        /**
         * @brief 获取顶点（支持数组访问，const版本）
         */
        const glm::vec<2, T>& operator[](size_t index) const
        {
            return vertices_[index];
        }
    };

    // 常用类型别名
    using Polygon2Df = Polygon2D<float>;    // 单精度浮点多边形（最常用）
    using Polygon2Dd = Polygon2D<double>;   // 双精度浮点多边形（高精度计算）

}//namespace hgl::math
