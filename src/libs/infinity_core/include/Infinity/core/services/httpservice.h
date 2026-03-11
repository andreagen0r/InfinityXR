/// @file httpservice.h
/// @brief Concrete implementation of the HTTP service using Qt Network modules.
/// @author André Luz <andreagenor@gmail.com>

#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <memory>

#include <Infinity/sdk/contracts/IHttpService.h>

#include <Infinity/core/core_export.h>

namespace qone::core {

/// @class HttpService
/// @brief Core-level implementation of the RESTful web request engine.
///
/// Wraps QNetworkAccessManager to provide the Fluent Builder API defined in the SDK.
class CORE_EXPORT HttpService final : public QObject, public sdk::IHttpService {

    Q_OBJECT
    Q_INTERFACES( qone::sdk::IHttpService )

public:
    /// @brief Default constructor.
    explicit HttpService( QObject* parent = nullptr );

    /// @brief Destructor.
    ~HttpService() override;

    /// @brief Initiates a new HTTP request builder.
    /// @param url The target endpoint URL.
    /// @return A unique pointer to the configured @ref sdk::IHttpRequest builder.
    [[nodiscard]] std::unique_ptr<sdk::IHttpRequest> request( const QUrl& url ) override;

    /// @brief Retrieves the internal Qt network manager.
    /// @return Pointer to the active @ref QNetworkAccessManager.
    QNetworkAccessManager* networkManager() const;

private:
    std::unique_ptr<QNetworkAccessManager> m_manager; ///< The underlying network engine.
};

} // namespace qone::core
