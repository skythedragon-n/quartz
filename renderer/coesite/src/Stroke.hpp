//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 14 Jul 2026.
//

#pragma once

#include "./id_sys.hpp"

#include <vector>
#include <quartz/core/drawing_utils.hpp>

namespace quartz::renderer::coesite {
    class Stroke {
        ::std::vector<core::CoesiteBezierSection> sections_;
        core::Color color_;
        core::num_t thickness_ = 0;
        core::CornerType corner_ = core::CornerType::Round;
        core::num_t miter_limit_ = 10.0;
        NodeId parent_ = NodeId::INVALID;

    public:
        Stroke(
            IdKey key,
            CoesiteRoot* file,
            NodeId parent,
            core::Color color,
            core::num_t thickness,
            core::CornerType corner,
            core::num_t miter_limit,
            StrokeId self
        );

        void add_section(const core::CoesiteBezierSection& section);
        void add_sections(const ::std::vector<core::CoesiteBezierSection>& sections);
        void set_sections(const ::std::vector<core::CoesiteBezierSection>& sections);

        core::CoesiteBezierSection& operator[](size_t index);
        const core::CoesiteBezierSection& operator[](size_t index) const;

        [[nodiscard]] core::Color color() const;
        [[nodiscard]] core::num_t thickness() const;
        [[nodiscard]] core::CornerType corner() const;
        [[nodiscard]] core::num_t miter_limit() const;
        [[nodiscard]] NodeId parent() const;

        void set_color(const core::Color& color);
        void set_thickness(core::num_t thickness);
        void set_corner(core::CornerType corner);
        void set_miter_limit(core::num_t miter_limit);
        void set_parent(const NodeId& parent);

        decltype(sections_.begin()) begin();
        decltype(sections_.end()) end();
        [[nodiscard]] decltype(sections_.size()) size() const;
    };
}
