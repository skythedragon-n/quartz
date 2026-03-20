//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 19 Mar 2026.
//

#include "metaphase.hpp"

namespace quartz::lib::parser {
    ::std::expected<void, MetaphaseError> metaphase(core::AnimFile& file, ::pugi::xml_document& doc) {
        ::pugi::xml_node doc_root = doc.child("quartz_document");

        if (!doc_root) {
            return ::std::unexpected(MetaphaseError::InvalidDocument);
        }

        ::pugi::xml_attribute spec_version = doc_root.attribute("spec");

        if (!spec_version) {
            return ::std::unexpected(MetaphaseError::InvalidDocument);
        }

        if (spec_version.value() != "0.1") {
            return ::std::unexpected(MetaphaseError::SpecTooNew);
        }

        ::pugi::xml_node size = doc_root.child("size");
        ::pugi::xml_node framerate = doc_root.child("framerate");
        ::pugi::xml_node background = doc_root.child("background");

        if (!size || !framerate || !background) {
            return ::std::unexpected(MetaphaseError::InvalidDocument);
        }

        ::pugi::xml_attribute width = size.attribute("width");
        ::pugi::xml_attribute height = size.attribute("height");

        if (!width || !height) {
            return ::std::unexpected(MetaphaseError::InvalidDocument);
        }

        file.set_size(width.as_ullong(), height.as_ullong());

        ::pugi::xml_attribute fps = framerate.attribute("fps");

        if (!fps) {
            return ::std::unexpected(MetaphaseError::InvalidDocument);
        }

        file.set_fps(fps.as_ullong());

        return {};
    }
}