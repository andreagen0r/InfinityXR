#include <Infinity/core/ui/sessionmanager.h>
#include <Infinity/core/utils/enumUtils.hpp>

#include <Infinity/sdk/logging/logger.h>

using namespace Qt::StringLiterals;

namespace qone::core {

SessionManager::SessionManager( QObject* parent )
    : QObject( parent ) {}

SessionManager::SessionState SessionManager::state() const {
    return m_state;
}

void SessionManager::finishInitialization() {
    if ( m_state == SessionState::Initializing ) {
        setState( SessionState::Authenticating );
    }
}

void SessionManager::loginSuccess() {
    // Here we could load the user profile before changing the state.
    setState( SessionState::Active );
}

void SessionManager::logout() {
    // Here we would clear sensitive data from memory.
    setState( SessionState::Authenticating );
}

void SessionManager::setState( SessionState newState ) {

    if ( m_state != newState ) {
        m_state = newState;
        Q_EMIT stateChanged( m_state );
        qCDebug( qoneCore ) << Infinity::Logging::Title( u"SessionManager"_s )
                            << u"State changed to: "_s % qone::core::utils::toString( m_state );
    }
}

} // namespace qone::core
