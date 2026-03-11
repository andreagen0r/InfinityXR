#include <Infinity/core/services/qsettingsbackend.h>

namespace qone::core {

using namespace Qt::StringLiterals;

QSettingsBackend::QSettingsBackend()
    : m_settings( "QOne"_L1, "Infinity"_L1 ) {}

QVariant QSettingsBackend::read( const QString& key, const QVariant& defaultValue ) const {
    return m_settings.value( key, defaultValue );
}

void QSettingsBackend::write( const QString& key, const QVariant& value ) {
    m_settings.setValue( key, value );
}

void QSettingsBackend::sync() {
    m_settings.sync();
}

} // namespace qone::core
