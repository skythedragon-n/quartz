//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 08 Mar 2026.
//

#pragma once

#include <expected>
#include <string>
#include <variant>
#include <quartz/core/AnimFile.hpp>
#include <quartz/core/id_sys.hpp>
#include <quartz/core/drawing_utils.hpp>

namespace quartz::lib::core_lib {
    enum class LayerOpFailure {
        SymbolWrongType,
        OutOfBoundsLayerIndex,
    };

    enum class FrameOpFailure {
        AlreadyKframe,
        OutOfBoundsOp,
    };

    // Move these one day, but don't delete these comments without SkyTheDragon's approval
    // rgb(255, 0, 156)
    // rgb(255, 0, 0)
    // rgb(242, 139, 0)
    // rgb(17, 202, 0)
    // rgb(0, 216, 169)
    // rgb(0, 183, 255)
    // rgb(94, 122, 255)
    // rgb(233, 51, 255)

    struct FrameSelection {
        core::AnimFile& file;
        core::FrameContainer<core::FrameId> container;
    };

    using LayerOpError = std::variant<core::ResolveFailure, LayerOpFailure>;

    using FrameOpError = ::std::variant<FrameOpFailure, core::ResolveFailure>;

    ::std::expected<core::AnimLayerId, LayerOpError>
    add_layer(core::AnimFile& file, core::SymbolId target, std::string name, core::Color color);

    ::std::expected<void, core::ResolveFailure>
    delete_layer(core::AnimFile& file, core::AnimLayerId target);

    ::std::expected<void, core::ResolveFailure>
    set_layer_name(core::AnimFile& file, core::AnimLayerId target, std::string name);

    ::std::expected<void, core::ResolveFailure>
    set_layer_color(core::AnimFile& file, core::AnimLayerId target, core::Color color);

    ::std::expected<void, LayerOpError>
    move_layer(core::AnimFile& file, core::SymbolId symbol, size_t source, size_t target);

    ::std::expected<core::FrameId, core::ResolveFailure>
    duplicate_frame(core::AnimFile& file, core::FrameId frame);

    ::std::expected<core::FrameId, FrameOpError>
    add_keyframe(core::AnimFile& file, const core::AnimLayerId& target, const core::Frame::FrameData& data, size_t index);

    ::std::expected<void, FrameOpError>
    move_frame(core::AnimFile& file, core::AnimLayerId target, size_t frame, size_t len, int64_t delta);

    ::std::expected<core::FatFrame, FrameOpError>
    get_frame(core::AnimFile& file, core::AnimLayerId target, size_t frame);

    ::std::expected<::std::vector<core::FatFrame>, FrameOpError>
    get_frames(core::AnimFile& file, core::AnimLayerId target, size_t frame, size_t count);
}
