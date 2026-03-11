#pragma once

#include <QObject>
#include <QUrl>

#include <Infinity/sdk/contracts/IPlugin.h>
#include <Infinity/sdk/contracts/ICommandContributor.h>
#include <Infinity/sdk/contracts/IRequiresContext.h>

#include "authservice.h"

namespace qone::sdk {
class IServiceRegistry;
}

class AuthPlugin : public QObject, public qone::sdk::IPlugin, public qone::sdk::IRequiresContext {

    Q_OBJECT
    Q_PLUGIN_METADATA( IID "qone.plugin.AuthPlugin" FILE "manifest.json" )
    Q_INTERFACES( qone::sdk::IPlugin qone::sdk::IRequiresContext )

public:
    explicit AuthPlugin( QObject* parent = nullptr );

    void onLoad() override;
    void onUnload() override;

    void setServiceRegistry( qone::sdk::IServiceRegistry* registry ) override;

    void* extension( const QString& extensionId ) override;


private:
    class CommandProvider : public qone::sdk::ICommandContributor {
    public:
        explicit CommandProvider( AuthService* service );

        QList<qone::sdk::CommandRegistration> getCommands() const override;

    private:
        AuthService* m_service;
    };

    AuthService m_authService; // Service instance
    CommandProvider m_commandProvider;
};
