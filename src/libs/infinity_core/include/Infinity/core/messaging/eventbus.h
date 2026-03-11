/// @file eventbus.h
/// @brief Concrete implementation of the system's Event Bus.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <shared_mutex>
#include <unordered_map>
#include <vector>
#include <memory>
#include <atomic>

#include <Infinity/sdk/contracts/IEventBus.h>

namespace qone::core {

/// @class EventBus
/// @brief Thread-safe implementation of the central event distribution system.
///
/// This class manages event subscriptions and dispatches published events to all
/// registered listeners without blocking the main execution thread.
class EventBus final : public sdk::IEventBus, public std::enable_shared_from_this<EventBus> {

public:
    /// @brief Factory method to create a shared instance of the EventBus.
    /// @return A std::shared_ptr managing the newly created EventBus.
    [[nodiscard]] static std::shared_ptr<EventBus> create();

    ~EventBus() override = default;

protected:
    /// @name SDK Contract Implementations
    /// @{

    /// @brief Dispatches an event to all subscribers registered to the given type index.
    /// @param typeIdx The RTTI index of the event type.
    /// @param eventData The type-erased event payload.
    /// @note `qCDebug( qoneCore ) << Infinity::Logging::Title( u"EVENT_BUS [DISPATCH]:"_s ) << typeIdx.name();`
    void publishImpl( std::type_index typeIdx, const void* eventData ) override;

    /// @brief Registers a new callback for a specific event type.
    /// @param typeIdx The RTTI index of the event type.
    /// @param callback The type-erased callback to execute when the event occurs.
    /// @return An @ref sdk::EventSubscription handle to manage the listener's lifecycle.
    sdk::EventSubscription subscribeImpl( std::type_index typeIdx,
                                          std::function<void( const void* )> callback ) override;
    /// @}

private:
    /// @brief Private constructor to enforce creation via the factory method.
    EventBus() = default;

    /// @brief Safely removes a subscriber from the registry.
    /// @param typeIdx The RTTI index of the event type.
    /// @param id The unique identifier of the subscription.
    void unsubscribe( std::type_index typeIdx, uint64_t id );

    /// @struct Handler
    /// @brief Internal representation of an event subscriber.
    struct Handler {
        uint64_t id;                                 ///< Unique subscription ID.
        std::function<void( const void* )> callback; ///< The execution routine.
    };

    std::unordered_map<std::type_index, std::vector<Handler>> m_subscribers; ///< The event routing table.
    mutable std::shared_mutex m_mutex;                                       ///< Ensures thread-safe access.
    std::atomic<uint64_t> m_nextId { 0 };                                    ///< Auto-incrementing ID generator.
};

} // namespace qone::core
