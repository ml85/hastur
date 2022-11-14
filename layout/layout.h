// SPDX-FileCopyrightText: 2021-2022 Robin Lindén <dev@robinlinden.eu>
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef LAYOUT_LAYOUT_H_
#define LAYOUT_LAYOUT_H_

#include "layout/box_model.h"

#include "css/property_id.h"
#include "geom/geom.h"
#include "style/styled_node.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace layout {

enum class LayoutType {
    Inline,
    Block,
    AnonymousBlock, // Holds groups of sequential inline boxes.
};

struct LayoutBox {
    style::StyledNode const *node;
    LayoutType type;
    BoxModel dimensions;
    std::vector<LayoutBox> children;
    [[nodiscard]] bool operator==(LayoutBox const &) const = default;

    template<css::PropertyId T>
    std::optional<std::string_view> get_property() const;
};

LayoutBox create_layout(style::StyledNode const &node, int width);

LayoutBox const *box_at_position(LayoutBox const &, geom::Position);

std::string to_string(LayoutBox const &box);

template<css::PropertyId T>
std::optional<std::string_view> LayoutBox::get_property() const {
    if (!node) {
        return std::nullopt;
    }

    return node->get_property<T>();
}

} // namespace layout

#endif
