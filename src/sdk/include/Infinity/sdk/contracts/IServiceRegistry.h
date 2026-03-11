/// @file IServiceRegistry.h
/// @brief Central registry for Dependency Injection and Service Locator pattern.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QString>

#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @class IServiceRegistry
/// @brief Allows plugins to expose and consume global services.
class INFINITYSDK_EXPORT IServiceRegistry {

public:
    virtual ~IServiceRegistry() = default;

    /// @brief Registers a service instance under a specific interface ID.
    /// @param iid The standard Interface ID (e.g., "qone.sdk.IHttpService").
    /// @param pluginId The ID of the plugin providing the service.
    /// @param instance Pointer to the QObject implementing the service.
    virtual void registerService( const QString& iid, const QString& pluginId, QObject* instance ) = 0;

    /// @brief Removes all services registered by a specific plugin.
    /// @param pluginId The ID of the plugin being unloaded.
    virtual void unregisterServicesFor( const QString& pluginId ) = 0;

    /// @brief Resolves and retrieves a service by its Interface ID.
    /// @param iid The Interface ID to lookup.
    /// @return Pointer to the service instance, or nullptr if not found.
    virtual QObject* resolveService( const QString& iid ) const = 0;

    /// @brief Type-safe wrapper for resolving a service.
    /// @tparam T The interface type to resolve (must have `T::IID`).
    /// @return Typed pointer to the service instance.
    template <typename T> [[nodiscard]] T* resolve() const {
        return qobject_cast<T*>( resolveService( QString::fromUtf8( T::IID ) ) );
    }
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IServiceRegistry, "qone.sdk.IServiceRegistry/1.0" )
