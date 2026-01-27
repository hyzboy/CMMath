/**
 * DistanceQuery.cpp - Implementation of distance calculations
 *
 * Implements distance measurements between geometry primitives.
 */
#include<hgl/math/geometry/queries/DistanceQuery.h>
#include<hgl/math/MathUtils.h>

namespace hgl::math
{
    //=============================================================================
    // Point-to-geometry distance
    //=============================================================================

    float DistanceQuery::Distance(const Vector3f& point, const Sphere& sphere)
    {
        return sphere.DistanceToPoint(point);
    }

    float DistanceQuery::Distance(const Vector3f& point, const Capsule& capsule)
    {
        return capsule.DistanceToPoint(point);
    }

    float DistanceQuery::Distance(const Vector3f& point, const AABB& box)
    {
        Vector3f closest = box.ClampPoint(point);
        return Length(point - closest);
    }

    float DistanceQuery::Distance(const Vector3f& point, const Cylinder& cylinder)
    {
        return cylinder.DistanceToPoint(point);
    }

    //=============================================================================
    // Geometry-to-geometry distance
    //=============================================================================

    float DistanceQuery::Distance(const Sphere& a, const Sphere& b)
    {
        float centerDist = Length(a.GetCenter() - b.GetCenter());
        float surfaceDist = centerDist - a.GetRadius() - b.GetRadius();
        return surfaceDist > 0.0f ? surfaceDist : 0.0f;
    }

    float DistanceQuery::Distance(const Sphere& sphere, const Capsule& capsule)
    {
        // Find closest point on capsule's line segment to sphere center
        Vector3f closestOnCapsule = capsule.ClosestPoint(sphere.GetCenter());
        float dist = Length(sphere.GetCenter() - closestOnCapsule) - sphere.GetRadius();
        return dist > 0.0f ? dist : 0.0f;
    }

    float DistanceQuery::Distance(const Capsule& a, const Capsule& b)
    {
        // Find closest points on both line segments
        auto result = ClosestPoints(a, b);
        return result.distance > 0.0f ? result.distance : 0.0f;
    }

    float DistanceQuery::Distance(const Cylinder& a, const Cylinder& b)
    {
        // Simplified: use center distance as approximation
        float centerDist = Length(a.GetCenter() - b.GetCenter());
        float approxDist = centerDist - a.GetRadius() - b.GetRadius();
        return approxDist > 0.0f ? approxDist : 0.0f;
    }

    //=============================================================================
    // Closest point pairs
    //=============================================================================

    ClosestPointsResult DistanceQuery::ClosestPoints(const Capsule& a, const Capsule& b)
    {
        ClosestPointsResult result = ClosestPointsOnLineSegments(
            a.GetStart(), a.GetEnd(),
            b.GetStart(), b.GetEnd()
        );

        // Expand from line segments to capsule surfaces
        Vector3f dir = result.pointOnB - result.pointOnA;
        float dist = Length(dir);

        if (dist > 0.0001f)
        {
            dir = dir / dist;
            result.pointOnA = result.pointOnA + dir * a.GetRadius();
            result.pointOnB = result.pointOnB - dir * b.GetRadius();
            result.distance = dist - a.GetRadius() - b.GetRadius();
            if (result.distance < 0.0f)
                result.distance = 0.0f;
        }
        else
        {
            // Capsules overlap at their axes
            result.distance = 0.0f;
        }

        return result;
    }

    ClosestPointsResult DistanceQuery::ClosestPoints(const Sphere& sphere, const Capsule& capsule)
    {
        ClosestPointsResult result;

        // Find closest point on capsule to sphere center
        Vector3f closestOnCapsule = capsule.ClosestPoint(sphere.GetCenter());

        Vector3f dir = closestOnCapsule - sphere.GetCenter();
        float dist = Length(dir);

        if (dist > 0.0001f)
        {
            dir = dir / dist;
            result.pointOnA = sphere.GetCenter() + dir * sphere.GetRadius();
            result.pointOnB = closestOnCapsule;
            result.distance = dist - sphere.GetRadius();
            if (result.distance < 0.0f)
                result.distance = 0.0f;
        }
        else
        {
            result.pointOnA = sphere.GetCenter();
            result.pointOnB = closestOnCapsule;
            result.distance = 0.0f;
        }

        return result;
    }

    //=============================================================================
    // Helper functions
    //=============================================================================

    Vector3f DistanceQuery::ClosestPointOnLineSegment(const Vector3f& point,
                                                      const Vector3f& segmentStart,
                                                      const Vector3f& segmentEnd)
    {
        Vector3f segment = segmentEnd - segmentStart;
        Vector3f toPoint = point - segmentStart;

        float t = Dot(toPoint, segment) / Dot(segment, segment);
        t = clamp(t, 0.0f, 1.0f);

        return segmentStart + segment * t;
    }

    ClosestPointsResult DistanceQuery::ClosestPointsOnLineSegments(
        const Vector3f& seg1Start, const Vector3f& seg1End,
        const Vector3f& seg2Start, const Vector3f& seg2End)
    {
        ClosestPointsResult result;

        Vector3f d1 = seg1End - seg1Start;
        Vector3f d2 = seg2End - seg2Start;
        Vector3f r = seg1Start - seg2Start;

        float a = Dot(d1, d1);
        float e = Dot(d2, d2);
        float f = Dot(d2, r);

        float s, t;

        // Check if either or both segments degenerate into points
        if (a <= 0.0001f && e <= 0.0001f)
        {
            // Both segments are points
            s = t = 0.0f;
            result.pointOnA = seg1Start;
            result.pointOnB = seg2Start;
            result.distance = Length(seg1Start - seg2Start);
            return result;
        }

        if (a <= 0.0001f)
        {
            // First segment is a point
            s = 0.0f;
            t = clamp(f / e, 0.0f, 1.0f);
        }
        else
        {
            float c = Dot(d1, r);
            if (e <= 0.0001f)
            {
                // Second segment is a point
                t = 0.0f;
                s = clamp(-c / a, 0.0f, 1.0f);
            }
            else
            {
                // General case
                float b = Dot(d1, d2);
                float denom = a * e - b * b;

                if (denom != 0.0f)
                {
                    s = clamp((b * f - c * e) / denom, 0.0f, 1.0f);
                }
                else
                {
                    s = 0.0f;
                }

                t = (b * s + f) / e;

                if (t < 0.0f)
                {
                    t = 0.0f;
                    s = clamp(-c / a, 0.0f, 1.0f);
                }
                else if (t > 1.0f)
                {
                    t = 1.0f;
                    s = clamp((b - c) / a, 0.0f, 1.0f);
                }
            }
        }

        result.pointOnA = seg1Start + d1 * s;
        result.pointOnB = seg2Start + d2 * t;
        result.distance = Length(result.pointOnA - result.pointOnB);

        return result;
    }

}//namespace hgl::math
