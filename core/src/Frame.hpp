//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#pragma once
#include <optional>
#include <variant>
#include <vector>

#include "Drawing.hpp"
#include "id_sys.hpp"

namespace quartz::core {
    namespace frame_types {
        struct Empty {};
        struct Normal {
            std::vector<InstanceId> instances;
            Drawing drawing;
        };

        template<typename T>
        concept FrameType = ::std::same_as<T, Empty> || ::std::same_as<T, Normal>;
    }

    class Frame {
        using FrameData = ::std::variant<frame_types::Empty, frame_types::Normal>;
        FrameData data_;
    public:
        Frame(IdKey, AnimFile*, const FrameData& data, FrameId);
        Frame(IdKey, AnimFile*, FrameId);
        Frame() = delete;

        [[nodiscard]] const FrameData& data() const { return data_; }

        template<frame_types::FrameType T>
        [[nodiscard]] bool is() const {
            return std::holds_alternative<T>(data_);
        }

        template<frame_types::FrameType T>
        [[nodiscard]] ::std::optional<T*> get() const {
            if (!::std::holds_alternative<T>(data_)) {
                return ::std::nullopt;
            }
            return ::std::get<T>(data_).drawing;
        }

        template<frame_types::FrameType T>
        void set(T data) {
            data_ = data;
        }

        template<typename Visitor>
        decltype(auto) visit(Visitor&& visitor) {
            return ::std::visit(::std::forward<Visitor>(visitor), data_);
        }

        template<typename Visitor>
        decltype(auto) visit(Visitor&& visitor) const {
            return std::visit(::std::forward<Visitor>(visitor), data_);
        }
    };
}
