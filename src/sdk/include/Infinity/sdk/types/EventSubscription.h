/// @file EventSubscription.h
/// @brief RAII handle for managing event subscriptions.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <functional>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @class EventSubscription
/// @brief Automatically unsubscribes from an event when destroyed.
///
/// Utilizing the RAII (Resource Acquisition Is Initialization) idiom,
/// this class ensures that dangling callbacks are prevented.
class INFINITYSDK_EXPORT EventSubscription {

public:
    /// @typedef UnsubscribeFunc
    /// @brief Signature for the internal unsubscribe callback.
    using UnsubscribeFunc = std::function<void()>;

    /// @brief Default constructor (creates an empty, inactive subscription).
    EventSubscription() = default;

    /// @brief Constructs a subscription with a specific cleanup routine.
    /// @param func The lambda/function to call upon destruction.
    explicit EventSubscription( UnsubscribeFunc func )
        : m_unsubscribe( std::move( func ) ) {}

    /// @brief Destructor that triggers the unsubscription logic.
    ~EventSubscription() {
        if ( m_unsubscribe != nullptr ) {
            // Note: Avoid heavy logging here to prevent performance hits during mass destruction.
            m_unsubscribe();
        }
    };

    /// @name Move Semantics
    /// Allows the subscription ownership to be transferred safely.
    /// @{
    EventSubscription( EventSubscription&& ) = default;
    EventSubscription& operator=( EventSubscription&& ) = default;
    /// @}

    /// @name Copy Semantics
    /// Deleted to prevent accidental multiple unsubscriptions.
    /// @{
    EventSubscription( const EventSubscription& ) = delete;
    EventSubscription& operator=( const EventSubscription& ) = delete;
    /// @}

private:
    UnsubscribeFunc m_unsubscribe; ///< The cleanup function to execute.
};

} // namespace qone::sdk
