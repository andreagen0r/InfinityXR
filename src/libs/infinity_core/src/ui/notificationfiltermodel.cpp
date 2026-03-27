#include <Infinity/core/ui/notificationfiltermodel.h>
#include <Infinity/core/ui/notificationmodel.h>

namespace qone::core {

NotificationFilterModel::NotificationFilterModel( QObject* parent )
    : QSortFilterProxyModel( parent ) {
    setDynamicSortFilter( true );
}

int NotificationFilterModel::acceptedPosition() const {
    return m_acceptedPosition;
}

void NotificationFilterModel::setAcceptedPosition( int position ) {

    if ( m_acceptedPosition != position ) {
        m_acceptedPosition = position;
        Q_EMIT acceptedPositionChanged();
        invalidateFilter();
    }
}

bool NotificationFilterModel::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const {
    if ( !sourceModel() ) {
        return false;
    }

    QModelIndex idx = sourceModel()->index( source_row, 0, source_parent );
    int itemPosition = sourceModel()->data( idx, NotificationModel::PositionRole ).toInt();

    return itemPosition == m_acceptedPosition;
}

} // namespace qone::core
