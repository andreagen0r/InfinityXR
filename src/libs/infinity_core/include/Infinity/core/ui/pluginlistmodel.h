/// @file pluginlistmodel.h
/// @brief QML model exposing the list of discovered and loaded plugins.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QAbstractListModel>

#include <Infinity/core/core_export.h>
#include <Infinity/core/kernel/pluginmanager.h>

namespace qone::core {

/// @class PluginListModel
/// @brief Adapts the PluginManager's internal list into a QAbstractListModel for QML views.
class CORE_EXPORT PluginListModel : public QAbstractListModel {

    Q_OBJECT

public:
    /// @enum Roles
    /// @brief Custom roles mapped to QML delegate properties.
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        VersionRole,
        VendorRole,
        DescriptionRole,
        CategoryRole,
        IsEnabledRole,
    };

    /// @brief Constructs the model utilizing the core Plugin Manager.
    explicit PluginListModel( PluginManager* pluginManager, QObject* parent = nullptr );

    [[nodiscard]] int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    [[nodiscard]] QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    /// @brief Toggles the enabled state of a specific plugin from the UI.
    /// @param pluginId The string ID of the plugin.
    /// @param enabled True to enable, false to disable.
    Q_INVOKABLE void setPluginEnabled( const QString& pluginId, bool enabled );

    /// @brief Forces the model to reload the plugin list from the manager.
    Q_INVOKABLE void refreshModel();

private:
    PluginManager* m_pluginManager;               ///< Pointer to the active Plugin Manager.
    QList<qone::sdk::PluginDescriptor> m_plugins; ///< Local cache of plugin descriptors.
};

} // namespace qone::core
