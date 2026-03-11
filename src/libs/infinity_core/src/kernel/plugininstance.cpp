
#include <Infinity/core/kernel/plugininstance.h>
#include <Infinity/sdk/types/PluginDescriptor.h>


namespace qone::core {

PluginInstance::PluginInstance( qone::sdk::PluginDescriptor descriptor, std::unique_ptr<QPluginLoader> loader,
                                qone::sdk::IPlugin* plugin )
    : m_descriptor( std::move( descriptor ) )
    , m_loader( std::move( loader ) )
    , m_plugin( plugin ) {}

PluginInstance::~PluginInstance() {
    unload();
}

bool PluginInstance::load() {
    if ( !m_plugin ) {
        return false;
    }

    // The object has already been instantiated by the PluginLoader at the time of discovery/loader.
    // Here we are simply calling the interface lifecycle.
    m_plugin->onLoad();

    return true;
}

void PluginInstance::unload() {
    if ( m_plugin ) {
        m_plugin->onUnload();
        // We don't manually delete m_plugin here.
        // QPluginLoader manages the root component, or the delete occurs when unloading the DLL.
        m_plugin = nullptr;
    }

    if ( m_loader && m_loader->isLoaded() ) {
        m_loader->unload();
    }
}

qone::sdk::IPlugin* PluginInstance::plugin() const {
    return m_plugin;
}

const qone::sdk::PluginDescriptor& PluginInstance::descriptor() const {
    return m_descriptor;
}

bool PluginInstance::isLoaded() const {
    return m_plugin != nullptr && m_loader && m_loader->isLoaded();
}

} // namespace qone::core
