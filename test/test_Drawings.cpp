//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 09 Apr 2026.
//
// Ok, I lied, I asked the AI to generate all of these tests because writing them myself would have been hell

#include <catch2/catch_test_macros.hpp>

#include <quartz/core/AnimFile.hpp>
#include <quartz/core/Drawing.hpp>
#include <quartz/core/Stroke.hpp>

namespace quartz::core {
    static bool point_eq(const Point& a, const Point& b) {
        return a.x == b.x && a.y == b.y;
    }

    static bool color_eq(const Color& a, const Color& b) {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }
}

TEST_CASE("Drawing can add and access points", "[Drawing]") {
    using namespace quartz;

    core::AnimFile file;
    core::DrawingId drawing_id = file.drawings.add();
    core::Drawing* drawing = *file.drawings.resolve(drawing_id);

    REQUIRE(drawing != nullptr);

    core::Point p0{1.0, 2.0};
    core::Point p1{-3.5, 4.25};

    drawing->add_point(p0);
    drawing->add_point(p1);

    REQUIRE(core::point_eq((*drawing)[0], p0));
    REQUIRE(core::point_eq((*drawing)[1], p1));

    const core::Drawing& const_ref = *drawing;
    REQUIRE(core::point_eq(const_ref[0], p0));
    REQUIRE(core::point_eq(const_ref[1], p1));
}

TEST_CASE("Drawing can add strokes", "[Drawing]") {
    using namespace quartz;

    core::AnimFile file;
    core::DrawingId drawing_id = file.drawings.add();
    core::Drawing* drawing = *file.drawings.resolve(drawing_id);

    REQUIRE(drawing != nullptr);

    core::Color color{10, 20, 30, 40};
    core::num_t thickness = 7.5;
    core::CornerType corner = core::CornerType::Miter;
    core::num_t miter_limit = 12.0;

    drawing->add_stroke(color, thickness, corner, miter_limit);

    REQUIRE(drawing->strokes().size() == 1);

    core::Stroke& stroke = drawing->get_stroke(0);

    REQUIRE(core::color_eq(stroke.color(), color));
    REQUIRE(stroke.thickness() == thickness);
    REQUIRE(stroke.corner_type() == corner);
    REQUIRE(stroke.miter_limit() == miter_limit);
    REQUIRE(stroke.drawing() == drawing_id);
}

TEST_CASE("Drawing supports multiple strokes and points", "[Drawing]") {
    using namespace quartz;

    core::AnimFile file;
    core::DrawingId drawing_id = file.drawings.add();
    core::Drawing* drawing = *file.drawings.resolve(drawing_id);

    REQUIRE(drawing != nullptr);

    drawing->add_point({0.0, 0.0});
    drawing->add_point({1.0, 1.0});
    drawing->add_point({2.0, 2.0});

    drawing->add_stroke({255, 0, 0, 255}, 1.0);
    drawing->add_stroke({0, 255, 0, 255}, 2.0, core::CornerType::Bevel, 3.0);

    REQUIRE(core::point_eq((*drawing)[0], {0.0, 0.0}));
    REQUIRE(core::point_eq((*drawing)[1], {1.0, 1.0}));
    REQUIRE(core::point_eq((*drawing)[2], {2.0, 2.0}));

    REQUIRE(drawing->strokes().size() == 2);
    REQUIRE(drawing->strokes().begin() != drawing->strokes().end());
}

TEST_CASE("Stroke initially has no points", "[Stroke]") {
    using namespace quartz;

    core::AnimFile file;
    core::DrawingId drawing_id = file.drawings.add();
    core::Drawing* drawing = *file.drawings.resolve(drawing_id);

    REQUIRE(drawing != nullptr);

    drawing->add_stroke({1, 2, 3, 4}, 5.0);
    core::Stroke& stroke = drawing->get_stroke(0);

    REQUIRE(stroke.points_size() == 0);
    REQUIRE(stroke.begin() == stroke.end());
}

TEST_CASE("Stroke can store point indices and expose iteration", "[Stroke]") {
    using namespace quartz;

    core::AnimFile file;
    core::DrawingId drawing_id = file.drawings.add();
    core::Drawing* drawing = *file.drawings.resolve(drawing_id);

    REQUIRE(drawing != nullptr);

    drawing->add_point({10.0, 20.0});
    drawing->add_point({30.0, 40.0});
    drawing->add_stroke({100, 110, 120, 130}, 2.0);

    core::Stroke& stroke = drawing->get_stroke(0);

    stroke.add_point(0);
    stroke.add_point(1);

    REQUIRE(stroke.points_size() == 2);

    auto it = stroke.begin();
    REQUIRE(it != stroke.end());
    ++it;
    REQUIRE(it != stroke.end());

    REQUIRE((stroke.end() - stroke.begin()) == 2);
}

TEST_CASE("Stroke thickness color corner and miter setters work", "[Stroke]") {
    using namespace quartz;

    core::AnimFile file;
    core::DrawingId drawing_id = file.drawings.add();
    core::Drawing* drawing = *file.drawings.resolve(drawing_id);

    REQUIRE(drawing != nullptr);

    drawing->add_stroke({1, 1, 1, 255}, 1.0);
    core::Stroke& stroke = drawing->get_stroke(0);

    stroke.set_thickness(9.0);
    stroke.set_color({9, 8, 7, 6});
    stroke.set_corner_type(core::CornerType::Bevel);
    stroke.set_miter_limit(4.5);

    REQUIRE(stroke.thickness() == 9.0);
    REQUIRE(core::color_eq(stroke.color(), {9, 8, 7, 6}));
    REQUIRE(stroke.corner_type() == core::CornerType::Bevel);
    REQUIRE(stroke.miter_limit() == 4.5);
}

TEST_CASE("Stroke can add sections and chop", "[Stroke]") {
    using namespace quartz;

    core::AnimFile file;
    core::DrawingId drawing_id = file.drawings.add();
    core::Drawing* drawing = *file.drawings.resolve(drawing_id);

    REQUIRE(drawing != nullptr);

    drawing->add_point({0.0, 0.0});
    drawing->add_point({1.0, 1.0});
    drawing->add_point({2.0, 2.0});
    drawing->add_stroke({255, 255, 255, 255}, 1.0);

    core::Stroke& stroke = drawing->get_stroke(0);

    core::BezierSection s0;
    s0.start = 0;
    s0.lastwise_tangent = {0.5, 0.5};
    s0.nextwise_tangent = {0.25, 0.25};

    core::BezierSection s1;
    s1.start = 1;
    s1.lastwise_tangent = {1.5, 1.5};
    s1.nextwise_tangent = {1.25, 1.25};

    stroke.add_section(s0);
    stroke.add_section(s1);

    REQUIRE(stroke.points_size() == 2);

    core::Stroke chopped = stroke.chop(1);
    REQUIRE(stroke.points_size() == 1);
    REQUIRE(chopped.points_size() == 1);
    REQUIRE(chopped.drawing() == drawing_id);
}

TEST_CASE("Drawing and stroke iterables are usable in range-for", "[Drawing][Stroke]") {
    using namespace quartz;

    core::AnimFile file;
    core::DrawingId drawing_id = file.drawings.add();
    core::Drawing* drawing = *file.drawings.resolve(drawing_id);

    REQUIRE(drawing != nullptr);

    drawing->add_point({1.0, 2.0});
    drawing->add_point({3.0, 4.0});
    drawing->add_stroke({10, 20, 30, 255}, 2.0);
    drawing->add_stroke({40, 50, 60, 255}, 3.0);

    size_t point_count = 0;
    for (const auto& p : drawing->points()) {
        (void)p;
        ++point_count;
    }
    REQUIRE(point_count == 2);

    size_t stroke_count = 0;
    for (const auto& s : drawing->strokes()) {
        (void)s;
        ++stroke_count;
    }
    REQUIRE(stroke_count == 2);
}
