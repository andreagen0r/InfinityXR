/// @file Types.h
/// @brief Core enumerations and common data structures for the Infinity XR SDK.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QString>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {
Q_NAMESPACE

/// @enum PluginCategory
/// @brief Defines the load order and classification of a plugin.
enum class PluginCategory {
    System,     ///< Loaded first (e.g., Auth, Database, Hardware Critical).
    Feature,    ///< User-facing functionalities (e.g., Monitor, Viewer).
    Background, ///< Non-visual background services.
};
Q_ENUM_NS( PluginCategory )


/// @enum PluginVendor
/// @brief Identifies the origin/creator of the plugin.
enum class PluginVendor {
    Official,   ///< First-party plugin developed by the core team.
    Partner,    ///< Certified partner plugin.
    Customer,   ///< Custom logic developed specifically for a customer.
    ThirdParty, ///< Community or independent developer plugin.
};
Q_ENUM_NS( PluginVendor )


/// @enum ShortcutContext
/// @brief Defines the scope in which a keyboard shortcut is active.
enum class ShortcutContext {
    Global,      ///< Active anywhere in the OS or Application.
    Application, ///< Active only when the application has focus.
    Widget,      ///< Active only when a specific UI widget has focus.
    Logical,     ///< Active based on internal application state/logic.
};
Q_ENUM_NS( ShortcutContext )


/// @struct AuthResult
/// @brief Contains the outcome of an authentication attempt.
struct INFINITYSDK_EXPORT AuthResult {
    bool success;      ///< True if authentication succeeded.
    QString message;   ///< Human-readable message (e.g., error description).
    QString userToken; ///< Session token generated upon successful login.
};

} // namespace qone::sdk
