/// @file IEventSubscriber.h
/// @brief Contract for subscribing to typed system events.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <typeindex>

#include <Infinity/sdk/logging/logger.h>
#include <Infinity/sdk/types/EventSubscription.h>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @class IEventSubscriber
/// @brief Allows components to listen to events broadcasted via the IEventBus.
class INFINITYSDK_EXPORT IEventSubscriber {

public:
    virtual ~IEventSubscriber() = default;

    /// @brief Subscribes to a specific event type.
    /// @tparam E The event struct type.
    /// @param callback The function to execute when the event is received.
    /// @return An @ref EventSubscription object managing the lifecycle of the subscription.
    template <typename E> [[nodiscard]] EventSubscription subscribe( std::function<void( const E& )> callback ) {
        using namespace Qt::StringLiterals;

        // Wrap the typed callback into a generic type-erased lambda
        auto erasedCallback = [cb = std::move( callback )]( const void* data ) {
            cb( *static_cast<const E*>( data ) );
        };

        // Log suggestion for implementation inside subscribeImpl:
        qCDebug( qoneSdkEventBus ) << Infinity::Logging::Title( u"EVENT_BUS [SUBSCRIBE]:"_s ) << typeid( E ).name();

        return subscribeImpl( std::type_index( typeid( E ) ), std::move( erasedCallback ) );
    }

protected:
    /// @brief Internal implementation for storing the type-erased callback.
    /// @param typeIdx The RTTI index of the event type.
    /// @param callback The type-erased generic callback.
    virtual EventSubscription subscribeImpl( std::type_index typeIdx, std::function<void( const void* )> callback ) = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IEventSubscriber, "qone.sdk.IEventSubscriber/1.0" )
