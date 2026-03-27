#include "coreservicesinstaller.h"

#include <QCoreApplication>
#include <QLocale>
#include <QUrl>

#include <Infinity/sdk/logging/logger.h>
#include <Infinity/sdk/contracts/IDialogService.h>
#include <Infinity/sdk/contracts/IEventBusService.h>
#include <Infinity/sdk/contracts/INotificationRepository.h>

#include <Infinity/core/messaging/eventbus.h>

#include <Infinity/core/services/qsettingsbackend.h>
#include <Infinity/core/services/settingsservice.h>
#include <Infinity/core/services/serviceregistry.h>
#include <Infinity/core/services/preferencesregistry.h>
#include <Infinity/core/services/httpservice.h>
#include <Infinity/core/services/notificationservice.h>
#include <Infinity/core/services/sqlitenotificationrepository.h>

#include <Infinity/core/interaction/commandregistry.h>
#include <Infinity/core/interaction/actiondispatcher.h>
#include <Infinity/core/interaction/showpreferencescommand.h>

#include <Infinity/core/ui/dialogservice.h>
#include <Infinity/core/ui/sessionmanager.h>
#include <Infinity/core/ui/viewregistry.h>
#include <Infinity/core/ui/dialogregistry.h>
#include <Infinity/core/ui/menumodel.h>
#include <Infinity/core/ui/preferencesmenumodel.h>
#include <Infinity/core/ui/pluginlistmodel.h>
#include <Infinity/core/ui/systemuimanifest.h>

#include <Infinity/core/kernel/pluginmanager.h>

#include <services/systemsignalhandler.h>

using namespace Qt::StringLiterals;

namespace qone::infinity {

EventBusServiceProvider::EventBusServiceProvider( qone::sdk::IEventBus* bus, QObject* parent )
    : qone::sdk::IEventBusService( parent )
    , m_bus( bus ) {}

qone::sdk::IEventBus* EventBusServiceProvider::bus() const {
    return m_bus;
}

// --------------------------------------------------------------------------------------------------------------------

void CoreServicesInstaller::installServices( AppBootstrapContext& ctx ) {

    ctx.eventBus = qone::core::EventBus::create();
    ctx.qsettingsBackend = std::make_unique<qone::core::QSettingsBackend>();
    ctx.commandRegistry = std::make_unique<qone::core::CommandRegistry>();
    ctx.dialogService = std::make_unique<qone::core::DialogService>();
    ctx.sessionManager = std::make_unique<qone::core::SessionManager>();
    ctx.serviceRegistry = std::make_unique<qone::core::ServiceRegistry>();
    ctx.viewRegistry = std::make_unique<qone::core::ViewRegistry>();
    ctx.dialogRegistry = std::make_unique<qone::core::DialogRegistry>();
    ctx.preferencesRegistry = std::make_unique<qone::core::PreferencesRegistry>();

    ctx.preferencesMenuModel = std::make_unique<qone::core::PreferencesMenuModel>( ctx.preferencesRegistry.get() );

    ctx.signalHandler = new qone::infinity::SystemSignalHandler( &ctx.qmlEngine );
    ctx.signalHandler->install();

    QObject::connect( ctx.signalHandler, &qone::infinity::SystemSignalHandler::gracefulShutdownRequested, []() {
        qCDebug( qoneApp ) << Infinity::Logging::Title( u"Graceful shutdown requested by OS."_s );
    } );

    qCInfo( qoneApp ) << Infinity::Logging::Title( u"Search Plugins in directory"_s ) << ctx.pluginsDir.absolutePath();

    ctx.settingsService = std::make_unique<qone::core::SettingsService>( std::move( ctx.qsettingsBackend ) );

    ctx.httpService = std::make_unique<qone::core::HttpService>();

    auto eventBusWrapper = new EventBusServiceProvider( ctx.eventBus.get() );

    ctx.actionDispatcher = std::make_unique<qone::core::ActionDispatcher>( *ctx.commandRegistry );
    ctx.menuModel = std::make_unique<qone::core::MenuModel>( ctx.commandRegistry.get() );

    ctx.pluginManager = std::make_unique<qone::core::PluginManager>( ctx.pluginsDir, ctx.commandRegistry.get(),
                                                                     *ctx.settingsService, ctx.eventBus.get() );

    ctx.pluginListModel = std::make_unique<qone::core::PluginListModel>( ctx.pluginManager.get() );

    ctx.serviceRegistry->registerService( QString::fromUtf8( qone::sdk::IDialogService::IID ), "core"_L1,
                                          ctx.dialogService.get() );

    ctx.serviceRegistry->registerService( QString::fromUtf8( qone::sdk::IEventBusService::IID ), "SystemCore"_L1,
                                          eventBusWrapper );

    ctx.serviceRegistry->registerService( QString::fromUtf8( qone::sdk::IHttpService::IID ), "SystemCore"_L1,
                                          ctx.httpService.get() );

    auto notificationRepo = std::make_shared<qone::core::SqliteNotificationRepository>();

    ctx.serviceRegistry->registerService( QString::fromUtf8( qone::sdk::INotificationRepository::IID ), "SystemCore"_L1,
                                          notificationRepo.get() );

    ctx.notificationService = std::make_shared<qone::core::NotificationService>( ctx.eventBus, notificationRepo );
}

void UiAndThemeInstaller::installServices( AppBootstrapContext& ctx ) {

    const QString locale { QLocale::system().name() };

    qCDebug( qoneApp ) << Infinity::Logging::Title( u"System locale"_s ) << locale;

    // TODO: Resolver o path para o resource
    if ( ctx.coreTranslator.load( ":/i18n/InfinityCore_"_L1 % locale ) ) {
        QCoreApplication::installTranslator( &ctx.coreTranslator );
    }

    // TODO: Resolver o path para o resource
    if ( ctx.appTranslator.load( ":/i18n/InfinityApp_"_L1 % locale ) ) {
        QCoreApplication::installTranslator( &ctx.appTranslator );
    }

    qCDebug( qoneApp ) << Infinity::Logging::Title( u"Registering Core UI"_s );

    ctx.viewRegistry->registerView( {
        .id = qone::core::Views::PreferencesDialog,
        .title = QCoreApplication::translate( "CoreUI", "Preferences" ),
        .icon = ""_L1,
        .source = QUrl( u"qrc:/qt/qml/Infinity/views/PreferencesDialog.qml"_s ),
        .type = sdk::ViewType::Overlay,
    } );

    qone::sdk::CommandDescriptor cmdPrefs { .id = qone::core::Commands::ShowPreferences,
                                            .text = QCoreApplication::translate( "CoreUI", "Preferences" ),
                                            .iconName = ""_L1,
                                            .description = QCoreApplication::translate( "CoreUI", "Preferences Panel" ),
                                            .menuPath = { QCoreApplication::translate( "CoreUI", "&Edit" ),
                                                          QCoreApplication::translate( "CoreUI", "Preferences" ) } };

    ctx.commandRegistry->registerCommand( cmdPrefs,
                                          [dialogSvc = ctx.dialogService.get()]( const QVariantMap& ) -> QUndoCommand* {
                                              if ( !dialogSvc ) {
                                                  return nullptr;
                                              }
                                              return new qone::core::ShowPreferencesCommand( dialogSvc );
                                          } );
}

} // namespace qone::infinity
