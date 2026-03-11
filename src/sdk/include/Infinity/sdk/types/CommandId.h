/// @file CommandId.h
/// @brief Strong type wrapper for Command Identifiers.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QString>
#include <QHash>
#include <QDebug>

#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @struct CommandId
/// @brief A strongly-typed string guaranteeing compile-time safety for command lookups.
struct INFINITYSDK_EXPORT CommandId {

    QString value; ///< The raw string value.

    CommandId() = default;

    /// @brief Explicit constructor to prevent accidental conversions from std::string or const char*.
    /// @param val The string identifier.// Construtores explícitos evitam conversões acidentais de std::string ou const
    /// char*
    explicit CommandId( QString val )
        : value( std::move( val ) ) {}

    /// @brief C++20 default equality operator.
    bool operator==( const CommandId& ) const = default;
};

/// @brief Required specialization to allow CommandId to be used as a key in QHash.
inline size_t qHash( const qone::sdk::CommandId& key, size_t seed = 0 ) {
    return qHash( key.value, seed );
}

/// @brief Stream operator for logging.
inline QDebug operator<<( QDebug dbg, const CommandId& id ) {
    dbg.nospace() << id.value;
    return dbg.space();
}

} // namespace qone::sdk
