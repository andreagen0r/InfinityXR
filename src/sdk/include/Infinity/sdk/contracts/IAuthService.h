/// @file IAuthService.h
/// @brief Service contract for authentication management.
///

#pragma once

#include <QObject>
#include <QString>
#include <functional>

#include <Infinity/sdk/infinitysdk_export.h>
#include <Infinity/sdk/types/Types.h>

namespace qone::sdk {

/// @class IAuthService
/// @brief Contract interface for authentication services.
///
/// This interface defines the methods required to manage the user session
/// lifecycle, including login, logout, and providing UI metadata.
///
/// @dot
/// digraph auth_state {
///   node [shape=box];
///   LoggedOut -> Authenticating [label="authenticate()"];
///   Authenticating -> LoggedIn [label="success"];
///   Authenticating -> LoggedOut [label="failure"];
///   LoggedIn -> LoggedOut [label="logout()"];
/// }
/// @enddot
class INFINITYSDK_EXPORT IAuthService : public QObject {

    Q_OBJECT

public:
    /// @brief Default constructor for the interface.
    explicit IAuthService( QObject* parent = nullptr )
        : QObject( parent ) {}

    /// @brief Unique Interface Identifier (IID).
    static constexpr auto IID { "qone.sdk.IAuthService" };

    /// @brief Virtual destructor to ensure proper cleanup in derived classes.
    virtual ~IAuthService() = default;

    /// @brief Initiates the authentication process asynchronously.
    /// @param username The username or email for authentication.
    /// @param password Plain-text password (securely handled by the implementation).
    /// @param callback Callback function invoked with an @ref AuthResult upon completion.
    /// @note Implementation should include a secure log that **does not print the password**:
    // clang-format off
    virtual void authenticate( const QString& username, const QString& password,
                               std::function<void( AuthResult )> callback ) = 0;
    // clang-format on

    /// @brief Checks if there is an active and authenticated session.
    /// @return `true` if the user is logged in, `false` otherwise.
    virtual bool isAuthenticated() const = 0;

    /// @brief Terminates the current session and clears access tokens.
    /// @post The @ref loggedOut signal will be emitted.
    virtual void logout() = 0;

    /// @brief Retrieves the unique identifier of the QML view associated with the login screen.
    /// @return QString containing the View ID (e.g., "com.infinity.login").
    virtual QString loginViewId() const = 0;

    /// @brief Provides the URL (local or remote) of the QML component for the login screen.
    /// @return QUrl pointing to the .qml file or compiled resource.
    virtual QUrl loginViewSource() const = 0;

Q_SIGNALS:

    /// @brief Emitted when an authentication attempt is finalized.
    /// @param result Structure containing the authentication status and metadata.
    void authenticated( const qone::sdk::AuthResult& result );

    /// @brief Emitted when the user successfully logs out of the system.
    void loggedOut();
};


} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IAuthService, "qone.sdk.IAuthService/1.0" )
