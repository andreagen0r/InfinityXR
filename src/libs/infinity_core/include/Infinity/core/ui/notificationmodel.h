#pragma once

#include <QAbstractListModel>

#include <Infinity/sdk/types/NotificationDescriptor.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

class CORE_EXPORT NotificationModel : public QAbstractListModel {

    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        SeverityRole,
        TitleRole,
        MessageRole,
        TimeoutRole,
        RequireInteractionRole,
        ActionsRole,
        PositionRole,
    };

    explicit NotificationModel( QObject* parent = nullptr );

    void addNotification( const sdk::NotificationDescriptor& desc );
    void removeNotification( const QString& id );
    [[nodiscard]] sdk::NotificationDescriptor getNotificationAt( int index ) const;

    int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<sdk::NotificationDescriptor> m_notifications;
};

} // namespace qone::core
