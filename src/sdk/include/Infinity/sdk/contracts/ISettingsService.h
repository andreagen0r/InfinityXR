/// @file ISettingsService.h
/// @brief Service for persisting and reading application settings.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QVariant>
#include <QString>

#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @class ISettingsService
/// @brief Contract for robust reading and writing of configuration values.
class INFINITYSDK_EXPORT ISettingsService {

public:
    virtual ~ISettingsService() = default;

    /// @brief Reads a value from the configuration store.
    /// @param key The unique key identifying the setting.
    /// @param def The default value to return if the key is not found.
    /// @return The retrieved value wrapped in a QVariant.
    virtual QVariant read( const QString& key, const QVariant& def = {} ) const = 0;

    /// @brief Writes a value to the configuration store.
    /// @param key The unique key identifying the setting.
    /// @param value The value to persist.
    virtual void write( const QString& key, const QVariant& value ) = 0;

    /// @brief Forces any pending changes to be written to disk immediately.
    virtual void sync() = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::ISettingsService, "qone.sdk.ISettingsService/1.0" )
