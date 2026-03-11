#include "logincommand.h"

#include <Infinity/sdk/logging/logger.h>

using namespace Qt::StringLiterals;

LoginCommand::LoginCommand( qone::sdk::IAuthService* authService, const QString& user, const QString& pass )
    : m_authService( authService )
    , m_user( user )
    , m_pass( pass ) {
    setText( u"Login User: "_s + user );
}

void LoginCommand::redo() {
    qCInfo( qoneCore ) << Infinity::Logging::Title( u"LoginCommand:"_s ) << u"Executing Redo (Login)..."_s;

    if ( !m_authService ) {
        return;
    }

    m_authService->authenticate( m_user, m_pass, []( qone::sdk::AuthResult result ) {
        if ( result.success ) {
            qCInfo( qoneCore ) << "Login Success! Token:" << result.userToken;
        } else {
            qCWarning( qoneCore ) << "Login Failed:" << result.message;
        }
    } );
}

void LoginCommand::undo() {
    qCInfo( qoneCore ) << Infinity::Logging::Title( u"LoginCommand: Undo (Logout)"_s );
    m_authService->logout();
}
