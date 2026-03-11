/// @file menumodel.h
/// @brief QML model providing a hierarchical structure of application menus.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <memory>

#include <QAbstractItemModel>

#include <Infinity/core/core_export.h>

#include "menunode_p.h"

namespace qone::core {

class CommandRegistry;

/// @class MenuModel
/// @brief Exposes the registered commands as a hierarchical tree model for UI rendering.
class CORE_EXPORT MenuModel : public QAbstractItemModel {

    Q_OBJECT

public:
    /// @enum ItemType
    /// @brief Classifies the nature of the menu node.
    enum ItemType {
        Action,    ///< Executable command.
        SubMenu,   ///< Expandable category.
        Separator, ///< Visual divider.
    };
    Q_ENUM( ItemType )

    enum Roles {
        TextRole = Qt::UserRole + 1,
        IconRole,
        CommandIdRole,
        EnabledRole,
        ShortcutRole,
        CheckableRole,
        CheckedRole,
        ItemTypeRole,
    };

    explicit MenuModel( qone::core::CommandRegistry* registry, QObject* parent = nullptr );
    ~MenuModel() override = default;

    QModelIndex index( int row, int column, const QModelIndex& parent = {} ) const override;
    QModelIndex parent( const QModelIndex& child ) const override;
    int rowCount( const QModelIndex& parent = {} ) const override;
    int columnCount( const QModelIndex& parent = {} ) const override;
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QModelIndex qmlIndex( int row, int column, const QVariant& parent );
    Q_INVOKABLE QModelIndex root() const;
    Q_INVOKABLE QVariant dataAt( const QModelIndex& index, int role ) const;
    Q_INVOKABLE QVariant dataAtRole( const QModelIndex& index, const QString& roleName ) const;

    /// @brief Reconstructs the internal tree based on the Command Registry.
    void rebuild();

    /// @brief Reacts to state changes (enabled/checked) of specific commands.
    void onCommandStateChanged( const QString& id );

private:
    qone::core::CommandRegistry* m_registry;
    std::unique_ptr<MenuNode> m_root;
};

} // namespace qone::core
