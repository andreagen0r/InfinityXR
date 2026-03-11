/// @file IViewContributor.h
/// @brief Extension interface for providing custom UI Views.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QList>

#include <Infinity/sdk/infinitysdk_export.h>
#include <Infinity/sdk/types/ViewDescriptor.h>

namespace qone::sdk {

/// @class IViewContributor
/// @brief Allows a plugin to register new visual components (Views) in the application.
class INFINITYSDK_EXPORT IViewContributor {

public:
    /// @brief Unique Interface Identifier (IID).
    static constexpr auto IID { "qone.sdk.IViewContributor" };

    virtual ~IViewContributor() = default;

    /// @brief Retrieves the descriptors for all views this plugin contributes.
    /// @return A list of @ref ViewDescriptor containing the metadata for each view.
    virtual QList<ViewDescriptor> views() const = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IViewContributor, "qone.sdk.IViewContributor/1.0" )
