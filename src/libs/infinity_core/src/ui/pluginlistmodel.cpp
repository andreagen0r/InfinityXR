#include <Infinity/core/ui/pluginlistmodel.h>

#include <Infinity/core/utils/enumUtils.hpp>

using namespace Qt::StringLiterals;

namespace qone::core {

PluginListModel::PluginListModel( PluginManager* pluginManager, QObject* parent )
    : QAbstractListModel( parent )
    , m_pluginManager( pluginManager ) {
    refreshModel();
}

void PluginListModel::refreshModel() {

    if ( !m_pluginManager ) {
        return;
    }

    beginResetModel();
    m_plugins = m_pluginManager->availablePlugins();
    endResetModel();
}

int PluginListModel::rowCount( const QModelIndex& parent ) const {

    if ( parent.isValid() ) {
        return 0;
    }
    return m_plugins.size();
}

QVariant PluginListModel::data( const QModelIndex& index, int role ) const {
    if ( !index.isValid() || index.row() >= m_plugins.size() ) {
        return {};
    }
    const auto& p = m_plugins[index.row()];

    switch ( role ) {
        case IdRole:
            return p.pluginId;
        case NameRole:
            return p.name;
        case VersionRole:
            return p.version.toString();
        case VendorRole:
            return qone::core::utils::toString( p.vendor );
        case DescriptionRole:
            return p.description;
        case CategoryRole:
            return qone::core::utils::toString( p.category );
        case IsEnabledRole:
            return p.enabled;
        default:
            return {};
    }
}

QHash<int, QByteArray> PluginListModel::roleNames() const {
    static const QHash<int, QByteArray> roles { { IdRole, "pluginId"_ba },
                                                { NameRole, "nameText"_ba },
                                                { VersionRole, "versionText"_ba },
                                                { VendorRole, "vendorText"_ba },
                                                { DescriptionRole, "descriptionText"_ba },
                                                { CategoryRole, "categoryText"_ba },
                                                { IsEnabledRole, "isEnabled"_ba } };
    return roles;
}

void PluginListModel::setPluginEnabled( const QString& pluginId, bool enabled ) {
    if ( m_pluginManager ) {
        // Substitua pelo método real que ativa/desativa plugins no seu Core
        m_pluginManager->setPluginEnabled( pluginId, enabled );
        refreshModel();
    }
}

} // namespace qone::core
