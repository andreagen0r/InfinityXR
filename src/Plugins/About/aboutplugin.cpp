#include "aboutplugin.h"
#include "showaboutcommand.h"

#include <Infinity/sdk/contracts/IServiceRegistry.h>
#include <Infinity/sdk/contracts/IRequiresContext.h>
#include <Infinity/sdk/contracts/IDialogService.h>
#include <Infinity/sdk/types/CommandId.h>

#include "aboutmanifest.h"

using namespace Qt::StringLiterals;

using namespace qone::sdk;


QList<CommandRegistration> AboutCommandContributor::getCommands() const {

    QList<qone::sdk::CommandRegistration> registrations;

    qone::sdk::CommandDescriptor cmdAbout {
        .id = Plugins::Commands::ShowAbout, // Compilador garante que isto é um CommandId válido
        .text = "About Infinity XR"_L1,
        .menuPath = { "&Help"_L1, "About"_L1 } };

    qone::sdk::CommandFactory factory = [registry = this->m_registry]( const QVariantMap& ) -> QUndoCommand* {
        auto* dialogService = registry->resolve<qone::sdk::IDialogService>();
        if ( !dialogService ) {
            return nullptr;
        }
        return new ShowAboutCommand( dialogService );
    };

    // Packs and moves without unnecessary copies in memory thanks to std::move
    registrations.append( { std::move( cmdAbout ), std::move( factory ) } );

    return registrations;
}

void AboutCommandContributor::setServiceRegistry( qone::sdk::IServiceRegistry* registry ) {
    m_registry = registry;
}


// ============================================================================
// UiShowcaseViewContributor
// ============================================================================
QList<ViewDescriptor> UiShowcaseViewContributor::views() const {
    ViewDescriptor aboutView;
    aboutView.id = Plugins::Views::AboutDialog;
    aboutView.title = "About Dialog"_L1;
    aboutView.source = QUrl( "qrc:/Plugins/About/AboutDialog.qml"_L1 );

    return { aboutView };
}

// ============================================================================
// AboutPlugin (Main Plugin Class)
// ============================================================================
AboutPlugin::AboutPlugin( QObject* parent )
    : QObject( parent )
    , m_viewContributor( std::make_unique<UiShowcaseViewContributor>() )
    , m_commandContributor( std::make_unique<AboutCommandContributor>() ) {}

void AboutPlugin::onLoad() {}

void AboutPlugin::onUnload() {}

void* AboutPlugin::extension( const QString& iid ) {


    if ( iid == QString::fromUtf8( IViewContributor::IID ) ) {
        return m_viewContributor.get();
    }
    if ( iid == QString::fromUtf8( ICommandContributor::IID ) ) {
        return m_commandContributor.get();
    }

    if ( iid == QString::fromUtf8( qone::sdk::IRequiresContext::IID ) ) {
        return static_cast<qone::sdk::IRequiresContext*>( m_commandContributor.get() );
    }

    return nullptr;
}
