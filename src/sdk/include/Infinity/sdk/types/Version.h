#pragma once

#include <QString>

#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

struct INFINITYSDK_EXPORT Version {
    int major { 1 };
    int minor { 0 };
    int patch { 0 };

    auto operator<=>( const Version& ) const = default;

    constexpr bool isCompatibleWith( const Version& other ) const {
        if ( major != other.major ) {
            return false;
        }

        if ( minor < other.minor ) {
            return false;
        }

        return true;
    }

    QString versionString() const {
        using namespace Qt::StringLiterals;
        return QString::number( major ) % "."_L1 % QString::number( minor ) % "."_L1 % QString::number( patch );
    }
};

} // namespace qone::sdk
