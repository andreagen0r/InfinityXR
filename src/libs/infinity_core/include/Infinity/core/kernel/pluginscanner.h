/// @file pluginscanner.h
/// @brief Responsible for discovering plugins in the filesystem.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QList>
#include <QDir>

#include <Infinity/sdk/types/PluginDescriptor.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class PluginScanner
/// @brief Traverses a specified directory to find and parse plugin manifests.
class CORE_EXPORT PluginScanner {

public:
    /// @brief Initializes the scanner targeting a specific base directory.
    /// @param pluginsPath The root folder to scan for plugin subdirectories.
    explicit PluginScanner( QDir pluginsPath );

    /// @brief Executes the discovery process.
    /// @return A list of @ref PluginDescriptor containing metadata for all valid plugins found.
    /// @note Logs warnings if the directory is missing or malformed.
    QList<qone::sdk::PluginDescriptor> scan() const;

private:
    QDir m_pluginsDir; ///< The root directory being scanned.

    /// @brief Attempts to parse the metadata embedded in the dynamic library.
    /// @param pluginDir The specific subdirectory containing the plugin binary.
    /// @return A populated descriptor, or an empty one if parsing fails.
    qone::sdk::PluginDescriptor parsePluginDir( const QDir& pluginDir ) const;
};

} // namespace qone::core
