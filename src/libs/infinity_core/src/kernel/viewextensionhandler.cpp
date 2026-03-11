#include <Infinity/core/kernel/viewextensionhandler.h>

#include <Infinity/sdk/contracts/IViewContributor.h>
#include <Infinity/sdk/logging/logger.h>

#include <Infinity/core/kernel/plugininstance.h>
#include <Infinity/core/ui/viewregistry.h>

using namespace Qt::StringLiterals;

namespace qone::core {

ViewExtensionHandler::ViewExtensionHandler( ViewRegistry* registry )
    : m_registry { registry } {}

void ViewExtensionHandler::onPluginLoaded( const QString& pluginId, PluginInstance* instance ) {

    if ( auto* viewContributor = instance->plugin()->getExtension<qone::sdk::IViewContributor>() ) {

        for ( const auto& view : viewContributor->views() ) {
            qCInfo( qoneCore ) << Infinity::Logging::Title( u"ViewExtensionHandler:"_s ) << u"Registering View ID:"_s
                               << view << u"from"_s << pluginId;
            m_registry->registerView( view );
        }
    }
}

void ViewExtensionHandler::onPluginUnloaded( const QString& pluginId, PluginInstance* instance ) {

    if ( auto* viewContributor = instance->plugin()->getExtension<qone::sdk::IViewContributor>() ) {

        for ( const auto& view : viewContributor->views() ) {
            m_registry->unregisterView( view.id );
        }
    }
}
} // namespace qone::core
