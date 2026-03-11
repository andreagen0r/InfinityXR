/// @file settingsservice.h
/// @brief Core wrapper over the physical settings storage mechanism.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <memory>

#include <Infinity/sdk/contracts/ISettingsService.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class SettingsService
/// @brief Encapsulates the backend storage, exposing the standardized SDK interface.
class CORE_EXPORT SettingsService {

public:
    /// @brief Constructs the service by taking ownership of a backend provider.
    /// @param backend A unique pointer to the storage implementation (e.g., QSettings).
    explicit SettingsService( std::unique_ptr<sdk::ISettingsService> backend );

    /// @brief Accesses the underlying settings engine.
    /// @return A reference to the @ref ISettingsService interface.
    sdk::ISettingsService& settings();

private:
    std::unique_ptr<sdk::ISettingsService> m_backend; ///< The persistence strategy.
};

} // namespace qone::core
