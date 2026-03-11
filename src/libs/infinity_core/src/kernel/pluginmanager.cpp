#include <Infinity/core/kernel/pluginmanager.h>

#include <QStringBuilder>

#include <Infinity/sdk/logging/logger.h>
#include <Infinity/sdk/contracts/IAuthService.h>
#include <Infinity/sdk/contracts/IViewContributor.h>
#include <Infinity/sdk/contracts/ICommandContributor.h>
#include <Infinity/sdk/contracts/PluginEvents.h>

#include <Infinity/core/ui/viewregistry.h>

namespace qone::core {

using namespace Qt::StringLiterals;

PluginManager::PluginManager( QDir pluginsDir, CommandRegistry* commandRegistry, SettingsService& settings,
                              sdk::IEventBus* eventBus, QObject* parent )
    : QObject { parent }
    , m_settings { settings }
    , m_commandRegistry( commandRegistry )
    , m_scanner { std::move( pluginsDir ) }
    , m_eventBus { eventBus } {}

void PluginManager::discoverPlugins() {

    m_descriptors = m_scanner.scan();

    for ( auto& descriptor : m_descriptors ) {
        const QString key = "plugins/"_L1 % descriptor.pluginId % "/enabled"_L1;

        descriptor.enabled = m_settings.settings().read( key, true ).toBool();

        Q_EMIT pluginDiscovered( descriptor );
    }
}

void PluginManager::loadSystemPlugins() {

    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Loading System Plugins"_s );

    for ( const auto& descriptor : m_descriptors ) {
        // Verifica se é categoria System e se está habilitado
        // TODO: Adicionar as outras categorias
        if ( descriptor.category == qone::sdk::PluginCategory::System && descriptor.enabled ) {

            qCInfo( qoneCore ) << Infinity::Logging::Title( u"Attempting to load system plugin:"_s )
                               << descriptor.pluginId;

            const bool success { loadPlugin( descriptor.pluginId ) };

            if ( !success ) {
                qCWarning( qoneCore ) << Infinity::Logging::Title( u"Failed to load system plugin:"_s )
                                      << descriptor.pluginId;

                if ( descriptor.isRequired ) {
                    // Here I need to decide whether to close the app or just send a notification.
                    qCCritical( qoneCore ) << Infinity::Logging::Title( u"Required system plugin failed to load!"_s );
                }
            }
        }
    }
}

QList<qone::sdk::PluginDescriptor> PluginManager::availablePlugins() const {
    return m_descriptors;
}

bool PluginManager::isPluginLoaded( const QString& pluginId ) const {
    return std::any_of( m_instances.begin(), m_instances.end(), [&]( const auto& instance ) {
        return instance->descriptor().pluginId == pluginId;
    } );
}

void PluginManager::setPluginEnabled( const QString& pluginId, bool enabled ) {

    auto* descriptor { findDescriptor( pluginId ) };
    if ( !descriptor ) {
        return;
    }

    if ( descriptor->enabled == enabled ) {
        return;
    }

    descriptor->enabled = enabled;

    const QString key { "plugins/"_L1 % descriptor->pluginId % "enabled"_L1 };

    m_settings.settings().write( key, enabled );
    m_settings.settings().sync();

    if ( enabled ) {
        loadPlugin( pluginId );
    } else {
        unloadPlugin( pluginId );
    }
}


bool PluginManager::loadPlugin( const QString& pluginId ) {

    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Entering loadPlugin for:"_s ) << pluginId;

    if ( isPluginLoaded( pluginId ) ) {
        qCInfo( qoneCore ) << Infinity::Logging::Title( u"Plugin already loaded."_s );
        return true;
    }

    const auto* descriptor { findDescriptor( pluginId ) };
    if ( !descriptor ) {
        qCWarning( qoneCore ) << Infinity::Logging::Title( u"Descriptor not found!"_s );
        return false;
    }

    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Descriptor found. Enabled:"_s ) << descriptor->enabled;

    if ( !descriptor->enabled ) {
        return false;
    }

    auto instanceOpt { m_loader.load( *descriptor ) };

    if ( !instanceOpt ) {
        qCCritical( qoneCore ) << Infinity::Logging::Title( u"QPluginLoader failed to load binary!"_s );
        return false;
    }

    auto instance = std::make_unique<PluginInstance>( std::move( *instanceOpt ) );


    if ( !instance->load() ) {
        qCCritical( qoneCore ) << Infinity::Logging::Title( u"Plugin instance->load() returned false!"_s );
        return false;
    }

    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Plugin Loaded"_s );

    // Notifies all handlers that the plugin has been loaded.
    for ( auto& handler : m_extensionHandlers ) {
        handler->onPluginLoaded( pluginId, instance.get() );
    }

    // Registra instância
    m_instances.push_back( std::move( instance ) );

    // EventBus
    if ( m_eventBus != nullptr ) {
        m_eventBus->publish( qone::sdk::events::PluginLoaded { pluginId } );
    }

    Q_EMIT pluginLoaded( pluginId );

    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Plugin load flow complete."_s );

    return true;
}


void PluginManager::addExtensionHandler( std::unique_ptr<IPluginExtensionHandler> handler ) {
    m_extensionHandlers.push_back( std::move( handler ) );
}

void PluginManager::unloadPlugin( const QString& pluginId ) {

    auto it = std::find_if( m_instances.begin(), m_instances.end(), [&]( const auto& instance ) {
        return instance->descriptor().pluginId == pluginId;
    } );

    if ( it == m_instances.end() ) {
        return;
    }

    Q_EMIT aboutToUnloadPlugin( pluginId );

    // Notify the handlers before unloading.
    for ( auto& handler : m_extensionHandlers ) {
        handler->onPluginUnloaded( pluginId, ( *it ).get() );
    }

    ( *it )->unload();
    m_instances.erase( it );

    Q_EMIT pluginUnloaded( pluginId );
}

qone::sdk::PluginDescriptor* PluginManager::findDescriptor( const QAnyStringView& pluginId ) {

    auto it = std::find_if( m_descriptors.begin(), m_descriptors.end(), [&]( const auto& descriptor ) {
        return descriptor.pluginId == pluginId;
    } );

    return ( it != m_descriptors.end() ) ? &( *it ) : nullptr;
}

PluginInstance* PluginManager::findInstance( const QAnyStringView& pluginId ) {

    auto it = std::find_if( m_instances.begin(), m_instances.end(), [&]( const auto& instance ) {
        return instance->descriptor().pluginId == pluginId;
    } );

    return ( it != m_instances.end() ) ? it->get() : nullptr;
}

} // namespace qone::core
