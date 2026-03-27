#pragma once

#include <QSortFilterProxyModel>

#include <Infinity/sdk/types/NotificationDescriptor.h>
#include <Infinity/core/core_export.h>

namespace qone::core {

class CORE_EXPORT NotificationFilterModel : public QSortFilterProxyModel {

    Q_OBJECT
    Q_PROPERTY( int acceptedPosition READ acceptedPosition WRITE setAcceptedPosition NOTIFY acceptedPositionChanged )

public:
    explicit NotificationFilterModel( QObject* parent = nullptr );

    [[nodiscard]] int acceptedPosition() const;
    void setAcceptedPosition( int position );

protected:
    bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const override;

Q_SIGNALS:
    void acceptedPositionChanged();

private:
    int m_acceptedPosition { static_cast<int>( qone::sdk::NotificationPosition::BottomRight ) };
};

} // namespace qone::core
