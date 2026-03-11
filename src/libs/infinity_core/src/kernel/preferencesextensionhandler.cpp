#include <Infinity/core/kernel/preferencesextensionhandler.h>
#include <Infinity/sdk/contracts/IPreferencesContributor.h>
#include <Infinity/core/kernel/plugininstance.h>

namespace qone::core {

PreferencesExtensionHandler::PreferencesExtensionHandler( PreferencesRegistry* registry )
    : m_registry( registry ) {}

void PreferencesExtensionHandler::onPluginLoaded( const QString& pluginId, PluginInstance* instance ) {
    Q_UNUSED( pluginId );

    // Triple protection: Does the record exist? Is the instance valid? Did the internal plugin load successfully?
    if ( !m_registry || !instance || !instance->plugin() ) {
        return;
    }

    // We access IPlugin through instance->plugin()
    auto* contributor = instance->plugin()->getExtension<qone::sdk::IPreferencesContributor>();

    if ( contributor ) {
        const auto descriptors = contributor->getPreferences();

        for ( const auto& descriptor : descriptors ) {
            m_registry->registerPreference( descriptor );
        }
    }
}

void PreferencesExtensionHandler::onPluginUnloaded( const QString& pluginId, PluginInstance* instance ) {
    Q_UNUSED( pluginId );
    Q_UNUSED( instance );
    // FIXME: To simplify the first iteration, I ignore dynamic removal
    // Note: A complete implementation would require iterating through the m_registry and removing the corresponding
    // ViewIds.
}

} // namespace qone::core
