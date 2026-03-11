/// @file PreferenceDescriptor.h
/// @brief Metadata for registering settings UI panels.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QString>
#include <Infinity/sdk/types/ViewId.h>
#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @struct PreferenceDescriptor
/// @brief Defines how and where a preference page should be displayed in the settings UI.
struct INFINITYSDK_EXPORT PreferenceDescriptor {

    QString categoryId;      ///< Internal grouping ID (e.g., "core.system").
    QString categoryName;    ///< Translated text displayed in the QML UI (e.g., "System").
    QString categoryIcon;    ///< Optional theme icon identifier for the category label.

    QString subCategoryId;   ///< Internal sub-grouping ID (e.g., "core.system.general").
    QString subCategoryName; ///< Translated text for the sub-category (e.g., "General").

    ViewId viewId;           ///< Strongly-typed identifier pointing to the QML view to load.

    int weight = 0;          ///< Sorting priority (lower values appear higher in the list).
};

} // namespace qone::sdk
