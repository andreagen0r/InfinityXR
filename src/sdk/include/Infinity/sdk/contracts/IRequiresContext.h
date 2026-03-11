/// @file IRequiresContext.h
/// @brief Contract for injecting the Service Registry into dependent plugins.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QString>

#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

class IServiceRegistry;


/// @class IRequiresContext
/// @brief Interface that plugins implement to receive the global dependency registry.
class INFINITYSDK_EXPORT IRequiresContext {

public:
    /// @brief Standardized IID ensuring `getExtension<T>()` works seamlessly.
    static constexpr const auto IID = "qone.sdk.IRequiresContext";

    virtual ~IRequiresContext() = default;

    /// @brief Called by the Core to inject the service registry during initialization.
    /// @param registry Pointer to the global @ref IServiceRegistry.
    virtual void setServiceRegistry( IServiceRegistry* registry ) = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IRequiresContext, "qone.sdk.IRequiresContext/1.0" )
