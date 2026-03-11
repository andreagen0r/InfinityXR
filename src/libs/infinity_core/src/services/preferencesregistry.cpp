#include <Infinity/core/services/preferencesregistry.h>
#include <algorithm>

namespace qone::core {

PreferencesRegistry::PreferencesRegistry( QObject* parent )
    : QObject( parent ) {}

void PreferencesRegistry::registerPreference( const qone::sdk::PreferenceDescriptor& descriptor ) {
    std::unique_lock writeLock( m_mutex );

    auto catIt = std::find_if( m_categories.begin(), m_categories.end(), [&]( const CategoryNode& c ) {
        return c.id == descriptor.categoryId;
    } );

    if ( catIt == m_categories.end() ) {
        m_categories.append(
            { descriptor.categoryId, descriptor.categoryName, descriptor.categoryIcon, descriptor.weight, {} } );
        catIt = std::prev( m_categories.end() );
    }

    auto& subCats = catIt->subCategories;
    auto subIt = std::find_if( subCats.begin(), subCats.end(), [&]( const SubCategoryNode& sc ) {
        return sc.id == descriptor.subCategoryId;
    } );

    if ( subIt == subCats.end() ) {
        subCats.append( { descriptor.subCategoryId, descriptor.subCategoryName, descriptor.weight, {} } );
        subIt = std::prev( subCats.end() );
    }

    if ( !subIt->views.contains( descriptor.viewId ) ) {
        subIt->views.append( descriptor.viewId );
    }

    std::stable_sort( m_categories.begin(), m_categories.end(), []( const CategoryNode& a, const CategoryNode& b ) {
        return a.weight < b.weight;
    } );

    std::stable_sort( subCats.begin(), subCats.end(), []( const SubCategoryNode& a, const SubCategoryNode& b ) {
        return a.weight < b.weight;
    } );

    writeLock.unlock();
    Q_EMIT preferencesChanged();
}

QList<CategoryNode> PreferencesRegistry::getCategories() const {
    std::shared_lock readLock( m_mutex );
    return m_categories;
}

} // namespace qone::core
