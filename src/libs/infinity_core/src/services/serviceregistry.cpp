#include <Infinity/core/services/serviceregistry.h>

#include <Infinity/sdk/logging/logger.h>

using namespace Qt::StringLiterals;

namespace qone::core {

void ServiceRegistry::registerService( const QString& iid, const QString& pluginId, QObject* servicePtr ) {

    std::unique_lock writeLock( m_mutex );

    if ( m_services.contains( iid ) ) {
        qCWarning( qoneCore ) << Infinity::Logging::Title( u"Overwriting service"_s ) << iid;
    }

    m_services.insert( iid, { {}, servicePtr } );
    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Registered"_s ) << iid;
}

void ServiceRegistry::unregisterServicesFor( const QString& pluginId ) {

    std::unique_lock writeLock( m_mutex );

    auto it = m_services.begin();

    while ( it != m_services.end() ) {
        if ( it->pluginId == pluginId ) {
            qCInfo( qoneCore ) << Infinity::Logging::Title( u"Unregistered"_s ) << it.key() << u"from"_s << pluginId;
            it = m_services.erase( it );
        } else {
            ++it;
        }
    }
}

QObject* ServiceRegistry::resolveService( const QString& iid ) const {

    std::shared_lock readLock( m_mutex );

    const auto it = m_services.constFind( iid );

    return ( it != m_services.constEnd() ) ? it->ptr : nullptr;
}

} // namespace qone::core
