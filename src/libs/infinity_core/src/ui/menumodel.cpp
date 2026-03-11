#include <Infinity/core/ui/menumodel.h>

#include <Infinity/sdk/logging/logger.h>
#include <Infinity/core/interaction/commandregistry.h>

using namespace Qt::StringLiterals;

namespace qone::core {


MenuModel::MenuModel( qone::core::CommandRegistry* registry, QObject* parent )
    : QAbstractItemModel( parent )
    , m_registry( registry )
    , m_root( std::make_unique<MenuNode>() ) {

    qCDebug( qoneApp ) << Infinity::Logging::Title( u"MenuModel: Constructor"_s );

    connect( m_registry, &qone::core::CommandRegistry::layoutChanged, this, &MenuModel::rebuild );
    connect( m_registry, &qone::core::CommandRegistry::commandStateChanged, this, &MenuModel::onCommandStateChanged );
}

QModelIndex MenuModel::index( int row, int column, const QModelIndex& parent ) const {

    if ( !hasIndex( row, column, parent ) ) {
        return {};
    }

    MenuNode* parentNode { parent.isValid() ? static_cast<MenuNode*>( parent.internalPointer() ) : m_root.get() };

    if ( row < 0 || row >= static_cast<int>( parentNode->children.size() ) ) {
        return {};
    }

    return createIndex( row, column, parentNode->children[static_cast<size_t>( row )].get() );
}

QModelIndex MenuModel::parent( const QModelIndex& child ) const {

    if ( !child.isValid() ) {
        return {};
    }

    auto childNode { static_cast<MenuNode*>( child.internalPointer() ) };
    auto parentNode { childNode->parent };

    if ( !parentNode || parentNode == m_root.get() ) {
        return {};
    }

    return createIndex( parentNode->row(), 0, parentNode );
}

int MenuModel::rowCount( const QModelIndex& parent ) const {

    MenuNode* parentNode { parent.isValid() ? static_cast<MenuNode*>( parent.internalPointer() ) : m_root.get() };

    return static_cast<int>( parentNode->children.size() );
}

int MenuModel::columnCount( const QModelIndex& ) const {
    return 1;
}

QVariant MenuModel::data( const QModelIndex& index, int role ) const {

    if ( !index.isValid() ) {
        return {};
    }

    auto node { static_cast<MenuNode*>( index.internalPointer() ) };

    const auto* desc { !node->commandId.isEmpty() ? m_registry->descriptor( qone::sdk::CommandId( node->commandId ) )
                                                  : nullptr };

    switch ( role ) {
        case TextRole:
            return node->text;
        case CommandIdRole:
            return node->commandId;
        case IconRole:
            return desc ? desc->iconName : QString();
        case ShortcutRole:
            return desc ? desc->shortcut.toString( QKeySequence::NativeText ) : QString();
        case EnabledRole:
            return desc ? desc->isEnabled() : true;
        case CheckableRole:
            return desc ? desc->checkable : false;
        case CheckedRole:
            return desc ? desc->isChecked() : false;
        case ItemTypeRole: {
            if ( node->isSeparator ) {
                return Separator;
            }

            if ( !node->commandId.isEmpty() ) {
                return Action;
            }

            return SubMenu;
        }
        default:
            return {};
    }
}

void MenuModel::rebuild() {

    qCDebug( qoneCore ) << Infinity::Logging::Title( u"Rebuild MenuModel"_s );

    beginResetModel();

    m_root = std::make_unique<MenuNode>();

    const auto commands { m_registry->getAllCommands() };

    for ( const auto* cmd : commands ) {

        if ( cmd->menuPath.isEmpty() ) {
            continue;
        }

        MenuNode* current { m_root.get() };

        for ( int i = 0; i < cmd->menuPath.size(); ++i ) {

            const bool isLeaf { ( i == cmd->menuPath.size() - 1 ) };
            const QString segment { cmd->menuPath[i] };

            if ( cmd->separator && isLeaf ) {
                current->addSeparator();
                break;
            }

            auto* child { current->findChild( segment ) };

            if ( !child ) {
                child = current->addChild( segment );
            }

            if ( isLeaf && !cmd->separator ) {
                child->commandId = cmd->id.value;
            }

            current = child;
        }
    }

    endResetModel();
}


QHash<int, QByteArray> MenuModel::roleNames() const {
    // clang-format off
    static const QHash<int, QByteArray> outRoleNames {
        { TextRole, "text"_ba },
        { IconRole, "iconName"_ba },
        { CommandIdRole, "commandId"_ba },
        { EnabledRole, "enabled"_ba },
        { ShortcutRole, "shortcut"_ba },
        { CheckableRole, "checkable"_ba },
        { CheckedRole, "checked"_ba },
        { ItemTypeRole, "itemType"_ba }
    };
    // clang-format on

    return outRoleNames;
}

QModelIndex MenuModel::qmlIndex( int row, int column, const QVariant& parent ) {

    QModelIndex parentIndex;

    if ( parent.isValid() && parent.canConvert<QModelIndex>() ) {
        parentIndex = parent.value<QModelIndex>();
    }

    return index( row, column, parentIndex );
}

void MenuModel::onCommandStateChanged( const QString& id ) {

    Q_UNUSED( id );

    QList<int> rolesChanged = { EnabledRole, CheckedRole, CheckableRole };

    Q_EMIT dataChanged( index( 0, 0 ), index( rowCount() - 1, 0 ), rolesChanged );
}

QModelIndex MenuModel::root() const {
    return QModelIndex();
}

QVariant MenuModel::dataAt( const QModelIndex& index, int role ) const {
    return data( index, role );
}

QVariant MenuModel::dataAtRole( const QModelIndex& index, const QString& roleName ) const {

    const auto roles { roleNames() };

    auto it { std::find_if( roles.begin(), roles.end(), [&]( auto v ) {
        return v == roleName.toUtf8();
    } ) };

    if ( it == roles.end() ) {
        return {};
    }

    return data( index, it.key() );
}

} // namespace qone::core
