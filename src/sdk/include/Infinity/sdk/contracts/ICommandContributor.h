/// @file ICommandContributor.h
/// @brief Extension interface for contributing commands via Plugins.

#pragma once

#include <QObject>
#include <QHash>
#include <functional>
#include <QVariantMap>
#include <QUndoCommand>

#include <Infinity/sdk/types/CommandId.h>
#include <Infinity/sdk/types/CommandDescriptor.h>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @typedef CommandFactory
/// @brief Function signature for dynamic command creation.
///
/// The Factory is responsible for instantiating a @ref QUndoCommand using parameters
/// provided via a QVariantMap.
/// @note Memory management of the returned pointer is the responsibility of the invoking CommandManager.
using CommandFactory = std::function<QUndoCommand*( const QVariantMap& )>;

/// @struct CommandRegistration
/// @brief Packaging structure for registering commands in the system.
///
/// Binds the metadata (Descriptor) to the creation logic (Factory).
struct INFINITYSDK_EXPORT CommandRegistration {
    CommandDescriptor descriptor; ///< Command metadata (ID, Icon, Text, Shortcut).
    CommandFactory factory;       ///< Generator function to instantiate the command.
};

/// @class ICommandContributor
/// @brief Extension interface for plugins to inject new commands.
///
/// Classes implementing this interface allow the Infinity XR system to discover
/// and register new commands that can be triggered via UI, shortcuts, or scripts.
class INFINITYSDK_EXPORT ICommandContributor {

public:
    /// @brief Unique Interface Identifier (Contract).
    static constexpr auto IID { "qone.sdk.ICommandContributor" };

    /// @brief Virtual destructor ensuring proper cleanup of implementations.
    virtual ~ICommandContributor() = default;

    /// @brief Retrieves the list of commands provided by this contributor.
    ///
    /// This method is called during the plugin initialization phase or when
    /// the command registry needs to be updated.
    ///
    /// @return QList<CommandRegistration> List of structures containing descriptors and factories.
    /// @pre The plugin must be properly loaded into the system.
    /// @note Example trace log for implementation:
    virtual QList<CommandRegistration> getCommands() const = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::ICommandContributor, "qone.sdk.ICommandContributor/1.0" )
