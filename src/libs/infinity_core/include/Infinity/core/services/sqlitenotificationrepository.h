#pragma once

#include <QObject>
#include <QSqlDatabase>

#include <Infinity/sdk/contracts/INotificationRepository.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

class CORE_EXPORT SqliteNotificationRepository final : public QObject, public sdk::INotificationRepository {

public:
    explicit SqliteNotificationRepository( QObject* parent = nullptr );
    ~SqliteNotificationRepository() override;

    void save( const sdk::NotificationDescriptor& descriptor, sdk::NotificationStatus initialStatus ) override;
    void updateStatus( const QString& notificationId, sdk::NotificationStatus newStatus ) override;

private:
    void initializeDatabase();
    QSqlDatabase m_db;
};

} // namespace qone::core
