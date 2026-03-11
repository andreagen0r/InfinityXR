#include <Infinity/core/services/httpservice.h>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

using namespace Qt::StringLiterals;

namespace qone::core {

// ---------------------------------------------------------
// ASYNCHRONOUS JOB
// ---------------------------------------------------------
class HttpJob : public QObject {

public:
    HttpJob( QNetworkAccessManager* manager, QNetworkRequest request, QByteArray payload, QString verb, int timeoutMs,
             int retries, sdk::IHttpRequest::Callback callback )
        : m_manager( manager )
        , m_request( std::move( request ) )
        , m_payload( std::move( payload ) )
        , m_verb( std::move( verb ) )
        , m_timeoutMs( timeoutMs )
        , m_retries( retries )
        , m_callback( std::move( callback ) ) {}

    void start() {
        m_isTimeout = false;

        if ( m_verb == "GET"_L1 ) {
            m_reply = m_manager->get( m_request );
        } else if ( m_verb == "POST"_L1 ) {
            m_reply = m_manager->post( m_request, m_payload );
        } else if ( m_verb == "PUT"_L1 ) {
            m_reply = m_manager->put( m_request, m_payload );
        } else if ( m_verb == "DELETE"_L1 ) {
            m_reply = m_manager->sendCustomRequest( m_request, "DELETE"_ba );
        }

        m_timeoutTimer.setSingleShot( true );
        m_timeoutTimer.setInterval( m_timeoutMs );
        connect( &m_timeoutTimer, &QTimer::timeout, this, &HttpJob::onTimeout );
        m_timeoutTimer.start();

        connect( m_reply, &QNetworkReply::finished, this, &HttpJob::onFinished );
    }

private:
    void onTimeout() {
        if ( m_reply && m_reply->isRunning() ) {
            m_isTimeout = true;
            m_reply->abort(); // Forces onFinished with a cancellation error.
        }
    }

    void onFinished() {
        m_timeoutTimer.stop();
        sdk::HttpResult result;

        if ( m_reply->error() == QNetworkReply::NoError ) {
            sdk::HttpResponse response;
            response.statusCode = m_reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
            response.body = m_reply->readAll();

            for ( const auto& headerName : m_reply->rawHeaderList() ) {
                response.headers.insert( QString::fromUtf8( headerName ),
                                         QString::fromUtf8( m_reply->rawHeader( headerName ) ) );
            }
            result.response = std::move( response );
            finalize( std::move( result ) );
            return;
        }

        // Retry Mechanism
        if ( m_retries > 0 && !m_isTimeout ) {
            m_retries--;
            m_reply->deleteLater();
            m_reply = nullptr;
            start(); // Restart the job internally.
            return;
        }

        // Qt Error Translation to SDK
        sdk::HttpError error;
        error.message = m_reply->errorString();
        error.httpStatusCode = m_reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();

        if ( m_isTimeout ) {
            error.type = sdk::HttpError::Type::Timeout;
            error.message = tr( "Request timed out." );
        } else {
            error.type = sdk::HttpError::Type::NetworkError;
        }

        result.error = std::move( error );
        finalize( std::move( result ) );
    }

    void finalize( sdk::HttpResult result ) {

        if ( m_callback ) {
            m_callback( std::move( result ) );
        }

        if ( m_reply ) {
            m_reply->deleteLater();
        }

        this->deleteLater();
    }

    QNetworkAccessManager* m_manager;
    QNetworkRequest m_request;
    QByteArray m_payload;
    QString m_verb;
    int m_timeoutMs;
    int m_retries;
    bool m_isTimeout { false };
    sdk::IHttpRequest::Callback m_callback;
    QNetworkReply* m_reply { nullptr };
    QTimer m_timeoutTimer;
};

// ---------------------------------------------------------
// FLUENT BUILDER
// ---------------------------------------------------------
class HttpRequestBuilder : public sdk::IHttpRequest {

public:
    HttpRequestBuilder( HttpService* service, const QUrl& url )
        : m_service( service )
        , m_request( url ) {}

    sdk::IHttpRequest& withHeader( const QString& key, const QString& value ) override {
        m_request.setRawHeader( key.toUtf8(), value.toUtf8() );
        return *this;
    }

    sdk::IHttpRequest& withTimeout( int milliseconds ) override {
        m_timeoutMs = milliseconds;
        return *this;
    }

    sdk::IHttpRequest& withRetries( int maxRetries ) override {
        m_retries = maxRetries;
        return *this;
    }

    void get( Callback callback ) override {
        ( new HttpJob( m_service->networkManager(), m_request, QByteArray(), "GET"_L1, m_timeoutMs, m_retries,
                       std::move( callback ) ) )
            ->start();
    }

    void post( const QByteArray& payload, Callback callback ) override {
        ( new HttpJob( m_service->networkManager(), m_request, payload, "POST"_L1, m_timeoutMs, m_retries,
                       std::move( callback ) ) )
            ->start();
    }

    void put( const QByteArray& payload, Callback callback ) override {
        ( new HttpJob( m_service->networkManager(), m_request, payload, "PUT"_L1, m_timeoutMs, m_retries,
                       std::move( callback ) ) )
            ->start();
    }

    void del( Callback callback ) override {
        ( new HttpJob( m_service->networkManager(), m_request, QByteArray(), "DELETE"_L1, m_timeoutMs, m_retries,
                       std::move( callback ) ) )
            ->start();
    }

private:
    HttpService* m_service;
    QNetworkRequest m_request;
    int m_timeoutMs { 10000 }; // Default: 10 seconds
    int m_retries { 0 };       // Default: 0 retries
};

// ---------------------------------------------------------
// CORE SERVICE
// ---------------------------------------------------------
HttpService::HttpService( QObject* parent )
    : QObject { parent }
    , m_manager( std::make_unique<QNetworkAccessManager>() ) {}

HttpService::~HttpService() = default;

QNetworkAccessManager* HttpService::networkManager() const {
    return m_manager.get();
}


std::unique_ptr<sdk::IHttpRequest> HttpService::request( const QUrl& url ) {
    return std::make_unique<HttpRequestBuilder>( this, url );
}

} // namespace qone::core
