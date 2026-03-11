/// @file CommandDescriptor.h
/// @brief Metadata structure for defining UI/System commands.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <functional>

#include <QString>
#include <QStringList>
#include <QKeySequence>
#include <QDebug>

#include <Infinity/sdk/infinitysdk_export.h>
#include <Infinity/sdk/types/CommandId.h>

namespace qone::sdk {

/// @typedef CommandAction
/// @brief Alias for a parameterless void function used to execute commands.
using CommandAction = std::function<void()>;

/// @typedef StatePredicate
/// @brief Alias for a predicate function evaluating the state of a command (e.g., enabled/checked).
using StatePredicate = std::function<bool()>;


/// @struct CommandDescriptor
/// @brief Describes a command that can be invoked via UI, shortcuts, or scripts.
struct INFINITYSDK_EXPORT CommandDescriptor {

    CommandId id;               ///< Unique command identifier (e.g., "monitor.view.toggle_grid").
    QString text;               ///< Display text in the UI (e.g., "Show Grid").
    QString iconName;           ///< Theme icon name (e.g., "view-grid-symbolic").
    QString description;        ///< Extended description, typically used for tooltips.

    QKeySequence shortcut = {}; ///< Associated keyboard shortcut.

    bool separator = false;     ///< True if this item represents a menu separator.
    bool checkable = false;     ///< True if the command acts as a toggle.

                                /// @brief Lambda to query the current checked state (if checkable).
    /// Defaults to returning false.
    StatePredicate isChecked = [] {
        return false;
    };

    /// @brief Lambda to query if the command is currently enabled.
    /// Defaults to returning true.
    StatePredicate isEnabled = [] {
        return true;
    };

    QList<QString> menuPath; ///< Hierarchical path for UI menu placement.
};

/// @brief Custom QDebug formatting for CommandDescriptor.
inline QDebug operator<<( QDebug dbg, const CommandDescriptor& cd ) {
    QDebugStateSaver saver( dbg );
    dbg.nospace() << "CommandDescriptor("
                  << "id=" << cd.id.value << ", "
                  << "text=" << cd.text << ", "
                  << "iconName=" << cd.iconName << ", "
                  << "description" << cd.description << ", "
                  << "shortcut" << cd.shortcut.toString() << ", "
                  << "separator" << cd.separator << ", "
                  << "checkable" << cd.checkable << ", "
                  << "menuPath" << cd.menuPath << ")";
    return dbg;
}

} // namespace qone::sdk
