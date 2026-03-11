/// @file pluginmanager.h
/// @brief The central orchestrator for the entire plugin ecosystem.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QString>
#include <QList>

#include <memory>
#include <vector>

#include <Infinity/sdk/types/PluginDescriptor.h>
#include <Infinity/sdk/contracts/IEventBus.h>

#include <Infinity/core/core_export.h>
#include <Infinity/core/interaction/commandregistry.h>
#include <Infinity/core/services/settingsservice.h>

#include "pluginscanner.h"
#include "pluginloader.h"
#include "plugininstance.h"
#include "IPluginExtensionHandler.h"

namespace qone::core {

/// @class PluginManager
/// @brief Orchestrates plugin discovery, loading sequences, and extension distribution.
class CORE_EXPORT PluginManager : public QObject {

    Q_OBJECT

public:
    /// @brief Constructs the master Plugin Manager.
    explicit PluginManager( QDir pluginsDir, CommandRegistry* commandRegistry, SettingsService& settings,
                            qone::sdk::IEventBus* eventBus, QObject* parent = nullptr );

    /// @name Discovery & Initialization
    /// @{

    /// @brief Scans the configured directories and builds the internal descriptor list.
    void discoverPlugins();

    /// @brief Forces the immediate loading of system-critical plugins.
    void loadSystemPlugins();
    /// @}

    /// @name State Queries
    /// @{

    /// @brief Retrieves metadata for all discovered plugins (loaded or not).
    QList<qone::sdk::PluginDescriptor> availablePlugins() const;

    /// @brief Checks if a specific plugin is actively running.
    bool isPluginLoaded( const QString& pluginId ) const;
    /// @}

    /// @name Lifecycle Controls
    /// @{

    /// @brief Attempts to dynamically load a specific plugin by ID.
    bool loadPlugin( const QString& pluginId );

    /// @brief Unloads and frees resources for a specific plugin.
    void unloadPlugin( const QString& pluginId );

    /// @brief Persistently enables or disables a plugin via user settings.
    void setPluginEnabled( const QString& pluginId, bool enabled );
    /// @}

    /// @brief Registers a new handler that reacts to plugin load events.
    /// @param handler The specific extension processor (e.g., ViewExtensionHandler).
    void addExtensionHandler( std::unique_ptr<IPluginExtensionHandler> handler );

Q_SIGNALS:
    void pluginDiscovered( const qone::sdk::PluginDescriptor& descriptor );
    void pluginLoaded( const QString& pluginId );
    void aboutToUnloadPlugin( const QString& pluginId );
    void pluginUnloaded( const QString& pluginId );
    void uiPluginUnregistered( const QString& viewId );

private:
    qone::sdk::PluginDescriptor* findDescriptor( const QAnyStringView& pluginId );
    PluginInstance* findInstance( const QAnyStringView& pluginId );

private:
    PluginScanner m_scanner;
    PluginLoader m_loader;
    SettingsService& m_settings;
    QList<qone::sdk::PluginDescriptor> m_descriptors;
    std::vector<std::unique_ptr<PluginInstance>> m_instances;
    CommandRegistry* m_commandRegistry;
    std::vector<std::unique_ptr<IPluginExtensionHandler>> m_extensionHandlers;
    qone::sdk::IEventBus* m_eventBus;
};

} // namespace qone::core
