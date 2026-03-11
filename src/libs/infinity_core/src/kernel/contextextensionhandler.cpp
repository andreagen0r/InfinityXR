#include <Infinity/core/kernel/contextextensionhandler.h>


qone::core::ContextExtensionHandler::ContextExtensionHandler( sdk::IServiceRegistry* registry )
    : m_registry( registry ) {}

void qone::core::ContextExtensionHandler::onPluginLoaded( const QString& pluginId, PluginInstance* instance ) {

    // OCP, if the plugin needs context, we provide it!
    if ( auto* requiresContext = instance->plugin()->getExtension<qone::sdk::IRequiresContext>() ) {
        requiresContext->setServiceRegistry( m_registry );
    }
}

void qone::core::ContextExtensionHandler::onPluginUnloaded( const QString& pluginId, PluginInstance* instance ) {

    // Safe cleaning to prevent dangling pointers.
    if ( auto* requiresContext = instance->plugin()->getExtension<qone::sdk::IRequiresContext>() ) {
        requiresContext->setServiceRegistry( nullptr );
    }
}
