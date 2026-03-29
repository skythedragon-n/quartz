//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENCE
//
// Created by skythedragon on 2/7/26.
//

#include "Library.hpp"

#include <utility>

#include "LibraryFolder.hpp"

namespace quartz::core {
    Library::Library(IdKey, AnimFile* file, ::std::string group, LibraryId id) :
    root_(id),
    group_(::std::move(group)),
    file_(file)
    {}

    ::std::expected<void, AddFailure> Library::add_symbol(const ::std::string& name, SymbolId id) {
        return root_.add_symbol(name, id);
    }

    ::std::optional<SymbolId> Library::remove_symbol(const ::std::string& name) {
        return root_.remove_symbol(name);
    }

    ::std::optional<SymbolId> Library::find_symbol(const ::std::string& name) const {
        return root_.find_symbol(name);
    }

    ::std::expected<void, AddFailure> Library::add_folder(const ::std::string& name, FolderId id) {
        return root_.add_folder(name, id);
    }

    ::std::optional<FolderId> Library::remove_folder(const ::std::string& name) {
        return root_.remove_folder(name);
    }

    ::std::optional<FolderId> Library::find_folder(const ::std::string& name) const {
        return root_.find_folder(name);
    }

    ::std::optional<::std::variant<SymbolId, FolderId>> Library::find(const ::std::string& name) const {
        return root_.find(name);
    }

    ::std::optional<::std::variant<SymbolId, FolderId>> Library::remove(const ::std::string& name) {
        return root_.remove(name);
    }

    ::std::expected<void, RenameFailure> Library::rename(const ::std::string& old_name, const ::std::string& new_name) {
        return root_.rename(old_name, new_name);
    }

    void Library::set_group(const ::std::string& group) {
        group_ = group;
    }

    bool Library::available(const std::string& name) const {
        return root_.available(name);
    }
}
