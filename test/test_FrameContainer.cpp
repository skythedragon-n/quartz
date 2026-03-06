//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#include <iostream>
#include <quartz/core/FrameContainer.hpp>
#include <catch2/catch_test_macros.hpp>

#define REQUIRE_ERROR_TYPE(res, type) \
REQUIRE_FALSE(res.has_value()); \
REQUIRE(std::holds_alternative<type>(res.error()))

bool check_contents(quartz::core::FrameContainer<int>& container,
                    const std::vector<int>& expected,
                    const std::vector<size_t>& expected_from_first,
                    const std::vector<size_t>& expected_to_next) {
    if (container.size() != expected.size()) {
        return false;
    }

    if (expected.size() != expected_from_first.size() || expected.size() != expected_to_next.size()) {
        return false;
    }

    bool equivalent = true;

    for (size_t i = 0; i < container.size(); ++i) {
        auto [content, from_first, to_next] = container[i];
        if (content != expected[i] ||
            from_first != expected_from_first[i] ||
            to_next != expected_to_next[i])
        {
            equivalent = false;
            break;
        }
    }

    if (!equivalent) {
        for (size_t i = 0; i < container.size(); ++i) {
            auto [content, from_first, to_next] = container[i];
            std::println(std::cout, "{}: E {} {} {} A {} {} {}", i, expected[i], expected_from_first[i], expected_to_next[i], content, from_first, to_next);
        }
    }

    return equivalent;
}

TEST_CASE("Append Frames", "[FrameContainer]") {
    ::quartz::core::FrameContainer<int> test_container;

    CHECK(test_container.size() == 0);

    SECTION("Append single frame") {
        test_container.append_frame(1);
        CHECK(test_container.size() == 1);

        ::std::vector expected{1};
        ::std::vector<size_t> expected_from_first{0};
        ::std::vector<size_t> expected_to_next{1};

        check_contents(test_container, expected, expected_from_first, expected_to_next);
    }

    SECTION("Append multiple frames") {
        test_container.append_frame(1);
        test_container.append_frame(2, 4);

        CHECK(test_container.size() == 5);

        test_container.append_frame(3, 2);

        CHECK(test_container.size() == 7);

        ::std::vector expected{1, 2, 2, 2, 2, 3, 3};
        ::std::vector<size_t> expected_from_first{0, 0, 1, 2, 3, 0, 1};
        ::std::vector<size_t> expected_to_next{1, 4, 3, 2, 1, 2, 1};

        check_contents(test_container, expected, expected_from_first, expected_to_next);
    }
}

TEST_CASE("Insert frames", "[FrameContainer]") {
    using ::quartz::core::FrameContainer;
    FrameContainer<int> test_container;

    REQUIRE(test_container.size() == 0);

    SECTION("Insert frames at end") {

        FrameContainer<int>::InsertResult res = test_container.insert_frame(2, 0);
        REQUIRE_ERROR_TYPE(res, FrameContainer<int>::NoFrames);

        res = test_container.insert_frame(0, 1);
        if (auto* success = ::std::get_if<FrameContainer<int>::AddedToEnd>(&res.value())) {
            CHECK(success->count == 1);
        } else {
            FAIL("Expected FrameContainer::AddedToEnd{1}, not an error");
        }

        res = test_container.insert_frame(5, 2);
        if (auto* success = ::std::get_if<FrameContainer<int>::AddedToEnd>(&res.value())) {
            CHECK(success->count == 5);
        } else {
            FAIL("Expected FrameContainer::AddedToEnd{5}, not an error");
        }

        ::std::vector expected{1, 1, 1, 1, 1, 2};
        ::std::vector<size_t> expected_from_first{0, 1, 2, 3, 4, 0};
        ::std::vector<size_t> expected_to_next{5, 4, 3, 2, 1, 1};

        REQUIRE(check_contents(test_container, expected, expected_from_first, expected_to_next));
    }

    SECTION("Insert frames in middle") {
        test_container.append_frame(1, 5);
        test_container.append_frame(2);

        FrameContainer<int>::InsertResult res = test_container.insert_frame(3, 3);
        REQUIRE(res.has_value());
        REQUIRE(::std::holds_alternative<FrameContainer<int>::Normal>(*res));

        res = test_container.insert_frame(0, 1);
        REQUIRE_ERROR_TYPE(res, FrameContainer<int>::SameFrames);

        res = test_container.insert_frame(3, 4);

        ::std::vector expected{1, 1, 1, 4, 4, 2};
        ::std::vector<size_t> expected_from_first{0, 1, 2, 0, 1, 0};
        ::std::vector<size_t> expected_to_next{3, 2, 1, 2, 1, 1};

        REQUIRE(check_contents(test_container, expected, expected_from_first, expected_to_next));
    }

    SECTION("Run stitching") {
        test_container.append_frame(1, 5);
        test_container.append_frame(2);

        FrameContainer<int>::InsertResult res = test_container.insert_frame(3, 2);

        res = test_container.insert_frame(1, 3);
        res = test_container.insert_frame(1, 2);

        ::std::vector expected{1, 2, 2, 2, 2, 2};
        ::std::vector<size_t> expected_from_first{0, 0, 1, 2, 3, 4};
        ::std::vector<size_t> expected_to_next{1, 5, 4, 3, 2, 1};

        REQUIRE(check_contents(test_container, expected, expected_from_first, expected_to_next));
    }
}

TEST_CASE("Replace frame", "[FrameContainer]") {
    using ::quartz::core::FrameContainer;
    FrameContainer<int> test_container;

    test_container.append_frame(1, 3);
    test_container.append_frame(2, 3);
    test_container.append_frame(3, 1);

    test_container.replace_frame(3, 1);
    test_container.replace_frame(0, 3);

    test_container.insert_frame(3, 1);

    test_container.replace_frame(4, 2);

    std::vector expected = {3, 3, 3, 2, 2, 2, 2};
    std::vector<size_t> expected_from_first = {0, 1, 2, 0, 1, 2, 3};
    std::vector<size_t> expected_to_next = {3, 2, 1, 4, 3, 2, 1};
}

TEST_CASE("Remove Frames", "[FrameContainer]") {
    using ::quartz::core::FrameContainer;
    FrameContainer<int> test_container;

    test_container.append_frame(1, 3);
    test_container.append_frame(2, 3);
    test_container.append_frame(3, 1);

    test_container.remove_frame(3);

    std::vector expected = {1, 1, 1, 2, 2, 2, 2};
    std::vector<size_t> expected_from_first = {0, 1, 2, 0, 1, 2, 3};
    std::vector<size_t> expected_to_next = {3, 2, 1, 4, 3, 2, 1};

    REQUIRE(check_contents(test_container, expected, expected_from_first, expected_to_next));
}

TEST_CASE("Trim container", "[FrameContainer]") {
    using ::quartz::core::FrameContainer;
    FrameContainer<int> test_container;

    test_container.append_frame(1, 3);
    test_container.append_frame(2, 3);

    test_container.trim_to_size(4);

    std::vector expected = {1, 1, 1, 2};
    std::vector<size_t> expected_from_first = {0, 1, 2, 0};
    std::vector<size_t> expected_to_next = {3, 2, 1, 1};

    REQUIRE(check_contents(test_container, expected, expected_from_first, expected_to_next));
}
