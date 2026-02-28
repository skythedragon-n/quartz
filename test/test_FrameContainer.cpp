//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 2/7/26.
//

#include <quartz/core/FrameContainer.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Append & remove frames", "[frame container]") {
    ::quartz::core::FrameContainer<int> test_container;

    REQUIRE(test_container.size() == 0);

    SECTION("Append frames") {
        test_container.append_frame(1);
        REQUIRE(test_container.size() == 1);

        test_container.append_frame(2, 4);

        REQUIRE(test_container.size() == 5);

        test_container.append_frame(3, 2);

        REQUIRE(test_container.size() == 7);

        ::std::vector expected{1, 2, 2, 2, 2, 3, 3};
        ::std::vector<size_t> expected_from_first{0, 0, 1, 2, 3, 0, 1};
        ::std::vector<size_t> expected_to_next{1, 4, 3, 2, 1, 2, 1};

        for (size_t i = 0; i < test_container.size(); ++i) {
            int actual = ::std::get<0>(test_container[i]);

            REQUIRE(actual == expected[i]);

            size_t actual_from_first = ::std::get<1>(test_container[i]);

            REQUIRE(actual_from_first == expected_from_first[i]);

            size_t actual_to_next = ::std::get<2>(test_container[i]);

            REQUIRE(actual_to_next == expected_to_next[i]);
        }
    }
}
