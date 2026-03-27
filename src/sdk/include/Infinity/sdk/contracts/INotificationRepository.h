#pragma once
#include <Infinity/sdk/types/NotificationDescriptor.h>

#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

enum class NotificationStatus {
    Active,
    ClosedByUser,
    Timeout,
    ActionInvoked,
    SystemDismissed,
};

class INFINITYSDK_EXPORT INotificationRepository {

public:
    static constexpr const char* IID = "org.qone.infinity.sdk.INotificationRepository";
    virtual ~INotificationRepository() = default;

    virtual void save( const NotificationDescriptor& descriptor, NotificationStatus initialStatus ) = 0;
    virtual void updateStatus( const QString& notificationId, NotificationStatus newStatus ) = 0;
};

} // namespace qone::sdk
