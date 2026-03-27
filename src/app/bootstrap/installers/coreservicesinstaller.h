#pragma once

#include <ibootstrapinstallers.h>
#include <Infinity/sdk/contracts/IEventBusService.h>

namespace qone::infinity {

class EventBusServiceProvider : public qone::sdk::IEventBusService {

    Q_OBJECT
    Q_INTERFACES( qone::sdk::IEventBusService )

public:
    explicit EventBusServiceProvider( qone::sdk::IEventBus* bus, QObject* parent = nullptr );
    qone::sdk::IEventBus* bus() const override;

private:
    qone::sdk::IEventBus* m_bus;
};

class CoreServicesInstaller final : public IServiceInstaller {

public:
    void installServices( AppBootstrapContext& ctx ) override;
};

class UiAndThemeInstaller final : public IServiceInstaller {

public:
    void installServices( AppBootstrapContext& ctx ) override;
};

} // namespace qone::infinity
