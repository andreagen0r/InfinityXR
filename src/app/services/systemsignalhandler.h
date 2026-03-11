#pragma once

#include <QObject>

class QSocketNotifier;

namespace qone::infinity {

class SystemSignalHandler : public QObject {

    Q_OBJECT

public:
    explicit SystemSignalHandler( QObject* parent = nullptr );
    ~SystemSignalHandler() override;

    void install();

Q_SIGNALS:
    void gracefulShutdownRequested();

private:
    static void posixGracefulSignalHandler( int signal );
    static void posixFatalSignalHandler( int signal );

    // Self-Pipe
    static int sigFd[2];
    QSocketNotifier* m_notifier = nullptr;

private Q_SLOTS:
    void handleGracefulSignal();
};

} // namespace qone::infinity
