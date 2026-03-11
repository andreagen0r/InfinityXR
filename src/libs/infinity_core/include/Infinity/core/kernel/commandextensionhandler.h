/// @file commandextensionhandler.h
/// @brief Registers commands provided by plugins into the global Command Registry.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <Infinity/core/core_export.h>
#include <Infinity/core/kernel/IPluginExtensionHandler.h>

namespace qone::core {

class CommandRegistry;

/// @class CommandExtensionHandler
/// @brief Extracts and registers @ref ICommandContributor commands from loaded plugins.
class CORE_EXPORT CommandExtensionHandler : public IPluginExtensionHandler {

public:
    /// @brief Constructs the handler with a reference to the global command registry.
    /// @param registry The master @ref CommandRegistry where new commands will be registered.
    explicit CommandExtensionHandler( CommandRegistry* registry );

    /// @brief Discovers and registers commands exposed by the newly loaded plugin.
    /// @param pluginId The unique identifier of the loaded plugin.
    /// @param instance Pointer to the active @ref PluginInstance wrapper.
    void onPluginLoaded( const QString& pluginId, PluginInstance* instance ) override;

    /// @brief Unregisters the commands associated with the unloading plugin.
    /// @param pluginId The unique identifier of the unloading plugin.
    /// @param instance Pointer to the active @ref PluginInstance wrapper.
    void onPluginUnloaded( const QString& pluginId, PluginInstance* instance ) override;

private:
    CommandRegistry* m_registry;
};

} // namespace qone::core
