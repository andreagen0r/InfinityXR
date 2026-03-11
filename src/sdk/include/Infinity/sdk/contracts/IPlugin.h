/// @file IPlugin.h
/// @brief Core entry point interface for all Infinity XR plugins.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @class IPlugin
/// @brief Defines the lifecycle and extension resolution for plugins.
///
/// Every dynamically loaded library must export an implementation of this interface.
/// @dot
/// digraph plugin_lifecycle {
///   node [shape=box];
///   Discovered -> Loaded [label="onLoad()"];
///   Loaded -> Active [label="resolve dependencies"];
///   Active -> Unloaded [label="onUnload()"];
/// }
/// @enddot
class INFINITYSDK_EXPORT IPlugin {

public:
    virtual ~IPlugin() = default;

    /// @brief Invoked immediately after the plugin library is loaded into memory.
    /// @note Use this phase to register services and allocate basic resources.
    virtual void onLoad() = 0;

    /// @brief Invoked before the plugin is unloaded.
    /// @note Free resources and unregister services here.
    virtual void onUnload() = 0;

    /// @brief Extension Pattern: Requests a specific functionality contract by ID.
    /// @param extensionId The IID of the requested contract (e.g., "qone.sdk.IAuthService").
    /// @return Raw pointer to the implementation, or nullptr if unsupported.
    virtual void* extension( const QString& extensionId ) = 0;

    /// @brief Type-safe helper to retrieve a specific extension contract.
    /// @tparam T The requested interface type (must define a static `IID`).
    /// @return Typed pointer to the extension, or nullptr.
    template <typename T> auto getExtension() -> T* {
        return static_cast<T*>( extension( QString::fromUtf8( T::IID ) ) );
    }
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IPlugin, "qone.sdk.IPlugin/1.0" )
