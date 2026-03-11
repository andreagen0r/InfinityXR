/// @file dialogregistry.h
/// @brief Simple registry to map Dialog IDs to their QML files.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QHash>
#include <QUrl>
#include <QDebug>

#include <Infinity/sdk/logging/logger.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class DialogRegistry
/// @brief Maintains paths to QML dialog components provided by plugins.
class CORE_EXPORT DialogRegistry : public QObject {

    Q_OBJECT

public:
    /// @brief Registers a new dialog file.
    /// @param dialogId Target string identifier.
    /// @param qmlPath URL to the component file.
    void registerDialog( const QString& dialogId, const QUrl& qmlPath ) {
        using namespace Qt::StringLiterals;

        if ( m_registry.contains( dialogId ) ) {
            qCWarning( qoneCore ) << Infinity::Logging::Title( u"Duplicate dialog ID overwritten:"_s ) << dialogId;
        }

        m_registry[dialogId] = qmlPath;
        qCDebug( qoneCore ) << Infinity::Logging::Title( u"Registered Dialog ID:"_s ) << dialogId << "->" << qmlPath;
    }

    /// @brief Retrieves the source file for a given dialog.
    /// @param dialogId Target string identifier.
    /// @return The corresponding QUrl.
    Q_INVOKABLE QUrl getUrl( const QString& dialogId ) const {
        return m_registry.value( dialogId, QUrl() );
    }

private:
    QHash<QString, QUrl> m_registry;
};

} // namespace qone::core
