#include "appbootstrap.h"

#include <Infinity/sdk/logging/logger.h>
#include <Infinity/core/kernel/pluginmanager.h>
#include <Infinity/core/interaction/commandregistry.h>

#include <Infinity/core/services/qsettingsbackend.h>
#include <Infinity/core/services/settingsservice.h>
#include <Infinity/core/services/serviceregistry.h>
#include <Infinity/core/services/preferencesregistry.h>
#include <Infinity/core/services/httpservice.h>

#include <Infinity/core/interaction/commandregistry.h>
#include <Infinity/core/interaction/actiondispatcher.h>

#include <Infinity/core/ui/dialogservice.h>
#include <Infinity/core/ui/sessionmanager.h>
#include <Infinity/core/ui/viewregistry.h>
#include <Infinity/core/ui/dialogregistry.h>
#include <Infinity/core/ui/menumodel.h>
#include <Infinity/core/ui/preferencesmenumodel.h>
#include <Infinity/core/ui/pluginlistmodel.h>

using namespace Qt::StringLiterals;

namespace qone::infinity {

AppBootstrapContext::AppBootstrapContext( QQmlApplicationEngine& engine )
    : qmlEngine( engine ) {}
AppBootstrapContext::~AppBootstrapContext() = default;

// --------------------------------------------------------------------------------------------------------------------

AppBootstrap::AppBootstrap( QQmlApplicationEngine& engine, QObject* parent )
    : QObject { parent }
    , m_context { std::make_unique<AppBootstrapContext>( engine ) } {
    qCInfo( qoneApp ) << Infinity::Logging::Title( u"Bootstraping"_s ) << u"Builder Created"_s;
}

AppBootstrap::~AppBootstrap() = default;

AppBootstrap& AppBootstrap::setPluginsDirectory( const QDir& dir ) {
    m_context->pluginsDir = dir;
    return *this;
}

AppBootstrap& AppBootstrap::addInstaller( std::unique_ptr<IServiceInstaller> installer ) {
    m_serviceInstallers.push_back( std::move( installer ) );
    return *this;
}

AppBootstrap& AppBootstrap::addInstaller( std::unique_ptr<IQmlInstaller> installer ) {
    m_qmlInstallers.push_back( std::move( installer ) );
    return *this;
}

AppBootstrap& AppBootstrap::addInstaller( std::unique_ptr<IPluginInstaller> installer ) {
    m_pluginInstallers.push_back( std::move( installer ) );
    return *this;
}

void AppBootstrap::build() {

    for ( const auto& installer : m_serviceInstallers ) {
        installer->installServices( *m_context );
    }

    for ( const auto& installer : m_qmlInstallers ) {
        installer->registerQmlTypes( *m_context );
    }

    for ( const auto& installer : m_pluginInstallers ) {
        installer->configurePlugins( *m_context );
    }

    if ( m_context->pluginManager && m_context->commandRegistry ) {
        m_context->pluginManager->discoverPlugins();
        m_context->commandRegistry->beginBatch();
        m_context->pluginManager->loadSystemPlugins();
        m_context->commandRegistry->endBatch();
    } else {
        qCFatal( qoneApp ) << Infinity::Logging::Title( u"Plugin Manager is not available!"_s );
        return;
    }

    qCDebug( qoneApp ) << Infinity::Logging::Title( u"Bootstrap Built successfully!"_s );
}

} // namespace qone::infinity
