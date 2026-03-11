#pragma once

#include <ibootstrapinstallers.h>

namespace qone::infinity {

class QmlTypesInstaller final : public IQmlInstaller {

public:
    void registerQmlTypes( AppBootstrapContext& ctx ) override;
};

class PluginSystemInstaller final : public IPluginInstaller {

public:
    void configurePlugins( AppBootstrapContext& ctx ) override;
};

} // namespace qone::infinity
