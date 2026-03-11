#include <Infinity/core/interaction/actiondispatcher.h>

#include <QUndoCommand>

#include <Infinity/sdk/logging/logger.h>
#include <Infinity/core/interaction/commandregistry.h>

using namespace Qt::StringLiterals;

namespace qone::core {

ActionDispatcher::ActionDispatcher( CommandRegistry& registry, QObject* parent )
    : QObject( parent )
    , m_registry( registry )
    , m_undoStack( std::make_unique<QUndoStack>( this ) ) {

    connect( m_undoStack.get(), &QUndoStack::canUndoChanged, this, &ActionDispatcher::canUndoChanged );
    connect( m_undoStack.get(), &QUndoStack::canRedoChanged, this, &ActionDispatcher::canRedoChanged );
    connect( m_undoStack.get(), &QUndoStack::undoTextChanged, this, &ActionDispatcher::undoTextChanged );
    connect( m_undoStack.get(), &QUndoStack::redoTextChanged, this, &ActionDispatcher::redoTextChanged );
}

void ActionDispatcher::execute( QUndoCommand* command, bool isUndoable ) {

    if ( !command ) {
        qCCritical( qoneCore ) << Infinity::Logging::Title( u"Command doesn't exist"_s );
        return;
    }

    qCDebug( qoneCore ) << Infinity::Logging::Title( u"Executing command:"_s ) << command->text();

    if ( isUndoable ) {
        m_undoStack->push( command );
    } else {
        command->redo();
        delete command;
    }
}

void ActionDispatcher::dispatch( const QString& commandId, const QVariantMap& params ) {

    qCInfo( qoneCore ) << Infinity::Logging::Title( u"ActionDispatcher"_s ) << u"Dispatching:"_s << commandId
                       << u"Params:"_s << params;

    auto* command = m_registry.createCommand( qone::sdk::CommandId( commandId ), params );

    if ( command ) {
        execute( command );
    } else {
        qCWarning( qoneCore ) << Infinity::Logging::Title( u"ActionDispatcher"_s ) << u"Failed to create command"_s
                              << commandId;
    }
}

void ActionDispatcher::undo() {
    m_undoStack->undo();
}
void ActionDispatcher::redo() {
    m_undoStack->redo();
}
bool ActionDispatcher::canUndo() const {
    return m_undoStack->canUndo();
}
bool ActionDispatcher::canRedo() const {
    return m_undoStack->canRedo();
}
QString ActionDispatcher::undoText() const {
    return m_undoStack->undoText();
}
QString ActionDispatcher::redoText() const {
    return m_undoStack->redoText();
}

} // namespace qone::core
