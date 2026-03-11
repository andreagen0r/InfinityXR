/// @file PluginEvents.h
/// @brief Data Transfer Objects (DTOs) for plugin lifecycle events.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QString>
#include <Infinity/sdk/types/PluginDescriptor.h>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk::events {

/// @struct PluginDiscovered
/// @brief Emitted when a new plugin is found on disk but not yet loaded.
struct INFINITYSDK_EXPORT PluginDiscovered {
    qone::sdk::PluginDescriptor descriptor;
};


/// @struct PluginLoaded
/// @brief Emitted after a plugin successfully executes its `onLoad` routine.
struct INFINITYSDK_EXPORT PluginLoaded {
    QString pluginId;
};


/// @struct PluginAboutToUnload
/// @brief Emitted just before a plugin is instructed to unload (allows dependents to cleanup).
struct INFINITYSDK_EXPORT PluginAboutToUnload {
    QString pluginId;
};


/// @struct PluginUnloaded
/// @brief Emitted after a plugin has been completely removed from memory.
struct INFINITYSDK_EXPORT PluginUnloaded {
    QString pluginId;
};

} // namespace qone::sdk::events
