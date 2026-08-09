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
            std::vector<InstanceId> instances{};
            DrawingId drawing;
        };

        template<typename T>
        concept FrameType = ::std::same_as<T, Empty> || ::std::same_as<T, Normal>;
    }

    /**
     * @class Frame
     * @brief Container for frame data
     *
     * @authors SkyTheDragon
     *
     * Contains data for a single frame in an animation. Uses a very similar API to that of Symbol, as it has very
     * similar needs.
     */
    class Frame {
    public:
        using FrameData = ::std::variant<frame_types::Empty, frame_types::Normal>;
    private:
        FrameData data_;
    public:
        Frame(IdKey, AnimFile*, const FrameData& data, FrameId);
        Frame(IdKey, AnimFile*, const Frame* frame, FrameId);
        Frame(IdKey, AnimFile*, FrameId);
        Frame() = delete;

        /**
         * @brief Checks if the frame is of the specified type
         * @tparam T Type to check for
         * @return Is the frame of that type
         */
        template<frame_types::FrameType T>
        [[nodiscard]] bool is() const {
            return std::holds_alternative<T>(data_);
        }

        /**
         * @brief Tries to get a pointer to frame data of specified type.
         * @tparam T Frame type to get data for
         * @return An optional pointer to the value of the data
         */
        template<frame_types::FrameType T>
        [[nodiscard]] ::std::optional<T*> get() const {
            if (!::std::holds_alternative<T>(data_)) {
                return ::std::nullopt;
            }
            return ::std::get<T>(data_).drawing;
        }

        /**
         * @brief Overwrites frame data, including frame type
         * @tparam T Type of data to set
         * @param data Value of data to set
         */
        template<frame_types::FrameType T>
        void set(T data) {
            data_ = data;
        }

        /**
        * @brief Allows matching a Frame by its type
        * @tparam Visitor Visitor type to be used
        * @param visitor instance of visitor type
        * @return Result of visitor visiting the frame
        */
        template<typename Visitor>
        decltype(auto) visit(Visitor&& visitor) {
            return ::std::visit(::std::forward<Visitor>(visitor), data_);
        }

        /**
        * @brief Allows matching a Frame by its type, but const
        * @tparam Visitor Visitor type to be used
        * @param visitor instance of visitor type
        * @return Result of visitor visiting the frame
        */
        template<typename Visitor>
        decltype(auto) visit(Visitor&& visitor) const {
            return std::visit(::std::forward<Visitor>(visitor), data_);
        }
    };
}
