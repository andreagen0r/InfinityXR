/// @file main.cpp
/// @brief Ponto de entrada principal para o sistema Infinity X-Ray.
/// @details Este arquivo gerencia a inicialização do framework Qt, configuração de logs,
/// carregamento de módulos QML e instanciamento do bootstrap da aplicação.

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStringBuilder>
#include <QSettings>
#include <QJsonDocument>
#include <QDir>

#include <Infinity/sdk/logging/logger.h>
#include <Infinity/version.hpp>

#include "appbootstrap.h"
#include "installers/coreservicesinstaller.h"
#include "installers/qmltypesinstaller.h"

using namespace Qt::StringLiterals;

void setupLogging();

/// @brief Entry point do executável.
/// @param argc Quantidade de argumentos de linha de comando.
/// @param argv Vetor de argumentos de linha de comando.
/// @return int Código de status de saída da aplicação (0 para sucesso).
auto main( int argc, char* argv[] ) -> int {

    QGuiApplication app( argc, argv );

    // Configurações globais de identidade da aplicação para persistência e metadados
    QGuiApplication::setOrganizationName( "OneXRay"_L1 );
    QGuiApplication::setOrganizationDomain( "infinity.com"_L1 );
    QGuiApplication::setApplicationName( "Infinity"_L1 );

    setupLogging();

    Infinity::Logging::printBanner( "INFINITY X-RAY SYSTEM STARTUP"_L1, Infinity::Logging::Type::Warning );

    const QString version = "Version Control: "_L1 % QLatin1StringView( Version::GIT_TAG ) % ", Hash: "_L1
                          % QLatin1StringView( Version::GIT_HASH ) % ", Branch: "_L1
                          % QLatin1StringView( Version::GIT_BRANCH );
    Infinity::Logging::printBanner( version );

    QQmlApplicationEngine engine;


    // Define os caminhos de busca para módulos QML e Plugins
    const QString appPath { QCoreApplication::applicationDirPath() };
    const QString styleDir { appPath % "/style"_L1 };
    const QString modulesDir { appPath % "/modules"_L1 };
    const QString pluginsDir { appPath % "/Plugins"_L1 };

    const QStringList modulePaths { styleDir, modulesDir, pluginsDir };

    for ( const auto& path : modulePaths ) {
        engine.addImportPath( path );
    }

    qCDebug( qoneApp ) << Infinity::Logging::Title( u"Engine Path list:"_s ) << engine.importPathList().join( "; "_L1 );


    /// @class AppBootstrap
    /// @brief Responsável por injetar contextos e preparar a lógica de negócio no motor QML.
    qone::infinity::AppBootstrap bootstrap( engine );
    // clang-format off
    bootstrap
        .setPluginsDirectory(pluginsDir)
        .addInstaller(std::make_unique<qone::infinity::CoreServicesInstaller>())
        .addInstaller(std::make_unique<qone::infinity::UiAndThemeInstaller>())
        .addInstaller(std::make_unique<qone::infinity::QmlTypesInstaller>())
        .addInstaller(std::make_unique<qone::infinity::PluginSystemInstaller>())
        .build();

    // Conexão de segurança: Encerra a aplicação se o objeto QML falhar ao ser criado
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() {
            QCoreApplication::exit( -1 );
        }, Qt::QueuedConnection );
    // clang-format on

    // Carrega o módulo principal de UI (Main.qml)
    engine.loadFromModule( "Infinity"_L1, "Main"_L1 );

    const auto exit { QGuiApplication::exec() };

    if ( exit == 0 ) {
        qCDebug( qoneApp ) << Infinity::Logging::Title( u"The application was successfully finished"_s );
    } else {
        qCCritical( qoneApp ) << Infinity::Logging::Title( u"The application exited with an error"_s );
    }

    return exit;
}


/// @brief Configura o sistema de logging da aplicação.
/// @details Registra tipos de meta-objetos necessários, lê as configurações de um arquivo
/// de persistência (QSettings) e aplica as regras de filtragem de categorias do Qt.
void setupLogging() {
    // Registra a estrutura de configuração para o sistema de Meta-Objetos do Qt
    qRegisterMetaType<Infinity::Logging::Config>();

    QSettings settings;
    // Recupera a configuração de log persistida no sistema/registro
    auto loggingCfg = settings.value( "LOGGING/config"_L1 ).value<Infinity::Logging::Config>();
    loggingCfg.level = 0;
    loggingCfg.filterRules = "qml.*=false\n"
                             "qone.Core.*=true\n"_L1;

    // Inicializa o SDK de Logger da Infinity
    Infinity::Logging::init( loggingCfg );

    // Define as regras de filtro para as categorias de log (ex: *.debug=false)
    QLoggingCategory::setFilterRules( loggingCfg.filterRules );
}
