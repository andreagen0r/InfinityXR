/// @file contextextensionhandler.h
/// @brief Injects the core service registry into plugins requiring context.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <Infinity/sdk/contracts/IServiceRegistry.h>
#include <Infinity/sdk/contracts/IRequiresContext.h>

#include <Infinity/core/kernel/IPluginExtensionHandler.h>
#include <Infinity/core/kernel/plugininstance.h>

namespace qone::core {

/// @class ContextExtensionHandler
/// @brief Intercepts plugin loads to fulfill Dependency Injection requirements.
///
/// If a loaded plugin implements the @ref IRequiresContext interface, this handler
/// automatically injects the global Service Registry into it, allowing the plugin
/// to resolve core services.
class ContextExtensionHandler : public IPluginExtensionHandler {

public:
    /// @brief Constructs the handler with a reference to the global registry.
    /// @param registry The master @ref IServiceRegistry to be injected.
    explicit ContextExtensionHandler( qone::sdk::IServiceRegistry* registry );

    /// @brief Evaluates the plugin and injects the context if required.
    /// @param pluginId The unique identifier of the loaded plugin.
    /// @param instance Pointer to the active @ref PluginInstance wrapper.
    void onPluginLoaded( const QString& pluginId, PluginInstance* instance ) override;

    /// @brief Cleans up the injected context to prevent dangling pointers.
    /// @param pluginId The unique identifier of the unloading plugin.
    /// @param instance Pointer to the active @ref PluginInstance wrapper.
    void onPluginUnloaded( const QString& pluginId, PluginInstance* instance ) override;

private:
    qone::sdk::IServiceRegistry* m_registry;
};

} // namespace qone::core
