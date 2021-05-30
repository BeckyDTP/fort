#include "logmanager.h"

#include <QCoreApplication>
#include <QDebug>

#include "../conf/confmanager.h"
#include "../driver/drivercommon.h"
#include "../driver/drivermanager.h"
#include "../driver/driverworker.h"
#include "../fortmanager.h"
#include "../stat/statmanager.h"
#include "../util/dateutil.h"
#include "../util/osutil.h"
#include "logbuffer.h"
#include "logentryblocked.h"
#include "logentryblockedip.h"
#include "logentryprocnew.h"
#include "logentrystattraf.h"
#include "logentrytime.h"

LogManager::LogManager(FortManager *fortManager, QObject *parent) :
    QObject(parent), m_fortManager(fortManager)
{
}

ConfManager *LogManager::confManager() const
{
    return fortManager()->confManager();
}

StatManager *LogManager::statManager() const
{
    return fortManager()->statManager();
}

DriverWorker *LogManager::driverWorker() const
{
    return fortManager()->driverManager()->driverWorker();
}

void LogManager::setActive(bool active)
{
    if (m_active != active) {
        m_active = active;

        if (m_active) {
            readLogAsync();
        } else {
            cancelAsyncIo();
        }

        emit activeChanged();
    }
}

void LogManager::setErrorMessage(const QString &errorMessage)
{
    if (m_errorMessage != errorMessage) {
        m_errorMessage = errorMessage;
        emit errorMessageChanged();
    }
}

qint64 LogManager::currentUnixTime() const
{
    return m_currentUnixTime != 0 ? m_currentUnixTime : DateUtil::getUnixTime();
}

void LogManager::setCurrentUnixTime(qint64 unixTime)
{
    m_currentUnixTime = unixTime;
}

void LogManager::initialize()
{
    connect(driverWorker(), &DriverWorker::readLogResult, this, &LogManager::processLogBuffer,
            Qt::QueuedConnection);
}

void LogManager::close()
{
    QCoreApplication::sendPostedEvents(this);

    disconnect(driverWorker());
}

void LogManager::readLogAsync()
{
    LogBuffer *logBuffer = getFreeBuffer();

    if (!driverWorker()->readLogAsync(logBuffer)) {
        addFreeBuffer(logBuffer);
    }
}

void LogManager::cancelAsyncIo()
{
    driverWorker()->cancelAsyncIo();
}

LogBuffer *LogManager::getFreeBuffer()
{
    if (m_freeBuffers.isEmpty())
        return new LogBuffer(DriverCommon::bufferSize(), this);

    return m_freeBuffers.takeLast();
}

void LogManager::addFreeBuffer(LogBuffer *logBuffer)
{
    m_freeBuffers.append(logBuffer);
}

void LogManager::processLogBuffer(LogBuffer *logBuffer, bool success, quint32 errorCode)
{
    if (m_active) {
        readLogAsync();
    }

    if (success) {
        readLogEntries(logBuffer);
    } else {
        const auto errorMessage = OsUtil::errorMessage(errorCode);
        setErrorMessage(errorMessage);
    }

    logBuffer->reset();
    addFreeBuffer(logBuffer);
}

void LogManager::readLogEntries(LogBuffer *logBuffer)
{
    for (;;) {
        const auto logType = logBuffer->peekEntryType();

        switch (logType) {
        case LogEntry::AppBlocked: {
            LogEntryBlocked blockedEntry;
            logBuffer->readEntryBlocked(&blockedEntry);
            confManager()->logBlockedApp(blockedEntry);
        } break;
        case LogEntry::AppBlockedIp: {
            LogEntryBlockedIp blockedIpEntry;
            logBuffer->readEntryBlockedIp(&blockedIpEntry);
            statManager()->logBlockedIp(blockedIpEntry, currentUnixTime());
        } break;
        case LogEntry::ProcNew: {
            LogEntryProcNew procNewEntry;
            logBuffer->readEntryProcNew(&procNewEntry);
            statManager()->logProcNew(procNewEntry, currentUnixTime());
        } break;
        case LogEntry::StatTraf: {
            LogEntryStatTraf statTrafEntry;
            logBuffer->readEntryStatTraf(&statTrafEntry);
            statManager()->logStatTraf(statTrafEntry, currentUnixTime());
        } break;
        case LogEntry::Time: {
            LogEntryTime timeEntry;
            logBuffer->readEntryTime(&timeEntry);
            setCurrentUnixTime(timeEntry.unixTime());
        } break;
        default:
            if (logBuffer->offset() < logBuffer->top()) {
                qCritical() << "Unknown Log entry:" << logType << logBuffer->offset()
                            << logBuffer->top()
                            << QByteArray(logBuffer->array().constData() + logBuffer->offset(),
                                       logBuffer->top() - logBuffer->offset());
            }
            return;
        }
    }
}
