#include <Infinity/core/ui/dialogservice.h>

#include <Infinity/sdk/logging/logger.h>

using namespace Qt::StringLiterals;

qone::core::DialogService::DialogService( QObject* parent )
    : QObject( parent ) {}

void qone::core::DialogService::openDialog( const sdk::ViewId& dialogId ) {
    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Preparing to send a signal to"_s ) << dialogId;

    Q_EMIT openDialogRequested( dialogId.value );

    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Signal successfully transmitted!"_s );
}
