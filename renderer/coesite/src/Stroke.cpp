//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 14 Jul 2026.
//

#include "Stroke.hpp"

namespace quartz::renderer::coesite {
    Stroke::Stroke(
        IdKey key,
        CoesiteRoot* file,
        NodeId parent,
        core::Color color,
        core::num_t thickness,
        core::CornerType corner,
        core::num_t miter_limit,
        StrokeId self
    ) :
        color_(color),
        thickness_(thickness),
        corner_(corner),
        miter_limit_(miter_limit),
        parent_(parent)
    {}

    void Stroke::add_section(const core::CoesiteBezierSection& section) {
        sections_.push_back(section);
    }

    void Stroke::add_sections(const ::std::vector<core::CoesiteBezierSection>& sections) {
        sections_.insert(sections_.end(), sections.begin(), sections.end());
    }

    void Stroke::set_sections(const ::std::vector<core::CoesiteBezierSection>& sections) {
        sections_ = sections;
    }

    core::CoesiteBezierSection& Stroke::operator[](size_t index) {
        return sections_[index];
    }

    const core::CoesiteBezierSection& Stroke::operator[](size_t index) const {
        return sections_[index];
    }

    core::Color Stroke::color() const {
        return color_;
    }

    void Stroke::set_color(const core::Color& color) {
        color_ = color;
    }

    core::num_t Stroke::thickness() const {
        return thickness_;
    }

    void Stroke::set_thickness(core::num_t thickness) {
        thickness_ = thickness;
    }

    core::CornerType Stroke::corner() const {
        return corner_;
    }

    void Stroke::set_corner(core::CornerType corner) {
        corner_ = corner;
    }

    core::num_t Stroke::miter_limit() const {
        return miter_limit_;
    }

    void Stroke::set_miter_limit(core::num_t miter_limit) {
        miter_limit_ = miter_limit;
    }

    NodeId Stroke::parent() const {
        return parent_;
    }

    void Stroke::set_parent(const NodeId& parent) {
        parent_ = parent;
    }

    decltype(Stroke::sections_.begin()) Stroke::begin() {
        return sections_.begin();
    }

    decltype(Stroke::sections_.end()) Stroke::end() {
        return sections_.end();
    }

    decltype(Stroke::sections_.size()) Stroke::size() const {
        return sections_.size();
    }
}
