#pragma once

#include <QObject>
#include <Infinity/sdk/contracts/IAuthService.h>
#include <Infinity/sdk/contracts/IViewContributor.h>
#include <Infinity/sdk/contracts/IHttpService.h>

namespace qone::sdk {

class IEventBus;
}

class AuthService : public qone::sdk::IAuthService, public qone::sdk::IViewContributor {

    Q_OBJECT
    Q_INTERFACES( qone::sdk::IViewContributor )

public:
    explicit AuthService( QObject* parent = nullptr );

    void authenticate( const QString& user, const QString& pass,
                       std::function<void( qone::sdk::AuthResult )> cb ) override;
    bool isAuthenticated() const override;
    void logout() override;
    QString loginViewId() const override;
    QUrl loginViewSource() const override;

    QList<qone::sdk::ViewDescriptor> views() const override;

    void setEventBus( qone::sdk::IEventBus* eventBus );
    void setHttpService( qone::sdk::IHttpService* httpService );

private:
    bool m_auth = false;
    qone::sdk::IEventBus* m_eventBus { nullptr };
    qone::sdk::IHttpService* m_httpService { nullptr };
};
