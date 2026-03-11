/// @file AuthEvents.h
/// @brief Data Transfer Objects (DTOs) for authentication-related events.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QString>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk::events {

/// @struct UserLoggedIn
/// @brief Emitted when a user successfully authenticates into the system.
struct INFINITYSDK_EXPORT UserLoggedIn {
    QString username; ///< The username or identifier of the authenticated user.
                      // TODO: Add more fields in the future: roles, timestamp, token, etc.
};

} // namespace qone::sdk::events
