/// @file dialogservice.h
/// @brief Concrete implementation of the IDialogService for the Core system.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QString>

#include <Infinity/sdk/contracts/IDialogService.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class DialogService
/// @brief Manages the dispatching of dialog open requests to the UI layer.
class CORE_EXPORT DialogService : public QObject, public qone::sdk::IDialogService {

    Q_OBJECT
    Q_INTERFACES( qone::sdk::IDialogService )

public:
    /// @brief Default constructor.
    /// @param parent The standard QObject parent.
    explicit DialogService( QObject* parent = nullptr );

    /// @brief Implements the SDK interface to request opening a dialog.
    /// @param dialogId Strongly typed identifier of the target dialog.
    void openDialog( const qone::sdk::ViewId& dialogId ) override;

Q_SIGNALS:
    /// @brief Emitted internally so the QML/UI layer can react and present the dialog.
    /// @param dialogId String representation of the requested dialog.
    void openDialogRequested( QString dialogId );
};

} // namespace qone::core
