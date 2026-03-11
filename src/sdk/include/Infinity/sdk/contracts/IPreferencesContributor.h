/// @file IPreferencesContributor.h
/// @brief Extension interface for registering custom preference panels.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QList>

#include <Infinity/sdk/infinitysdk_export.h>

#include <Infinity/sdk/types/PreferenceDescriptor.h>

namespace qone::sdk {

/// @class IPreferencesContributor
/// @brief Allows plugins to inject UI panels into the application's global Preferences window.
class INFINITYSDK_EXPORT IPreferencesContributor {

public:
    /// @brief Unique Interface Identifier (IID).
    static constexpr auto IID { "qone.sdk.IPreferencesContributor" };

    virtual ~IPreferencesContributor() = default;

    /// @brief Retrieves the descriptors for the preference pages this plugin provides.
    /// @return A list of @ref PreferenceDescriptor.
    [[nodiscard]] virtual QList<PreferenceDescriptor> getPreferences() const = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IPreferencesContributor, "qone.sdk.IPreferencesContributor/1.0" )
