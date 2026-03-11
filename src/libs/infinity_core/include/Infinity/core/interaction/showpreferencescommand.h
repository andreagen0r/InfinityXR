/// @file showpreferencescommand.h
/// @brief Command implementation for opening the global preferences dialog.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QUndoCommand>

#include <Infinity/sdk/contracts/IDialogService.h>
#include <Infinity/sdk/logging/logger.h>

#include <Infinity/core/ui/systemuimanifest.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class ShowPreferencesCommand
/// @brief Encapsulates the action of opening the system preferences.
///
/// This is a non-undoable command whose sole purpose is to abstract the
/// dialog service call into the standardized command execution flow.
class CORE_EXPORT ShowPreferencesCommand : public QUndoCommand {

public:
    /// @brief Constructs the command with the required Dialog Service.
    /// @param service Pointer to the @ref IDialogService responsible for the UI.
    explicit ShowPreferencesCommand( qone::sdk::IDialogService* service )
        : m_dialogService( service ) {
        using namespace Qt::StringLiterals;
        setText( u"Open Preferences"_s );
    }

    /// @brief Executes the command, requesting the dialog to open.
    void redo() override {
        using namespace Qt::StringLiterals;
        if ( m_dialogService ) {
            m_dialogService->openDialog( qone::core::Views::PreferencesDialog );
            return;
        }
        qCCritical( qoneApp ) << Infinity::Logging::Title( u"Dialog Service is nullptr!"_s );
    }

private:
    qone::sdk::IDialogService* m_dialogService;
};

} // namespace qone::core
