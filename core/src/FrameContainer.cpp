//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 19 Feb 2026.
//

#include "FrameContainer.hpp"

#include <ranges>
#include <format>

#include "id_sys.hpp"
#include "panic.hpp"

namespace quartz::core {
    template <typename ItemT>
    ::std::expected<typename FrameContainer<ItemT>::InsertSuccess, typename FrameContainer<ItemT>::InsertFailure>
    FrameContainer<ItemT>::insert_frame(size_t index, ItemT item) {
        if (index >= items_.size()) {
            if (items_.empty()) {
                //You can't fill the list with the last frame in it up to 'index' if there is no last frame
                if (index != 0) {
                    return ::std::unexpected(NoFrames{});
                }

                //This is fine. No fill required. Just append 'item'
                items_.emplace_back(Item{item, 0, 1});
                return AddedToEnd{1};
            }

            Item last_item = items_.back();

            if (last_item.content == item) {
                size_t last_dist = last_item.from_first;

                size_t items_size = items_.size();

                for (size_t i : ::std::views::iota(static_cast<size_t>(1), index - items_.size() + 2)) {
                    items_.emplace_back(Item{item, last_dist + i, Item::INVALID_INDEX});
                }

                size_t first_frame = items_.size() - last_dist - 1;

                for (size_t i : ::std::views::iota(first_frame, items_.size())) {
                    items_[i].to_next = items_.size() - i;
                }

                return AddedToEnd{index - items_size + 1};
            }

            if (index == items_.size()) {
                items_.emplace_back(Item{item, 0, 1});
                return AddedToEnd{1};
            }

            size_t for_len = index - items_.size() + 1;
            size_t first_frame = items_.size() - last_item.from_first - 1;

            for (size_t i : ::std::views::iota(static_cast<size_t>(1), for_len)) {
                items_.emplace_back(Item{last_item.content, last_item.from_first + i, Item::INVALID_INDEX});
            }

            for (size_t i : ::std::views::iota(first_frame, items_.size())) {
                items_[i].to_next = items_.size() - i;
            }

            items_.emplace_back(Item{item, 0, 1});

            return AddedToEnd{for_len};
        }

        if (items_[index].content == item) {
            return std::unexpected(SameFrames{});
        }

        if (items_[index].from_first == 0) {
            Item replace_target = items_[index];

            size_t end_run = index;

            for (size_t i : ::std::views::iota(index, items_.size())) {
                if (items_[i].content != replace_target.content) {
                    end_run = i;
                    break;
                }
                items_[i].content = item;
            }

            size_t start_run = index;

            bool do_redex = false;

            if (index != 0 && items_[index - 1].content == item) {
                start_run = index - items_[index - 1].from_first - 1;
                do_redex = true;
            }

            if (end_run < items_.size() - 1 && items_[end_run + 1].content == item) {
                end_run += items_[end_run + 1].to_next;
                do_redex = true;
            }

            if (do_redex) {
                for (size_t i : ::std::views::iota(start_run, end_run + 1)) {
                    items_[i].to_next = end_run - i + 1;
                    items_[i].from_first = i - start_run;
                }
            }

            return Normal{};
        }

        Item replace_target = items_[index];

        size_t first_replace = index - replace_target.from_first;

        size_t new_last = index + replace_target.to_next;

        if (new_last < items_.size() && items_[new_last].content == item) {
            new_last += items_[new_last].to_next;
        }

        if (items_[first_replace].content == item) {
            index = first_replace;
        }

        for (size_t i : ::std::views::iota(first_replace, index)) {
            items_[i].to_next = index - i;
        }

        for (size_t i : ::std::views::iota(index, new_last)) {
            items_[i].from_first = i - index;
            items_[i].content = item;
            items_[i].to_next = new_last - i;
        }

        return Normal{};
    }

    template <typename ItemT>
    void FrameContainer<ItemT>::append_frame(ItemT item, size_t count) {
        if (!items_.empty()) {
            if (items_.back().content == item) {
                size_t last_dist = items_.back().from_first;

                for (size_t i : ::std::views::iota(static_cast<size_t>(1), count)) {
                    items_.emplace_back(Item{item, last_dist + i, Item::INVALID_INDEX});
                }

                size_t first_frame = items_.size() - last_dist - 1;

                for (size_t i : ::std::views::iota(first_frame, items_.size())) {
                    items_[i].to_next = items_.size() - i;
                }

                return;
            }
        }

        for (size_t i : ::std::views::iota(static_cast<size_t>(0), count)) {
            items_.emplace_back(Item{item, i, count - i});
        }
    }

    template <typename ItemT>
    void FrameContainer<ItemT>::replace_frame(size_t index, ItemT replace) {
        if (index >= items_.size()) {
            return;
        }

        size_t start = index - items_[index].from_first;
        size_t end = index + items_[index].to_next;

        for (size_t i : std::views::iota(start, end)) {
            items_[i].content = replace;
        }
    }

    template <typename ItemT>
    void FrameContainer<ItemT>::remove_frame(size_t index) {
        if (index >= items_.size()) {
            return;
        }

        size_t target_start = index - items_[index].from_first;

        if (target_start == 0) {
            return;
        }

        ItemT replace = items_[target_start - 1].content;

        size_t end_run = index + items_[index].to_next;

        for (size_t i : ::std::views::iota(target_start, end_run)) {
            items_[i].content = replace;
        }

        target_start -= items_[target_start - 1].from_first + 1;

        for (size_t i : ::std::views::iota(target_start, end_run)) {
            items_[i].from_first = i - target_start;
            items_[i].to_next = end_run - i;
        }
    }

    template <typename ItemT>
    void FrameContainer<ItemT>::move_frame(size_t from, size_t to) {
        if (from >= items_.size() || to >= items_.size()) {
            return;
        }

        Item& item = items_[from];

        remove_frame(from);

        InsertResult res = insert_frame(to, item.content);
    }

    template <typename ItemT>
    void FrameContainer<ItemT>::trim_to_size(size_t size) {
        if (size >= items_.size()) {
            return;
        }

        items_.resize(size);

        if (items_.empty()) {
            return;
        }

        if (items_.back().to_next != 1) {
            ItemT& last = items_.back().content;

            for (size_t i : ::std::views::iota(static_cast<size_t>(0), items_.size()) | ::std::views::reverse) {
                if (items_[i].content != last) {
                    break;
                }
                items_[i].to_next = items_.size() - i;
            }
        }
    }

    template <typename ItemT>
    FrameContainer<ItemT>::Iterator FrameContainer<ItemT>::at(size_t index) {
        return Iterator{this, index};
    }

    template <typename ItemT>
    FrameContainer<ItemT>::Item FrameContainer<ItemT>::operator[](size_t index) const {
        if (index >= size()) {
            ::qtil::panic(::std::format("Index {} greater than size {}\n", index, size()));
        }
        Item& item = items_[index];
        return std::make_tuple(item.content, item.from_first, item.to_next);
    }

    template <typename ItemT>
    FrameContainer<ItemT>::Iterator FrameContainer<ItemT>::begin() {
        return Iterator{this, 0};
    }

    template <typename ItemT>
    FrameContainer<ItemT>::Iterator FrameContainer<ItemT>::end() {
        return Iterator{this, items_.size()};
    }

    template <typename ItemT>
    size_t FrameContainer<ItemT>::size() const {
        return items_.size();
    }

    template <typename ItemT>
    void FrameContainer<ItemT>::clear() {
        items_.clear();
    }

    template class FrameContainer<int>;
    template class FrameContainer<FrameId>;
}
