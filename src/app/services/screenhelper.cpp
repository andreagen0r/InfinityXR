#include "screenhelper.h"

#include <QScreen>

#include <Infinity/sdk/logging/logger.h>

using namespace Qt::StringLiterals;

namespace qone::infinity {


ScreenHelper::ScreenHelper( QObject* parent )
    : QObject { parent } {}

/// \brief Calcula a densidade de pixels (pixels por milímetro) de uma tela específica.
///
/// O algoritmo busca a tela pelo nome fornecido. Se encontrada, utiliza as dimensões
/// físicas e a resolução atual para calcular a densidade real via diagonal (Pitágoras).
/// Isso evita problemas com a orientação (Portrait/Landscape) da tela.
///
/// \param screenName Nome identificador da tela (ex: "eDP-1").
/// \return A densidade calculada em px/mm ou um valor de fallback (4.0) em caso de erro.
double ScreenHelper::getDensityByName( const QString& screenName ) {


    constexpr double fallbackDensity { 4.0 };

    if ( screenName.isEmpty() ) {
        qCWarning( qoneApp ) << Infinity::Logging::Title( u"Screen name is empty."_s )
                             << ( u" Returning default fallback: "_s % QString::number( fallbackDensity ) );
        return fallbackDensity; // Fallback
    }

    qCDebug( qoneApp ) << Infinity::Logging::Title( u"Density Lookup:"_s )
                       << ( u"Searching for screen: "_s % screenName );

    QList<QScreen*> screens { QGuiApplication::screens() };
    QScreen* targetScreen { nullptr };

    // Procura o ponteiro real baseado no nome
    for ( QScreen* s : screens ) {
        if ( s->name() == screenName ) {
            targetScreen = s;
            break;
        }
    }

    if ( !targetScreen ) {
        qCWarning( qoneApp ) << Infinity::Logging::Title( u"Monitor not found by name:"_s )
                             << ( screenName % u"| Available: "_s % QString::number( screens.size() )
                                  % u" screens."_s );
        return fallbackDensity;
    }

    const QSizeF physSize { targetScreen->physicalSize() };
    const QSize res { targetScreen->size() };

    auto quote = u"\""_s;
    // clang-format off
    qCDebug( qoneApp ).noquote() << Infinity::Logging::Title( u"Hardware Name:"_s )
                       << (
                            quote
                            % screenName
                            % quote
                            % u" Resolution: "_s
                            % QString::number( res.width() )
                            % u"x"_s
                            % QString::number( res.height() )
                            % u" PhysSize (mm): "_s
                            % QString::number( physSize.width() )
                            % u"x"_s
                            % QString::number( physSize.height() ) );
    // clang-format on

    if ( physSize.width() <= 0 || physSize.height() <= 0 ) {
        const double logicalDpiDensity = targetScreen->logicalDotsPerInch() / 25.4;
        qCWarning( qoneApp ) << Infinity::Logging::Title( u"Invalid physical size reported by hardware."_s )
                             << ( u"Falling back to logical DPI density: "_s % QString::number( logicalDpiDensity ) );
        return logicalDpiDensity;
    }

    // Cálculo pela diagonal, Pitágoras, para ignorar rotação
    const double diagonalPixels { qHypot( res.width(), res.height() ) };
    const double diagonalMm { qHypot( physSize.width(), physSize.height() ) };

    if ( diagonalMm == 0 ) {
        qCCritical( qoneApp ) << Infinity::Logging::Title( u"Diagonal in mm is zero for screen:"_s )
                              << ( screenName % u". Calculation impossible."_s );
        return fallbackDensity;
    }

    const double density { diagonalPixels / diagonalMm };

    qCDebug( qoneApp ) << Infinity::Logging::Title( u"Density Calculated"_s )
                       << ( u"Screen: "_s % screenName % u" Result: "_s % QString::number( density ) % u" px/mm"_s );

    return density;
}

} // namespace qone::infinity
