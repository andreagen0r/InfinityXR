/// @file ViewId.h
/// @brief Strong type wrapper for View Identifiers.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QString>
#include <QHash>
#include <QDebug>

#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @struct ViewId
/// @brief A strongly-typed identifier to prevent mixing up view strings with other strings.
struct INFINITYSDK_EXPORT ViewId {
    QString value;

    ViewId() = default;

    /// @brief Explicit constructor from QString.
    explicit ViewId( QString val )
        : value( std::move( val ) ) {}

    /// @brief C++20 default equality operator.
    bool operator==( const ViewId& ) const = default;
};

/// @brief Required specialization to allow ViewId to be used as a key in QHash.
inline size_t qHash( const ViewId& key, size_t seed = 0 ) {
    return qHash( key.value, seed );
}

/// @brief Stream operator for logging.
inline QDebug operator<<( QDebug dbg, const ViewId& id ) {
    dbg.nospace() << id.value;
    return dbg.space();
}

} // namespace qone::sdk
