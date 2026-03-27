#include <Infinity/core/services/notificationservice.h>
#include <Infinity/sdk/types/NotificationEvents.h>

#include <Infinity/sdk/logging/logger.h>

using namespace Qt::StringLiterals;

namespace qone::core {

NotificationService::NotificationService( std::shared_ptr<sdk::IEventBus> eventBus,
                                          std::shared_ptr<sdk::INotificationRepository> repository, QObject* parent )
    : QObject( parent )
    , m_eventBus( std::move( eventBus ) )
    , m_repository( std::move( repository ) ) {

    m_model = new NotificationModel( this );

    qCInfo( qoneCore ) << ">>> NOTIFICATION SERVICE CRIADO! Pointer do EventBus (Core):" << m_eventBus.get();

    m_subscriptions.push_back( m_eventBus->subscribe<sdk::events::NotificationRequested>( [this]( const auto& event ) {
        qCDebug( qoneCore ) << "RECEBI O EVENTO DE NOTIFICAÇÃO:" << event.descriptor.title;

        qDebug() << "NOTIFICATION SERVICE: A Iniciar subscrição no EventBus... Pointer do Bus:" << m_eventBus.get();

        QMetaObject::invokeMethod(
            this,
            [this, event]() {
                m_model->addNotification( event.descriptor );
                m_repository->save( event.descriptor, sdk::NotificationStatus::Active );
            },
            Qt::QueuedConnection );
    } ) );

    m_subscriptions.push_back(
        m_eventBus->subscribe<sdk::events::NotificationDismissRequested>( [this]( const auto& event ) {
            qDebug() << "NOTIFICATION SERVICE: A Iniciar subscrição no NotificationDismissRequested... Pointer do Bus:"
                     << m_eventBus.get();

            QMetaObject::invokeMethod(
                this,
                [this, event]() {
                    m_model->removeNotification( event.notificationId );
                    m_repository->updateStatus( event.notificationId, sdk::NotificationStatus::SystemDismissed );
                },
                Qt::QueuedConnection );
        } ) );
}

QAbstractListModel* NotificationService::model() const {
    return m_model;
}

void NotificationService::dismissByUser( const QString& notificationId ) {

    if ( !notificationId.isEmpty() ) {
        m_model->removeNotification( notificationId );
        m_repository->updateStatus( notificationId, sdk::NotificationStatus::ClosedByUser );
    }
}

void NotificationService::dismissByTimeout( const QString& notificationId ) {

    if ( !notificationId.isEmpty() ) {
        m_model->removeNotification( notificationId );
        m_repository->updateStatus( notificationId, sdk::NotificationStatus::Timeout );
    }
}

void NotificationService::dismissByAction( const QString& notificationId ) {

    if ( !notificationId.isEmpty() ) {
        m_model->removeNotification( notificationId );
        m_repository->updateStatus( notificationId, sdk::NotificationStatus::ActionInvoked );
    }
}

} // namespace qone::core
