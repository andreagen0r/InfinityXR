/// @file serviceregistry.h
/// @brief Concrete implementation of the global Service Locator and Registry.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QHash>

#include <shared_mutex>

#include <Infinity/sdk/contracts/IServiceRegistry.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class ServiceRegistry
/// @brief Thread-safe central repository for dependency injection.
///
/// Allows plugins and core components to register and lookup shared services dynamically.
class CORE_EXPORT ServiceRegistry final : public qone::sdk::IServiceRegistry {

public:
    /// @brief Type-safe wrapper ensuring that registered services inherit from QObject.
    /// @tparam T The specific interface type.
    /// @param iid The standard Interface ID (e.g., "qone.sdk.IHttpService").
    /// @param pluginId The origin plugin.
    /// @param servicePtr The service instance.
    template <typename T> void registerSafeService( const QString& iid, const QString& pluginId, T* servicePtr ) {
        static_assert( std::is_base_of_v<QObject, T>, "O serviço deve herdar de QObject." );
        registerService( iid, pluginId, static_cast<QObject*>( servicePtr ) );
    }

    /// @brief Registers a raw service instance under a specific IID.
    void registerService( const QString& iid, const QString& pluginId, QObject* servicePtr ) override;

    /// @brief Purges all services registered by a specific plugin ID.
    void unregisterServicesFor( const QString& pluginId ) override;

    /// @brief Looks up a service instance by its string identifier.
    QObject* resolveService( const QString& iid ) const override;

    /// @brief Type-safe helper to resolve and cast a service.
    /// @tparam T The interface type expected.
    /// @param iid The Interface ID to lookup.
    /// @return A properly casted pointer, or nullptr if not found/invalid.
    template <typename T> T* resolve( const QString& iid ) const {
        return qobject_cast<T*>( resolveService( iid ) );
    }

private:
    /// @struct Entry
    /// @brief Maps an interface ID to its provider plugin and memory instance.
    struct Entry {
        QString pluginId; ///< ID of the plugin that owns this service.
        QObject* ptr;     ///< Pointer to the live service object.
    };

    QHash<QString, Entry> m_services;  ///< The primary routing map.
    mutable std::shared_mutex m_mutex; ///< Concurrency protection for read/write operations.
};

} // namespace qone::core
