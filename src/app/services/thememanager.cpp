#include "thememanager.h"

#include <Infinity/sdk/logging/logger.h>

#include <QFile>

using namespace Qt::StringLiterals;

namespace qone::infinity {

ThemeManager::ThemeManager( QObject* parent )
    : QObject { parent } {}

QJsonDocument ThemeManager::loadFromFile( const QUrl& theme ) {

    QString filePath;

    if ( theme.isLocalFile() ) {
        filePath = theme.toLocalFile();
    } else if ( theme.scheme() == "qrc"_L1 ) {
        filePath = ":"_L1 % theme.path();
    } else {
        qCWarning( qoneStyle ) << Infinity::Logging::Title( u"Unsupported URL scheme:"_s ) << theme;
        return {};
    }

    QFile file( filePath );
    if ( !file.open( QIODevice::ReadOnly ) ) {
        qCWarning( qoneStyle ) << Infinity::Logging::Title( u"Could not open theme file:"_s ) << filePath;
        return {};
    }

    QJsonParseError parseError;
    QJsonDocument doc { QJsonDocument::fromJson( file.readAll(), &parseError ) };

    if ( parseError.error != QJsonParseError::NoError ) {
        qCWarning( qoneStyle ) << Infinity::Logging::Title( u"JSON parse error"_s ) << u" File:"_s << filePath
                               << u" Error:"_s << parseError.errorString() << u" at offset:"_s << parseError.offset;
        return {};
    }

    qCInfo( qoneStyle ) << Infinity::Logging::Title( u"Theme file loaded successfully from"_s ) << filePath;

    return doc;
}

} // namespace qone::infinity
