#pragma once

#include <QUndoCommand>

#include <Infinity/sdk/logging/logger.h>

#include <Infinity/sdk/contracts/IDialogService.h>

#include "aboutmanifest.h"

class ShowAboutCommand : public QUndoCommand {

public:
    explicit ShowAboutCommand( qone::sdk::IDialogService* service )
        : m_dialogService( service ) {

        using namespace Qt::StringLiterals;

        setText( u"Open About Dialog"_s );
    }

    void redo() override {
        using namespace Qt::StringLiterals;

        if ( m_dialogService ) {
            m_dialogService->openDialog( Plugins::Views::AboutDialog );
            qCDebug( qoneApp ) << Infinity::Logging::Title( u"ShowAboutCommand Requested..."_s )
                               << Plugins::Views::AboutDialog;
            return;
        }

        qCCritical( qoneApp ) << Infinity::Logging::Title( u"Dispatcher is nullptr!"_s );
    }

private:
    qone::sdk::IDialogService* m_dialogService;
};
