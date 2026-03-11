#pragma once

#include <QObject>
#include <QtPlugin>
#include <memory>

#include <Infinity/sdk/contracts/IPlugin.h>
#include <Infinity/sdk/contracts/IViewContributor.h>
#include <Infinity/sdk/contracts/ICommandContributor.h>
#include <Infinity/sdk/contracts/IRequiresContext.h>

class UiShowcaseViewContributor : public qone::sdk::IViewContributor {

public:
    QList<qone::sdk::ViewDescriptor> views() const override;
};

class AboutCommandContributor : public qone::sdk::ICommandContributor, public qone::sdk::IRequiresContext {

public:
    QList<qone::sdk::CommandRegistration> getCommands() const override;

    void setServiceRegistry( qone::sdk::IServiceRegistry* registry ) override;

private:
    qone::sdk::IServiceRegistry* m_registry = nullptr;
};

class AboutPlugin : public QObject, public qone::sdk::IPlugin {

    Q_OBJECT
    Q_PLUGIN_METADATA( IID "qone.plugin.AboutPlugin" FILE "manifest.json" )
    Q_INTERFACES( qone::sdk::IPlugin )

public:
    explicit AboutPlugin( QObject* parent = nullptr );
    ~AboutPlugin() override = default;

    void onLoad() override;
    void onUnload() override;
    void* extension( const QString& extensionId ) override;

private:
    std::unique_ptr<UiShowcaseViewContributor> m_viewContributor;
    std::unique_ptr<AboutCommandContributor> m_commandContributor;
};
