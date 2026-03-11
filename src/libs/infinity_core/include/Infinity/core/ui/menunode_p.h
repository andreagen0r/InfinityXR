/// @file menunode_p.h
/// @brief Private structural node for hierarchical menu building.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QString>
#include <vector>
#include <memory>

#include <Infinity/core/core_export.h>

/// @struct MenuNode
/// @brief Represents a single element in a hierarchical menu tree.
struct CORE_EXPORT MenuNode {

    QString text;                                    ///< Display text for this node.
    QString commandId;                               ///< Target command ID (if it's an executable action).
    bool isSeparator = false;                        ///< True if this node is just a visual separator.
    MenuNode* parent = nullptr;                      ///< Pointer to the parent node.

    std::vector<std::unique_ptr<MenuNode>> children; ///< Owned child nodes.

    /// @brief Locates a child node by its text name.
    MenuNode* findChild( const QString& name ) const {

        auto it = std::find_if( children.begin(), children.end(), [&]( const auto& n ) {
            return n->text == name;
        } );

        return it != children.end() ? it->get() : nullptr;
    }

    /// @brief Appends a new sub-menu node.
    MenuNode* addChild( const QString& name ) {

        auto node = std::make_unique<MenuNode>();
        node->text = name;
        node->parent = this;

        auto* nodePtr = node.get();
        children.push_back( std::move( node ) );

        return nodePtr;
    }

    /// @brief Appends a separator node.
    MenuNode* addSeparator() {

        auto node = std::make_unique<MenuNode>();
        node->isSeparator = true;
        node->parent = this;

        auto* nodePtr = node.get();
        children.push_back( std::move( node ) );

        return nodePtr;
    }

    /// @brief Calculates the index row of this node relative to its parent.
    int row() const {

        if ( !parent ) {
            return 0;
        }

        auto it = std::find_if( parent->children.begin(), parent->children.end(), [&]( const auto& p ) {
            return p.get() == this;
        } );

        return static_cast<int>( std::distance( parent->children.begin(), it ) );
    }
};
