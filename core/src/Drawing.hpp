//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#pragma once

#include <vector>

#include <qtil/vector_util.hpp>

#include "./Stroke.hpp"

namespace quartz::core {

    class Drawing {
        ::std::vector<Stroke> strokes_;
        ::std::vector<Point> points_;
        AnimFile* file_;
        DrawingId id_;

    public:

        Drawing(IdKey, AnimFile*, DrawingId);

        /**
         * @brief Adds a new Stroke to the drawing
         * @param color Color of new stroke
         * @param thickness Thickness of new stroke
         * @param corner_type Type of corner of new stroke
         * @param miter_limit Stroke's miter limit
         * @return Reference to added stroke
         */
        Stroke& add_stroke(Color color, num_t thickness, CornerType corner_type = CornerType::Round, num_t miter_limit = 10.0);
        /**
         * @brief Gets stroke at index
         * @param index Index of stroke to be gotten
         * @return Stroke at index
         */
        Stroke& get_stroke(size_t index);

        /**
         * @brief Adds a new point to the stroke
         * @param point Point to be added
         */
        void add_point(const Point& point);

        /**
         * @brief Gets an iterable thing over all of the Points in the Drawing
         * @return Iterable thing
         */
        ::qtil::VecIterable<Point> points() { return ::qtil::VecIterable{points_}; }
        /**
         * @brief Gets an iterable thing over all of the Strokes in the Drawing
         * @return Iterable thing
         */
        ::qtil::VecIterable<Stroke> strokes() { return ::qtil::VecIterable{strokes_}; }

        /**
         * @brief Indexes point
         * @param index Index of point
         * @return Point at index
         */
        Point& operator[](size_t index);
        /**
         * @brief Indexes point, but const
         * @param index Index of point
         * @return Point at index
         */
        const Point& operator[](size_t index) const;
    };
}
