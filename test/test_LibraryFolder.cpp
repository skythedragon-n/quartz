//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 29 Mar 2026.
//

#include <quartz/core/AnimFile.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("LibraryFolder add/find/remove folder", "[LibraryFolder]") {
    using namespace quartz;

    core::AnimFile file;
    core::LibraryId library_id = *file.add_library("test");
    core::Library* library_p = *file.libraries.resolve(library_id);

    core::FolderId folder_id = file.folders.add("folder", library_id);

    SECTION("Add a folder and find it") {
        REQUIRE(library_p->add_folder("folder", folder_id));
        REQUIRE(library_p->find_folder("folder") == folder_id);
        REQUIRE(library_p->find("folder").has_value());
        REQUIRE(std::holds_alternative<core::FolderId>(*library_p->find("folder")));
        REQUIRE(library_p->available("folder") == false);
    }

    SECTION("Remove a folder") {
        REQUIRE(library_p->add_folder("folder", folder_id));

        auto removed = library_p->remove_folder("folder");
        REQUIRE(removed.has_value());
        REQUIRE(*removed == folder_id);

        REQUIRE_FALSE(library_p->find_folder("folder").has_value());
        REQUIRE_FALSE(library_p->find("folder").has_value());
        REQUIRE(library_p->available("folder"));
    }

    SECTION("Removing nonexistent folder returns empty") {
        REQUIRE_FALSE(library_p->remove_folder("missing").has_value());
    }
}

TEST_CASE("LibraryFolder add/find/remove symbol", "[LibraryFolder]") {
    using namespace quartz;

    core::AnimFile file;
    core::LibraryId library_id = *file.add_library("test");
    core::Library* library_p = *file.libraries.resolve(library_id);

    core::SymbolId symbol_id = file.symbols.add("symbol", library_id);
    SECTION("Add a symbol and find it") {
        REQUIRE(library_p->add_symbol("symbol", symbol_id));
        REQUIRE(library_p->find_symbol("symbol") == symbol_id);
        REQUIRE(library_p->find("symbol").has_value());
        REQUIRE(std::holds_alternative<core::SymbolId>(*library_p->find("symbol")));
        REQUIRE(library_p->available("symbol") == false);
    }

    SECTION("Remove a symbol") {
        REQUIRE(library_p->add_symbol("symbol", symbol_id));

        auto removed = library_p->remove_symbol("symbol");
        REQUIRE(removed.has_value());
        REQUIRE(*removed == symbol_id);

        REQUIRE_FALSE(library_p->find_symbol("symbol").has_value());
        REQUIRE_FALSE(library_p->find("symbol").has_value());
        REQUIRE(library_p->available("symbol"));
    }

    SECTION("Removing nonexistent symbol returns empty") {
        REQUIRE_FALSE(library_p->remove_symbol("missing").has_value());
    }
}

TEST_CASE("LibraryFolder name collisions", "[LibraryFolder]") {
    using namespace quartz;

    core::AnimFile file;
    core::LibraryId library_id = *file.add_library("test");
    core::Library* library_p = *file.libraries.resolve(library_id);

    core::FolderId folder_id = file.folders.add("folder", library_id);
    core::SymbolId symbol_id = file.symbols.add("symbol", folder_id);

    REQUIRE(library_p->add_folder("folder", folder_id));

    SECTION("Cannot add duplicate folder name") {
        REQUIRE_FALSE(library_p->add_folder("folder", folder_id));
        REQUIRE(library_p->add_folder("other", folder_id));
        REQUIRE_FALSE(library_p->add_folder("other", folder_id));
    }

    SECTION("Cannot add symbol with existing folder name") {
        REQUIRE_FALSE(library_p->add_symbol("folder", symbol_id));
    }

    SECTION("Cannot add duplicate symbol name") {
        REQUIRE(library_p->add_symbol("symbol", symbol_id));
        REQUIRE_FALSE(library_p->add_symbol("symbol", symbol_id));
    }

    SECTION("Cannot add folder with existing symbol name") {
        REQUIRE(library_p->add_symbol("symbol", symbol_id));
        REQUIRE_FALSE(library_p->add_folder("symbol", folder_id));
    }
}

TEST_CASE("LibraryFolder rename", "[LibraryFolder]") {
    using namespace quartz;

    core::AnimFile file;
    core::LibraryId library_id = *file.add_library("test");
    core::Library* library_p = *file.libraries.resolve(library_id);

    core::FolderId folder_id = file.folders.add("folder", library_id);
    core::SymbolId symbol_id = file.symbols.add("symbol", folder_id);

    REQUIRE(library_p->add_folder("folder", folder_id));
    REQUIRE(library_p->add_symbol("symbol", symbol_id));

    SECTION("Rename folder") {
        REQUIRE(library_p->rename("folder", "renamed"));
        REQUIRE_FALSE(library_p->find_folder("folder").has_value());
        REQUIRE(library_p->find_folder("renamed") == folder_id);
        REQUIRE_FALSE(library_p->available("renamed"));
        REQUIRE(library_p->available("folder"));
    }

    SECTION("Rename symbol") {
        REQUIRE(library_p->rename("symbol", "renamed_symbol"));
        REQUIRE_FALSE(library_p->find_symbol("symbol").has_value());
        REQUIRE(library_p->find_symbol("renamed_symbol") == symbol_id);
    }

    SECTION("Rename to existing name fails") {
        REQUIRE_FALSE(library_p->rename("folder", "symbol").has_value());
        REQUIRE_FALSE(library_p->rename("symbol", "folder").has_value());
    }

    SECTION("Rename missing source fails") {
        REQUIRE_FALSE(library_p->rename("missing", "new_name").has_value());
    }
}

TEST_CASE("LibraryFolder remove by generic name", "[LibraryFolder]") {
    using namespace quartz;

    core::AnimFile file;
    core::LibraryId library_id = *file.add_library("test");
    core::Library* library_p = *file.libraries.resolve(library_id);

    core::FolderId folder_id = file.folders.add("folder", library_id);
    core::SymbolId symbol_id = file.symbols.add("symbol", folder_id);

    REQUIRE(library_p->add_folder("folder", folder_id));
    REQUIRE(library_p->add_symbol("symbol", symbol_id));

    SECTION("Remove symbol via generic remove") {
        auto removed = library_p->remove("symbol");
        REQUIRE(removed.has_value());
        REQUIRE(std::holds_alternative<core::SymbolId>(*removed));
        REQUIRE(std::get<core::SymbolId>(*removed) == symbol_id);
        REQUIRE_FALSE(library_p->find_symbol("symbol").has_value());
    }

    SECTION("Remove folder via generic remove") {
        auto removed = library_p->remove("folder");
        REQUIRE(removed.has_value());
        REQUIRE(std::holds_alternative<core::FolderId>(*removed));
        REQUIRE(std::get<core::FolderId>(*removed) == folder_id);
        REQUIRE_FALSE(library_p->find_folder("folder").has_value());
    }

    SECTION("Remove missing item") {
        REQUIRE_FALSE(library_p->remove("missing").has_value());
    }
}

TEST_CASE("LibraryFolder available checks", "[LibraryFolder]") {
    using namespace quartz;

    core::AnimFile file;
    core::LibraryId library_id = *file.add_library("test");
    core::Library* library_p = *file.libraries.resolve(library_id);

    REQUIRE(library_p->available("free_name"));

    core::FolderId folder_id = file.folders.add("folder", library_id);
    REQUIRE(library_p->add_folder("folder", folder_id));
    REQUIRE_FALSE(library_p->available("folder"));

    REQUIRE(library_p->rename("folder", "folder_2"));
    REQUIRE(library_p->available("folder"));
    REQUIRE_FALSE(library_p->available("folder_2"));
}