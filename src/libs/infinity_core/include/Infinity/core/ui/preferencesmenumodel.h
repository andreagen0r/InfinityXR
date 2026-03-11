/// @file preferencesmenumodel.h
/// @brief QML-friendly model for rendering the structured Preferences Menu.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QAbstractListModel>
#include <QVariantList>

#include <Infinity/core/core_export.h>
#include <Infinity/core/services/preferencesregistry.h>

namespace qone::core {

/// @class PreferencesMenuModel
/// @brief Flattens the hierarchical preference registry into a list model for QML views.
class CORE_EXPORT PreferencesMenuModel : public QAbstractListModel {
    Q_OBJECT

public:
    /// @enum Roles
    /// @brief Custom roles for data binding in QML delegates.
    enum Roles {
        CategoryIdRole = Qt::UserRole + 1,
        CategoryNameRole,
        CategoryIconRole,
        SubCategoryIdRole,
        SubCategoryNameRole,
        ViewsRole,
        ExpandedRole,
    };

    /// @brief Constructor wrapping the internal registry.
    /// @param registry Pointer to the central @ref PreferencesRegistry.
    explicit PreferencesMenuModel( PreferencesRegistry* registry, QObject* parent = nullptr );

    // QAbstractListModel overrides
    [[nodiscard]] int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    [[nodiscard]] QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    /// @brief Toggles the expanded/collapsed state of a given category in the UI.
    /// @param category The string name of the category to toggle.
    Q_INVOKABLE void toggleCategory( const QString& category );

    /// @brief Checks if a specific category is currently expanded.
    /// @param category The string name of the category.
    /// @return `true` if expanded, `false` if collapsed.
    Q_INVOKABLE bool isCategoryExpanded( const QString& category ) const;

private Q_SLOTS:
    /// @brief Rebuilds the internal flat list when the registry updates.
    void rebuild();

private:
    /// @struct FlatItem
    /// @brief Internal representation of a flattened tree node.
    struct FlatItem {
        QString categoryId;
        QString categoryName;
        QString categoryIcon;
        QString subCategoryId;
        QString subCategoryName;
        QVariantList views;
        bool expanded = true;
    };

    PreferencesRegistry* m_registry; ///< Reference to the backend registry.
    QList<FlatItem> m_flatList;      ///< The flattened list consumed by QML.
};

} // namespace qone::core
