//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/9/26.
//

#pragma once

#include <atomic>
#include <deque>
#include <limits>
#include <expected>
#include <vector>

#include "./core_errors.hpp"

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
    class Instance;

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
        template<typename>
        friend class IdTestHelpers;
    };

    template<typename T>
    struct IdTag;

    struct SymbolTag {};
    struct InstanceTag {};
    struct FolderTag {};
    struct LibraryTag {};
    struct FrameTag {};
    struct AnimLayerTag {};
    struct DrawingTag {};

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

    template<>
    struct IdTag<Drawing> { using type = DrawingTag; };

    template<typename T>
    using Id = TypedId<typename IdTag<T>::type>;

    /**
     * @brief If you need a AnimFile*, but not an valid AnimFile at it's location, use this
     * @warning NEVER DEREFERENCE A POINTER TO CHEETO, NOR SET IT TO ANYTHING ELSE. IT MUST BE CLEAR WHAT POINTERS ARE
     * CHEETO, AND WHICH ARE NOT
     * If you use this outside of tests, your commit and/or PR will be rejected
     */
    static inline AnimFile* CHEETO = reinterpret_cast<AnimFile*>(0xC83360u);

    /**
     * @brief See CHEETO
     * @warning NEVER DEREFERENCE A POINTER TO FLAMING_HOT_CHEETO, NOR SET IT TO ANYTHING ELSE. IT MUST BE CLEAR WHAT
     * POINTERS ARE CHEETO, AND WHICH ARE NOT
     * @warning ♫Flaming Hot Cheetoes, eat the whole damn thing and you will know♪ Eat in one sitting, then you'll
     * black out, forget everything♫
     *
     * If you use this outside of tests, your commit and/or PR will be rejected
     */
    static inline AnimFile* FLAMING_HOT_CHEETO = reinterpret_cast<AnimFile*>(0xC83360FFu);

    /**
     * @brief Is it a CHEETO
     * @param file Is it a CHEETO
     * @return Is it a CHEETO
     */
    inline bool is_CHEETO(const AnimFile* file) {
        return file == CHEETO || file == FLAMING_HOT_CHEETO;
    }

    template<typename T>
    class Lease;
    template<typename T>
    class MutLease;

    class LeaseKey {
        template<typename T>
        friend class IdContainer;
        template<typename T>
        friend class Lease;

        LeaseKey() = default;
    };

    struct Slot {
        size_t generation = 0;
        ::std::atomic<size_t> held_const = 0;
        bool free = false;
        ::std::atomic<bool> held_mut = false;
        ::std::atomic<bool> locked = false;

        void wait_until_unleased() const {
            while (held_mut.load(::std::memory_order_acquire)) {
                held_mut.wait(true, ::std::memory_order_relaxed);
            }

            while (size_t count = held_const.load(::std::memory_order_acquire)) {
                held_const.wait(count, ::std::memory_order_relaxed);
            }
        }

        void wait_until_no_mut() const {
            while (held_mut.load(::std::memory_order_acquire)) {
                held_mut.wait(true, ::std::memory_order_relaxed);
            }
        }
    };

    /**
     * @brief Generic implementation of ID logic
     * @tparam T Type for ID
     *
     * Implements ID logic, including addition, resolution, freeing & stale ID reuse
     */
    template<typename T>
    class IdContainer {
        static_assert(sizeof(T) > 0, "sizeof(Container)==0?");
        static constexpr size_t CHUNK_SIZE = ([]() {
            if (1'048'576u / sizeof(T) != 0) {
                return 1'048'576u / sizeof(T);
            } else {
                return static_cast<size_t>(4);
            }
        })();

        ::std::vector<T> data_;
        ::std::vector<Slot> slots_;
        ::std::deque<size_t> freelist_;
        AnimFile* file_ = nullptr;

        friend class MutLease<T>;
        friend class Lease<T>;
    public:

        IdContainer(AnimFile* file) : file_(file) {}

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
        ::std::expected<void, ResolveFailure> free(Id<T> id) {
            slots_[id.storage_.id].locked = true;
            slots_[id.storage_.id].wait_until_unleased();

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

            if (id.storage_.gen != data_[id.storage_.id].generation) {
                return ::std::unexpected(ResolveFailure::TargetDeleted);
            }

            slots_[id.storage_.id].free = true;
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
        ::std::pair<Id<T>, MutLease<T>> add_wl(Args&&... args) {
            if (freelist_.empty()) {
                size_t capacity = data_.capacity();

                if (capacity == data_.size()) {
                    size_t doubled_capacity = capacity ? capacity * 2 : 1;

                    bool doubled_ok = capacity && (doubled_capacity / 2 == capacity) && doubled_capacity > capacity;

                    if (capacity == 0 || (doubled_ok && doubled_capacity <= CHUNK_SIZE)) {
                        data_.reserve(doubled_capacity);
                        slots_.reserve(doubled_capacity);
                    } else {
                        size_t target = capacity + CHUNK_SIZE;
                        if (target < capacity) target = std::numeric_limits<size_t>::max();
                        data_.reserve(target);
                        slots_.reserve(target);
                    }
                }

                data_.emplace_back(T{
                    IdKey{},
                    file_,
                    std::forward<Args>(args)...,
                    Id<T>{data_.size(), file_}
                });

                slots_.emplace_back();

                slots_.back().held_mut = true;

                return ::std::pair{Id<T>{data_.size() - 1, file_}, MutLease<T>{&slots_.back(), &data_.back(), LeaseKey()}};
            }

            size_t id = freelist_.back();
            size_t gen = slots_[id].generation + 1;
            freelist_.pop_front();

            ::std::construct_at(
                ::std::addressof(data_[id]),
                IdKey{},
                file_,
                ::std::forward<Args>(args)...,
                Id<T>{id, file_, gen});

            slots_[id].generation = gen;
            slots_[id].free = false;
            slots_[id].locked = false;
            slots_[id].held_mut = true;

            return ::std::pair{Id<T>{id, file_, gen}, MutLease<T>{&slots_[id], &data_[id], LeaseKey()}};
        }

        /**
         * @brief Adds object, and gives it an Id.
         * @tparam Args Constructor arguments types
         * @param args Constructor arguments
         * @return Id of object added
         */
        template<typename... Args>
        Id<T> add(Args... args) {
            return add_wl(std::forward<Args>(args)...).first;
        }

        /**
         * @brief Resolves Id to a pointer to object
         * @param id Id to resolve
         * @return Object pointer or error
         */
        ::std::expected<Lease<T>, ResolveFailure> resolve(Id<T> id) {
            if (!id) {
                return ::std::unexpected(ResolveFailure::InvalidId);
            }

            if (id.storage_.file != file_) {
                return ::std::unexpected(ResolveFailure::WrongFile);
            }

            if (id.storage_.id >= data_.size()) {
                return ::std::unexpected(ResolveFailure::NoSuchObject);
            }

            if (slots_[id.storage_.id].free) {
                return ::std::unexpected(ResolveFailure::TargetDeleted);
            }

            if (slots_[id.storage_.id].generation != id.storage_.gen) {
                return ::std::unexpected(ResolveFailure::TargetDeleted);
            }

            if (slots_[id.storage_.id].locked) {
                return ::std::unexpected(ResolveFailure::TargetLocked);
            }

            slots_[id.storage_.id].wait_until_no_mut();

            slots_[id.storage_.id].held_const++;
            return Lease<T>{&slots_[id.storage_.id], &data_[id.storage_.id], LeaseKey()};
        }

        /**
         * @brief Resolves an Id, and gives the user a mutable lease to the object
         * @param id Id to resolve
         * @return Mutable lease to object or an error
         */
        ::std::expected<MutLease<T>, ResolveFailure> resolve_mut(Id<T> id) {
            if (!id) {
                return ::std::unexpected(ResolveFailure::InvalidId);
            }

            if (id.storage_.file != file_) {
                return ::std::unexpected(ResolveFailure::WrongFile);
            }

            if (id.storage_.id >= data_.size()) {
                return ::std::unexpected(ResolveFailure::NoSuchObject);
            }

            if (slots_[id.storage_.id].free) {
                return ::std::unexpected(ResolveFailure::TargetDeleted);
            }

            if (slots_[id.storage_.id].generation != id.storage_.gen) {
                return ::std::unexpected(ResolveFailure::TargetDeleted);
            }

            if (slots_[id.storage_.id].locked) {
                return ::std::unexpected(ResolveFailure::TargetLocked);
            }

            slots_[id.storage_.id].wait_until_unleased();

            slots_[id.storage_.id].held_mut = true;
            return MutLease<T>{&data_[id.storage_.id], LeaseKey()};
        }

        size_t size() {
            return data_.size() - freelist_.size();
        }

        size_t capacity() {
            return data_.capacity();
        }
    };

    template<typename T>
    class MutLease {
        Slot* slot_;
        T* object_;
        bool valid_;

    public:
        MutLease(Slot* slot, T* object, LeaseKey) :
            slot_(slot),
            object_(object),
            valid_(true)
        {}

        MutLease() :
            object_(nullptr),
            slot_(nullptr),
            valid_(false)
        {}

        MutLease(const MutLease&) = delete;
        MutLease& operator=(const MutLease&) = delete;

        MutLease(MutLease&& other) noexcept :
            slot_(other.slot_),
            object_(other.object_),
            valid_(other.valid_)
        {
            other.slot_ = nullptr;
            other.object_ = nullptr;
            other.valid_ = false;
        }

        MutLease& operator=(MutLease&& other) noexcept {
            if (this != &other) {
                return_lease();
                this->slot_ = other.slot_;
                this->object_ = other.object_;
                this->valid_ = other.valid_;

                other.slot_ = nullptr;
                other.object_ = nullptr;
                other.valid_ = false;
            }
            return *this;
        }

        T* operator->() noexcept { return valid_ ? object_ : nullptr; }

        template<typename Index>
        decltype(auto) operator[](Index&& i)
            requires requires(T& obj, Index&& i) { obj[::std::forward<Index>(i)]; }
        {
            return (*object_)[::std::forward<Index>(i)];
        }

        void return_lease() {
            if (!valid_) {
                return;
            }
            valid_ = false;
            slot_->held_mut = false;
        }

        ~MutLease() {
            return_lease();
        }

        [[nodiscard]] bool valid() const noexcept { return valid_; }
    };

    template<typename T>
    class Lease {
        Slot* slot_;
        T* object_;
        bool valid_;

    public:
        Lease(Slot* slot, T* object, LeaseKey) :
            slot_(slot),
            object_(object),
            valid_(true)
        {}

        Lease() :
            slot_(nullptr),
            object_(nullptr),
            valid_(false)
        {}

        Lease(const Lease&) = delete;
        Lease& operator=(const Lease&) = delete;

        Lease(Lease&& other) noexcept :
            slot_(other.slot_),
            object_(other.object_),
            valid_(other.valid_)
        {
            other.slot_ = nullptr;
            other.object_ = nullptr;
            other.valid_ = false;
        }

        Lease& operator=(Lease&& other) noexcept {
            if (this != &other) {
                return_lease();
                this->slot_ = other.slot_;
                this->object_ = other.object_;
                this->valid_ = other.valid_;

                other.slot_ = nullptr;
                other.object_ = nullptr;
                other.valid_ = false;
            }
            return *this;
        }

        const T* operator->() const noexcept { return valid_? object_ : nullptr; }

        template<typename Index>
        decltype(auto) operator[](Index&& i) const
            requires requires(const T& obj, Index&& i) { obj[::std::forward<Index>(i)]; }
        {
            return (*object_)[::std::forward<Index>(i)];
        }

        Lease duplicate_lease() const noexcept {
            slot_->held_const++;
            return Lease{slot_, object_, LeaseKey{}};
        }

        void return_lease() noexcept {
            if (!valid_) {
                return;
            }
            valid_ = false;
            slot_->held_const--;
        }

        ~Lease() {
            return_lease();
        }

        [[nodiscard]] bool valid() const noexcept { return valid_; }
    };

    using SymbolId = Id<Symbol>;
    using InstanceId = Id<Instance>;
    using FolderId = Id<LibraryFolder>;
    using LibraryId = Id<Library>;
    using FrameId = Id<Frame>;
    using AnimLayerId = Id<AnimatedLayer>;
    using DrawingId = Id<Drawing>;
}
