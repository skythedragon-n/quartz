//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 08 Mar 2026.
//

#include "./monolayer_timeline_system.hpp"

#include <numeric>
#include <ranges>

#include <qtil/panic.hpp>
#include <qtil/vector_util.hpp>
#include <quartz/core/Symbol.hpp>
#include <quartz/core/AnimFile.hpp>
#include <quartz/core/AnimatedLayer.hpp>
#include <qtil/casts.hpp>

namespace quartz::lib::core_lib {
    ::std::expected<core::AnimLayerId, LayerOpError>
    add_layer(core::AnimFile& file, core::SymbolId target, std::string name, core::Color color) {
        auto sym = file.symbols.resolve(target);

        if (!sym) {
            return ::std::unexpected(sym.error());
        }

        core::Symbol* target_sym = *sym;

        if (!target_sym->is<core::symbol_types::LayeredAnimation>()) {
            return ::std::unexpected(LayerOpFailure::SymbolWrongType);
        }

        auto layers = *target_sym->get<core::symbol_types::LayeredAnimation>();

        core::AnimLayerId layer = file.layers.add(name, color, target);

        layers->layers.emplace_back(layer);

        return layer;
    }

    ::std::expected<void, core::ResolveFailure>
    delete_layer(core::AnimFile& file, core::AnimLayerId target) {
        auto layer_res = file.layers.resolve(target);

        if (!layer_res) {
            return ::std::unexpected(layer_res.error());
        }

        auto layer = layer_res.value();

        auto symbol_res = file.symbols.resolve(layer->parent());

        if (!symbol_res) {
            return ::std::unexpected(symbol_res.error());
        }

        core::Symbol* symbol = *symbol_res;

        if (!symbol->is<core::symbol_types::LayeredAnimation>()) {
            ::qtil::panic("Symbol's type does not match layer which claims it as parent!");
        }

        auto layers = *symbol->get<core::symbol_types::LayeredAnimation>();

        for (auto [i, id] : ::std::views::enumerate(layers->layers)) {
            if (id == target) {
                layers->layers.erase(::qtil::iter_at(layers->layers, i));
                file.layers.free(target);
                return {};
            }
        }

        ::qtil::panic("Layer who claims Symbol as parent not found in Symbol!");
    }

    ::std::expected<void, core::ResolveFailure>
    set_layer_name(core::AnimFile& file, core::AnimLayerId target, std::string name) {
        auto layer_res = file.layers.resolve(target);

        if (!layer_res) {
            return ::std::unexpected(layer_res.error());
        }

        auto layer = layer_res.value();

        layer->set_name(name);

        return {};
    }

    ::std::expected<void, core::ResolveFailure>
    set_layer_color(core::AnimFile& file, core::AnimLayerId target, core::Color color) {
        auto layer_res = file.layers.resolve(target);

        if (!layer_res) {
            return ::std::unexpected(layer_res.error());
        }

        core::AnimatedLayer* layer = layer_res.value();

        layer->set_color(color);

        return {};
    }

    ::std::expected<void, LayerOpError>
    move_layer(core::AnimFile& file, core::SymbolId symbol, size_t source, size_t target) {
        auto symbol_res = file.symbols.resolve(symbol);

        if (!symbol_res) {
            return ::std::unexpected(symbol_res.error());
        }

        core::Symbol* symbol_p = *symbol_res;

        if (!symbol_p->is<core::symbol_types::LayeredAnimation>()) {
            return ::std::unexpected(LayerOpFailure::SymbolWrongType);
        }

        if (source == target) {
            return {};
        }

        auto layers = *symbol_p->get<core::symbol_types::LayeredAnimation>();

        if (source >= layers->layers.size()) {
            return ::std::unexpected(LayerOpFailure::OutOfBoundsLayerIndex);
        }

        core::AnimLayerId layer = layers->layers[source];

        layers->layers.erase(layers->layers.begin() + source);

        if (target > source) {
            target--;
        }

        if (target >= layers->layers.size()) {
            return ::std::unexpected(LayerOpFailure::OutOfBoundsLayerIndex);
        }

        layers->layers.emplace(layers->layers.begin() + target, layer);

        return {};
    }

    ::std::expected<core::FrameId, core::ResolveFailure>
    duplicate_frame(core::AnimFile& file, core::FrameId frame) {
        auto frame_res = file.frames.resolve(frame);

        if (!frame_res) {
            return ::std::unexpected(frame_res.error());
        }

        core::Frame* frame_p = *frame_res;

        return file.frames.add(frame_p->data());
    }

    ::std::expected<core::FrameId, FrameOpError>
    add_keyframe(core::AnimFile& file, const core::AnimLayerId& target, const core::Frame::FrameData& data, size_t index) {
        auto layer_res = file.layers.resolve(target);

        if (!layer_res) {
            return ::std::unexpected(layer_res.error());
        }

        core::AnimatedLayer* layer = layer_res.value();

        if (layer->frames.size() == 0) {
            return ::std::unexpected(FrameOpFailure::OutOfBoundsOp);
        }

        if (layer->frames[index].from_first == 0) {
            return ::std::unexpected(FrameOpFailure::AlreadyKframe);
        }

        core::FrameId frame = file.frames.add(data);

        auto insert_res = layer->frames.insert_frame(index, frame);

        if (!insert_res) {
            if (index >= layer->frames.size()) {
                ::qtil::panic("FrameContainer misbehaving, insert at end returned error");
            }

            ::qtil::panic("FrameContainer misbehaving, normal insert returned error");
        }

        return frame;
    }

    ::std::expected<void, FrameOpError>
    move_frame(core::AnimFile& file, core::AnimLayerId target, size_t frame, size_t len, int64_t delta) {
        auto layer_res = file.layers.resolve(target);

        auto u_delta = ::qtil::pun_cast<size_t>(delta);

        if (!layer_res) {
            return ::std::unexpected(layer_res.error());
        }

        core::AnimatedLayer* layer = layer_res.value();

        if (layer->frames.size() >= frame + len || layer->frames.size() >= frame) {
            return ::std::unexpected(FrameOpFailure::OutOfBoundsOp);
        }

        if (frame + len + u_delta >= layer->frames.size() || frame + u_delta >= layer->frames.size()) {
            return ::std::unexpected(FrameOpFailure::OutOfBoundsOp);
        }

        if (layer->frames[frame].from_first != 0) {
            core::FrameId frame_id = layer->frames[frame].content;
            auto frame_res = duplicate_frame(file, frame_id);

            if (!frame_res) {
                ::qtil::panic("Invalid frame id in layer!");
            }

            core::FrameId duplicate_frame = *frame_res;

            layer->frames.replace_frame(frame, duplicate_frame);

            auto insert_res = layer->frames.insert_frame(frame, frame_id);

            if (!insert_res) {
                ::qtil::panic("FrameContainer misbehaving, normal insert returned error");
            }
        }

        size_t next_index = frame;
        size_t max = frame + len;

        ::std::vector<size_t> frames;

        while (next_index <= max) {
            frames.emplace_back(next_index);
            next_index += layer->frames[next_index].to_next;
        }

        for (size_t frame_index : frames) {
            layer->frames.move_frame(frame_index, frame_index + u_delta);
        }

        return {};
    }

    ::std::expected<core::FatFrame, FrameOpError>
    get_frame(core::AnimFile& file, core::AnimLayerId target, size_t frame) {
        auto layer_res = file.layers.resolve(target);

        if (!layer_res) {
            return ::std::unexpected(layer_res.error());
        }

        core::AnimatedLayer* layer = layer_res.value();

        if (frame >= layer->frames.size()) {
            return ::std::unexpected(FrameOpFailure::OutOfBoundsOp);
        }

        return layer->frames[frame];
    }

    ::std::expected<::std::vector<core::FatFrame>, FrameOpError>
    get_frames(core::AnimFile& file, core::AnimLayerId target, size_t frame, size_t count) {
        auto layer_res = file.layers.resolve(target);

        if (!layer_res) {
            return ::std::unexpected(layer_res.error());
        }

        core::AnimatedLayer* layer = layer_res.value();

        if (layer->frames.size() >= frame || layer->frames.size() > frame + count) {
            return ::std::unexpected(FrameOpFailure::OutOfBoundsOp);
        }

        ::std::vector<core::FatFrame> return_value;

        for (size_t i : ::std::views::iota(frame, frame + count)) {
            return_value.emplace_back(layer->frames[i]);
        }

        return return_value;
    }
}
