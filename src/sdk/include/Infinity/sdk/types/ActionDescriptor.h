/// @file ActionDescriptor.h
/// @brief Metadata structure for defining executable actions.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <functional>

#include <QString>
#include <QKeySequence>
#include <QDebug>

#include <Infinity/sdk/infinitysdk_export.h>

#include "Types.h"

namespace qone::sdk {


/// @struct ActionDescriptor
/// @brief Encapsulates the configuration and execution logic of an Action.
struct INFINITYSDK_EXPORT ActionDescriptor {

    QString id;                                                ///< Globally unique identifier for the action.
    QString text;                                              ///< Display text (supports '&' for mnemonics).
    QString icon;                                              ///< The theme-based icon name.

    bool checkable { false };                                  ///< Determines if the action has an on/off state.
    bool checked { false };                                    ///< The current state of the action if checkable.

    QKeySequence defaultShortcut;                              ///< The default keyboard shortcut.
    ShortcutContext shortcutContext = ShortcutContext::Global; ///< Context in which the shortcut is active.
    QString logicalContextId; ///< Target context ID (used if ShortcutContext is Logical).

    int priority { 0 };       ///< Sorting/Display priority (higher values may appear first).

                              /// @brief The callback executed when the action is triggered.
    /// @param checked The new checked state (if the action is checkable).
    std::function<void( bool )> execute;
};

/// @brief Custom QDebug formatting for ActionDescriptor.
inline QDebug operator<<( QDebug dbg, const ActionDescriptor& ad ) {
    QDebugStateSaver saver( dbg );
    dbg.nospace() << "ActionDescriptor("
                  << "id=" << ad.id << ", "
                  << "text=" << ad.text << ", "
                  << "icon=" << ad.icon << ", "
                  << "checkable" << ad.checkable << ", "
                  << "checked" << ad.checked << ", "
                  << "defaultShortcut" << static_cast<int>( ad.shortcutContext ) << ", "
                  << "logicalContextId" << ad.logicalContextId << ", "
                  << "priority" << ad.priority << ")";
    return dbg;
}

} // namespace qone::sdk
