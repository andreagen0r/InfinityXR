#include "qmltypesinstaller.h"

#include <QQmlEngine>

#include <Infinity/sdk/logging/logger.h>

#include <Infinity/core/interaction/commandregistry.h>
#include <Infinity/core/interaction/actiondispatcher.h>

#include <Infinity/core/ui/dialogservice.h>
#include <Infinity/core/ui/dialogregistry.h>
#include <Infinity/core/ui/viewregistry.h>
#include <Infinity/core/ui/sessionmanager.h>
#include <Infinity/core/ui/menumodel.h>
#include <Infinity/core/ui/preferencesmenumodel.h>
#include <Infinity/core/ui/pluginlistmodel.h>

#include <Infinity/core/kernel/pluginmanager.h>
#include <Infinity/core/kernel/contextextensionhandler.h>
#include <Infinity/core/kernel/viewextensionhandler.h>
#include <Infinity/core/kernel/commandextensionhandler.h>
#include <Infinity/core/kernel/preferencesextensionhandler.h>

#include <Infinity/core/services/serviceregistry.h>

#include <authextensionhandler.h>

using namespace Qt::StringLiterals;

namespace qone::infinity {

void QmlTypesInstaller::registerQmlTypes( AppBootstrapContext& ctx ) {

    Infinity::Logging::printBanner( u"Qml Register Types"_s );

    constexpr auto uri { "Infinity.Core" };

    // @uri Infinity.Core
    qmlRegisterSingletonInstance<qone::core::CommandRegistry>( uri, 1, 0, "CommandRegistry",
                                                               ctx.commandRegistry.get() );
    qmlRegisterSingletonInstance<qone::core::MenuModel>( uri, 1, 0, "MenuModel", ctx.menuModel.get() );
    qmlRegisterSingletonInstance<qone::core::ActionDispatcher>( uri, 1, 0, "ActionDispatcher",
                                                                ctx.actionDispatcher.get() );
    qmlRegisterSingletonInstance<qone::core::DialogService>( uri, 1, 0, "DialogService", ctx.dialogService.get() );
    qmlRegisterSingletonInstance<qone::core::DialogRegistry>( uri, 1, 0, "DialogRegistry", ctx.dialogRegistry.get() );
    qmlRegisterSingletonInstance<qone::core::ViewRegistry>( uri, 1, 0, "ViewRegistry", ctx.viewRegistry.get() );
    qmlRegisterSingletonInstance<qone::core::SessionManager>( uri, 1, 0, "SessionManager", ctx.sessionManager.get() );
    qmlRegisterUncreatableType<qone::core::SessionManager>( uri, 1, 0, "SessionState", u"Enum access only"_s );
    qmlRegisterSingletonInstance<qone::core::PreferencesMenuModel>( uri, 1, 0, "PreferencesMenuModel",
                                                                    ctx.preferencesMenuModel.get() );
    qmlRegisterSingletonInstance<qone::core::PluginListModel>( uri, 1, 0, "PluginListModel",
                                                               ctx.pluginListModel.get() );
}

void PluginSystemInstaller::configurePlugins( AppBootstrapContext& ctx ) {

    if ( !ctx.pluginManager ) {
        return;
    }

    ctx.pluginManager->addExtensionHandler(
        std::make_unique<qone::core::ContextExtensionHandler>( ctx.serviceRegistry.get() ) );
    ctx.pluginManager->addExtensionHandler(
        std::make_unique<qone::core::ViewExtensionHandler>( ctx.viewRegistry.get() ) );
    ctx.pluginManager->addExtensionHandler(
        std::make_unique<qone::core::CommandExtensionHandler>( ctx.commandRegistry.get() ) );
    ctx.pluginManager->addExtensionHandler( std::make_unique<AuthExtensionHandler>( ctx.sessionManager.get() ) );
    ctx.pluginManager->addExtensionHandler(
        std::make_unique<qone::core::PreferencesExtensionHandler>( ctx.preferencesRegistry.get() ) );
}

} // namespace qone::infinity
