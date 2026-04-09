//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#include "./Stroke.hpp"

#include <ranges>
#include <qtil/panic.hpp>

#include "./Drawing.hpp"
#include "./AnimFile.hpp"

namespace quartz::core {
    Stroke::Stroke(
        AnimFile* file,
        DrawingId drawing,
        num_t thickness,
        Color color,
        CornerType corner_type,
        num_t miter_limit
    ) :
        file_(file),
        thickness_(thickness),
        color_(color),
        corner_type_(corner_type),
        miter_limit_(miter_limit),
        drawing_(drawing)
    {}

    Stroke::Iterator::Iterator(size_t index, Stroke* stroke, Drawing* drawing) noexcept :
        index_(index),
        stroke_(stroke),
        drawing_(drawing)
    {}

    constexpr Stroke::Iterator::Iterator() noexcept :
        index_(::std::numeric_limits<size_t>::max()),
        stroke_(nullptr),
        drawing_(nullptr)
    {}

    Stroke::Iterator::Iterator(Stroke* stroke, size_t index) : index_(index) {
        stroke_ = stroke;
        drawing_ = stroke->file_->drawings.resolve(stroke->drawing_).or_else([] {
            ::qtil::panic("Drawing of stroke has been deleted? Really, well-behaving code should NOT do this!");
        });
    }

    constexpr Stroke::Iterator::reference Stroke::Iterator::operator*() const {
        BezierSection section = stroke_->points_[index_];

        const Point& middle = (*drawing_)[section.start];

        return reference{section.lastwise_tangent, middle, section.lastwise_tangent};
    }

    constexpr Stroke::Iterator::pointer Stroke::Iterator::operator->() const {
        BezierSection section = stroke_->points_[index_];

        const Point& middle = (*drawing_)[section.start];

        return pointer{&section.lastwise_tangent, &middle, &section.lastwise_tangent};
    }

    constexpr Stroke::Iterator::reference Stroke::Iterator::operator[](difference_type n) const {
        size_t index = index_ + n;
        BezierSection section = stroke_->points_[index];

        const Point& middle = (*drawing_)[section.start];

        return reference{section.lastwise_tangent, middle, section.lastwise_tangent};
    }

    constexpr Stroke::Iterator& Stroke::Iterator::operator++() {
        index_++;

        return *this;
    }

    constexpr Stroke::Iterator Stroke::Iterator::operator++(int) {
        Iterator tmp = *this;
        index_++;

        return tmp;
    }

    constexpr Stroke::Iterator& Stroke::Iterator::operator--() {
        index_--;

        return *this;
    }

    constexpr Stroke::Iterator Stroke::Iterator::operator--(int) {
        Iterator tmp = *this;
        index_--;

        return tmp;
    }

    constexpr Stroke::Iterator& Stroke::Iterator::operator+=(difference_type n) {
        index_ += n;

        return *this;
    }

    constexpr Stroke::Iterator Stroke::Iterator::operator+(difference_type n) const {
        return Iterator{index_ + n, stroke_, drawing_};
    }

    constexpr Stroke::Iterator& Stroke::Iterator::operator-=(difference_type n) {
        index_ -= n;

        return *this;
    }

    constexpr Stroke::Iterator Stroke::Iterator::operator-(difference_type n) const {
        return Iterator{index_ - n, stroke_, drawing_};
    }

    constexpr Stroke::Iterator::difference_type Stroke::Iterator::operator-(const Iterator& other) const {
        return std::bit_cast<difference_type>(index_ - other.index_);
    }

    constexpr bool Stroke::Iterator::operator==(const Iterator& other) const {
        return other.index_ == index_ && other.stroke_ == stroke_;
    }

    constexpr bool Stroke::Iterator::operator!=(const Iterator& other) const {
        return !(*this == other);
    }

    constexpr bool Stroke::Iterator::operator>(const Iterator& other) const {
        return index_ > other.index_;
    }

    constexpr bool Stroke::Iterator::operator<(const Iterator& other) const {
        return index_ < other.index_;
    }

    constexpr bool Stroke::Iterator::operator>=(const Iterator& other) const {
        return index_ >= other.index_;
    }

    constexpr bool Stroke::Iterator::operator<=(const Iterator& other) const {
        return index_ <= other.index_;
    }

    size_t Stroke::points_size() const {
        return points_.size();
    }

    void Stroke::add_point(const size_t& point_index) {
        points_.emplace_back(point_index, Point{0,0}, Point{0,0});
    }

    void Stroke::set_tangent(TagentDirection direction, size_t target, const Point& value) {
        BezierSection& section = points_[target];

        switch (direction) {
            case TagentDirection::Lastwise:
                section.lastwise_tangent = value;
                break;
            case TagentDirection::Nextwise:
                section.nextwise_tangent = value;
                break;
        }
    }

    void Stroke::remove_tangent(TagentDirection direction, size_t index) {
        BezierSection& section = points_[index];

        switch (direction) {
            case TagentDirection::Lastwise:
                section.lastwise_tangent = Point{0,0};
                break;
            case TagentDirection::Nextwise:
                section.nextwise_tangent = Point{0,0};
                break;
        }
    }

    void Stroke::add_section(const BezierSection& section) {
        points_.push_back(section);
    }

    Stroke& Stroke::chop(size_t index) {
        Stroke chopped{file_, drawing_, thickness_, color_, corner_type_, miter_limit_};

        for (BezierSection& section : points_ | ::std::views::drop(index)) {
            chopped.add_section(section);
        }

        points_.erase(points_.begin() + static_cast<decltype(points_)::difference_type>(index), points_.end());

        return chopped;
    }

    Stroke::item_ref_t Stroke::operator[](size_t index) const {
        BezierSection section = points_[index];

        const Drawing* drawing =  file_->drawings.resolve(drawing_).or_else([] {
            ::qtil::panic("Drawing of stroke has been deleted? Really, well-behaving code should NOT do this!");
        });

        return item_ref_t{section.lastwise_tangent, (*drawing)[section.start], section.lastwise_tangent};
    }

    Stroke::Iterator Stroke::begin() {
        return Iterator{this, 0};
    }

    Stroke::Iterator Stroke::end() {
        return Iterator{this, points_.size()};
    }
}
