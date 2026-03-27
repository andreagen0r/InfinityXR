#pragma once

#include <QObject>

#include <Infinity/sdk/contracts/IEventBus.h>
#include <Infinity/sdk/contracts/INotificationRepository.h>
#include <Infinity/core/ui/notificationmodel.h>

#include <Infinity/core/core_export.h>

#include <memory>

namespace qone::core {

class CORE_EXPORT NotificationService : public QObject {

    Q_OBJECT
    Q_PROPERTY( QAbstractListModel* model READ model CONSTANT )

public:
    explicit NotificationService( std::shared_ptr<sdk::IEventBus> eventBus,
                                  std::shared_ptr<sdk::INotificationRepository> repository, QObject* parent = nullptr );

    QAbstractListModel* model() const;

    Q_INVOKABLE void dismissByUser( const QString& notificationId );
    Q_INVOKABLE void dismissByTimeout( const QString& notificationId );
    Q_INVOKABLE void dismissByAction( const QString& notificationId );

private:
    std::shared_ptr<sdk::IEventBus> m_eventBus;
    std::shared_ptr<sdk::INotificationRepository> m_repository;
    NotificationModel* m_model;
    std::vector<sdk::EventSubscription> m_subscriptions;
};

} // namespace qone::core
