//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/8/26.
//

#pragma once
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
        ::std::variant<frame_types::Empty, frame_types::Normal> data_;
        Frame() = delete;
        explicit Frame(::std::variant<frame_types::Empty, frame_types::Normal> data);

    public:
        static Frame empty();
        static Frame normal(Drawing drawing = Drawing());

        template<frame_types::FrameType T>
        T& get_data() {
            return ::std::get<T>(data_).drawing;
        }
    };
}
