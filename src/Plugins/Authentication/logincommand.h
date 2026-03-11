#pragma once

#include <QUndoCommand>
#include <QObject>
#include <Infinity/sdk/contracts/IAuthService.h>

class LoginCommand : public QUndoCommand {
public:
    LoginCommand( qone::sdk::IAuthService* authService, const QString& user, const QString& pass );

    void redo() override;
    void undo() override;

private:
    qone::sdk::IAuthService* m_authService;
    QString m_user;
    QString m_pass;
};
