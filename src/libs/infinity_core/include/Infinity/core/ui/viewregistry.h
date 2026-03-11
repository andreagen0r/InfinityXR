/// @file viewregistry.h
/// @brief Central registry for mapping View IDs to their QML source files.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QHash>
#include <QUrl>

#include <optional>
#include <shared_mutex>

#include <Infinity/sdk/types/ViewDescriptor.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class ViewRegistry
/// @brief Thread-safe registry maintaining all visual components contributed by plugins.
class CORE_EXPORT ViewRegistry : public QObject {

    Q_OBJECT

public:
    /// @brief Resolves a View ID into its corresponding QML URL for the UI engine.
    /// @param viewId The string representation of the view identifier.
    /// @return The mapped QUrl, or an empty QUrl if not found.
    [[nodiscard]] Q_INVOKABLE QUrl getViewSource( const QString& viewId ) const;

    /// @brief Alternative accessor for the view URL.
    /// @param viewId The string representation of the view identifier.
    /// @return The mapped QUrl.
    [[nodiscard]] Q_INVOKABLE QUrl getUrl( const QString& viewId ) const;

    /// @brief Registers a new view descriptor into the system.
    /// @param descriptor The view metadata.
    void registerView( const qone::sdk::ViewDescriptor& descriptor );

    /// @brief Removes a view from the registry.
    /// @param viewId The strongly-typed identifier of the view.
    void unregisterView( const sdk::ViewId& viewId );

    /// @brief Modern C++ API to safely retrieve a view descriptor.
    /// @param viewId The strongly-typed identifier of the view.
    /// @return An optional containing the descriptor if found.
    [[nodiscard]] std::optional<qone::sdk::ViewDescriptor> findView( const sdk::ViewId& viewId ) const;


private:
    QHash<qone::sdk::ViewId, qone::sdk::ViewDescriptor> m_views; ///< The routing map for views.
    mutable std::shared_mutex m_mutex;                           ///< Concurrency protection.
};

} // namespace qone::core
