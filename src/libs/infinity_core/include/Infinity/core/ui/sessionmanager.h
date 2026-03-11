/// @file sessionmanager.h
/// @brief Manages the high-level application lifecycle and session states.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class SessionManager
/// @brief Controls state transitions like Splash Screen, Authentication, and Main UI.
class CORE_EXPORT SessionManager : public QObject {
    Q_OBJECT
    Q_PROPERTY( SessionState state READ state NOTIFY stateChanged )

public:
    /// @enum SessionState
    /// @brief Defines the current operational phase of the application.
    enum class SessionState {
        Initializing,   ///< Application is booting (e.g., showing a Splash Screen).
        Authenticating, ///< Waiting for user login credentials.
        Active,         ///< User is authenticated and using the main application.
        Locked          ///< Session is temporarily locked (requires re-auth).
    };
    Q_ENUM( SessionState )

    /// @brief Default constructor.
    explicit SessionManager( QObject* parent = nullptr );

    /// @brief Retrieves the current session state.
    /// @return The active @ref SessionState.
    SessionState state() const;

    /// @name State Transitions
    /// Methods exposed to QML to trigger state changes.
    /// @{

    /// @brief Transitions from Initializing to Authenticating.
    Q_INVOKABLE void finishInitialization(); // Splash -> Auth

    /// @brief Transitions from Authenticating to Active upon successful login.
    Q_INVOKABLE void loginSuccess();

    /// @brief Transitions from Active back to Authenticating, clearing the session.
    Q_INVOKABLE void logout();

Q_SIGNALS:
    /// @brief Emitted whenever the session state transitions.
    /// @param state The newly entered state.
    void stateChanged( SessionState state );

private:
    /// @brief Internal helper to safely update the state and emit signals.
    /// @param newState The target state to apply.
    void setState( SessionState newState );

    SessionState m_state { SessionState::Initializing }; ///< The active state variable.
};

} // namespace qone::core
