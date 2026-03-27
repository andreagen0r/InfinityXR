#include <Infinity/core/kernel/pluginscanner.h>

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVersionNumber>
#include <QPluginLoader>

#include <Infinity/sdk/logging/logger.h>
#include <Infinity/sdk/types/Types.h>

#include <Infinity/core/utils/enumUtils.hpp>

namespace qone::core {

using namespace Qt::StringLiterals;

namespace {

qone::sdk::PluginVendor vendorFromString( const QString& value ) {
    // TODO: Check if validation is needed for a secure return.
    return qone::core::utils::fromString<qone::sdk::PluginVendor>( value );
}

} // unnamed namespace

PluginScanner::PluginScanner( QDir pluginsPath )
    : m_pluginsDir( std::move( pluginsPath ) ) {}

QList<qone::sdk::PluginDescriptor> PluginScanner::scan() const {

    QList<qone::sdk::PluginDescriptor> result;

    if ( !m_pluginsDir.exists() ) {
        qCWarning( qoneCore ) << Infinity::Logging::Title( u"Directory does not exist:"_s )
                              << m_pluginsDir.absolutePath();
        return result;
    }

    const auto entries { m_pluginsDir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot ) };

    for ( const auto& entry : entries ) {
        const QDir pluginDir( entry.absoluteFilePath() );

        auto descriptor { parsePluginDir( pluginDir ) };
        if ( !descriptor.pluginId.isEmpty() ) {
            result.push_back( std::move( descriptor ) );
        }
    }

    return result;
}

qone::sdk::PluginDescriptor PluginScanner::parsePluginDir( const QDir& pluginDir ) const {

    qone::sdk::PluginDescriptor descriptor;

    // Determine the expected library name based on the folder name.
    // Convention: Folder "Authentication" -> Library "libAuthentication.so" (Linux) or "Authentication.dll" (Win)
    // Nota: Qt automatically adds the "lib" prefix in Linux if not handled, but let's build it manually.
#if defined( Q_OS_WIN )
    const QString libName { pluginDir.dirName() % ".dll"_L1 };
#elif defined( Q_OS_MACOS )
    const QString libName = { "lib"_L1 % pluginDir.dirName() % ".dylib"_L1 };
#else
    const QString libName { "lib"_L1 % pluginDir.dirName() % ".so"_L1 };
#endif

    descriptor.libraryPath = pluginDir.filePath( libName );

    // Check if the binary exists.
    if ( !QFile::exists( descriptor.libraryPath ) ) {
        // Fallback: Tenta encontrar sem o prefixo 'lib' caso o CMake tenha mudado
        if ( !QFile::exists( descriptor.libraryPath ) ) {
            qCDebug( qoneCore ) << Infinity::Logging::Title( u"PluginScanner: Library not found for"_s )
                                << pluginDir.dirName();
            return {};
        }
        descriptor.libraryPath = pluginDir.filePath( QString( pluginDir.dirName() % ".so"_L1 ) );
    }

    // Extracting Metadata using QPluginLoader
    // This reads the embedded JSON via Q_PLUGIN_METADATA without loading the DLL into memory
    QPluginLoader loader( descriptor.libraryPath );
    // Importante para não ter erros de name mangling entre os módulos e plugins
    loader.setLoadHints( QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint );

    const QJsonObject metaData = loader.metaData();

    const QJsonObject obj = metaData.value( "MetaData"_L1 ).toObject();

    if ( obj.isEmpty() ) {
        qCDebug( qoneCore ) << Infinity::Logging::Title( u"PluginScanner: No metadata found in"_s )
                            << descriptor.libraryPath;
        return {};
    }

    // TODO: I need to adjust the JSON to make it complete; this is just a minimal example.
    descriptor.pluginId = obj.value( "uri"_L1 ).toString();
    descriptor.name = obj.value( "name"_L1 ).toString();
    descriptor.description = obj.value( "description"_L1 ).toString();
    descriptor.version = QVersionNumber::fromString( obj.value( "version"_L1 ).toString() );
    descriptor.vendor = vendorFromString( obj.value( "vendor"_L1 ).toString() );
    descriptor.hasUi = ( !obj.value( "mainElement"_L1 ).toString().isEmpty() );
    descriptor.requiresLicense = obj.value( "requiresLicense"_L1 ).toBool( false );
    descriptor.enabledByDefault = obj.value( "enabledByDefault"_L1 ).toBool( true );
    descriptor.category = qone::core::utils::fromString<sdk::PluginCategory>( obj.value( "category"_L1 ).toString() );
    descriptor.isRequired = obj.value( "required"_L1 ).toBool( false );

    // Minimum Validation
    if ( descriptor.pluginId.isEmpty() ) {
        qCDebug( qoneCore ) << Infinity::Logging::Title( u"PluginScanner:"_s ) << u"Plugin ID (uri) is missing in"_s
                            << descriptor.libraryPath;
        return {};
    }

    return descriptor;
}

} // namespace qone::core
