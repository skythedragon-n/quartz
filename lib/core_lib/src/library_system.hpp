//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 16 Feb 2026.
//

#pragma once

#include <expected>
#include <string>
#include <quartz/core/core_errors.hpp>
#include <quartz/core/id_sys.hpp>
#include <quartz/core/AnimFile.hpp>

namespace quartz::lib::core_lib {
    ::std::expected<core::SymbolId, core::FindFailure> find_symbol_in_folder(
        core::AnimFile& file,
        core::FolderId folder,
        const ::std::string& path);

    ::std::expected<core::FolderId, core::FindFailure> find_folder_in_folder(
        core::AnimFile& file,
        core::FolderId folder,
        const ::std::string& path);

    ::std::expected<core::FolderId, ::std::variant<core::ResolveFailure, core::AddFailure>> add_folder(
        core::AnimFile& file,
        core::FolderId parent,
        ::std::string name);

    ::std::expected<core::SymbolId, ::std::variant<core::ResolveFailure, core::AddFailure>> add_symbol(
        core::AnimFile& file,
        core::FolderId parent,
        ::std::string name);

    ::std::expected<void, ::std::variant<core::ResolveFailure, core::RenameFailure>> rename_symbol(
        core::AnimFile& file,
        core::SymbolId symbol,
        ::std::string new_name);

    ::std::expected<void, ::std::variant<core::ResolveFailure, core::RenameFailure>> rename_folder(
        core::AnimFile& file,
        core::FolderId folder,
        const ::std::string& new_name);

    ::std::expected<core::SymbolId, core::FindFailure> find_symbol(
        core::AnimFile& file,
        ::std::string path);

    ::std::expected<core::FolderId, core::FindFailure> find_folder(
        core::AnimFile& file,
        ::std::string path);
}
