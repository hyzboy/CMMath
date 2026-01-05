#include <hgl/math/geometry/HeightMapContour.h>
#include <glm/glm.hpp>

namespace hgl::math
{
    // Helper: 点到线段的距离平方
    static float PointLineDistSq(const Vector2f& p, const Vector2f& a, const Vector2f& b)
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
    }

    void SimplifyPolygonDP(const std::vector<Vector2f>& vertices,
                           float epsilon,
                           std::vector<Vector2f>& out_vertices)
    {
        if (vertices.size() < 3)
        {
            out_vertices = vertices;
            return;
        }

        std::function<void(int, int, std::vector<bool>&)> dpRecursive;
        dpRecursive = [&](int start, int end, std::vector<bool>& keep) -> void
        {
            if (end - start < 2)
                return;

            float max_dist_sq = 0.0f;
            int max_index = start;

            for (int i = start + 1; i < end; ++i)
            {
                float dist_sq = PointLineDistSq(vertices[i], vertices[start], vertices[end]);
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

    void SmoothPolygonChaikin(const std::vector<Vector2f>& vertices,
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

                next.push_back(p0 * 0.75f + p1 * 0.25f);
                next.push_back(p0 * 0.25f + p1 * 0.75f);
            }

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

    void ProcessContours(std::vector<ContourPolygon>& contours, bool smooth, float simplify_epsilon)
    {
        for (auto& contour : contours)
        {
            // 简化
            if (simplify_epsilon > 0.0f && contour.vertices.size() > 2)
            {
                std::vector<Vector2f> simplified;
                SimplifyPolygonDP(contour.vertices, simplify_epsilon, simplified);
                contour.vertices = std::move(simplified);
            }

            // 平滑
            if (smooth && contour.vertices.size() > 2)
            {
                std::vector<Vector2f> smoothed;
                SmoothPolygonChaikin(contour.vertices, 2, contour.is_closed, smoothed);
                contour.vertices = std::move(smoothed);
            }
        }
    }

}//namespace hgl::math
