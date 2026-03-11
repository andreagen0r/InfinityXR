/// @file IHttpService.h
/// @brief Service and builder interfaces for RESTful HTTP requests in the SDK.

#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QUrl>
#include <QHash>

#include <functional>
#include <optional>
#include <memory>

#include <Infinity/sdk/infinitysdk_export.h>

namespace qone::sdk {

/// @struct HttpError
/// @brief Encapsulates information about network request failures.
struct INFINITYSDK_EXPORT HttpError {

    /// @enum Type
    /// @brief Classification of the HTTP layer error.
    enum class Type {
        NetworkError, ///< Infrastructure failure (e.g., no connection).
        Timeout,      ///< Request exceeded the maximum allowed time.
        ServerError,  ///< Server returned an error code (e.g., 500).
        ParseError,   ///< Failed to parse the response payload.
        Cancelled,    ///< The request was actively aborted.
    };

    Type type;                ///< The category of the error.
    int httpStatusCode { 0 }; ///< HTTP status code (if applicable).
    QString message;          ///< Detailed message for logging and debugging.
};


/// @struct HttpResponse
/// @brief Represents a successful HTTP response.
struct INFINITYSDK_EXPORT HttpResponse {
    int statusCode { 0 };            ///< HTTP status code (e.g., 200, 201).
    QByteArray body;                 ///< Raw payload data.
    QHash<QString, QString> headers; ///< Response headers.
};


/// @struct HttpResult
/// @brief Wraps the outcome of an HTTP request, providing either a response or an error.
struct INFINITYSDK_EXPORT HttpResult {

    /// @brief Helper to check if the request was successful.
    /// @return `true` if there are no errors, `false` otherwise.
    [[nodiscard]] bool isSuccess() const {
        return !error.has_value();
    }

    std::optional<HttpResponse> response; ///< Contains the response if successful.
    std::optional<HttpError> error;       ///< Contains the error details if it failed.
};

/// @class IHttpRequest
/// @brief Fluent Builder interface for configuring HTTP requests.
class INFINITYSDK_EXPORT IHttpRequest {

public:
    virtual ~IHttpRequest() = default;

    /// @brief Appends a custom header to the request.
    /// @param key The header name (e.g., "Authorization").
    /// @param value The header value.
    /// @return A reference to the builder for method chaining.
    virtual IHttpRequest& withHeader( const QString& key, const QString& value ) = 0;

    /// @brief Sets a timeout for the request.
    /// @param milliseconds Time in milliseconds before the request aborts.
    /// @return A reference to the builder.
    virtual IHttpRequest& withTimeout( int milliseconds ) = 0;

    /// @brief Configures automatic retries upon failure.
    /// @param maxRetries Maximum number of retry attempts.
    /// @return A reference to the builder.
    virtual IHttpRequest& withRetries( int maxRetries ) = 0;

    /// @typedef Callback
    /// @brief Signature for the asynchronous HTTP response handler.
    using Callback = std::function<void( HttpResult )>;

    /// @name Execution Verbs
    /// Methods to trigger the HTTP request asynchronously.
    /// @{
    virtual void get( Callback callback ) = 0;
    virtual void post( const QByteArray& payload, Callback callback ) = 0;
    virtual void put( const QByteArray& payload, Callback callback ) = 0;
    virtual void del( Callback callback ) = 0;
    /// @}
};


/// @class IHttpService
/// @brief Injectable service for safe and structured web requests.
class INFINITYSDK_EXPORT IHttpService {

public:
    static constexpr const char* IID = "qone.infinity.sdk.IHttpService";
    virtual ~IHttpService() = default;

    /// @brief Initiates the creation of a new HTTP request.
    /// @param url The target endpoint URL.
    /// @return A unique pointer to a Builder for request configuration.
    /// @note Implementations should log the initialization:
    [[nodiscard]] virtual std::unique_ptr<IHttpRequest> request( const QUrl& url ) = 0;
};

} // namespace qone::sdk

Q_DECLARE_INTERFACE( qone::sdk::IHttpService, "qone.sdk.IHttpService/1.0" )
