#include <Infinity/core/services/settingsservice.h>

namespace qone::core {

SettingsService::SettingsService( std::unique_ptr<sdk::ISettingsService> backend )
    : m_backend( std::move( backend ) ) {}

sdk::ISettingsService& SettingsService::settings() {
    return *m_backend;
}

} // namespace qone::core
