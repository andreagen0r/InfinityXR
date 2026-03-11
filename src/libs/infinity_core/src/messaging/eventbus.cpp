#include <Infinity/core/messaging/eventbus.h>

namespace qone::core {

std::shared_ptr<EventBus> EventBus::create() {
    return std::shared_ptr<EventBus>( new EventBus() );
}

void EventBus::publishImpl( std::type_index typeIdx, const void* eventData ) {
    std::vector<std::function<void( const void* )>> snapshot;

    {
        // read lock scope
        std::shared_lock lock( m_mutex );
        if ( auto it = m_subscribers.find( typeIdx ); it != m_subscribers.end() ) {
            for ( const auto& handler : it->second ) {
                snapshot.push_back( handler.callback );
            }
        }
    }

    // Executes outside of a lock, free from deadlocks/reentry.
    for ( const auto& cb : snapshot ) {
        cb( eventData );
    }
}

sdk::EventSubscription EventBus::subscribeImpl( std::type_index typeIdx, std::function<void( const void* )> callback ) {
    uint64_t id = 0;

    {
        // write lock scope
        std::unique_lock lock( m_mutex );
        id = ++m_nextId;
        m_subscribers[typeIdx].push_back( { id, std::move( callback ) } );
    }

    std::weak_ptr<EventBus> weak_this = shared_from_this();

    return sdk::EventSubscription( [weak_this, typeIdx, id]() {
        if ( auto bus = weak_this.lock() ) {
            bus->unsubscribe( typeIdx, id );
        }
    } );
}

void EventBus::unsubscribe( std::type_index typeIdx, uint64_t id ) {
    std::unique_lock lock( m_mutex );
    if ( auto it = m_subscribers.find( typeIdx ); it != m_subscribers.end() ) {
        std::erase_if( it->second, [id]( const auto& h ) {
            return h.id == id;
        } );
    }
}

} // namespace qone::core
