#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QJsonDocument>
#include <QUrl>

namespace qone::infinity {

class ThemeManager : public QObject {

    Q_OBJECT
    QML_ELEMENT

public:
    explicit ThemeManager( QObject* parent = nullptr );

    static QJsonDocument loadFromFile( const QUrl& theme );
};

} // namespace qone::infinity
