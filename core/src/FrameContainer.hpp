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

#include <quartz/core/id_sys.hpp>

namespace quartz::core {

    template<typename ItemT>
    class FrameContainer {
    public:
        struct Item {
            static constexpr size_t INVALID_INDEX = ::std::numeric_limits<size_t>::max();

            ItemT content;
            size_t from_first;
            size_t to_next;
        };

    private:
        ::std::vector<Item> items_{};

    public:

        struct NoFrames {};

        struct SameFrames {};

        struct Normal {};

        struct AddedToEnd {
            size_t count;
        };

        using InsertFailure = ::std::variant<NoFrames, SameFrames>;
        using InsertSuccess = ::std::variant<AddedToEnd, Normal>;

        using InsertResult = ::std::expected<InsertSuccess, InsertFailure>;

        class Iterator {
            size_t index_ = 0;
            const FrameContainer* container_ = nullptr;

            friend class FrameContainer;

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

        InsertResult insert_frame(size_t index, ItemT item);
        void append_frame(ItemT item, size_t count = 1);

        //TODO: better error handling
        void replace_frame(size_t index, ItemT replace);
        void remove_frame(size_t index);
        void move_frame(size_t from, size_t to);

        void trim_to_size(size_t size);

        Iterator at(size_t index);

        Item operator[](size_t index) const;

        Iterator begin();
        Iterator end();

        [[nodiscard]] size_t size() const;

        void clear();
    };

    using FatFrame = FrameContainer<FrameId>::Item;
}
