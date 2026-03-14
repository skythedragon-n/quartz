//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/9/26.
//

#pragma once

#include <cstddef>
#include <expected>
#include <vector>

#include "core_errors.hpp"

/**
 * @file id_sys.hpp
 *
 * @authors SkyTheDragon
 *
 * Contains the Id system
 */
namespace quartz::core {
    class AnimFile;
    class LibraryFolder;
    class Library;
    class Symbol;
    class Frame;
    class AnimatedLayer;
    class Stroke;
    class Drawing;
    struct Instance;

    namespace detail {
        struct IdStorage {
            size_t id;
            size_t gen;
            AnimFile* file;
        };
    }

    enum class ObjectStage {
        Alive,
        Ghosted,
        DeadWRef,
        Dead
    };

    struct IdKey {
    private:
        IdKey() = default;
        template<typename>
        friend class IdContainer;
        friend class AnimFile;
    };

    template<typename Tag>
    class TypedId {
    public:
        TypedId() = default;

        friend bool operator==(const TypedId&, const TypedId&) = default;
        friend bool operator!=(const TypedId&, const TypedId&) = default;

        operator bool() const { return storage_.id != static_cast<size_t>(-1) && storage_.file != nullptr; }

        [[nodiscard]] bool is_valid() const {
            return storage_.id != static_cast<size_t>(-1) && storage_.file != nullptr;
        }

    private:
        detail::IdStorage storage_;

        TypedId(size_t id, AnimFile* file, size_t gen = 0) noexcept
            : storage_(id, gen, file) {}

        TypedId(detail::IdStorage storage) noexcept
            : storage_(storage) {}

        template<typename>
        friend class IdContainer;
    };

    template<typename T>
    struct IdTag;

    struct SymbolTag {};
    struct InstanceTag {};
    struct FolderTag {};
    struct LibraryTag {};
    struct FrameTag {};
    struct AnimLayerTag {};

    template<>
    struct IdTag<Symbol> { using type = SymbolTag; };

    template<>
    struct IdTag<Instance> { using type = InstanceTag; };

    template<>
    struct IdTag<LibraryFolder> { using type = FolderTag; };

    template<>
    struct IdTag<Library> { using type = LibraryTag; };

    template<>
    struct IdTag<FolderTag> { using type = FolderTag; };

    template<>
    struct IdTag<AnimLayerTag> { using type = AnimLayerTag; };

    /**
     * @page ids ID explanation
     * Id's contain two members, an `id` member, which is an index into the AnimFile's object table for that type, and
     * a `file` member, which is a pointer to the Id's file.
     *
     * DO NOT SET THESE MEMBERS UNDER ANY CIRCUMSTANCES. SUCH IS UNDEFINED BEHAVIOR, AND COULD CAUSE ANYTHING TO HAPPEN,
     * INCLUDING, BUT NOT LIMITED TO YOUR COMPUTER FILLING YOUR HOME WITH AN INORDINATE NUMBER OF PUPPIES & RAINBOWS.
     */

    template<typename T>
    using Id = TypedId<typename IdTag<T>::type>;

    template<typename T>
    class IdContainer {
        struct Container {
            T object;
            size_t generation = 1;
            bool free = false;
        };

        ::std::vector<Container> data_;
        ::std::vector<size_t> freelist_;
        AnimFile* file_ = nullptr;
    public:

        IdContainer(AnimFile* file) : file_(file) {}

        static Id<T> invalid() { return Id<T>{}; }

        void free(Id<T> id) {
            if (is_valid(id)) {
                data_[id.storage_.id].free = true;
                freelist_.push_back(id.storage_.id);
            }
        }

        template<typename... Args>
        Id<T> add(Args... args) {
            if (freelist_.empty()) {
                data_.emplace_back(args..., Id<T>{data_.size(), file_});
                return Id<T>{data_.size() - 1, file_};
            }

            size_t id = freelist_.back();
            size_t gen = data_[id].generation + 1;
            freelist_.pop_back();
            data_[id] = Container{T{IdKey{}, file_, args..., Id<T>{id, file_, gen}}, gen};

            return Id<T>{id, file_, gen};
        }

        ::std::expected<T*, ResolveFailure> resolve(Id<T> id) {
            if (!id) {
                return ::std::unexpected(ResolveFailure::InvalidId);
            }

            if (id.storage_.file != file_) {
                return ::std::unexpected(ResolveFailure::WrongFile);
            }

            if (id.storage_.id >= data_.size()) {
                return ::std::unexpected(ResolveFailure::NoSuchObject);
            }

            if (data_[id.storage_.id].free) {
                return ::std::unexpected(ResolveFailure::TargetDeleted);
            }

            if (data_[id.storage_.id].generation != id.storage_.gen) {
                return ::std::unexpected(ResolveFailure::TargetDeleted);
            }

            return &data_[id.storage_.id].object;
        }
    };

    struct SymbolId {size_t id; AnimFile* file = nullptr; };
    struct InstanceId { size_t id; AnimFile* file = nullptr; };
    struct FolderId { size_t id; AnimFile* file = nullptr; };
    struct LibraryId { size_t id; AnimFile* file = nullptr; };
    struct FrameId { size_t id; AnimFile* file = nullptr; };
    struct AnimLayerId { size_t id; AnimFile* file = nullptr; };

    bool operator==(const SymbolId& lhs, const SymbolId& rhs);
    bool operator==(const InstanceId& lhs, const InstanceId& rhs);
    bool operator==(const FolderId& lhs, const FolderId& rhs);
    bool operator==(const LibraryId& lhs, const LibraryId& rhs);
    bool operator==(const FrameId& lhs, const FrameId& rhs);
    bool operator==(const AnimLayerId& lhs, const AnimLayerId& rhs);

    constexpr SymbolId SYMBOL_ID_INVALID = {size_t(-1)};
    constexpr InstanceId INSTANCE_ID_INVALID = {size_t(-1)};
    constexpr FolderId FOLDER_ID_INVALID = {size_t(-1)};
    constexpr LibraryId LIBRARY_ID_INVALID = {size_t(-1)};
    constexpr FrameId FRAME_ID_INVALID = {size_t(-1)};
    constexpr AnimLayerId ANIM_LAYER_ID_INVALID = {size_t(-1)};
}
