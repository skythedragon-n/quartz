//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#pragma once
#include <vector>

#include "./drawing_utils.hpp"
#include "./id_sys.hpp"

namespace quartz::core {
    /**
     * @class Stroke
     * @brief Represents a stroke with various attributes.
     *
     * @authors Skythedragon
     *
     * The Stroke class is designed to encapsulate the details of a stroke in the user's animation.
     * It has various properties for things such as thickness, color, and corner type.
     *
     * This class provides methods to configure and retrieve stroke attributes. It gets the points
     * from the parent drawing, such that when you edit a point in multiple strokes, all of them
     * will be automatically affected without overcomplicated point linking logic.
     */
    class Stroke {
        AnimFile* file_;
        ::std::vector<BezierSection> points_;
        num_t thickness_;
        Color color_;
        CornerType corner_type_;
        num_t miter_limit_;
        DrawingId drawing_;

    public:
        Stroke(
            AnimFile* file,
            DrawingId drawing,
            num_t thickness,
            Color color,
            CornerType corner_type = CornerType::Round,
            num_t miter_limit = 10.0
        );

        /**
         * The type that most methods involving indexing strokes return.
         */
        using item_ref_t = ::std::tuple<Point&, const Point&, Point&>;
        /**
         * Decay that into a tuple of values instead
         */
        using item_val_t = ::std::tuple<Point, Point, Point>;

        /**
         * Iterator into Stroke's data
         *
         * I'm not documenting the rest of this cosmic horror
         *
         * <a href=https://en.cppreference.com/w/cpp/iterator/random_access_iterator.html>Look at this IG</a>
         */
        class Iterator {
            size_t index_;
            Stroke* stroke_;
            Drawing* drawing_;

            Iterator(size_t index, Stroke* stroke, Drawing* drawing) noexcept;

        public:
            using value_type = item_val_t;
            using reference = item_ref_t;
            using pointer = ::std::tuple<Point*, const Point*, Point*>;
            using difference_type = ::std::ptrdiff_t;
            using iterator_category = ::std::random_access_iterator_tag;

            constexpr Iterator() noexcept;
            Iterator(Stroke* stroke, size_t index);

            constexpr Iterator(const Iterator&) = default;
            Iterator& operator=(const Iterator&) = default;

            constexpr reference operator*() const;
            constexpr pointer operator->() const;
            constexpr reference operator[](difference_type n) const;

            constexpr Iterator& operator++();
            constexpr Iterator operator++(int);
            constexpr Iterator& operator--();
            constexpr Iterator operator--(int);

            constexpr Iterator& operator+=(difference_type n);
            constexpr Iterator operator+(difference_type n) const;
            constexpr Iterator& operator-=(difference_type n);
            constexpr Iterator operator-(difference_type n) const;

            constexpr difference_type operator-(const Iterator& other) const;

            constexpr bool operator==(const Iterator& other) const;
            constexpr bool operator!=(const Iterator& other) const;
            constexpr bool operator>(const Iterator& other) const;
            constexpr bool operator<(const Iterator& other) const;
            constexpr bool operator>=(const Iterator& other) const;
            constexpr bool operator<=(const Iterator& other) const;

            friend class Stroke;
        };

        /**
         * @brief Gives the size of the point list
         * @return Size points list
         */
        [[nodiscard]] size_t points_size() const;
        /**
         * @brief Add a point to the stroke
         * @param point_index The index of the point in the owning Drawing
         */
        void add_point(const size_t& point_index);
        /**
         * @brief Set the value of a tangent
         * @param direction Direction of the tangent
         * @param target Target point
         * @param value The thing to set the tangent to
         */
        void set_tangent(TagentDirection direction, size_t target, const Point& value);
        /**
         * @brief Reset the value of the tangent
         * @param direction Direction of the tanget relative to the index point
         * @param index Index of the point
         */
        void remove_tangent(TagentDirection direction, size_t index);
        /**
         * @brief Add a whole BezierSection all at once
         * @param section Section to add
         */
        void add_section(const BezierSection& section);
        /**
         * @brief Chops off the end of the stroke and returns the chopped section
         * @param index The index at which to chop (chops including this index)
         * @return The bit you chopped off
         */
        [[nodiscard]] Stroke& chop(size_t index);

        /**
         * @brief Indexes into data, and returns item at index. Couldn't be simpler
         * @param index Index at which to index to
         * @return Item at index
         */
        item_ref_t operator[](size_t index) const;

        /**
         * @brief Gives iterator to first item
         * @return Iterator to first item
         */
        Iterator begin();
        /**
         * @brief Gives iterator to after last item
         * @return Iterator to after last item
         */
        Iterator end();

        /**
         * @brief Gives thickness of stroke
         * @return Thickness of stroke
         */
        [[nodiscard]] num_t thickness() const {
            return thickness_;
        }

        /**
         * @brief Gives color of stroke
         * @return Color of stroke
         */
        [[nodiscard]] Color color() const {
            return color_;
        }

        /**
         * @brief Gives the type of corner the stroke uses
         * @return Stroke's corner type
         */
        [[nodiscard]] CornerType corner_type() const {
            return corner_type_;
        }

        /**
         * @brief Gives stroke's miter imit
         * @return The Stroke's miter limit
         */
        [[nodiscard]] num_t miter_limit() const {
            return miter_limit_;
        }

        /**
         * @brief Gives Id of Stroke's containing drawing
         * @return Id of containing drawing, because yeah
         */
        [[nodiscard]] DrawingId drawing() const {
            return drawing_;
        }

        /**
         * @brief Sets thickness of stroke
         * @param thickness New thickness of stroke (in pixels)
         */
        void set_thickness(num_t thickness) {
            thickness_ = thickness;
        }

        /**
         * @brief Sets color of stroke
         * @param color New color of Stroke
         */
        void set_color(const Color& color) {
            color_ = color;
        }

        /**
         * @brief Sets Stroke's corner type
         * @param corner_type New corner type for stroke
         */
        void set_corner_type(CornerType corner_type) {
            corner_type_ = corner_type;
        }

        /**
         * @brief Set Stroke's miter limit
         * @param miter_limit New miter limit
         */
        void set_miter_limit(num_t miter_limit) {
            miter_limit_ = miter_limit;
        }
    };

    inline Stroke::Iterator operator+(Stroke::Iterator::difference_type a, const Stroke::Iterator& b) {
        return b + a;
    }

    static_assert(::std::random_access_iterator<Stroke::Iterator>);
}
