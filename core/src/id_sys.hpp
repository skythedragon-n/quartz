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

            bool operator==(const IdStorage& other) const {
                return other.id == id && other.gen == gen && other.file == file;
            }
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

        static constexpr TypedId INVALID = TypedId{};

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
    struct IdTag<Frame> { using type = FrameTag; };

    template<>
    struct IdTag<AnimatedLayer> { using type = AnimLayerTag; };

    /**
     * @page ids ID explanation
     * Id's contain two members, an `id` member, which is an index into the AnimFile's object table for that type, and
     * a `file` member, which is a pointer to the Id's file.
     *
     * **DO NOT SET THESE MEMBERS UNDER ANY CIRCUMSTANCES. SUCH IS UNDEFINED BEHAVIOR, AND COULD CAUSE ANYTHING TO HAPPEN,
     * INCLUDING, BUT NOT LIMITED TO YOUR COMPUTER FILLING YOUR HOME WITH AN INORDINATE NUMBER OF PUPPIES & RAINBOWS.**
     */

    template<typename T>
    using Id = TypedId<typename IdTag<T>::type>;

    /**
     * @brief Generic implementation of ID logic
     * @tparam T Type for ID
     *
     * Implements ID logic, including addition, resolution, freeing & stale ID reuse
     */
    template<typename T>
    class IdContainer {
        struct Container {
            T object;
            size_t generation = 1;
            bool free = false;
        };

        static_assert(sizeof(Container) > 0, "sizeof(Container)==0?");
        constexpr size_t CHUNK_SIZE = ([]() {
            if (1'048'576u / sizeof(Container) != 0) {
                return 1'048'576u / sizeof(Container);
            } else {
                return 4;
            }
        })();

        ::std::vector<Container> data_;
        ::std::vector<size_t> freelist_;
        AnimFile* file_ = nullptr;
    public:

        IdContainer(AnimFile* file) : file_(file) {}

        /**
         * @brief Returns invalid ID of container's type
         * @return Invalid ID
         */
        static Id<T> invalid() { return Id<T>{}; }

        /**
         * @brief Frees Id, allowing for reuse
         * @param id Id to free
         *
         * Attempts to free Id and add index to freelist
         */
        void free(Id<T> id) {
            if (id) {
                if (id.storage_.file != file_) {
                    return;
                }

                if (id.storage_.id >= data_.size()) {
                    return;
                }

                if (data_[id.storage_.id].free) {
                    return;
                }

                data_[id.storage_.id].free = true;
                freelist_.push_back(id.storage_.id);
            }
        }

        /**
         * @brief Adds object, and gives it an Id.
         * @tparam Args Constructor arguments types
         * @param args Constructor arguments
         * @return Id of object added
         */
        template<typename... Args>
        Id<T> add(Args&&... args) {
            if (freelist_.empty()) {
                size_t capacity = data_.capacity();

                if (capacity == data_.size()) {
                    size_t doubled_capacity = capacity ? capacity * 2 : 1;

                    bool doubled_ok = capacity && (doubled_capacity / 2 == capacity) && doubled_capacity > capacity;

                    if (capacity == 0 || (doubled_ok && doubled_capacity <= CHUNK_SIZE)) {
                        data_.reserve(doubled_capacity);
                    } else {
                        size_t target = capacity + CHUNK_SIZE;
                        if (target < capacity) target = std::numeric_limits<size_t>::max();
                        data_.reserve(target);
                    }
                }

                data_.emplace_back(T{
                    IdKey{},
                    file_,
                    std::forward<Args>(args)...,
                    Id<T>{data_.size(), file_}
                });
                return Id<T>{data_.size() - 1, file_};
            }

            size_t id = freelist_.back();
            size_t gen = data_[id].generation + 1;
            freelist_.pop_back();

            ::std::destroy_at(::std::addressof(data_[id].object));
            ::std::construct_at(
                ::std::addressof(data_[id].object),
                IdKey{},
                file_,
                ::std::forward<Args>(args)...,
                Id<T>{id, file_, gen});

            data_[id].generation = gen;
            data_[id].free = false;

            return Id<T>{id, file_, gen};
        }

        /**
         * @brief Resolves Id to a pointer to object
         * @param id Id to resolve
         * @return Object pointer or error
         */
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

    using SymbolId = Id<Symbol>;
    using InstanceId = Id<Instance>;
    using FolderId = Id<LibraryFolder>;
    using LibraryId = Id<Library>;
    using FrameId = Id<Frame>;
    using AnimLayerId = Id<AnimatedLayer>;
}
