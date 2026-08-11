//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 29 Jun 2026
//

#pragma once

#include <deque>
#include <limits>
#include <expected>
#include <vector>

#include <quartz/core/core_errors.hpp>

/**
 * @file id_sys.hpp
 *
 * @authors SkyTheDragon
 *
 * Contains the Id system (but Coesite)
 */
namespace quartz::renderer::coesite {
    class CoesiteRoot;
    struct Scene;
    struct Node;
    class Stroke;

    namespace detail {
        struct IdStorage {
            size_t id;
            size_t gen;
            CoesiteRoot* file;

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
        friend class CoesiteRoot;
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

        TypedId(size_t id, CoesiteRoot* file, size_t gen = 0) noexcept
            : storage_(id, gen, file) {}

        TypedId(detail::IdStorage storage) noexcept
            : storage_(storage) {}

        template<typename>
        friend class IdContainer;
        template<typename>
        friend class IdTestHelpers;
    };

    template<typename T>
    struct IdTag;

    template<>
    struct IdTag<Scene>;

    template<>
    struct IdTag<Node>;

    template<>
    struct IdTag<Stroke>;

    struct SceneTag;

    template<typename T>
    using Id = TypedId<typename IdTag<T>::type>;

    /**
     * @brief If you need a CoesiteRoot *, but not a valid CoesiteRoot at its location, use this
     * @warning NEVER DEREFERENCE A POINTER TO LUBRICANT, NOR SET IT TO ANYTHING ELSE. IT MUST BE CLEAR WHAT POINTERS ARE
     * LUBRICANT, AND WHICH ARE NOT.
     * If you use this outside of tests, your commit and/or PR will be rejected
     */
    static inline CoesiteRoot* LUBRICANT = reinterpret_cast<CoesiteRoot*>(0xC83360u);

    /**
     * @brief See LUBRICANT
     * @warning NEVER DEREFERENCE A POINTER TO LAUNDRY_LUBE, NOR SET IT TO ANYTHING ELSE. IT MUST BE CLEAR WHAT
     * POINTERS ARE LUBRICANT, AND WHICH ARE NOT
     * @warning ♫ Keep the clothes clean when you know you should ♪ Get all that dirt out with laundry lube! ♫
     *
     * If you use this outside of tests, your commit and/or PR will be rejected
     */
    static inline CoesiteRoot* LAUNDRY_LUBE = reinterpret_cast<CoesiteRoot*>(0xC83360FFu);

    /**
     * @brief Is it LUBRICANT
     * @param root Is it LUBRICANT
     * @return Is it LUBRICANT
     */
    inline bool is_LUBRICANT(const CoesiteRoot* root) {
        return root == LUBRICANT || root == LAUNDRY_LUBE;
    }

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
            size_t generation = 0;
            bool free = false;
        };

        static_assert(sizeof(Container) > 0, "sizeof(Container)==0?");
        static constexpr size_t CHUNK_SIZE = ([]() {
            if (1'048'576u / sizeof(Container) != 0) {
                return 1'048'576u / sizeof(Container);
            } else {
                return static_cast<size_t>(4);
            }
        })();

        ::std::vector<Container> data_;
        ::std::deque<size_t> freelist_;
        CoesiteRoot* file_;
    public:

        IdContainer(CoesiteRoot* file) : file_(file) {}

        /**
         * @brief Returns invalid ID of container's type
         * @return Invalid ID
         * @deprecated Ok, we don't need this anymore, just use the Id's INVALID constant. It's more clear
         */
        static Id<T> invalid() { return Id<T>{}; }

        /**
         * @brief Frees Id, allowing for reuse
         * @param id Id to free
         *
         * Attempts to free Id and add index to freelist
         */
        ::std::expected<void, core::ResolveFailure> free(Id<T> id) {
            if (!id) {
                return ::std::unexpected(core::ResolveFailure::InvalidId);
            }

            if (id.storage_.file != file_) {
                return ::std::unexpected(core::ResolveFailure::WrongFile);
            }

            if (id.storage_.id >= data_.size()) {
                return ::std::unexpected(core::ResolveFailure::NoSuchObject);
            }

            if (data_[id.storage_.id].free) {
                return ::std::unexpected(core::ResolveFailure::TargetDeleted);
            }

            if (id.storage_.gen != data_[id.storage_.id].generation) {
                return ::std::unexpected(core::ResolveFailure::TargetDeleted);
            }

            data_[id.storage_.id].free = true;
            freelist_.push_back(id.storage_.id);
            ::std::destroy_at(::std::addressof(data_[id.storage_.id].object));

            return {};
        }

        /**
         * @brief Adds object, and gives it an Id.
         * @tparam Args Constructor arguments types
         * @param args Constructor arguments
         * @return Id of object added, along with a pointer to it
         */
        template<typename... Args>
        ::std::pair<Id<T>, T*> add_wp(Args&&... args) {
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
                return ::std::pair{Id<T>{data_.size() - 1, file_}, ::std::addressof(data_.back().object)};
            }

            size_t id = freelist_.back();
            size_t gen = data_[id].generation + 1;
            freelist_.pop_front();

            ::std::construct_at(
                ::std::addressof(data_[id].object),
                IdKey{},
                file_,
                ::std::forward<Args>(args)...,
                Id<T>{id, file_, gen});

            data_[id].generation = gen;
            data_[id].free = false;

            return ::std::pair{Id<T>{id, file_, gen}, ::std::addressof(data_[id].object)};
        }

        /**
         * @brief Adds object, and gives it an Id.
         * @tparam Args Constructor arguments types
         * @param args Constructor arguments
         * @return Id of object added
         */
        template<typename... Args>
        Id<T> add(Args... args) {
            return add_wp(std::forward<Args>(args)...).first;
        }

        /**
         * @brief Resolves Id to a pointer to object
         * @param id Id to resolve
         * @return Object pointer or error
         */
        ::std::expected<T*, core::ResolveFailure> resolve(Id<T> id) {
            if (!id) {
                return ::std::unexpected(core::ResolveFailure::InvalidId);
            }

            if (id.storage_.file != file_) {
                return ::std::unexpected(core::ResolveFailure::WrongFile);
            }

            if (id.storage_.id >= data_.size()) {
                return ::std::unexpected(core::ResolveFailure::NoSuchObject);
            }

            if (data_[id.storage_.id].free) {
                return ::std::unexpected(core::ResolveFailure::TargetDeleted);
            }

            if (data_[id.storage_.id].generation != id.storage_.gen) {
                return ::std::unexpected(core::ResolveFailure::TargetDeleted);
            }

            return ::std::addressof(data_[id.storage_.id].object);
        }

        size_t size() {
            return data_.size() - freelist_.size();
        }

        size_t capacity() {
            return data_.capacity();
        }
    };

    using SceneId = Id<Scene>;
    using NodeId = Id<Node>;
    using StrokeId = Id<Stroke>;
}
