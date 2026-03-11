/// @file IPluginExtensionHandler.h
/// @brief Base interface for creating hooks into the plugin lifecycle.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QString>

#include <Infinity/core/core_export.h>

namespace qone::core {

class PluginInstance;

/// @class IPluginExtensionHandler
/// @brief Defines a contract for handlers that react to plugin load/unload events.
///
/// Implementations of this interface are responsible for extracting specific
/// extensions (like Views, Commands, or Services) from a newly loaded plugin
/// and registering them into the core system.
class CORE_EXPORT IPluginExtensionHandler {

public:
    virtual ~IPluginExtensionHandler() = default;

    /// @brief Invoked immediately after a plugin is instantiated and successfully loaded.
    /// @param pluginId The unique identifier of the loaded plugin.
    /// @param instance Pointer to the active @ref PluginInstance wrapper.
    virtual void onPluginLoaded( const QString& pluginId, PluginInstance* instance ) = 0;

    /// @brief Invoked right before a plugin's instance is destroyed.
    /// @param pluginId The unique identifier of the unloading plugin.
    /// @param instance Pointer to the active @ref PluginInstance wrapper.
    virtual void onPluginUnloaded( const QString& pluginId, PluginInstance* instance ) = 0;
};

} // namespace qone::core
