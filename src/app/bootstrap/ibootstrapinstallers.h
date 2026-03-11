#pragma once

#include "appbootstrapcontext.h"

namespace qone::infinity {

class IServiceInstaller {

public:
    virtual ~IServiceInstaller() = default;
    virtual void installServices( AppBootstrapContext& context ) = 0;
};

class IQmlInstaller {

public:
    virtual ~IQmlInstaller() = default;
    virtual void registerQmlTypes( AppBootstrapContext& context ) = 0;
};

class IPluginInstaller {

public:
    virtual ~IPluginInstaller() = default;
    virtual void configurePlugins( AppBootstrapContext& context ) = 0;
};

} // namespace qone::infinity
