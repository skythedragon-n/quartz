//
// copyleft 🄯 2026 by SkyTheDragon
// licenced under the GPLv3-or-later, details located in LICENSE
//
// Created by skythedragon on 22 Mar 2026.
//

#include "telophase.hpp"

#include <format>
#include <quartz/core/AnimFile.hpp>
#include <quartz/lib/core_lib/library_system.hpp>
#include <quartz/core/Instance.hpp>

#include <qtil/overloads.hpp>
#include <qtil/panic.hpp>



namespace quartz::lib::parser {
    ::std::optional<telophase_errors::InputMismatch> match_err(
        ::std::vector<telophase_errors::InvalidDocument>& problems,
        TelophaseError err) {
        return  ::qtil::match(err,
            [&problems](::std::vector<telophase_errors::InvalidDocument>& error) -> ::std::optional<telophase_errors::InputMismatch> {
                for (auto problem : error) {
                    problems.emplace_back(problem);
                }
                return ::std::nullopt;
            },
            [](telophase_errors::InputMismatch mismatch) -> ::std::optional<telophase_errors::InputMismatch> {
                return mismatch;
            }
        );
    }

    ::std::expected<void, TelophaseError> telophase(core::AnimFile& file, ::pugi::xml_document& doc) {
        using namespace telophase_errors::document_problem;
        using namespace telophase_errors::document_mismatch;
        ::std::vector<telophase_errors::InvalidDocument> problems;

        ::pugi::xml_node doc_root = doc.child("quartz-document");

        if (!doc_root) {
            problems.emplace_back(NoRoot{});
            return ::std::unexpected(problems);
        }

        for (const auto& [name, id] : file.libraries_i()) {
            auto library_nodes = doc_root.select_nodes(::std::format("./library[@group = \"{}\"]", name).c_str());

            if (library_nodes.empty()) {
                return ::std::unexpected(LibraryNotFound{name});
            }

            ::pugi::xml_node library_node = library_nodes.first().node();

            auto res = telophase_parse_library(file, id, library_node);

            if (!res) {
                auto is_err = match_err(problems, res.error());

                if (is_err) {
                    return ::std::unexpected(*is_err);
                }
            }
        }

        return {};
    }

    ::std::expected<void, TelophaseError> telophase_parse_library(
        core::AnimFile& file,
        core::LibraryId library,
        ::pugi::xml_node library_node) {
        using namespace telophase_errors::document_problem;
        using namespace telophase_errors::document_mismatch;
        ::std::vector<telophase_errors::InvalidDocument> problems;

        auto library_res = file.libraries.resolve(library);

        if (!library_res) {
            ::qtil::panic("Library's Id non-extant!!!! UH-OH!");
        }

        core::Library* library_p = *library_res;

        for (auto [name, id] : library_p->folders()) {
            auto folder_nodes = library_node.select_nodes(::std::format("./folder[@name = \"{}\"]", name).c_str());

            if (folder_nodes.empty()) {
                return ::std::unexpected(FolderNotFound{name});
            }

            ::pugi::xml_node folder_node = folder_nodes.first().node();

            auto res = telophase_parse_folder(file, id, folder_node);

            if (!res) {
                ::std::optional<telophase_errors::InputMismatch> is_err = match_err(problems, res.error());

                if (is_err) {
                    return ::std::unexpected(*is_err);
                }
            }
        }

        for (auto [name, id] : library_p->symbols()) {
            auto symbol_nodes = library_node.select_nodes(::std::format("./symbol[@name = \"{}\"]", name).c_str());

            if (symbol_nodes.empty()) {
                return ::std::unexpected(SymbolNotFound{name});
            }

            ::pugi::xml_node symbol_node = symbol_nodes.first().node();

            auto res = telophase_parse_symbol(file, id, symbol_node);

            if (!res) {
                ::std::optional<telophase_errors::InputMismatch> is_err = match_err(problems, res.error());

                if (is_err) {
                    return ::std::unexpected(*is_err);
                }
            }
        }

        if (!problems.empty()) {
            return ::std::unexpected(problems);
        }

        return {};
    }

    ::std::expected<void, TelophaseError> telophase_parse_folder(
        core::AnimFile& file,
        core::FolderId folder_id,
        ::pugi::xml_node folder_node) {
        using namespace telophase_errors::document_problem;
        using namespace telophase_errors::document_mismatch;
        ::std::vector<telophase_errors::InvalidDocument> problems;

        auto folder_res = file.folders.resolve(folder_id);

        if (!folder_res) {
            ::qtil::panic("Folder's Id non-existent. This is bad.");
        }

        core::LibraryFolder* folder_p = *folder_res;

        for (const auto& [name, id] : folder_p->folders()) {
            auto subfolder_nodes = folder_node.select_nodes(::std::format("./folder[@name = \"{}\"]", name).c_str());

            if (subfolder_nodes.empty()) {
                return ::std::unexpected(FolderNotFound{name});
            }

            ::pugi::xml_node subfolder_node = subfolder_nodes.first().node();

            core::FolderId subfolder_id = *folder_p->find_folder(name);

            auto res = telophase_parse_folder(file, folder_id, subfolder_node);

            if (!res) {
                ::std::optional<telophase_errors::InputMismatch> is_err = match_err(problems, res.error());

                if (is_err) {
                    return ::std::unexpected(*is_err);
                }
            }
        }

        for (const auto& [name, id] : folder_p->symbols()) {
            auto symbol_nodes = folder_node.select_nodes(::std::format("./symbol[@name = \"{}\"]", name).c_str());

            if (symbol_nodes.empty()) {
                return ::std::unexpected(SymbolNotFound{name});
            }

            ::pugi::xml_node symbol_node = symbol_nodes.first().node();

            core::SymbolId symbol_id = *folder_p->find_symbol(name);

            auto res = telophase_parse_symbol(file, symbol_id, symbol_node);

            if (!res) {
                ::std::optional<telophase_errors::InputMismatch> is_err = match_err(problems, res.error());

                if (is_err) {
                    return ::std::unexpected(*is_err);
                }
            }
        }

        return {};
    }

    ::std::expected<void, TelophaseError> telophase_parse_symbol(
        core::AnimFile& file,
        core::SymbolId symbol_id,
        ::pugi::xml_node symbol_node) {
        using namespace telophase_errors::document_problem;
        using namespace telophase_errors::document_mismatch;
        ::std::vector<telophase_errors::InvalidDocument> problems;

        auto symbol_res = file.symbols.resolve(symbol_id);

        if (!symbol_res) {
            ::qtil::panic("Symbol's id non-existent. Bad.");
        }

        auto type_res = symbol_node.attribute("type");

        if (!type_res) {
            problems.emplace_back(SymbolMissingType{symbol_node});
        }

        ::std::string type = type_res.value();

        if (type == "layered-anim" || type == "scene") {
            auto res = telophase_parse_layered_anim_symbol(file, symbol_id, symbol_node);

            if (!res) {
                auto is_err = match_err(problems, res.error());

                if (is_err) {
                    return ::std::unexpected(*is_err);
                }
            }
        } else if (type == "drawing") {
            auto res = telophase_parse_drawing_symbol(file, symbol_id, symbol_node);

            if (!res) {
                auto is_err = match_err(problems, res.error());

                if (is_err) {
                    return ::std::unexpected(*is_err);
                }
            }
        } else if (type == "void") {
            return {};
        } else {
            problems.emplace_back(SymbolHasInvalidType{symbol_node, type});
        }

        if (!problems.empty()) {
            return ::std::unexpected(problems);
        }

        return {};
    }


    ::std::expected<void, TelophaseError> telophase_parse_layered_anim_symbol(
        core::AnimFile& file,
        core::SymbolId symbol_id,
        ::pugi::xml_node symbol_node) {
        using namespace telophase_errors::document_problem;
        using namespace telophase_errors::document_mismatch;
        ::std::vector<telophase_errors::InvalidDocument> problems;

        auto symbol_res = file.symbols.resolve(symbol_id);

        if (!symbol_res) {
            ::qtil::panic("Symbol failed to resolve. Uh-oh.");
        }

        core::Symbol* symbol_p = *symbol_res;

        core::symbol_types::LayeredAnimation layers;

        for (::pugi::xml_node layer_node : symbol_node.children("layer")) {
            core::AnimLayerId layer_id = file.layers.add("", core::Color{113, 196, 243, 0}, symbol_id);

            auto res = telophase_parse_anim_layer(file, symbol_p, layer_id, layer_node);

            if (!res) {
                auto is_err = match_err(problems, res.error());

                if (is_err) {
                    return ::std::unexpected(*is_err);
                }
            }
        }

        if (!problems.empty()) {
            return ::std::unexpected(problems);
        }

        symbol_p->set(layers);

        return {};
    }

    std::expected<void, TelophaseError> telophase_parse_drawing_symbol(
        core::AnimFile& file,
        core::SymbolId symbol_id,
        pugi::xml_node symbol_node) {
        using namespace telophase_errors::document_problem;
        using namespace telophase_errors::document_mismatch;
        ::std::vector<telophase_errors::InvalidDocument> problems;

        auto symbol_res = file.symbols.resolve(symbol_id);

        if (!symbol_res) {
            ::qtil::panic("Symbol failed to resolve. Uh-oh.");
        }

        core::Symbol* symbol_p = *symbol_res;

        core::symbol_types::DrawingSymbol drawing_symbol{file.drawings.add()};

        auto drawing_parse_res = telophase_parse_drawing(file, drawing_symbol.drawing, symbol_node);

        if (!drawing_parse_res) {
            auto& err = drawing_parse_res.error();

            problems.insert(problems.end(), err.begin(), err.end());
        }

        if (!problems.empty()) {
            return ::std::unexpected(problems);
        }

        symbol_p->set(drawing_symbol);

        return {};
    }

    ::std::expected<void, TelophaseError> telophase_parse_anim_layer(
        core::AnimFile& file,
        core::Symbol* symbol_p,
        core::AnimLayerId layer_id,
        ::pugi::xml_node layer_node) {
        using namespace telophase_errors::document_problem;
        using namespace telophase_errors::document_mismatch;
        ::std::vector<telophase_errors::InvalidDocument> problems;

        auto layers_res = file.layers.resolve(layer_id);

        if (!layers_res) {
            ::qtil::panic("Layer id not found. This is bad.");
        }

        core::AnimatedLayer* layer_p = *layers_res;

        if (!layer_node.attribute("name")) {
            problems.push_back(FieldMissing {
                layer_node,
                "name"
            });
        }

        layer_p->set_name(layer_node.attribute("name").value());

        if (!layer_node.attribute("color")) {
            problems.push_back(FieldMissing {
                layer_node,
                "color"
            });
            layer_node.attribute("color").set_value("#000000");
        }

        ::std::string color_str = layer_node.attribute("color").value();

        auto color_res = parse_color(color_str);

        if (!color_res) {
            problems.push_back(FieldParseError {
                layer_node.attribute("color"),
                color_res.error().problem
            });
        }

        for (::pugi::xml_node frame_node : layer_node.children("frame")) {
            size_t len = frame_node.attribute("length").as_ullong(1);

            core::FrameId frame_id = file.frames.add();

            layer_p->frames.append_frame(frame_id, len);

            auto res = telophase_parse_frame(file, frame_id, frame_node);

            if (!res) {
                auto is_err = match_err(problems, res.error());

                if (is_err) {
                    return ::std::unexpected(*is_err);
                }
            }
        }

        if (!problems.empty()) {
            return ::std::unexpected(problems);
        }

        return {};
    }

    ::std::expected<void, TelophaseError> telophase_parse_frame(
        core::AnimFile& file,
        const core::FrameId& frame_id,
        const ::pugi::xml_node frame_node) {
        using namespace telophase_errors::document_problem;
        using namespace telophase_errors::document_mismatch;
        ::std::vector<telophase_errors::InvalidDocument> problems;

        const auto frame_res = file.frames.resolve(frame_id);

        if (!frame_res) {
            ::qtil::panic("Frame resolution failed!");
        }

        core::Frame* frame_p = *frame_res;

        auto [drawing_id, drawing_p] = file.drawings.add_wp();

        core::frame_types::Normal frame_content{.drawing = drawing_id};

        for (::pugi::xml_node instance_node : frame_node.children("instance")) {
            ::pugi::xml_attribute symbol_name_attr = instance_node.attribute("symbol");

            if (!symbol_name_attr) {
                problems.emplace_back(InstanceMissingSymbol{instance_node});
                continue;
            }

            ::std::string symbol_name = symbol_name_attr.value();
            auto [instance_id, instance_p] = file.instances.add_wp(core::SymbolId::INVALID, core::Transform{});

            auto symbol_res = core_lib::find_symbol(file, symbol_name);

            ::pugi::xml_attribute transform_attr = instance_node.attribute("transform");

            core::Transform transform;

            //TODO: Parse transforms

            core::SymbolId symbol_id;

            if (!symbol_res) {
                problems.emplace_back(InstanceUnknownRef{instance_node, instance_id});
            } else {
                symbol_id = *symbol_res;
            }

            instance_p->set_source(symbol_id);
            instance_p->set_transform(transform);


            frame_content.instances.emplace_back(instance_id);
        }

        if (!problems.empty()) {
            return ::std::unexpected(problems);
        }

        frame_p->set(frame_content);

        return {};
    }

    ::std::expected<void, TelophaseErrorL> telophase_parse_drawing(
        core::AnimFile& file,
        const core::DrawingId& drawing_id,
        const ::pugi::xml_node drawing_node) {
        using namespace telophase_errors::document_problem;
        using namespace telophase_errors::document_mismatch;
        ::std::vector<telophase_errors::InvalidDocument> problems;

        auto drawing_res = file.drawings.resolve(drawing_id);

        if (!drawing_res) {
            ::qtil::panic("Drawing resolution failed!");
        }

        core::Drawing* drawing_p = *drawing_res;

        for (::pugi::xml_node points_node : drawing_node.children("points")) {
            ::pugi::xml_attribute points_attribute = points_node.attribute("coords");

            ::std::vector<core::Point> points;

            if (points_attribute) {
                auto maybe_points = parse_points(points_attribute.value());

                if (!maybe_points) {
                    return ::std::unexpected(TelophaseErrorL{FieldParseError {
                        points_attribute,
                        maybe_points.error().problem
                    }});
                }

                points = *maybe_points;
            }

            for (const core::Point& point : points) {
                drawing_p->add_point(point);
            }
        }

        for (::pugi::xml_node stroke_node : drawing_node.children("stroke")) {
            auto res = telophase_parse_stroke(drawing_p, stroke_node);

            if (!res) {
                for (const auto& problem : res.error()) {
                    problems.emplace_back(problem);
                }
            }
        }

        if (!problems.empty()) {
            return ::std::unexpected(problems);
        }

        return {};
    }

    ::std::expected<void, TelophaseErrorL> telophase_parse_stroke(
        core::Drawing* stroke_drawing,
        ::pugi::xml_node stroke_node) {
        using namespace telophase_errors::document_problem;
        using namespace telophase_errors::document_mismatch;
        ::std::vector<telophase_errors::InvalidDocument> problems;

        ::pugi::xml_attribute color_attribute = stroke_node.attribute("color");

        if (!color_attribute) {
            color_attribute.set_value("#000000");
        }

        auto color_res = parse_color(color_attribute.value());

        if (!color_res) {
            return ::std::unexpected(TelophaseErrorL{FieldParseError {
                color_attribute,
                ::std::format("Invalid color: {}", color_attribute.value())
            }});
        }

        core::Color color = *color_res;

        ::pugi::xml_attribute thickness_attribute = stroke_node.attribute("thickness");

        if (!thickness_attribute) {
            problems.push_back( FieldMissing {
                stroke_node,
                "thickness"
            });
            thickness_attribute.set_value("0");
        }

        core::num_t thickness = core::parse_num(thickness_attribute.value());

        core::Stroke& stroke = stroke_drawing->add_stroke(color, thickness);

        ::pugi::xml_attribute sections_attribute = stroke_node.attribute("points");

        if (!sections_attribute) {
            problems.push_back( FieldMissing {
                stroke_node,
                "points"
            });
            sections_attribute.set_value("0");
        }

        auto maybe_sections = parse_bezier_sections(sections_attribute.value());

        if (!maybe_sections) {
            problems.push_back( FieldParseError {
                sections_attribute,
                maybe_sections.error().problem
            });
            maybe_sections = ::std::vector{ core::BezierSection {} };
        }

        stroke.add_sections(*maybe_sections);

        if (!problems.empty()) {
            return ::std::unexpected(problems);
        }

        return {};
    }

    std::expected<std::vector<core::Point>, PartialFieldParseError> parse_points(::std::string_view points_list_str) {
        using namespace telophase_errors::document_problem;
        ::std::vector<core::Point> points;

        size_t index = 0;

        if (points_list_str.empty()) {
            return points;
        }

        while (index < points_list_str.size()) {
            ::std::string_view point_str = points_list_str.substr(index);
            auto point_res = parse_point(point_str);
            auto [point, len] = *point_res;
            points.emplace_back(point);
            index += len;
            if (index == points_list_str.size()) {
                break;
            }
            if (points_list_str[index] != ',') {
                return ::std::unexpected( PartialFieldParseError {
                    ::std::format("Expected ',', got {}", points_list_str[index])
                });
            }
            index++;
        }

        return points;
    }

    std::expected<std::pair<core::Point, size_t>, PartialFieldParseError> parse_point(::std::string_view point_str) {
        size_t index = 0;

        if (point_str[index] != '(') {
            ::qtil::panic("Expected '('");
        }

        index++;

        ::std::string num_str;

        if (point_str[index] == '-') {
            num_str += point_str[index];
            index++;
        }

        while (index < point_str.size()) {
            if (std::isdigit(point_str[index]) || point_str[index] == '.') {
                num_str += point_str[index];
                index++;
            } else {
                break;
            }
        }

        if (num_str.empty() || num_str == "-") {
            return ::std::unexpected( PartialFieldParseError {
                "Expected a number"
            });
        }

        core::num_t x = core::parse_num(num_str);

        num_str.clear();

        if (index >= point_str.size() || point_str[index] != ',') {
            return ::std::unexpected( PartialFieldParseError {
                ::std::format("Expected ',', got {}", point_str[index])
            });
        }

        index++;

        while (index < point_str.size()) {
            if (::std::isdigit(point_str[index]) || point_str[index] == '.') {
                num_str += point_str[index];
                index++;
            } else {
                break;
            }
        }

        core::num_t y = core::parse_num(num_str);

        if (index >= point_str.size() || point_str[index] != ')') {
            return ::std::unexpected( PartialFieldParseError {
                ::std::format("Expected ')', got {}", point_str[index])
            });
        }

        index++;

        return ::std::make_pair(core::Point{x, y}, index);
    }

    std::expected<std::vector<core::BezierSection>, PartialFieldParseError> parse_bezier_sections(
        std::string_view bezier_sections_list_str
    ) {
        ::std::vector<core::BezierSection> sections;

        size_t index = 0;

        if (bezier_sections_list_str.empty()) {
            return sections;
        }

        while (index < bezier_sections_list_str.size()) {
            core::BezierSection section{};

            if (bezier_sections_list_str[index] == '(') {
                auto point_res = parse_point(bezier_sections_list_str.substr(index));

                if (!point_res) {
                    return ::std::unexpected(point_res.error());
                }

                auto [point, len] = *point_res;
                section.lastwise_tangent = point;
                index += len;
            }

            if (index >= bezier_sections_list_str.size() || !::std::isdigit(bezier_sections_list_str[index])) {
                return ::std::unexpected( PartialFieldParseError {
                    ::std::format("Expected a digit, got {}", bezier_sections_list_str[index])
                });
            }

            ::std::string start_str;

            while (index < bezier_sections_list_str.size() && ::std::isdigit(bezier_sections_list_str[index])) {
                start_str += bezier_sections_list_str[index];
                index++;
            }

            section.start = ::std::stoull(start_str);

            if (index < bezier_sections_list_str.size() && bezier_sections_list_str[index] == '(') {
                auto point_res = parse_point(bezier_sections_list_str.substr(index));

                if (!point_res) {
                    return ::std::unexpected( PartialFieldParseError {
                        "Failed to parse point"
                    });
                }

                auto [point, len] = *point_res;
                section.nextwise_tangent = point;
                index += len;
            }

            sections.emplace_back(section);

            if (index == bezier_sections_list_str.size()) {
                break;
            }

            if (bezier_sections_list_str[index] != ',') {
                return ::std::unexpected( PartialFieldParseError {
                    ::std::format("Expected ',', got {}", bezier_sections_list_str[index])
                });
            }

            index++;
        }

        return sections;
    }

    std::expected<core::Color, PartialFieldParseError> parse_color(std::string_view color_str) {
        auto hex_value = [](char c) -> ::std::optional<uint8_t> {
            if (c >= '0' && c <= '9') {
                return static_cast<uint8_t>(c - '0');
                }

            if (c >= 'a' && c <= 'f') {
                return static_cast<uint8_t>(c - 'a' + 10);
            }

            if (c >= 'A' && c <= 'F') {
                return static_cast<uint8_t>(c - 'A' + 10);
            }

            return ::std::nullopt;
        };

        auto parse_byte = [hex_value](char high, char low) -> ::std::optional<uint8_t> {
        auto high_value = hex_value(high);
        auto low_value = hex_value(low);

        if (!high_value || !low_value) {
                return ::std::nullopt;
            }

            return static_cast<uint8_t>((*high_value << 4) | *low_value);
        };

        if (color_str.size() != 9 || color_str[0] != '#') {
            return ::std::unexpected( PartialFieldParseError {
                ::std::format("Expected hex color in format #RRGGBBAA, but got {}", color_str)
            });
        }

        auto r = parse_byte(color_str[1], color_str[2]);
        auto g = parse_byte(color_str[3], color_str[4]);
        auto b = parse_byte(color_str[5], color_str[6]);
        auto a = parse_byte(color_str[7], color_str[8]);

        if (!r || !g || !b || !a) {
            return ::std::unexpected( PartialFieldParseError {
                ::std::format("Failed to parse color {}. Look, I'm too lazy to get my program to figure "
                              "out what the problem is RN, so you're smart, figure it out yourself", color_str)
            });
        }

        return core::Color{*r, *g, *b, *a};
    }
}
