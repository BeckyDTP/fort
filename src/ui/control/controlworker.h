#ifndef CONTROLWORKER_H
#define CONTROLWORKER_H

#include <QObject>
#include <QVariant>

#include "control.h"

QT_FORWARD_DECLARE_CLASS(QLocalSocket)

class ControlWorker : public QObject
{
    Q_OBJECT

public:
    explicit ControlWorker(QLocalSocket *socket, QObject *parent = nullptr);

    bool isServiceClient() const { return m_isServiceClient; }
    void setIsServiceClient(bool v) { m_isServiceClient = v; }

    bool isClientValidated() const { return m_isClientValidated; }
    void setIsClientValidated(bool v) { m_isClientValidated = v; }

    QLocalSocket *socket() const { return m_socket; }

    void setupForAsync();

    bool sendCommand(Control::Command command, const QVariantList &args);

    bool waitForSent(int msecs = 1000) const;

    static QVariantList buildArgs(const QStringList &list);

signals:
    void requestReady(Control::Command command, const QVariantList &args);

public slots:
    void abort();

private slots:
    void processRequest();

private:
    void clearRequest();
    bool readRequest();

    bool readRequestHeader();

private:
    struct RequestHeader
    {
        RequestHeader(Control::Command command = Control::CommandNone, quint32 dataSize = 0) :
            m_command(command), m_dataSize(dataSize)
        {
        }

        Control::Command command() const { return static_cast<Control::Command>(m_command); }
        quint32 dataSize() const { return m_dataSize; }

        void clear()
        {
            m_command = Control::CommandNone;
            m_dataSize = 0;
        }

    private:
        quint32 m_command : 8;
        quint32 m_dataSize : 24;
    };

    bool m_isServiceClient : 1;
    bool m_isClientValidated : 1;

    RequestHeader m_requestHeader;
    QByteArray m_requestBuffer;

    QLocalSocket *m_socket = nullptr;
};

#endif // CONTROLWORKER_H
