#pragma once

#include <ibootstrapinstallers.h>

namespace qone::infinity {

class CoreServicesInstaller final : public IServiceInstaller {

public:
    void installServices( AppBootstrapContext& ctx ) override;
};

class UiAndThemeInstaller final : public IServiceInstaller {

public:
    void installServices( AppBootstrapContext& ctx ) override;
};

} // namespace qone::infinity
