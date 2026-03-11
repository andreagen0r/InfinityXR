#include "systemsignalhandler.h"

#include <QCoreApplication>
#include <QSocketNotifier>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>

#include <Infinity/sdk/logging/logger.h>

using namespace Qt::StringLiterals;

namespace qone::infinity {

int SystemSignalHandler::sigFd[2];

SystemSignalHandler::SystemSignalHandler( QObject* parent )
    : QObject { parent } {
    // Cria um par de sockets conectados para o Self-Pipe Trick
    if ( ::socketpair( AF_UNIX, SOCK_STREAM, 0, sigFd ) ) {
        qCFatal( qoneApp ) << Infinity::Logging::Title( u"Couldn't create signal socketpair"_s );
    }

    m_notifier = new QSocketNotifier( sigFd[1], QSocketNotifier::Read, this );
    connect( m_notifier, &QSocketNotifier::activated, this, &SystemSignalHandler::handleGracefulSignal );
}

SystemSignalHandler::~SystemSignalHandler() {
    m_notifier->setEnabled( false );
    ::close( sigFd[0] );
    ::close( sigFd[1] );
}

void SystemSignalHandler::install() {
    struct sigaction gracefulAction;
    gracefulAction.sa_handler = posixGracefulSignalHandler;
    sigemptyset( &gracefulAction.sa_mask );
    gracefulAction.sa_flags = 0;
    // Sinais de paragem segura
    sigaction( SIGTERM, &gracefulAction, nullptr );
    sigaction( SIGINT, &gracefulAction, nullptr );
    sigaction( SIGQUIT, &gracefulAction, nullptr );
    sigaction( SIGHUP, &gracefulAction, nullptr );

    struct sigaction fatalAction;
    fatalAction.sa_handler = posixFatalSignalHandler;
    sigemptyset( &fatalAction.sa_mask );
    fatalAction.sa_flags = static_cast<int>( SA_RESETHAND );
    // Sinais de Crash
    sigaction( SIGSEGV, &fatalAction, nullptr );
    sigaction( SIGFPE, &fatalAction, nullptr );
    sigaction( SIGABRT, &fatalAction, nullptr );
    sigaction( SIGILL, &fatalAction, nullptr );
    sigaction( SIGBUS, &fatalAction, nullptr );

    // Ignorar SIGPIPE para evitar fechos abruptos em uso de rede
    signal( SIGPIPE, SIG_IGN );
}

// Handler para Sinais Limpos Executa num contexto de interrupção!
void SystemSignalHandler::posixGracefulSignalHandler( int signal ) {
    char a = static_cast<char>( signal );
    // Escreve de forma async-signal-safe no pipe
    ::write( sigFd[0], &a, sizeof( a ) );
}

// Handler para Crash
void SystemSignalHandler::posixFatalSignalHandler( int signal ) {

    qCFatal( qoneApp ) << Infinity::Logging::Title(
        u"Sinal fatal interceptado (%d). O sistema irá encerrar para proteger a memória." )
                       << signal;
}

// Esta slot é ativada no Event Loop principal do Qt
void SystemSignalHandler::handleGracefulSignal() {

    m_notifier->setEnabled( false );
    char sig;
    ::read( sigFd[1], &sig, sizeof( sig ) );

    qInfo( qoneApp ) << "Sinal de encerramento recebido:" << static_cast<int>( sig ) << ". A preparar paragem segura.";

    Q_EMIT gracefulShutdownRequested();

    QCoreApplication::quit();
}

} // namespace qone::infinity
