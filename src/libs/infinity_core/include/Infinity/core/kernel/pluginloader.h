/// @file pluginloader.h
/// @brief Handles the physical loading of shared libraries into memory.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <optional>
#include <QString>

#include <Infinity/sdk/types/PluginDescriptor.h>

#include <Infinity/core/core_export.h>

#include "plugininstance.h"

namespace qone::core {

/// @class PluginLoader
/// @brief Factory for instantiating plugins from their descriptors.
class CORE_EXPORT PluginLoader {

public:
    /// @brief Attempts to load the dynamic library specified in the descriptor.
    /// @param descriptor The metadata identifying the target library.
    /// @return An optional containing the @ref PluginInstance if successful, or `std::nullopt` on failure.
    /// @note Implementations should log failures rigorously:
    std::optional<PluginInstance> load( const qone::sdk::PluginDescriptor& descriptor ) const;
};

} // namespace qone::core
