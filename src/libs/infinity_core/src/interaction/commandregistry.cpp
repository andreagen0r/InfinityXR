#include <Infinity/core/interaction/commandregistry.h>
#include <Infinity/sdk/logging/logger.h>

namespace qone::core {

using namespace Qt::StringLiterals;

CommandRegistry::CommandRegistry( QObject* parent )
    : QObject( parent ) {}

void CommandRegistry::registerCommand( const qone::sdk::CommandDescriptor& descriptor,
                                       qone::sdk::CommandFactory factory ) {

    if ( m_entries.contains( descriptor.id ) ) {
        qCWarning( qoneCore ) << Infinity::Logging::Title( u"Override command:"_s ) << descriptor.id.value;
    }

    m_entries.insert( descriptor.id, { descriptor, std::move( factory ) } );
    m_order.append( descriptor.id );

    if ( !m_isBatching ) {
        Q_EMIT layoutChanged();
    } else {
        m_hasPendingLayoutChange = true;
    }
}

QUndoCommand* CommandRegistry::createCommand( const qone::sdk::CommandId& id, const QVariantMap& params ) {
    const auto it = m_entries.constFind( id );

    if ( it == m_entries.constEnd() ) {
        qCWarning( qoneCore ) << Infinity::Logging::Title( u"Attempt to create unregistered command:"_s ) << id.value;
        return nullptr;
    }

    if ( !it->factory ) {
        return nullptr;
    }

    return it->factory( params );
}

const qone::sdk::CommandDescriptor* CommandRegistry::descriptor( const qone::sdk::CommandId& id ) const {
    const auto it = m_entries.constFind( id );
    return ( it != m_entries.constEnd() ) ? &it->desc : nullptr;
}


void CommandRegistry::dispatch( const QString& idStr ) {
    const qone::sdk::CommandId safeId( idStr );

    if ( auto* cmd = createCommand( safeId ) ) {
        // In a real-world scenario, you would inject this into a QundoStack.
        cmd->redo();
        delete cmd;
    }
}

QList<const qone::sdk::CommandDescriptor*> CommandRegistry::getAllCommands() const {

    QList<const qone::sdk::CommandDescriptor*> list;
    list.reserve( m_order.size() );

    for ( const auto& id : m_order ) {
        auto it = m_entries.constFind( id );
        if ( it != m_entries.constEnd() ) {
            list.append( &it->desc );
        }
    }
    return list;
}

void CommandRegistry::requestStateUpdate() {

    for ( const auto& id : m_order ) {
        Q_EMIT commandStateChanged( id.value );
    }
}

void CommandRegistry::beginBatch() {
    m_isBatching = true;
    m_hasPendingLayoutChange = false;
}

void CommandRegistry::endBatch() {
    m_isBatching = false;
    if ( m_hasPendingLayoutChange ) {
        Q_EMIT layoutChanged();
        m_hasPendingLayoutChange = false;
    }
}

} // namespace qone::core
