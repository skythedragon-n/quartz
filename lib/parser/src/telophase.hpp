//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 22 Mar 2026.
//

#pragma once
#include <expected>

#include <quartz/core/id_sys.hpp>

#include <pugixml.hpp>

#include "quartz/core/Symbol.hpp"

/**
 * @file telophase.hpp
 *
 * @authors SkyTheDragon
 *
 * Contains the last phase of file parsing, the telophase, which finally parses in all of the symbol data, now
 * that all of the symbols exist & can be found by path resolution
 */
namespace quartz::lib::parser {
    namespace telophase_errors {
        namespace parse_error {
        }
        namespace document_problem {
            struct NoRoot {};

            struct SymbolMissingType {
                ::pugi::xml_node offender;
            };

            struct SymbolHasInvalidType {
                ::pugi::xml_node offender;
                ::std::string type;
            };

            struct InstanceMissingSymbol {
                ::pugi::xml_node offender;
            };

            struct InstanceUnknownRef {
                ::pugi::xml_node offender;
                core::InstanceId instance;
            };

            struct FieldParseError {
                ::pugi::xml_attribute offender;
                ::std::string problem;
            };
        }

        namespace document_mismatch {
            struct LibraryNotFound {
                ::std::string library_name;
            };

            struct FolderNotFound {
                ::std::string path;
            };

            struct SymbolNotFound {
                ::std::string path;
            };
        }

        /**
         * @brief Variant between all document problems
         */
        using InvalidDocument = ::std::variant<
            document_problem::NoRoot,
            document_problem::SymbolMissingType,
            document_problem::SymbolHasInvalidType,
            document_problem::InstanceMissingSymbol,
            document_problem::InstanceUnknownRef,
            document_problem::FieldParseError>;
        using InputMismatch = ::std::variant<
            document_mismatch::LibraryNotFound,
            document_mismatch::FolderNotFound,
            document_mismatch::SymbolNotFound>;
    }

    using TelophaseError = ::std::variant<
        ::std::vector<telophase_errors::InvalidDocument>,
        telophase_errors::InputMismatch
    >;
    using TelophaseErrorL = ::std::vector<telophase_errors::InvalidDocument>;

    struct PartialFieldParseError {
        ::std::string problem;
    };

    /**
     * @brief Parses symbol data
     * @param file File to perform operations on
     * @param doc Document to parse
     * @return Nothing or Error
     * Searches the library tree recursively, in each symbol, parsing in the type and attributes, along with
     * all the symbol's data, until every symbol has been parsed.
     */
    ::std::expected<void, TelophaseError> telophase(
        core::AnimFile& file,
        ::pugi::xml_document& doc
    );

    /**
     * @brief Parses the data of symbols in this library
     * @param file File to perform operations on
     * @param library Id of library to parse symbols in
     * @param library_node Libraries XML node
     * @return Nothing, or error
     * Searches this library recursively for symbols, and parses their data
     */
    ::std::expected<void, TelophaseError> telophase_parse_library(
        core::AnimFile& file,
        core::LibraryId library,
        ::pugi::xml_node library_node);

    ::std::expected<void, TelophaseError> telophase_parse_folder(
        core::AnimFile& file,
        core::FolderId folder_id,
        ::pugi::xml_node folder_node);

    ::std::expected<void, TelophaseError> telophase_parse_symbol(
        core::AnimFile& file,
        core::SymbolId symbol_id,
        ::pugi::xml_node symbol_node);

    ::std::expected<void, TelophaseError> telophase_parse_layered_anim_symbol(
        core::AnimFile& file,
        core::SymbolId symbol_id,
        ::pugi::xml_node symbol_node);

    ::std::expected<void, TelophaseError> telophase_parse_drawing_symbol(
        core::AnimFile& file,
        core::SymbolId symbol_id,
        ::pugi::xml_node symbol_node);

    ::std::expected<void, TelophaseError> telophase_parse_anim_layer(
        core::AnimFile& file,
        core::Symbol* symbol_p,
        core::AnimLayerId layer_id,
        ::pugi::xml_node layer_node);

    ::std::expected<void, TelophaseError> telophase_parse_frame(
        core::AnimFile& file,
        const core::FrameId& frame_id,
        ::pugi::xml_node frame_node);

    ::std::expected<void, TelophaseErrorL> telophase_parse_drawing(
        core::AnimFile& file,
        const core::DrawingId& drawing_id,
        ::pugi::xml_node drawing_node);

    ::std::expected<void, TelophaseErrorL> telophase_parse_stroke(
        core::Drawing* stroke,
        ::pugi::xml_node stroke_node);

    ::std::expected<::std::vector<core::Point>, PartialFieldParseError> parse_points(::std::string_view points_list_str);
    ::std::expected<::std::vector<core::BezierSection>, PartialFieldParseError> parse_bezier_sections(::std::string_view bezier_sections_list_str);
    ::std::expected<::std::pair<core::Point, size_t>, PartialFieldParseError> parse_point(::std::string_view point_str);
    ::std::expected<core::Color, void> parse_color(::std::string_view color_str);
    ::std::expected<core::num_t, void> parse_len_unit(::std::string_view len_unit_str);
}
