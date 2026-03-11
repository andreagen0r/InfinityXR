/// @file IEventPublisher.h
/// @brief Contract for publishing events to the system's event bus.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <typeindex>
#include <Infinity/sdk/logging/logger.h>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @class IEventPublisher
/// @brief Interface responsible for dispatching typed events throughout the system.
///
/// This interface uses a type-erasure pattern at the point of publication,
/// allowing generic events to be routed seamlessly without tight coupling.
class INFINITYSDK_EXPORT IEventPublisher {

public:
    /// @brief Default virtual destructor.
    virtual ~IEventPublisher() = default;

    /// @brief Publishes a generic event to the event bus.
    /// @tparam E The type of the event being published (deduced automatically).
    /// @param event The event instance to be broadcasted to subscribers.
    template <typename E> void publish( const E& event ) {
        using namespace Qt::StringLiterals;
        qCDebug( qoneSdkEventBus ) << Infinity::Logging::Title( u"PUBLISH:"_s ) << u"Publishing event of type:"_s
                                   << typeid( E ).name();
        publishImpl( std::type_index( typeid( E ) ), &event );
    }

protected:
    /// @brief Internal implementation for routing the event.
    /// @param typeIdx The type index (RTTI) of the event.
    /// @param eventData Opaque pointer (type-erased) to the event data.
    virtual void publishImpl( std::type_index typeIdx, const void* eventData ) = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IEventPublisher, "qone.sdk.IEventPublisher/1.0" )
