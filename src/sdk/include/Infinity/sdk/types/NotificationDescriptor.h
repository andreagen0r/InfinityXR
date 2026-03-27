#pragma once
#include <QString>
#include <QVariantMap>
#include <QList>
#include <QUuid>

namespace qone::sdk {

enum class NotificationSeverity {
    Info,
    Success,
    Warning,
    Critical,
    Neutral,
};

enum class NotificationPosition {
    TopLeft,
    TopCenter,
    TopRight,
    MiddleLeft,
    MiddleCenter,
    MiddleRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
    FillTop,
    FillMiddle,
    FillBottom,
};

struct NotificationAction {
    QString label;
    QString commandId;
    QVariantMap args;
};

struct NotificationDescriptor {
    QString id { QUuid::createUuidV7().toString( QUuid::WithoutBraces ) };
    NotificationSeverity severity { NotificationSeverity::Info };
    NotificationPosition position { NotificationPosition::BottomRight };

    QString title;
    QString message;
    QString iconName;

    int timeoutMs { 5000 };
    bool requireUserInteraction { false };

    QList<NotificationAction> actions;
};

} // namespace qone::sdk
