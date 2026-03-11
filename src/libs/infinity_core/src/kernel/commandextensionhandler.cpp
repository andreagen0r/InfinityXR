#include <Infinity/core/kernel/commandextensionhandler.h>
#include <Infinity/core/kernel/plugininstance.h>
#include <Infinity/core/interaction/commandregistry.h>

#include <Infinity/sdk/contracts/ICommandContributor.h>
#include <Infinity/sdk/logging/logger.h>

using namespace Qt::StringLiterals;

namespace qone::core {

CommandExtensionHandler::CommandExtensionHandler( CommandRegistry* registry )
    : m_registry( registry ) {}

void CommandExtensionHandler::onPluginLoaded( const QString& pluginId, PluginInstance* instance ) {

    if ( auto* cmdContributor = instance->plugin()->getExtension<qone::sdk::ICommandContributor>() ) {

        const auto registrations { cmdContributor->getCommands() };

        for ( const auto& reg : registrations ) {
            m_registry->registerCommand( reg.descriptor, reg.factory );
            qCInfo( qoneCore ) << Infinity::Logging::Title( u"Registered Command:"_s ) << reg.descriptor.id.value;
        }
    }
}

void CommandExtensionHandler::onPluginUnloaded( const QString& pluginId, PluginInstance* instance ) {
    // NOTE: The CommandRegistry does not have an unregisterCommand.
    // I'll leave it empty for now and implement the removal in the future.

    Q_UNUSED( pluginId )
    Q_UNUSED( instance )
}
} // namespace qone::core
