/// @file singleton.hpp
/// @brief Thread-safe Meyers Singleton implementation using CRTP.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class Singleton
/// @brief A template class providing a robust and thread-safe Singleton pattern.
///
/// This class utilizes the C++11 static initialization guarantees to ensure
/// safe instantiation in multithreaded environments.
///
/// @tparam T The class type that will inherit from this Singleton (CRTP).
///
/// @section singleton_usage Usage Example
/// @code{.cpp}
/// class Database : public qone::core::Singleton<Database> {
///     friend class qone::core::Singleton<Database>;
/// private:
///     Database() = default; // Mandatory private constructor
/// public:
///     void connect() { ... }
/// };
///
/// // Usage:
/// Database::instance().connect();
/// @endcode
template <class T> class CORE_EXPORT Singleton {

public:
    /// @brief Retrieves the single instance of type T.
    /// @return A static reference to the T instance.
    /// @note The instance is lazily created upon the first call.
    static T& instance() {
        static T instance;
        return instance;
    }

    /// @name Copy and Move Semantics
    /// Deleted to strictly enforce the single instance constraint.
    /// @{
    Singleton( const Singleton& ) = delete;
    Singleton( const Singleton&& ) = delete;
    Singleton& operator=( const Singleton& ) = delete;
    Singleton& operator=( const Singleton&& ) = delete;
    /// @}

protected:
    /// @brief Protected default constructor to allow CRTP inheritance.
    Singleton() = default;

    /// @brief Protected default destructor.
    ~Singleton() = default;
};

} // namespace qone::core
