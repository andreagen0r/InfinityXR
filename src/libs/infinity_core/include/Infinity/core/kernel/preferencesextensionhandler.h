/// @file preferencesextensionhandler.h
/// @brief Extracts and registers preference pages contributed by plugins.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <Infinity/sdk/contracts/IPlugin.h>

#include <Infinity/core/kernel/IPluginExtensionHandler.h>
#include <Infinity/core/services/preferencesregistry.h>

namespace qone::core {

/// @class PreferencesExtensionHandler
/// @brief Evaluates plugins for the @ref IPreferencesContributor interface.
///
/// Any preference descriptor found is automatically forwarded to the
/// @ref PreferencesRegistry to be displayed in the global settings UI.
class CORE_EXPORT PreferencesExtensionHandler : public IPluginExtensionHandler {

public:
    /// @brief Constructs the handler with a reference to the preferences registry.
    /// @param registry The master @ref PreferencesRegistry.
    explicit PreferencesExtensionHandler( PreferencesRegistry* registry );

    /// @brief Extracts preference descriptors and registers them.
    /// @param pluginId The unique identifier of the loaded plugin.
    /// @param instance Pointer to the active @ref PluginInstance wrapper.
    /// @note `qCInfo( qoneCore ) << Infinity::Logging::Title( u"PREFS_HANDLER [LOAD]:"_s ) << pluginId;`
    void onPluginLoaded( const QString& pluginId, PluginInstance* instance ) override;

    /// @brief Removes the preference descriptors associated with the unloading plugin.
    /// @param pluginId The unique identifier of the unloading plugin.
    /// @param instance Pointer to the active @ref PluginInstance wrapper.
    void onPluginUnloaded( const QString& pluginId, PluginInstance* instance ) override;

private:
    PreferencesRegistry* m_registry;
};

} // namespace qone::core
