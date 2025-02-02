// SPDX-FileCopyrightText: 2021-2023 Robin Lindén <dev@robinlinden.eu>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef STYLE_STYLED_NODE_H_
#define STYLE_STYLED_NODE_H_

#include "css/property_id.h"
#include "dom/dom.h"
#include "gfx/color.h"
#include "util/string.h"

#include <algorithm>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

namespace style {

enum class DisplayValue {
    None,
    Inline,
    Block,
};

enum class FontStyle {
    Normal,
    Italic,
    Oblique,
};

struct StyledNode {
    dom::Node const &node;
    std::vector<std::pair<css::PropertyId, std::string>> properties;
    std::vector<StyledNode> children;
    StyledNode const *parent{nullptr};

    std::string_view get_raw_property(css::PropertyId) const;

    template<css::PropertyId T>
    auto get_property() const {
        // Some of these branches have the same content, but we still want to
        // keep related properties grouped together and away from unrelated
        // ones, e.g. all border-<side>-color properties in the same branch.
        // NOLINTNEXTLINE(bugprone-branch-clone)
        if constexpr (T == css::PropertyId::BackgroundColor) {
            return get_color_property(T);
        } else if constexpr (T == css::PropertyId::BorderBottomColor || T == css::PropertyId::BorderLeftColor
                || T == css::PropertyId::BorderRightColor || T == css::PropertyId::BorderTopColor) {
            return get_color_property(T);
        } else if constexpr (T == css::PropertyId::Color) {
            return get_color_property(T);
        } else if constexpr (T == css::PropertyId::Display) {
            return get_display_property();
        } else if constexpr (T == css::PropertyId::FontFamily) {
            auto raw_font_family = get_raw_property(T);
            auto families = util::split(raw_font_family, ",");
            std::ranges::for_each(families, [](auto &family) { family = util::trim(family); });
            return families;
        } else if constexpr (T == css::PropertyId::FontSize) {
            return get_font_size_property();
        } else if constexpr (T == css::PropertyId::FontStyle) {
            return get_font_style_property();
        } else {
            return get_raw_property(T);
        }
    }

private:
    gfx::Color get_color_property(css::PropertyId) const;
    DisplayValue get_display_property() const;
    FontStyle get_font_style_property() const;
    int get_font_size_property() const;
};

[[nodiscard]] inline bool operator==(style::StyledNode const &a, style::StyledNode const &b) noexcept {
    return a.node == b.node && a.properties == b.properties && a.children == b.children;
}

inline std::string_view dom_name(StyledNode const &node) {
    return std::get<dom::Element>(node.node).name;
}

inline std::vector<StyledNode const *> dom_children(StyledNode const &node) {
    std::vector<StyledNode const *> children{};
    for (auto const &child : node.children) {
        if (!std::holds_alternative<dom::Element>(child.node)) {
            continue;
        }

        children.push_back(&child);
    }
    return children;
}

} // namespace style

#endif
