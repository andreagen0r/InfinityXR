#include <Infinity/core/ui/notificationmodel.h>

#include <Infinity/sdk/logging/logger.h>

using namespace Qt::StringLiterals;

namespace qone::core {

NotificationModel::NotificationModel( QObject* parent )
    : QAbstractListModel( parent ) {}

void NotificationModel::addNotification( const sdk::NotificationDescriptor& desc ) {

    qCDebug( qoneSdkEventBus ) << "INSERINDO NO MODELO QML. Total atual:" << m_notifications.size();

    beginInsertRows( QModelIndex(), m_notifications.size(), m_notifications.size() );
    m_notifications.append( desc );
    endInsertRows();
}

void NotificationModel::removeNotification( const QString& id ) {

    for ( int i = 0; i < m_notifications.size(); ++i ) {

        if ( m_notifications[i].id == id ) {
            beginRemoveRows( QModelIndex(), i, i );
            m_notifications.removeAt( i );
            endRemoveRows();
            break;
        }
    }
}

sdk::NotificationDescriptor NotificationModel::getNotificationAt( int index ) const {

    if ( index >= 0 && index < m_notifications.size() ) {
        return m_notifications[index];
    }

    return {};
}

int NotificationModel::rowCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent );
    return m_notifications.size();
}

QVariant NotificationModel::data( const QModelIndex& index, int role ) const {

    if ( !index.isValid() || index.row() >= m_notifications.size() ) {
        return {};
    }

    const auto& desc = m_notifications[index.row()];

    switch ( role ) {
        case IdRole:
            return desc.id;
        case SeverityRole:
            return static_cast<int>( desc.severity );
        case TitleRole:
            return desc.title;
        case MessageRole:
            return desc.message;
        case TimeoutRole:
            return desc.timeoutMs;
        case RequireInteractionRole:
            return desc.requireUserInteraction;
        case ActionsRole: {
            QVariantList actionsList;

            for ( const auto& action : desc.actions ) {
                QVariantMap map;
                map["label"_L1] = action.label;
                map["commandId"_L1] = action.commandId;
                map["args"_L1] = action.args;
                actionsList.append( map );
            }

            return actionsList;
        }
        case PositionRole: {
            return static_cast<int>( desc.position );
        }
    }
    return {};
}

QHash<int, QByteArray> NotificationModel::roleNames() const {
    static const QHash<int, QByteArray> roles {

        { IdRole, "idRole"_ba },           { SeverityRole, "severityRole"_ba },
        { TitleRole, "titleRole"_ba },     { MessageRole, "textRole"_ba },
        { TimeoutRole, "timeoutRole"_ba }, { RequireInteractionRole, "actionRequiredRole"_ba },
        { ActionsRole, "actionsRole"_ba }, { PositionRole, "position"_ba } };

    return roles;
}

} // namespace qone::core
