/// @file plugininstance.h
/// @brief Wrapper encapsulating a loaded plugin and its system resources.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <memory>

#include <QPluginLoader>

#include <Infinity/sdk/contracts/IPlugin.h>

#include <Infinity/sdk/types/PluginDescriptor.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class PluginInstance
/// @brief Manages the memory, metadata, and execution lifecycle of a single plugin.
class CORE_EXPORT PluginInstance {
public:
    /// @brief Constructs a managed instance of a plugin.
    /// @param descriptor The metadata associated with this plugin.
    /// @param loader The QPluginLoader responsible for the shared library memory.
    /// @param plugin Pointer to the primary @ref IPlugin interface implemented by the library.
    PluginInstance( qone::sdk::PluginDescriptor descriptor, std::unique_ptr<QPluginLoader> loader,
                    qone::sdk::IPlugin* plugin );

    /// @brief Safely unloads the plugin upon destruction.
    ~PluginInstance();

    /// @name Move Semantics
    /// Allows instances to be stored in standard containers safely.
    /// @{
    PluginInstance( PluginInstance&& ) noexcept = default;
    PluginInstance& operator=( PluginInstance&& ) noexcept = default;
    /// @}

    /// @name Copy Semantics
    /// Deleted to prevent double-free and strict ownership issues.
    /// @{
    PluginInstance( const PluginInstance& ) = delete;
    PluginInstance& operator=( const PluginInstance& ) = delete;
    /// @}

    /// @brief Triggers the plugin's internal initialization logic.
    /// @return `true` if successful, `false` otherwise.
    bool load();

    /// @brief Triggers the plugin's cleanup logic and releases the library.
    void unload();

    /// @brief Checks if the plugin is currently active in memory.
    /// @return `true` if loaded and executing, `false` otherwise.
    bool isLoaded() const;

    /// @brief Retrieves the raw plugin interface for extension querying.
    /// @return Pointer to the @ref IPlugin instance.
    qone::sdk::IPlugin* plugin() const;

    /// @brief Retrieves the metadata defining this plugin.
    /// @return A constant reference to the @ref PluginDescriptor.
    const qone::sdk::PluginDescriptor& descriptor() const;

private:
    qone::sdk::PluginDescriptor m_descriptor; ///< Associated metadata.
    std::unique_ptr<QPluginLoader> m_loader;  ///< Manages the dynamic library lifecycle.
    qone::sdk::IPlugin* m_plugin { nullptr }; ///< The raw interface exposed by the library.
};

} // namespace qone::core
