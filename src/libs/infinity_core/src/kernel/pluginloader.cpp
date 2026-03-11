#include <Infinity/core/kernel/pluginloader.h>

#include <QPluginLoader>
#include <optional>

namespace qone::core {

std::optional<PluginInstance> PluginLoader::load( const qone::sdk::PluginDescriptor& descriptor ) const {

    // We use QPluginLoader, which reads the metadata and manages the QObject instance.
    auto loader = std::make_unique<QPluginLoader>( descriptor.libraryPath );

    QObject* instance = loader->instance();

    if ( !instance ) {
        qWarning() << "PluginLoader: Failed to load instance for" << descriptor.pluginId
                   << "Error:" << loader->errorString();
        return std::nullopt;
    }

    // Checks if the loaded object implements the IPlugin interface.
    auto* pluginInterface = qobject_cast<qone::sdk::IPlugin*>( instance );

    if ( !pluginInterface ) {
        qWarning() << "PluginLoader: Object is not a valid IPlugin:" << descriptor.pluginId;
        loader->unload();
        return std::nullopt;
    }

    // Returns the PluginInstance wrapper.
    return std::optional<PluginInstance> { std::in_place, descriptor, std::move( loader ), pluginInterface };
}


} // namespace qone::core
