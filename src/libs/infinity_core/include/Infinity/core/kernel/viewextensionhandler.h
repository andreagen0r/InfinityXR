/// @file viewextensionhandler.h
/// @brief Registers visual components provided by plugins into the global View Registry.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <Infinity/core/core_export.h>
#include <Infinity/core/kernel/IPluginExtensionHandler.h>
#include <Infinity/core/ui/viewregistry.h>

namespace qone::core {

/// @class ViewExtensionHandler
/// @brief Discovers and registers @ref IViewContributor UI elements from plugins.
class CORE_EXPORT ViewExtensionHandler : public IPluginExtensionHandler {

public:
    /// @brief Constructs the handler with a reference to the global view registry.
    /// @param registry The master @ref ViewRegistry.
    explicit ViewExtensionHandler( ViewRegistry* registry );

    /// @brief Iterates over the contributed views and adds them to the registry.
    /// @param pluginId The unique identifier of the loaded plugin.
    /// @param instance Pointer to the active @ref PluginInstance wrapper.
    void onPluginLoaded( const QString& pluginId, PluginInstance* instance ) override;

    /// @brief Unregisters the views exposed by the plugin being unloaded.
    /// @param pluginId The unique identifier of the unloading plugin.
    /// @param instance Pointer to the active @ref PluginInstance wrapper.
    void onPluginUnloaded( const QString& pluginId, PluginInstance* instance ) override;

private:
    ViewRegistry* m_registry;
};

} // namespace qone::core
