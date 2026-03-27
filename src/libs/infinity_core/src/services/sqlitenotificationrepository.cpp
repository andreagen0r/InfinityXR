#include <Infinity/core/services/sqlitenotificationrepository.h>

#include <Infinity/sdk/logging/logger.h>

#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

using namespace Qt::StringLiterals;

namespace qone::core {

SqliteNotificationRepository::SqliteNotificationRepository( QObject* parent )
    : QObject( parent ) {
    initializeDatabase();
}

SqliteNotificationRepository::~SqliteNotificationRepository() {
    if ( m_db.isOpen() ) {
        m_db.close();
    }
}

void SqliteNotificationRepository::initializeDatabase() {

    QString dataPath = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
    QDir().mkpath( dataPath );

    m_db = QSqlDatabase::addDatabase( "QSQLITE"_L1, "NotificationConnection"_L1 );
    m_db.setDatabaseName( dataPath % "/notifications.db"_L1 );

    if ( !m_db.open() ) {
        qCWarning( qoneCoreSql ) << "Falha ao abrir banco de dados de notificações:" << m_db.lastError().text();
        return;
    }

    QSqlQuery query( m_db );
    QString createTableStr { R"(
        CREATE TABLE IF NOT EXISTS notification_history (
            id TEXT PRIMARY KEY,
            severity INTEGER,
            title TEXT,
            message TEXT,
            status INTEGER,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )"_L1 };

    if ( !query.exec( createTableStr ) ) {
        qCWarning( qoneCoreSql ) << "Falha ao criar tabela de notificações:" << query.lastError().text();
    }
}

void SqliteNotificationRepository::save( const sdk::NotificationDescriptor& descriptor,
                                         sdk::NotificationStatus initialStatus ) {
    if ( !m_db.isOpen() ) {
        return;
    }

    QSqlQuery query( m_db );
    query.prepare(
        "INSERT INTO notification_history (id, severity, title, message, status) VALUES (?, ?, ?, ?, ?)"_L1 );
    query.addBindValue( descriptor.id );
    query.addBindValue( static_cast<int>( descriptor.severity ) );
    query.addBindValue( descriptor.title );
    query.addBindValue( descriptor.message );
    query.addBindValue( static_cast<int>( initialStatus ) );

    if ( !query.exec() ) {
        qWarning() << "Falha ao salvar notificação no histórico:" << query.lastError().text();
    }
}

void SqliteNotificationRepository::updateStatus( const QString& notificationId, sdk::NotificationStatus newStatus ) {
    if ( !m_db.isOpen() ) {
        return;
    }

    QSqlQuery query( m_db );
    query.prepare( "UPDATE notification_history SET status = ? WHERE id = ?"_L1 );
    query.addBindValue( static_cast<int>( newStatus ) );
    query.addBindValue( notificationId );

    if ( !query.exec() ) {
        qWarning() << "Falha ao atualizar status da notificação:" << query.lastError().text();
    }
}

} // namespace qone::core
