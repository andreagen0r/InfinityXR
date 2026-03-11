#include <Infinity/core/services/serviceregistrationobserver.h>

#include <Infinity/sdk/logging/logger.h>
#include <Infinity/sdk/contracts/IAuthService.h>
#include <Infinity/sdk/contracts/IViewContributor.h>

#include <Infinity/core/services/serviceregistry.h>
#include <Infinity/core/ui/viewregistry.h>


qone::core::ServiceRegistrationObserver::ServiceRegistrationObserver( ViewRegistry* viewRegistry,
                                                                      ServiceRegistry* serviceRegistry )
    : m_viewRegistry { viewRegistry }
    , m_serviceRegistry { serviceRegistry } {}

void qone::core::ServiceRegistrationObserver::onPluginLoaded( const QString& pluginId, sdk::IPlugin* plugin ) {

    tryRegister<sdk::IAuthService>( pluginId, plugin );

    if ( auto* contributor = plugin->getExtension<sdk::IViewContributor>() ) {
        for ( const auto& descriptor : contributor->views() ) {

            if ( m_viewRegistry ) {
                m_viewRegistry->registerView( descriptor );
            }
            qCInfo( qoneCore ) << "ViewRegistry: Registered" << descriptor.id << "from" << pluginId;
        }
    }
}

void qone::core::ServiceRegistrationObserver::onPluginUnloading( const QString& pluginId ) {
    m_serviceRegistry->unregisterServicesFor( pluginId );
}

template <typename T>
void qone::core::ServiceRegistrationObserver::tryRegister( const QString& pluginId, sdk::IPlugin* plugin ) {
    if ( auto* svc = plugin->getExtension<T>() ) {
        m_serviceRegistry->registerService( QString::fromUtf8( T::IID ), pluginId, svc );
    }
}
