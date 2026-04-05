//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 04 Apr 2026.
//

#include <quartz/core/id_sys.hpp>
#include <quartz/core/AnimFile.hpp>
#include <catch2/catch_test_macros.hpp>

namespace quartz::core {
    template<typename T>
    class IdTestHelpers {
    public:
        static size_t idx(Id<T> id) {
            return id.storage_.id;
        }

        static size_t gen(Id<T> id) {
            return id.storage_.gen;
        }

        static AnimFile* file(Id<T> id) {
            return id.storage_.file;
        }
    };
}

namespace core = quartz::core;

TEST_CASE("id_sys", "[id_sys]") {
    using Helper = core::IdTestHelpers<core::Frame>;

    core::IdContainer<core::Frame> objects{core::CHEETO};

    SECTION("Adding objects to end") {
        core::FrameId frame1 = objects.add();
        core::FrameId frame2 = objects.add();

        REQUIRE(frame1 != frame2);
        REQUIRE(Helper::gen(frame1) == Helper::gen(frame2));
        REQUIRE_FALSE(Helper::idx(frame1) == Helper::idx(frame2));
        REQUIRE(Helper::file(frame1) == core::CHEETO);
        REQUIRE(Helper::file(frame2) == core::CHEETO);
        REQUIRE(objects.size() == 2);
    }

    SECTION("Remove object") {
        core::FrameId frame = objects.add();

        objects.free(frame);

        auto res = objects.resolve(frame);

        REQUIRE_FALSE(res.has_value());

        REQUIRE(res.error() == core::ResolveFailure::TargetDeleted);
    }

    SECTION("Id recycling") {
        core::FrameId frame1 = objects.add();
        core::FrameId frame2 = objects.add();

        auto res1 = objects.free(frame1);

        REQUIRE(res1.has_value());

        core::FrameId frame3 = objects.add();

        REQUIRE_FALSE(Helper::idx(frame3) == 2);
        REQUIRE(Helper::idx(frame3) == Helper::idx(frame1));

        auto res2 = objects.resolve(frame1);

        REQUIRE(!res2.has_value());
        REQUIRE(res2.error() == core::ResolveFailure::TargetDeleted);
    }

    SECTION("Resolve Errors") {
        auto res = objects.resolve(core::FrameId::INVALID);

        REQUIRE_FALSE(res.has_value());
        REQUIRE(res.error() == core::ResolveFailure::InvalidId);

        core::IdContainer<core::Frame> other{core::FLAMING_HOT_CHEETO};

        core::FrameId frame = objects.add();
        other.add();

        auto res1 = other.resolve(frame);

        REQUIRE_FALSE(res1.has_value());
        REQUIRE(res1.error() == core::ResolveFailure::WrongFile);
    }

    SECTION("Free errors") {
        auto res = objects.free(core::FrameId::INVALID);

        REQUIRE_FALSE(res.has_value());
        REQUIRE(res.error() == core::ResolveFailure::InvalidId);

        core::IdContainer<core::Frame> other{core::FLAMING_HOT_CHEETO};

        core::FrameId frame = objects.add();
        other.add();

        auto res1 = other.free(frame);

        REQUIRE_FALSE(res1.has_value());
        REQUIRE(res1.error() == core::ResolveFailure::WrongFile);
    }
}
