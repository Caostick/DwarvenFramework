#pragma once

template <typename type>
bool LinesIntersection(
    const TVec2<type>& startA, const TVec2<type>& endA,
    const TVec2<type>& startB, const TVec2<type>& endB,
    TVec2<type>& result) {

    const TVec2<type> dir1 = endA - startA;
    const TVec2<type> dir2 = endB - startB;

    if(Vec2::Cross<type>(dir1, dir2) == type(0)) {
        return false;
    }

    const type a1 = -dir1.Y;
    const type b1 = +dir1.X;
    const type d1 = -(a1 * startA.X + b1 * startA.Y);

    const type a2 = -dir2.Y;
    const type b2 = +dir2.X;
    const type d2 = -(a2 * startB.X + b2 * startB.Y);

    const type seg1_line2_start = a2 * startA.X + b2 * startA.Y + d2;
    const type seg1_line2_end = a2 * endA.X + b2 * endA.Y + d2;

    const type seg2_line1_start = a1 * startB.X + b1 * startB.Y + d1;
    const type seg2_line1_end = a1 * endB.X + b1 * endB.Y + d1;

    const type u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
    result = startA + u * dir1;

    return true;
}

template <typename type>
bool SegmentsIntersection(
    const TVec2<type>& startA, const TVec2<type>& endA,
    const TVec2<type>& startB, const TVec2<type>& endB,
    TVec2<type>& out) {

    const TVec2<type> dir1 = endA - startA;
    const TVec2<type> dir2 = endB - startB;

    const type a1 = -dir1.Y;
    const type b1 = +dir1.X;
    const type d1 = -(a1 * startA.X + b1 * startA.Y);

    const type a2 = -dir2.Y;
    const type b2 = +dir2.X;
    const type d2 = -(a2 * startB.X + b2 * startB.Y);

    const type seg1_line2_start = a2 * startA.X + b2 * startA.Y + d2;
    const type seg1_line2_end = a2 * endA.X + b2 * endA.Y + d2;

    const type seg2_line1_start = a1 * startB.X + b1 * startB.Y + d1;
    const type seg2_line1_end = a1 * endB.X + b1 * endB.Y + d1;

    if(seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0) {
        return false;
    }

    const type u = seg1_line2_start / (seg1_line2_start - seg1_line2_end);
    out = startA + u * dir1;

    return true;
}

template <typename type>
auto ProjectPoint(
    const TVec2<type>& lineStart, const TVec2<type>& lineEnd,
    const TVec2<type>& point) -> TVec2<type> {

    const TVec2<type> vec = TVec2<type>::Normalized(lineEnd - lineStart);
    const TVec2<type> pRel = point - lineStart;
    const TVec2<type> pProj = vec * TVec2<type>::Dot(vec, pRel);

    return pProj + lineStart;
}

template <typename type>
auto NearbyPointOnSegment(
    const TVec2<type>& segmentStart, const TVec2<type>& segmentEnd,
    const TVec2<type>& point) -> TVec2<type> {

    const TVec2<type> p_v1 = point - segmentStart;
    const TVec2<type> p_v2 = point - segmentEnd;

    const TVec2<type> v1 = segmentEnd - segmentStart;
    const TVec2<type> v2 = segmentStart - segmentEnd;

    const type dot_v1 = TVec2<type>::Dot(p_v1, v1);
    if(dot_v1 < type(0)) {
        return segmentStart;
    }

    const type dot_v2 = TVec2<type>::Dot(p_v2, v2);
    if(dot_v2 < type(0)) {
        return segmentEnd;
    }

    return ProjectPoint<type>(segmentStart, segmentEnd, point);
}

template <typename type>
bool IsPointInsidePolygon(
    const TVec2<type>* vertices, int numVertices,
    const TVec2<type>& point) {

    int i, j, c = 0;
    for(i = 0, j = numVertices - 1; i < numVertices; j = i++) {
        if((((vertices[i].Y <= point.Y) && (point.Y < vertices[j].Y)) || ((vertices[j].Y <= point.Y) && (point.Y < vertices[i].Y))) && (point.X < (vertices[j].X - vertices[i].X) * (point.Y - vertices[i].Y) / (vertices[j].Y - vertices[i].Y) + vertices[i].X)) {
            c = !c;
        }
    }
    return c;
}

template <typename type>
auto DistanceToPolygon(
    const TVec2<type>* vertices, int numVertices,
    const TVec2<type>& point) -> type {

    if(numVertices == 1) {
        return TVec2<type>::Length(vertices[0] - point);
    }

    TVec2<type> nearby = NearbyPointOnSegment<type>(vertices[0], vertices[numVertices - 1], point);
    type result = TVec2<type>::Length(point - nearby);
    for(int i = 0; i < numVertices - 1; i++) {
        nearby = NearbyPointOnSegment<type>(vertices[i], vertices[i + 1], point);
        type dist = TVec2<type>::Length(point - nearby);
        if(result < dist) {
            result = dist;
        }
    }
    return result;
}

template <typename type>
auto NearestSegmentIndexToConvexPolygon(
    const TVec2<type>* vertices, int numVertices,
    const TVec2<type>& point) -> int {

    int result = 0;
    TVec2<type> nearby = NearbyPointOnSegment<type>(vertices[0], vertices[1], point);
    TVec2<type> segment_direction = TVec2<type>::Normalized(vertices[1] - vertices[0]);
    TVec2<type> segment_normal = TVec2<type>(segment_direction.Y, -segment_direction.X);
    type max_cos = TVec2<type>::Dot(TVec2<type>::Normalized(point - nearby), segment_normal);
    type min_dist = -max_cos * TVec2<type>::Length(point - nearby);
    for(int i = 0; i < numVertices; i++) {
        int next_point_idx = (i + 1) % numVertices;

        nearby = NearbyPointOnSegment<type>(vertices[i], vertices[next_point_idx], point);
        type dist = TVec2<type>::Length(point - nearby);
        segment_direction = TVec2<type>::Normalized(vertices[next_point_idx] - vertices[i]);
        segment_normal = TVec2<type>(segment_direction.Y, -segment_direction.X);
        type cur_cos = -TVec2<type>::Dot(TVec2<type>::Normalized(point - nearby), segment_normal);

        if(cur_cos * dist < min_dist) {
            min_dist = cur_cos * dist;
            result = i;
        }
    }

    return result;
}

template <typename type>
bool IsPolygonConvex(const TVec2<type>* vertices, int numVertices) {

    TVec2<type> prev = vertices[numVertices - 1];
    TVec2<type> cur, dir, n;
    long double d, dp;
    int sgn, s;

    for(int i = 0; i < numVertices; i++) {
        cur = vertices[i];
        dir = cur - prev;
        n = TVec2<type>(dir.Y, -dir.X);

        d = -TVec2<type>::Dot(n, cur);
        sgn = 0;
        for(int j = 0; j < numVertices; j++) {
            dp = d + TVec2<type>::Dot(n, vertices[j]);

            if(Abs<type>(dp) < type(0.01)) {
                continue;
            }

            s = dp > 0.0 ? 1 : -1;
            if(sgn == 0) {
                sgn = s;
            } else {
                if(sgn != s) {
                    return false;
                }
            }
        }
        prev = cur;
    }
    return true;
}

template <typename type>
bool IsPolygonClockwise(const TVec2<type>* vertices, int numVertices) {

    type sum = 0.0f;
    for(int i = 0, prev = numVertices - 1; i < numVertices; prev = i++) {
        sum += TVec2<type>::Cross(vertices[prev], vertices[i]);
    }
    return sum < 0.0f;
}

template <typename type>
bool IsPolygonIntersectsSelf(const TVec2<type>* vertices, int numVertices) {

    TVec2<type> intersection;
    for(int s1B = 0, s1A = numVertices - 1; s1B < numVertices; s1A = s1B++) {
        for(int i = s1B + 1; i < s1B + numVertices - 2; i++) {
            int s2A = i % numVertices;
            int s2B = (i + 1) % numVertices;

            if(SegmentsIntersection<type>(vertices[s1A], vertices[s1B], vertices[s2A], vertices[s2B], &intersection)) {
                return true;
            }
        }
    }

    return false;
}
