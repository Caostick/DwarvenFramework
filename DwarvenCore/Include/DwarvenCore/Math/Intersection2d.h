#pragma once

/*
* Intersection of two lines, represented by two points
* Returns true, if intersection exists, false otherwise
* In case of success @result contains intersection point
*/
template <typename type>
bool LinesIntersection(const TVec2<type>& startA, const TVec2<type>& endA,const TVec2<type>& startB, const TVec2<type>& endB, TVec2<type>& result);

/*
* Intersection of two segments
* Returns true, if intersection exists, false otherwise
* In case of success @result contains intersection point
*/
template <typename type>
bool SegmentsIntersection(const TVec2<type>& startA, const TVec2<type>& endA, const TVec2<type>& startB, const TVec2<type>& endB, TVec2<type>& out);

/*
* Projects point onto line, represented by two points
* Returns projection point position
*/
template <typename type>
auto ProjectPoint(const TVec2<type>& lineStart, const TVec2<type>& lineEnd, const TVec2<type>& point)->TVec2<type>;

/*
* Finds nearby to given point on segment
*/
template <typename type>
auto NearbyPointOnSegment(const TVec2<type>& segmentStart, const TVec2<type>& segmentEnd, const TVec2<type>& point)->TVec2<type>;

/*
* Returns whether point inside polygon
*/
template <typename type>
bool IsPointInsidePolygon(const TVec2<type>* vertices, int numVertices, const TVec2<type>& point);

/*
* Returns distance between point and polygon
*/
template <typename type>
auto DistanceToPolygon(const TVec2<type>* vertices, int numVertices, const TVec2<type>& point) -> type;

template <typename type>
auto NearestSegmentIndexToConvexPolygon(const TVec2<type>* vertices, int numVertices, const TVec2<type>& point) -> int;

template <typename type>
bool IsPolygonConvex(const TVec2<type>* vertices, int numVertices);

template <typename type>
bool IsPolygonClockwise(const TVec2<type>* vertices, int numVertices);

template <typename type>
bool IsPolygonIntersectsSelf(const TVec2<type>* vertices, int numVertices);

#include <DwarvenCore/Math/Intersection2d.inl>