/// @file IViewFactory.h
/// @brief Interface for dynamic view instantiation.
/// @author André Luz <andreagenor@gmail.com>
///
#pragma once

#include <QObject>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @class IViewFactory
/// @brief Factory contract responsible for instantiating QObject-based views dynamically.
class INFINITYSDK_EXPORT IViewFactory {

public:
    virtual ~IViewFactory() = default;

    /// @brief Creates a new instance of a view.
    /// @param viewId The string identifier of the requested view.
    /// @param parent The QObject parent for memory management.
    /// @return A pointer to the newly created QObject, or nullptr if creation fails.
    /// @note `qCDebug( qoneSdkPlugin ) << Infinity::Logging::Title( u"FACTORY [CREATE_VIEW]:"_s ) << viewId;`
    virtual QObject* createView( const QString& viewId, QObject* parent ) = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IViewFactory, "qone.sdk.IViewFactory/1.0" )
