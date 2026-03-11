/// @file IEventBus.h
/// @brief Unified contract for the system's Event Bus.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <Infinity/sdk/contracts/IEventPublisher.h>
#include <Infinity/sdk/contracts/IEventSubscriber.h>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @class IEventBus
/// @brief Combines publishing and subscribing capabilities into a single bus interface.
///
/// Components holding a reference to this interface can both dispatch new events
/// to the system and listen to incoming events.
class INFINITYSDK_EXPORT IEventBus : public IEventPublisher, public IEventSubscriber {

public:
    virtual ~IEventBus() = default;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IEventBus, "qone.sdk.IEventBus/1.0" )
