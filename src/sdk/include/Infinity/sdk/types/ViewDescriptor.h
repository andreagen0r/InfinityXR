/// @file ViewDescriptor.h
/// @brief Visual metadata for registering and loading QML Views.

#pragma once

#include <QObject>
#include <QString>
#include <QUrl>
#include <QDebug>

#include <Infinity/sdk/infinitysdk_export.h>
#include <Infinity/sdk/types/ViewId.h>

namespace qone::sdk {

/// @enum ViewType
/// @brief Defines the docking behavior and window style of the View.
enum class ViewType {
    Panel,   ///< Panel integrated into the static UI.
    Dock,    ///< Dockable and floating window (QDockWidget logic).
    Tool,    ///< Independent tool window.
    Overlay, ///< Non-obstructive overlay layer.
};


/// @struct ViewDescriptor
/// @brief Packages the metadata required to instantiate and manage a user interface.
struct INFINITYSDK_EXPORT ViewDescriptor {
    ViewId id;               ///< Unique and strongly-typed identifier for the View.
    QString title;           ///< User-friendly title displayed in the UI.
    QString icon;            ///< Logical registration name of the icon in the system theme.
    QUrl source;             ///< Internal (qrc:/) or external path to the QML file.
    ViewType type;           ///< Layout behavior of the View.

    bool dockable { true };  ///< Defines whether the View can be dragged and docked.
    bool closable { true };  ///< Defines whether the user is allowed to close the View.
    bool singleton { true }; ///< If `true`, prevents multiple instances of the same View.
};

/// @brief Logging operator (QDebug) for `ViewDescriptor`.
/// @param dbg Current instance of the debug stream.
/// @param v Reference to the descriptor being logged.
/// @return Modified stream to allow chaining.
inline QDebug operator<<( QDebug dbg, const ViewDescriptor& v ) {
    QDebugStateSaver saver( dbg );
    dbg.nospace() << "ViewDescriptor("
                  << "id=" << v.id.value << ", "
                  << "title=" << v.title << ", "
                  << "icon=" << v.icon << ", "
                  << "source=" << v.source << ", "
                  << "type=" << static_cast<int>( v.type ) << ", "
                  << "dockable=" << v.dockable << ", "
                  << "closable=" << v.closable << ", "
                  << "singleton=" << v.singleton << ")";
    return dbg;
}

} // namespace qone::sdk
