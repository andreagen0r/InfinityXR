#include <Infinity/core/ui/viewregistry.h>

#include <Infinity/sdk/logging/logger.h>

using namespace Qt::StringLiterals;

namespace qone::core {

QUrl ViewRegistry::getViewSource( const QString& viewId ) const {

    qCDebug( qoneCore ) << Infinity::Logging::Title( u"QML requesting"_s ) << viewId;

    const qone::sdk::ViewId safeId( viewId );

    const auto view = findView( safeId );
    if ( view.has_value() ) {
        qCDebug( qoneCore ) << Infinity::Logging::Title( u"Found! Returning:"_s ) << view->source;
        return view->source;
    }

    qCWarning( qoneCore ) << Infinity::Logging::Title( u"View ID NOT FOUND:"_s ) << viewId;
    return QUrl();
}

void ViewRegistry::registerView( const qone::sdk::ViewDescriptor& descriptor ) {

    std::unique_lock writeLock( m_mutex );

    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Storing View:"_s ) << descriptor.id.value;

    m_views.insert( descriptor.id, descriptor );
}

void ViewRegistry::unregisterView( const sdk::ViewId& viewId ) {

    std::unique_lock writeLock( m_mutex );

    if ( !m_views.remove( viewId ) ) {
        qCWarning( qoneCore ) << Infinity::Logging::Title( u"Attempted to unregister unknown view"_s ) << viewId.value;
        return;
    }
    qCInfo( qoneCore ) << Infinity::Logging::Title( u"ViewRegistry: Unregistered view"_s ) << viewId.value;
}

std::optional<qone::sdk::ViewDescriptor> ViewRegistry::findView( const sdk::ViewId& viewId ) const {

    std::shared_lock readLock( m_mutex );

    const auto it = m_views.constFind( viewId );

    if ( it == m_views.constEnd() ) {
        return std::nullopt;
    }

    return *it;
}

QUrl ViewRegistry::getUrl( const QString& viewId ) const {

    const qone::sdk::ViewId safeId( viewId );

    return m_views.value( safeId ).source;
}
} // namespace qone::core
