#ifndef LOGENTRYBLOCKEDIP_H
#define LOGENTRYBLOCKEDIP_H

#include "logentryblocked.h"

class LogEntryBlockedIp : public LogEntryBlocked
{
public:
    enum LogBlockReason {
        ReasonNone = -1,
        // synchronize with FORT_BLOCK_REASON_*
        ReasonUnknown = 0,
        ReasonIpInet,
        ReasonReauth,
        ReasonProgram,
        ReasonAppGroupFound,
        ReasonAppGroupDefault
    };

    explicit LogEntryBlockedIp(quint8 blockReason = 0, quint8 proto = 0, quint16 localPort = 0,
            quint16 remotePort = 0, quint32 localIp = 0, quint32 remoteIp = 0, quint32 pid = 0,
            const QString &kernelPath = QString());

    LogEntry::LogType type() const override { return AppBlockedIp; }

    quint8 blockReason() const { return m_blockReason; }
    void setBlockReason(quint8 blockReason);

    quint8 proto() const { return m_proto; }
    void setProto(quint8 proto);

    quint16 localPort() const { return m_localPort; }
    void setLocalPort(quint16 port);

    quint16 remotePort() const { return m_remotePort; }
    void setRemotePort(quint16 port);

    quint32 localIp() const { return m_localIp; }
    void setLocalIp(quint32 ip);

    quint32 remoteIp() const { return m_remoteIp; }
    void setRemoteIp(quint32 ip);

private:
    quint8 m_blockReason = 0;
    quint8 m_proto = 0;
    quint16 m_localPort = 0;
    quint16 m_remotePort = 0;
    quint32 m_localIp = 0;
    quint32 m_remoteIp = 0;
};

#endif // LOGENTRYBLOCKEDIP_H
