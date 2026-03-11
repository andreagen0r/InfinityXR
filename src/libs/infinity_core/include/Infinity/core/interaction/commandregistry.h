/// @file commandregistry.h
/// @brief Central repository and manager for application commands.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QHash>
#include <QUndoCommand>

#include <Infinity/sdk/contracts/ICommandRegistry.h>
#include <Infinity/sdk/contracts/ICommandContributor.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class CommandRegistry
/// @brief Stores command definitions and acts as a factory for executing actions.
///
/// This class acts as the single source of truth for all executable actions in the
/// system, providing metadata for the UI and factories for the @ref ActionDispatcher.
class CORE_EXPORT CommandRegistry : public QObject, public sdk::ICommandRegistry {

    Q_OBJECT

public:
    /// @brief Default constructor.
    explicit CommandRegistry( QObject* parent = nullptr );

    /// @brief Registers a new command descriptor and its factory.
    /// @param descriptor The metadata defining the command.
    /// @param factory The function responsible for instantiating the command logic.
    void registerCommand( const qone::sdk::CommandDescriptor& descriptor, qone::sdk::CommandFactory factory ) override;

    /// @brief Instantiates a command object safely using its strong ID.
    /// @param id The strongly-typed @ref CommandId.
    /// @param params Optional arguments required for the command execution.
    /// @return A newly allocated @ref QUndoCommand, or nullptr if not found.
    /// @warning The caller assumes memory ownership of the returned pointer.
    [[nodiscard]] QUndoCommand* createCommand( const qone::sdk::CommandId& id, const QVariantMap& params = {} );

    /// @brief Forces all registered commands to re-evaluate their enabled/checked states.
    void requestStateUpdate() override;

    /// @brief Retrieves the metadata of a specific command.
    /// @param id The strongly-typed @ref CommandId.
    /// @return Pointer to the @ref CommandDescriptor, or nullptr if not found.
    const qone::sdk::CommandDescriptor* descriptor( const qone::sdk::CommandId& id ) const;

    /// @brief Immediately executes a command, bypassing the Undo stack.
    /// @param idStr The string representation of the command ID.
    /// @note This method is exposed to QML to allow direct invocation of simple toggle commands.
    Q_INVOKABLE void dispatch( const QString& idStr );

    /// @brief Retrieves the entire list of registered command descriptors in order.
    /// @return A QList containing pointers to all registered descriptors.
    [[nodiscard]] QList<const qone::sdk::CommandDescriptor*> getAllCommands() const;

    /// @name Transactional Layout Updates
    /// @brief Pauses UI updates during mass command registration.
    /// @{
    void beginBatch();
    void endBatch();
    /// @}

Q_SIGNALS:
    /// @brief Emitted when a specific command changes its enabled or checked state.
    /// @param id The string ID of the command (compatible with QML).
    void commandStateChanged( const QString& id );

    /// @brief Emitted when the overall structure (like a menu layout) needs to be rebuilt.
    void layoutChanged();

private:
    /// @struct Entry
    /// @brief Internal structure combining the descriptor and its factory.
    struct Entry {
        qone::sdk::CommandDescriptor desc;
        qone::sdk::CommandFactory factory;
    };

    QHash<qone::sdk::CommandId, Entry> m_entries; ///< The main strongly-typed registry map.
    QList<qone::sdk::CommandId> m_order;          ///< Maintains the registration order.

    bool m_isBatching = false;                    ///< Indicates if a batch registration is in progress.
    bool m_hasPendingLayoutChange = false;        ///< Flags a pending layout update after a batch completes.
};

} // namespace qone::core
