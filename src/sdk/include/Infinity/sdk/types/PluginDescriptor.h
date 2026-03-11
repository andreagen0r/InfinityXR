/// @file PluginDescriptor.h
/// @brief Metadata representation for a loaded Plugin.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QString>
#include <QVersionNumber>
#include <Infinity/sdk/types/Version.h>
#include <Infinity/sdk/types/Types.h>

#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @struct PluginDescriptor
/// @brief Contains the parsed metadata of a plugin derived from its manifest.
struct INFINITYSDK_EXPORT PluginDescriptor {

    QString pluginId;                                    ///< Unique global identifier for the plugin.
    QString name;                                        ///< Human-readable name.
    QString description;                                 ///< Brief explanation of the plugin's purpose.
    QVersionNumber version;                              ///< Standardized version number.

    PluginVendor vendor;                                 ///< Vendor classification (Official, ThirdParty, etc.).

    bool hasUi { false };                                ///< True if the plugin contributes UI elements.
    bool requiresLicense { false };                      ///< True if a specific license is needed to activate.
    bool enabledByDefault { true };                      ///< True if the plugin should start automatically.
    bool enabled { true };                               ///< True if currently enabled by the user/system.

    QString libraryPath;                                 ///< Absolute path to the dynamic library (.dll / .so).

    PluginCategory category { PluginCategory::Feature }; ///< The lifecycle load category.
    bool isRequired { false };                           ///< True if the application cannot boot without it.

    /// @brief Helper method to determine if this is a low-level system plugin.
    /// @return True if the category is `PluginCategory::System`.
    bool isSystem() const {
        return category == PluginCategory::System;
    }
};

} // namespace qone::sdk
