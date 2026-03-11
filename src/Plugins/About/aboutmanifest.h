#pragma once
#include <Infinity/sdk/types/CommandId.h>
#include <Infinity/sdk/types/ViewId.h>

namespace Plugins {

namespace Commands {
using namespace Qt::StringLiterals;
static inline const qone::sdk::CommandId ShowAbout { "core.help.about"_L1 };
}; // namespace Commands

} // namespace Plugins

namespace Plugins {

namespace Views {
using namespace Qt::StringLiterals;
static inline const qone::sdk::ViewId AboutDialog { "view.core.about"_L1 };
}; // namespace Views

} // namespace Plugins
