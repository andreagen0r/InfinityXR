/// @file systemuimanifest.h
/// @brief Centralized registry of strongly-typed global UI constants.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <Infinity/sdk/types/CommandId.h>
#include <Infinity/sdk/types/ViewId.h>

namespace qone::core {

using namespace Qt::StringLiterals;

/// @namespace Commands
/// @brief Core-provided Command Identifiers.
namespace Commands {
/// @brief Triggers the opening of the global preferences window.
static inline const qone::sdk::CommandId ShowPreferences { "core.edit.preferences"_L1 };
} // namespace Commands

/// @namespace Views
/// @brief Core-provided View Identifiers.
namespace Views {
/// @brief Identifier for the Global Preferences dialog.
static inline const qone::sdk::ViewId PreferencesDialog { "view.core.preferences"_L1 };

/// @brief Identifier for the System Plugin Manager panel.
static inline const qone::sdk::ViewId PluginManagerPanel { "view.core.plugins_manager"_L1 };
} // namespace Views

} // namespace qone::core
