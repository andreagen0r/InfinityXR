#pragma once

#include <memory>
#include <QDir>
#include <QQmlApplicationEngine>
#include <QTranslator>

namespace qone::sdk {

class IEventBus;

}

namespace qone::core {

class QSettingsBackend;
class CommandRegistry;
class DialogService;
class SessionManager;
class ServiceRegistry;
class ViewRegistry;
class DialogRegistry;
class PreferencesRegistry;
class SettingsService;
class ActionDispatcher;
class MenuModel;
class PluginManager;
class PluginListModel;
class PreferencesMenuModel;
class HttpService;
class NotificationService;

} // namespace qone::core


namespace qone::infinity {

class SystemSignalHandler;

struct AppBootstrapContext {

    QQmlApplicationEngine& qmlEngine;
    QDir pluginsDir;

    // Tradutores da aplicação
    QTranslator coreTranslator;
    QTranslator appTranslator;

    // Instâncias do Core
    std::shared_ptr<qone::sdk::IEventBus> eventBus;
    std::unique_ptr<qone::core::QSettingsBackend> qsettingsBackend;
    std::unique_ptr<qone::core::CommandRegistry> commandRegistry;
    std::unique_ptr<qone::core::DialogService> dialogService;
    std::unique_ptr<qone::core::SessionManager> sessionManager;
    std::unique_ptr<qone::core::ServiceRegistry> serviceRegistry;
    std::unique_ptr<qone::core::ViewRegistry> viewRegistry;
    std::unique_ptr<qone::core::DialogRegistry> dialogRegistry;
    std::unique_ptr<qone::core::PreferencesRegistry> preferencesRegistry;
    std::unique_ptr<qone::core::SettingsService> settingsService;
    std::unique_ptr<qone::core::ActionDispatcher> actionDispatcher;
    std::unique_ptr<qone::core::MenuModel> menuModel;
    std::unique_ptr<qone::core::PluginManager> pluginManager;
    std::unique_ptr<qone::core::PluginListModel> pluginListModel;
    std::unique_ptr<qone::core::PreferencesMenuModel> preferencesMenuModel;
    std::unique_ptr<qone::core::HttpService> httpService;
    std::shared_ptr<qone::core::NotificationService> notificationService;

    SystemSignalHandler* signalHandler { nullptr };

    explicit AppBootstrapContext( QQmlApplicationEngine& engine );
    ~AppBootstrapContext();
};

} // namespace qone::infinity
