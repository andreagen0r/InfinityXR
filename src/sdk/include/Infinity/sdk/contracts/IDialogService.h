/// @file IDialogService.h
/// @brief Service contract for displaying dialogs to the user.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QString>

#include <Infinity/sdk/infinitysdk_export.h>
#include <Infinity/sdk/types/ViewId.h>

namespace qone::sdk {

/// @class IDialogService
/// @brief Abstracts the creation and display of dialog windows.
///
/// Allows plugins to request the opening of a dialog without needing to know
/// the underlying implementation details (e.g., QML, QWidget, or Signals).
class INFINITYSDK_EXPORT IDialogService {

public:
    /// @brief Unique Interface Identifier (IID).
    static constexpr auto IID { "qone.sdk.IDialogService" };

    virtual ~IDialogService() = default;

    /// @brief Requests the system to open a specific dialog.
    /// @param dialogId The strongly-typed identifier of the dialog view.
    /// @note `qCInfo( qoneSdkPlugin ) << Infinity::Logging::Title( u"DIALOG [OPEN]:"_s ) << dialogId;`
    virtual void openDialog( const qone::sdk::ViewId& dialogId ) = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IDialogService, "qone.sdk.IDialogService/1.0" )
