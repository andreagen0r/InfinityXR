#pragma once
#include <Infinity/sdk/types/NotificationDescriptor.h>

namespace qone::sdk::events {

struct NotificationRequested {
    virtual ~NotificationRequested() = default;
    NotificationDescriptor descriptor;
};

struct NotificationDismissRequested {
    virtual ~NotificationDismissRequested() = default;
    QString notificationId;
};

} // namespace qone::sdk::events
