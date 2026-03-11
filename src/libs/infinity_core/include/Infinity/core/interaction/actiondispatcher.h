/// @file actiondispatcher.h
/// @brief Manages the execution and history (Undo/Redo) of commands.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <memory>

#include <QObject>
#include <QUndoStack>
#include <QVariantMap>

#include <Infinity/core/core_export.h>

class QUndoCommand;

namespace qone::core {

class CommandRegistry;


/// @class ActionDispatcher
/// @brief Orchestrates command execution and maintains the global Undo/Redo stack.
class CORE_EXPORT ActionDispatcher : public QObject {

    Q_OBJECT

    Q_PROPERTY( bool canUndo READ canUndo NOTIFY canUndoChanged )
    Q_PROPERTY( bool canRedo READ canRedo NOTIFY canRedoChanged )
    Q_PROPERTY( QString undoText READ undoText NOTIFY undoTextChanged )
    Q_PROPERTY( QString redoText READ redoText NOTIFY redoTextChanged )

public:
    /// @brief Constructs the dispatcher attached to a specific command registry.
    /// @param registry The @ref CommandRegistry used to resolve command IDs.
    explicit ActionDispatcher( CommandRegistry& registry, QObject* parent = nullptr );

    /// @brief Executes an instantiated command.
    /// @param command The @ref QUndoCommand object to execute.
    /// @param isUndoable If true, the command is pushed to the Undo stack. If false, it's executed and deleted.
    void execute( QUndoCommand* command, bool isUndoable = true );

    /// @brief Instantiates and executes a command directly via its string ID.
    /// @param commandId The unique string ID of the command.
    /// @param params Optional arguments passed to the command factory.
    /// @note `qCDebug( qoneCore ) << Infinity::Logging::Title( u"DISPATCHER [EXECUTE]:"_s ) << commandId;`
    Q_INVOKABLE void dispatch( const QString& commandId, const QVariantMap& params = {} );

    /// @name Property Accessors
    /// @{
    bool canUndo() const;
    bool canRedo() const;
    QString undoText() const;
    QString redoText() const;
    /// @}

public Q_SLOTS:
    /// @brief Rolls back the last undoable action.
    void undo();

    /// @brief Re-applies the last undone action.
    void redo();

Q_SIGNALS:
    void canUndoChanged();
    void canRedoChanged();
    void undoTextChanged();
    void redoTextChanged();

private:
    CommandRegistry& m_registry;             ///< Reference to the global command registry.
    std::unique_ptr<QUndoStack> m_undoStack; ///< The Qt UndoStack managing the history.
};

} // namespace qone::core
