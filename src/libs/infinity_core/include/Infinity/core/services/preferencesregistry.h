/// @file preferencesregistry.h
/// @brief Manages the hierarchical structure of the application's Preferences UI.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QList>
#include <QString>
#include <shared_mutex>

#include <Infinity/core/core_export.h>

#include <Infinity/sdk/types/PreferenceDescriptor.h>

namespace qone::core {

/// @struct SubCategoryNode
/// @brief Represents a secondary grouping level within the preferences tree.
struct SubCategoryNode {
    QString id;                     ///< Internal ID of the sub-category.
    QString name;                   ///< Localized display name.
    int weight { 0 };               ///< Sorting priority.
    QList<qone::sdk::ViewId> views; ///< Ordered list of views belonging to this group.
};

/// @struct CategoryNode
/// @brief Represents a primary grouping level (e.g., "System", "Graphics").
struct CategoryNode {
    QString id;                           ///< Internal ID of the category.
    QString name;                         ///< Localized display name.
    QString icon;                         ///< Theme icon name.
    int weight { 0 };                     ///< Sorting priority.
    QList<SubCategoryNode> subCategories; ///< Ordered list of child subgroups.
};

/// @class PreferencesRegistry
/// @brief Acts as a centralized dictionary for assembling the settings panel.
class CORE_EXPORT PreferencesRegistry : public QObject {

    Q_OBJECT

public:
    explicit PreferencesRegistry( QObject* parent = nullptr );

    /// @brief Parses a descriptor and places it in the correct place within the tree.
    /// @param descriptor The metadata configuring the preference page location.
    void registerPreference( const qone::sdk::PreferenceDescriptor& descriptor );

    /// @brief Retrieves the entire sorted tree of categories.
    /// @return A complete copy of the internal structure for UI rendering.
    [[nodiscard]] QList<CategoryNode> getCategories() const;

Q_SIGNALS:
    /// @brief Emitted when the internal structure changes (e.g., new plugin loaded).
    void preferencesChanged();

private:
    QList<CategoryNode> m_categories;  ///< The master list of categories.
    mutable std::shared_mutex m_mutex; ///< Thread safety lock.
};

} // namespace qone::core
