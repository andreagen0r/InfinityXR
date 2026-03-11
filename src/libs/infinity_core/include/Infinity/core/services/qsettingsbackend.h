/// @file qsettingsbackend.h
/// @brief Disk-backed configuration storage utilizing Qt's native QSettings.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QSettings>

#include <Infinity/sdk/contracts/ISettingsService.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class QSettingsBackend
/// @brief Concrete implementation of ISettingsService writing to the OS registry or INI files.
class CORE_EXPORT QSettingsBackend final : public sdk::ISettingsService {

public:
    /// @brief Initializes the QSettings object targeting the "Infinity" domain of "QOne".
    QSettingsBackend();

    /// @brief Retrieves a persisted value.
    /// @param key The unique key.
    /// @param defaultValue Fallback value if the key does not exist.
    /// @return The stored value.
    QVariant read( const QString& key, const QVariant& defaultValue ) const override;

    /// @brief Modifies or adds a configuration value in memory.
    /// @param key The unique key.
    /// @param value The value to persist.
    void write( const QString& key, const QVariant& value ) override;

    /// @brief Flushes any unsaved changes to the physical disk.
    void sync() override;

private:
    QSettings m_settings; ///< The underlying Qt storage engine.
};

} // namespace qone::core
