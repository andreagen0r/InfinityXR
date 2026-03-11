#include "authplugin.h"

#include <QTimer>
#include <QDir>

#include <Infinity/sdk/contracts/ICommandRegistry.h>
#include <Infinity/sdk/contracts/IViewContributor.h>
#include <Infinity/sdk/contracts/IServiceRegistry.h>

#include <Infinity/sdk/contracts/IEventBusService.h>
#include <Infinity/sdk/contracts/IEventBus.h>
#include <Infinity/sdk/logging/logger.h>
#include "logincommand.h"

using namespace Qt::StringLiterals;

AuthPlugin::AuthPlugin( QObject* parent )
    : QObject( parent )
    , m_commandProvider( &m_authService ) {}

void AuthPlugin::onLoad() {
    qCInfo( qoneCore ) << Infinity::Logging::Title( u"AuthPlugin: Loading System Plugin"_s );
}

void AuthPlugin::onUnload() {
    qCInfo( qoneCore ) << Infinity::Logging::Title( u"AuthPlugin: Unloading"_s );
}

void AuthPlugin::setServiceRegistry( qone::sdk::IServiceRegistry* registry ) {

    if ( registry ) {

        auto eventBusWrapper { registry->resolve<qone::sdk::IEventBusService>() };

        if ( eventBusWrapper != nullptr ) {
            m_authService.setEventBus( eventBusWrapper->bus() );
        }

        auto httpService { registry->resolve<qone::sdk::IHttpService>() };
        if ( httpService != nullptr ) {
            m_authService.setHttpService( httpService );
        }
    }
}

// The Extension Pattern - The key for Core to discover functionalities.
void* AuthPlugin::extension( const QString& extensionId ) {

    // Log in to EVERYTHING that is requested so we can verify that the ID is arriving correctly.
    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Extension Request"_s ) << extensionId;

    if ( extensionId == QString::fromUtf8( qone::sdk::IRequiresContext::IID ) ) {
        return static_cast<qone::sdk::IRequiresContext*>( this );
    }

    if ( extensionId == QString::fromUtf8( qone::sdk::IAuthService::IID ) ) {
        return &m_authService;
    }

    if ( extensionId == QString::fromUtf8( qone::sdk::IViewContributor::IID ) ) {
        qCInfo( qoneCore ) << Infinity::Logging::Title( u"AuthPlugin:"_s ) << u"Returning ViewContributor"_s;
        return static_cast<qone::sdk::IViewContributor*>( &m_authService );
    }

    if ( extensionId == QString::fromUtf8( qone::sdk::ICommandContributor::IID ) ) {
        qCInfo( qoneCore ) << Infinity::Logging::Title( u"AuthPlugin:"_s ) << u"Returning CommandContributor"_s;
        return static_cast<qone::sdk::ICommandContributor*>( &m_commandProvider );
    }

    return nullptr;
}

AuthPlugin::CommandProvider::CommandProvider( AuthService* service )
    : m_service( service ) {}

QList<qone::sdk::CommandRegistration> AuthPlugin::CommandProvider::getCommands() const {

    qone::sdk::CommandDescriptor desc {
        .id = qone::sdk::CommandId( "auth.login"_L1 ),
        .text = "Login"_L1,
        .description = "Authenticate user"_L1,
    };

    qone::sdk::CommandFactory factory = [this]( const QVariantMap& params ) -> QUndoCommand* {
        const QString user = params.value( "username"_L1 ).toString();
        const QString pass = params.value( "password"_L1 ).toString();
        return new LoginCommand( m_service, user, pass );
    };

    return { {
        std::move( desc ),
        std::move( factory ),
    } };
}
