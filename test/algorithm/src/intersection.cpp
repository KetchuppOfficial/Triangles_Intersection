#include <gtest/gtest.h>

#include "point_segment.hpp"
#include "point_triangle.hpp"
#include "segment_segment.hpp"
#include "segment_triangle.hpp"
#include "triangle_triangle.hpp"

using namespace yLab::geometry;

TEST(Intersection, Point_Segment)
{
    Point_3D pt{0.0};

    // point doesn't belong to the line containing segment
    Segment seg_1{Point_3D{1.0, 0.0, 0.0}, Point_3D{0.0, -1.0, 0.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (pt, seg_1));

    // point belongs to the line containing segment but not to segment itself
    Segment seg_2{Point_3D{1.0, 1.0, 0.0}, Point_3D{3.0, 3.0, 0.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (pt, seg_2));

    // point coincides with one end of segment
    Segment seg_3{Point_3D{0.0}, Point_3D{12.0, -14.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (pt, seg_3));

    // point belongs to the interior of segment
    Segment seg_4{Point_3D{-6.0, -6.0, 0.0}, Point_3D{3.0, 3.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (pt, seg_4));
}

TEST (Intersection, Point_Triangle)
{
    Point_3D pt{0.0};

    // point doesn't belong to the plane of triangle
    Triangle tr_1{Point_3D{1.0, 0.0, 0.0}, Point_3D{0.0, 1.0, 0.0}, Point_3D{1.0, 1.0, 5.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (pt, tr_1));

    // point belongs to the plane of triangle but doesn't belong to triangle itself
    Triangle tr_2{Point_3D{1.0, 0.0, 0.0}, Point_3D{0.0, 1.0, 0.0}, Point_3D{1.0, 1.0, 0.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (pt, tr_2));

    // point coincides with a vertex of triangle
    Triangle tr_3{Point_3D{0.0, 0.0, 0.0}, Point_3D{4.0, 12.0, 0.0}, Point_3D{-2.0, 7.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (pt, tr_3));

    // point belongs to a side of triangle
    Triangle tr_4{Point_3D{-1.0, -1.0, 0.0}, Point_3D{1.0, 1.0, 0.0}, Point_3D{1.0, -1.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (pt, tr_4));

    // point belongs to the interior of triangle
    Triangle tr_5{Point_3D{3.0, 3.0, 0.0}, Point_3D{-4.0, 4.0, 0.0}, Point_3D{0.0, -11.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (pt, tr_5));
}

TEST (Intersection, Segment_Segment)
{
    Segment seg {Point_3D{-1.0, 0.0, 0.0}, Point_3D{1.0, 0.0, 0.0}};

    // segments belong to crossing lines
    Segment seg_1{Point_3D{0.0, -1.0, 2.0}, Point_3D{0.0, 1.0, 2.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (seg, seg_1));

    // segments belong to intersecting lines
    // ===============================================================

    // one of segments contains the point at which the lines intersect
    // but segments don't intersect
    Segment seg_2{Point_3D{6.0, -5.0, 0.0}, Point_3D{0.0, 5.0, 0.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (seg, seg_2));

    // no segment contains the point at which the lines intersect
    Segment seg_3{Point_3D{3.0, 1.0, 0.0}, Point_3D{0.0, 6.0, 0.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (seg, seg_3));

    // one end of one segment coincide with one end of the other segment
    Segment seg_4{Point_3D{1.0, 0.0, 0.0}, Point_3D{1.0, 7.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (seg, seg_4));

    // one end of one segment belongs to the interior of the other segment
    Segment seg_5{Point_3D{0.5, 0.0, 0.0}, Point_3D{0.5, 9.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (seg, seg_5));

    // segments intersect at a point belonging to the interiors of both segments
    Segment seg_6{Point_3D{-0.4, 2.0, 0.0}, Point_3D{-0.4, -2.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (seg, seg_6));
    // ===============================================================

    // segments belong to parallel lines
    Segment seg_7{Point_3D{-6.0, -9.0, 0.0}, Point_3D{3.0, -9.0, 0.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (seg, seg_7));

    // segments belong to the same line
    // ===============================================================

    // segments don't intersect
    Segment seg_8{Point_3D{2.0, 0.0, 0.0}, Point_3D{3.0, 0.0, 0.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (seg, seg_8));

    // {P1   Q1}{P2    Q2}
    Segment seg_9{Point_3D{1.0, 0.0, 0.0}, Point_3D{4.0, 0.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (seg, seg_9));

    // {P1   Q1}
    //     {P2     Q2}
    Segment seg_10{Point_3D{0.0, 0.0, 0.0}, Point_3D{1.5, 0.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (seg, seg_10));

    // {P1           Q1}
    //     {P2   Q2}
    Segment seg_11{Point_3D{-7.0, 0.0, 0.0}, Point_3D{2.0, 0.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (seg, seg_11));

    // {P1   Q1}
    // {P2   Q2}
    EXPECT_TRUE (yLab::geometry::are_intersecting (seg, seg));

    // ===============================================================
}

TEST (Intersection, Segment_Triangle)
{
    Triangle tr{Point_3D{0.0, 1.0, 0.0}, Point_3D{1.0, 0.0, 0.0}, Point_3D{0.0, 0.0, 0.0}};
    
    // segment and triangle aren't coplanar
    // ================================================================

    // segment locates above the plane of triangle
    Segment seg_1{Point_3D{0.3, 0.3, 1.0}, Point_3D{0.3, 0.0, 2.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (tr, seg_1));

    // one end of segment belongs to the interior of triangle
    Segment seg_2{Point_3D{0.1, 0.2, 0.0}, Point_3D{4.0, -3.0, 2.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_2));

    // one end of segment coincides with a vertex of triangle
    Segment seg_3{Point_3D{0.0, 0.0, 0.0}, Point_3D{1.0, -13.0, -6.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_3));

    // one end of segment belongs to a side of triangle
    Segment seg_4{Point_3D{0.5, 0.0, 0.0}, Point_3D{4.0, 11.3, 0.7}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_4));

    // a vertex of triangle belongs to the interior of segment
    Segment seg_5{Point_3D{0.0, 0.0, -1.0}, Point_3D{0.0, 0.0, 1.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_5));

    // a side of triangle intersects with the interior of segment
    Segment seg_6{Point_3D{0.5, 0.0, -1.0}, Point_3D{0.5, 0.0, 1.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_6));

    // segment and triangle intersect by their interiors
    Segment seg_7{Point_3D{0.3, 0.3, -1.0}, Point_3D{0.3, 0.3, 1.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_7));

    // ================================================================

    // segment and triangle are coplanar
    // ================================================================

    // one end of segment belongs to the interior of triangle
    Segment seg_8{Point_3D{0.3, 0.3, 0.0}, Point_3D{2.0, 2.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_8));

    // one end of segment belongs to a side of triangle
    Segment seg_9{Point_3D{0.5, 0.0, 0.0}, Point_3D{0.5, -2.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_9));

    // one end of segment coincides with a vertex of triangle
    Segment seg_10{Point_3D{1.0, 0.0, 0.0}, Point_3D{2.0, 0.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_10));

    // seg.P_ belongs to R1, seg.Q_ belongs to R1
    Segment seg_11{Point_3D{1.0, 1.0, 0.0}, Point_3D{2.0, 2.0, 0.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (tr, seg_11));

    // seg.P belongs to R1, seg.Q_ belongs to RP-side of triangle
    Segment seg_12{Point_3D{1.0, 1.0, 0.0}, Point_3D{0.5, 0.5, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_12));

    // seg.P belongs to R1, seg.Q_ belongs to R12
    Segment seg_13{Point_3D{1.0, 1.0, 0.0}, Point_3D{1.5, -1.0, 0.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (tr, seg_13));

    // seg.P belongs to R1, seg.Q_ belongs to R13
    Segment seg_14{Point_3D{1.0, 1.0, 0.0}, Point_3D{-1.0, -1.0, 0.0}};
    EXPECT_TRUE (yLab::geometry::are_intersecting (tr, seg_14));

    // seg.P belongs to R1, seg.Q_ belongs to R14
    Segment seg_15{Point_3D{1.0, 1.0, 0.0}, Point_3D{-1.0, 1.5, 0.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (tr, seg_15));

    // ================================================================
}

TEST (Intersection, Triangle_Triangle)
{
    Triangle tr{Point_3D{0.0, 1.0, 0.0}, Point_3D{1.0, 0.0, 0.0}, Point_3D{0.0, 0.0, 0.0}};

    // triangles aren't coplanar
    // ================================================================

    // planes of triangles don't intersect
    Triangle tr_1{Point_3D{-1.0, 0.0, 1.0}, Point_3D{-1.0, 1.0, 1.0}, Point_3D{-1.0, 0.0, 2.0}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (tr, tr_1));

    // planes of triangles intersect but triangles themselves don't
    Triangle tr_2{Point_3D{-1.0, 0.0, -0.5}, Point_3D{-1.0, 1.0, -0.5}, Point_3D{-1.0, 0.0, 0.5}};
    EXPECT_FALSE (yLab::geometry::are_intersecting (tr, tr_2));
    EXPECT_FALSE (yLab::geometry::are_intersecting (tr_2, tr));

    // ================================================================
}
