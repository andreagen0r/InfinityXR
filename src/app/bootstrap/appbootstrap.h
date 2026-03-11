#pragma once

#include <QObject>
#include <memory>
#include <vector>

#include <appbootstrapcontext.h>
#include <ibootstrapinstallers.h>

namespace qone::infinity {

class AppBootstrap : public QObject {

    Q_OBJECT

public:
    explicit AppBootstrap( QQmlApplicationEngine& engine, QObject* parent = nullptr );
    ~AppBootstrap() override;

    AppBootstrap& setPluginsDirectory( const QDir& dir );

    AppBootstrap& addInstaller( std::unique_ptr<IServiceInstaller> installer );
    AppBootstrap& addInstaller( std::unique_ptr<IQmlInstaller> installer );
    AppBootstrap& addInstaller( std::unique_ptr<IPluginInstaller> installer );

    void build();

private:
    std::unique_ptr<AppBootstrapContext> m_context;

    std::vector<std::unique_ptr<IServiceInstaller>> m_serviceInstallers;
    std::vector<std::unique_ptr<IQmlInstaller>> m_qmlInstallers;
    std::vector<std::unique_ptr<IPluginInstaller>> m_pluginInstallers;
};

} // namespace qone::infinity
