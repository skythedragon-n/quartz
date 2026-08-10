//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 19 Feb 2026.
//

#pragma once

#include <vector>
#include <limits>
#include <variant>
#include <expected>

#include "./id_sys.hpp"

namespace quartz::core {
    /**
     * @brief Buffer for timeline data
     * @tparam ItemT Type of the item to be used
     *
     * A buffer for timeline data, which has a value at each frame, and has that frames distance from the start
     * of the keyframe, and the distance to the next keyframe also stored at each index
     */
    template<typename ItemT>
    class FrameContainer {
    public:
        /**
         * @brief an item in the frame container, with the distance from the first frame in a run of the same value, and the
         * distance to the first frame of a different value
         */
        struct Item {
            static constexpr size_t INVALID_INDEX = ::std::numeric_limits<size_t>::max();

            ItemT content;
            size_t from_first;
            size_t to_next;
        };

    private:
        ::std::vector<Item> items_{};

    public:
        /**
         * You tried to insert a frame away from the start into an empty FrameContainer
         */
        struct NoFrames {};

        /**
         * The index already has that frame at that location
         */
        struct SameFrames {};

        /**
         * A frame was inserted normally
         */
        struct Normal {};

        /**
         * Extended the end of the FrameContainer by copying the last frame up to the index selected
         */
        struct AddedToEnd {
            size_t count;
        };

        using InsertFailure = ::std::variant<NoFrames, SameFrames>;
        using InsertSuccess = ::std::variant<AddedToEnd, Normal>;

        using InsertResult = ::std::expected<InsertSuccess, InsertFailure>;

        /**
         * Iterator into frame data
         */
        class Iterator {
            size_t index_ = 0;
            const FrameContainer* container_ = nullptr;

            friend class FrameContainer;

        public:
            Iterator(const FrameContainer* container, size_t index) : index_(index), container_(container) {}

            Iterator& operator++() {
                index_++;
                return *this;
            }

            Iterator& operator--() {
                index_--;
                return *this;
            }

            ItemT operator*() { return container_->items_[index_].content; }
        };

        FrameContainer() = default;

        /**
         * @brief Inserts a keyframe into the timeline at the specified index.
         * @param index Index to inserted to
         * @param item Content of frame to be inserted
         * @return Info about the result of the insertion
         * Inserts a keyframe into the timeline at the specified index. If the index is after the end of the timeline,
         * it extends the last keyframe, until this keyframe can simply be inserted at the end.
         */
        InsertResult insert_frame(size_t index, ItemT item);
        /**
         * @brief Appends a keyframe at the end
         * @param item Keyframe to append
         * @param count Length to make the keyframe
         */
        void append_frame(ItemT item, size_t count = 1);

        //TODO: better error handling
        /**
         * @brief Replaces keyframe at index. If no keyframe starts, it finds and replaces from the start of the keyframe at index
         * @param index Index to replace
         * @param replace Value to replace
         */
        void replace_frame(size_t index, ItemT replace);
        /**
         * @brief Removes keyframe at index. If a keyframe does not start at that index, it finds the first frame of
         * the keyframe at the index, and removes from there
         * @param index Index to remove frame at
         * @return Removed keyframe
         */
        ::std::expected<ItemT, int> remove_frame(size_t index);
        /**
         * @brief Moves the keyframe at from to to, essential removing it at from, and inserting at to
         * @param from Source keyframe's position
         * @param to Destination for moved kframe
         */
        void move_frame(size_t from, size_t to);

        /**
         * @brief Removes frames from the FrameContainer until it reaches size
         * @param size New timeline size
         */
        void trim_to_size(size_t size);

        /**
         * @brief Gets an iterator to position index
         * @param index Index to get iterator for
         * @return Iterator
         */
        Iterator at(size_t index);

        /**
         * @brief Indexes into to FrameContainer, returning a value with the keyframe value + to_next and from_last
         * @param index Index to index at
         * @return Value at position
         */
        Item operator[](size_t index) const;

        /**
         * @brief Gets an iterator the start of the FrameContainer
         * @return Iterator to start
         */
        Iterator begin();
        /**
         * @brief Gets an iterator the end of the FrameContainer
         * @return Iterator to end
         */
        Iterator end();

        /**
         * @brief Gets the size of the FrameContainer
         * @return Size of the FrameContainer
         */
        [[nodiscard]] size_t size() const;

        /**
         * @brief Clears all frames from the FrameContainer
         */
        void clear();
    };

    using FatFrame = FrameContainer<FrameId>::Item;
}
