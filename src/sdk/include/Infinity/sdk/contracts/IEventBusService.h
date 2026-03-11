/// @file IEventBusService.h
/// @brief Injectable service providing access to the global Event Bus.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <Infinity/sdk/contracts/IEventBus.h>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @class IEventBusService
/// @brief Service interface used to retrieve the primary Event Bus instance.
class INFINITYSDK_EXPORT IEventBusService : public QObject {

    Q_OBJECT
public:
    /// @brief Unique Interface Identifier (IID).
    static constexpr const char* IID = "qone.sdk.IEventBusService";

    /// @brief Default constructor.
    explicit IEventBusService( QObject* parent = nullptr )
        : QObject( parent ) {}

    virtual ~IEventBusService() = default;

    /// @brief Retrieves the actual Event Bus instance.
    /// @return Pointer to the active @ref IEventBus.
    virtual IEventBus* bus() const = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IEventBusService, "qone.sdk.IEventBusService/1.0" )
