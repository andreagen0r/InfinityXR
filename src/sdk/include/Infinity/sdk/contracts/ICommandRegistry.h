/// @file ICommandRegistry.h
/// @brief Central registry for system commands.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <functional>

#include <QObject>
#include <QUndoCommand>
#include <QVariantMap>

#include <Infinity/sdk/infinitysdk_export.h>
#include <Infinity/sdk/types/CommandDescriptor.h>

namespace qone::sdk {

/// @typedef CommandFactory
/// @brief Signature for the creation of QUndoCommand instances.
using CommandFactory = std::function<QUndoCommand*( const QVariantMap& )>;


/// @class ICommandRegistry
/// @brief Interface for registering and updating global commands dynamically.
class INFINITYSDK_EXPORT ICommandRegistry {

public:
    virtual ~ICommandRegistry() = default;

    /// @brief Registers a new command into the system.
    /// @param descriptor The metadata of the command to register.
    /// @param factory The factory function to instantiate the command (optional).
    virtual void registerCommand( const CommandDescriptor& descriptor, CommandFactory factory = {} ) = 0;

    /// @brief Forces the UI and underlying systems to re-evaluate the state (enabled/checked) of all commands.
    virtual void requestStateUpdate() = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::ICommandRegistry, "qone.sdk.ICommandRegistry/1.0" )
