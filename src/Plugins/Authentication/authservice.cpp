#include "authservice.h"

#include <Infinity/sdk/contracts/AuthEvents.h>
#include <Infinity/sdk/contracts/IEventBus.h>
#include <Infinity/sdk/logging/logger.h>

#include <Infinity/sdk/types/NotificationEvents.h>

#include <QJsonDocument>
#include <QJsonObject>

#include <random>

using namespace Qt::StringLiterals;

AuthService::AuthService( QObject* parent )
    : qone::sdk::IAuthService( parent ) {}

void AuthService::authenticate( const QString& username, const QString& password,
                                std::function<void( qone::sdk::AuthResult )> callback ) {

    if ( !m_httpService ) {
        qCWarning( qoneCore ) << "HTTP Service is not available!";
        if ( callback ) {
            callback( { false, "Internal Error: Network unavailable"_L1, ""_L1 } );
        }
        return;
    }

    // Monta o Payload JSON para a API REST
    QJsonObject jsonReq;
    jsonReq["email"_L1] = username;
    jsonReq["password"_L1] = password;

    const QByteArray payload = QJsonDocument( jsonReq ).toJson( QJsonDocument::Compact );

    qCInfo( qoneNetwork ) << Infinity::Logging::Title( u"Enviando requisicao de login para a API..."_s );

    // API fake só para testes
    m_httpService->request( QUrl( u"https://httpbin.org/post"_s ) )
        ->withHeader( "Content-Type"_L1, "application/json"_L1 )
        .withHeader( "User-Agent"_L1, "Mozilla/5.0"_L1 )
        .withTimeout( 5000 )
        .withRetries( 1 )
        .post( payload, [this, username, callback]( qone::sdk::HttpResult result ) {
            qone::sdk::AuthResult authResult;

            if ( result.isSuccess() ) {
                m_auth = true;

                // Exemplo de como fazer parse do token de resposta
                QJsonDocument jsonRes = QJsonDocument::fromJson( result.response->body );
                QString token = jsonRes.object()["token"_L1].toString();

                authResult = { true, "Success"_L1, token };

                Q_EMIT authenticated( authResult );

                if ( m_eventBus != nullptr ) {
                    qone::sdk::events::UserLoggedIn event;
                    event.username = username;
                    m_eventBus->publish( event );


                    {
                        // Cria diversas notificações com tipos e timeouts diferentes

                        std::random_device rd;
                        std::mt19937 gen( rd() );
                        std::uniform_int_distribution<> dist(
                            0, static_cast<int>( qone::sdk::NotificationSeverity::Neutral ) );

                        std::uniform_int_distribution<> distTimeout( 3500, 6000 );

                        for ( int i = 0; i <= 8; ++i ) {
                            qone::sdk::events::NotificationRequested evt;

                            const int value = dist( gen ); // Gera notificações de diferentes níveis
                            evt.descriptor.severity = static_cast<qone::sdk::NotificationSeverity>( value );

                            evt.descriptor.title = tr( "Teste de Estabilidade" );
                            evt.descriptor.message = tr( "Notificação deslizando na zona: " ) + QString::number( i );

                            const int timeout = distTimeout( gen );
                            evt.descriptor.timeoutMs = timeout;

                            evt.descriptor.requireUserInteraction = ( i == 2 ); // Torna o TopRight required

                            // Faz o cast do número do loop para o Enum de Posição
                            evt.descriptor.position = static_cast<qone::sdk::NotificationPosition>( i );

                            m_eventBus->publish( evt );
                        }
                    }
                }
            } else {
                m_auth = false;
                // Exibe se foi Timeout, Erro de Servidor, etc.
                authResult = { false, result.error->message, ""_L1 };
                qCWarning( qoneNetwork ) << "Falha no Login:"_L1 << result.error->message << "HTTP Status:"_L1
                                         << result.error->httpStatusCode;
            }

            // Avisa o comando local
            if ( callback ) {
                callback( authResult );
            }
        } );
}

bool AuthService::isAuthenticated() const {
    return m_auth;
}

void AuthService::logout() {
    m_auth = false;
}

QString AuthService::loginViewId() const {
    return "view.login"_L1;
}


QUrl AuthService::loginViewSource() const {
    return QUrl( u"qrc:/Plugins/Authentication/LoginView.qml"_s );
}

QList<qone::sdk::ViewDescriptor> AuthService::views() const {
    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Generating View Descriptor for Login"_s );

    const QUrl sourceUrl( loginViewSource() );

    // Confirmação visual da URL gerada
    qCInfo( qoneCore ) << Infinity::Logging::Title( u"Generated URL:"_s ) << sourceUrl;

    qone::sdk::ViewDescriptor desc {
        .id = qone::sdk::ViewId( loginViewId() ),
        .title = "Login"_L1,
        .icon = ""_L1,
        .source = sourceUrl,
        .type = qone::sdk::ViewType::Overlay,
        .dockable = false,
    };
    return { desc };
}

void AuthService::setEventBus( qone::sdk::IEventBus* eventBus ) {
    m_eventBus = eventBus;
}

void AuthService::setHttpService( qone::sdk::IHttpService* httpService ) {
    m_httpService = httpService;
}
