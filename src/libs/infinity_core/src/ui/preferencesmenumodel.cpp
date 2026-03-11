#include <Infinity/core/ui/preferencesmenumodel.h>

using namespace Qt::StringLiterals;

namespace qone::core {

PreferencesMenuModel::PreferencesMenuModel( PreferencesRegistry* registry, QObject* parent )
    : QAbstractListModel( parent )
    , m_registry( registry ) {

    if ( m_registry ) {
        connect( m_registry, &PreferencesRegistry::preferencesChanged, this, &PreferencesMenuModel::rebuild );
        rebuild();
    }
}

int PreferencesMenuModel::rowCount( const QModelIndex& parent ) const {

    if ( parent.isValid() ) {
        return 0;
    }

    return m_flatList.size();
}

QVariant PreferencesMenuModel::data( const QModelIndex& index, int role ) const {

    if ( !index.isValid() || index.row() >= m_flatList.size() ) {
        return {};
    }

    const auto& item = m_flatList[index.row()];

    switch ( role ) {
        case CategoryIdRole:
            return item.categoryId;
        case CategoryNameRole:
            return item.categoryName;
        case CategoryIconRole:
            return item.categoryIcon;
        case SubCategoryIdRole:
            return item.subCategoryId;
        case Qt::DisplayRole:
        case SubCategoryNameRole:
            return item.subCategoryName;
        case ViewsRole:
            return item.views;
        case ExpandedRole:
            return item.expanded;
        default:
            return {};
    }
}

QHash<int, QByteArray> PreferencesMenuModel::roleNames() const {
    static const QHash<int, QByteArray> roles {
        { CategoryIdRole, "categoryId"_ba },
        { CategoryNameRole, "categoryName"_ba }, // Usado no QML como 'section'
        { CategoryIconRole, "categoryIcon"_ba },
        { SubCategoryIdRole, "subCategoryId"_ba },
        { SubCategoryNameRole, "subCategoryName"_ba }, // O nome que aparece na linha
        { ViewsRole, "viewIds"_ba },
        { ExpandedRole, "expanded"_ba } };

    return roles;
}

void PreferencesMenuModel::rebuild() {

    beginResetModel();

    m_flatList.clear();

    if ( !m_registry ) {
        endResetModel();
        return;
    }

    const auto categories { m_registry->getCategories() };

    for ( const auto& cat : categories ) {

        for ( const auto& subCat : cat.subCategories ) {

            QVariantList stringViews;

            for ( const auto& viewId : subCat.views ) {
                stringViews.append( viewId.value );
            }

            m_flatList.append( { cat.id, cat.name, cat.icon, subCat.id, subCat.name, stringViews, true } );
        }
    }

    endResetModel();
}

void PreferencesMenuModel::toggleCategory( const QString& category ) {

    for ( int i = 0; i < m_flatList.size(); ++i ) {

        if ( m_flatList[i].categoryName == category ) {

            bool newState { !m_flatList[i].expanded };
            m_flatList[i].expanded = newState;

            QModelIndex idx { index( i ) };

            Q_EMIT dataChanged( idx, idx, { ExpandedRole } );
        }
    }
}

bool PreferencesMenuModel::isCategoryExpanded( const QString& category ) const {

    for ( const auto& item : m_flatList ) {

        if ( item.categoryName == category ) {
            return item.expanded;
        }
    }

    return true; // fallback default
}

} // namespace qone::core
