#pragma once

#include <Infinity/sdk/logging/logger.h>
#include <Infinity/sdk/contracts/IAuthService.h>

#include <Infinity/core/kernel/IPluginExtensionHandler.h>
#include <Infinity/core/ui/sessionmanager.h>
#include <Infinity/core/kernel/plugininstance.h>

namespace qone::infinity {

class AuthExtensionHandler : public qone::core::IPluginExtensionHandler {

public:
    explicit AuthExtensionHandler( qone::core::SessionManager* sessionManager )
        : m_sessionManager( sessionManager ) {}

    void onPluginLoaded( const QString& pluginId, qone::core::PluginInstance* instance ) override {

        using namespace Qt::StringLiterals;

        if ( auto* authService = instance->plugin()->getExtension<qone::sdk::IAuthService>() ) {

            qCInfo( qoneApp ) << Infinity::Logging::Title( u"AuthExtensionHandler: Wiring AuthService from"_s )
                              << pluginId;

            QObject::connect( authService, &qone::sdk::IAuthService::authenticated, m_sessionManager,
                              [this]( const qone::sdk::AuthResult& res ) {
                                  if ( res.success ) {
                                      m_sessionManager->loginSuccess();
                                  }
                              } );
        }
    }

    void onPluginUnloaded( const QString& pluginId, qone::core::PluginInstance* instance ) override {
        Q_UNUSED( pluginId )
        Q_UNUSED( instance )
    }

private:
    qone::core::SessionManager* m_sessionManager;
};

} // namespace qone::infinity
