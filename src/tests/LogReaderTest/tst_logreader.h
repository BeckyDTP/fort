#pragma once

#include <QDebug>
#include <QFile>
#include <QSignalSpy>

#include <googletest.h>

#include <conf/addressgroup.h>
#include <conf/appgroup.h>
#include <conf/firewallconf.h>
#include <fortcommon.h>
#include <log/logbuffer.h>
#include <log/logentryblockedip.h>
#include <log/logentrytime.h>
#include <util/conf/confappswalker.h>
#include <util/conf/confutil.h>
#include <util/device.h>
#include <util/envmanager.h>
#include <util/fileutil.h>
#include <util/net/netutil.h>
#include <util/osutil.h>
#include <util/processinfo.h>

class LogReaderTest : public Test
{
    // Test interface
protected:
    void SetUp();
    void TearDown();
};

void LogReaderTest::SetUp() { }

void LogReaderTest::TearDown() { }

namespace {

void validateDriver(Device &device)
{
    ConfUtil confUtil;
    QByteArray buf;

    const int verSize = confUtil.writeVersion(buf);
    ASSERT_NE(verSize, 0);

    ASSERT_TRUE(device.ioctl(FortCommon::ioctlValidate(), buf.data(), verSize));
}

void setConf(Device &device)
{
    EnvManager envManager;
    FirewallConf conf;

    conf.setProvBoot(true);
    conf.setLogBlockedIp(true);

    // Address Groups
    AddressGroup *inetGroup = conf.inetAddressGroup();

    inetGroup->setIncludeAll(true);
    inetGroup->setExcludeAll(false);

    inetGroup->setExcludeText(NetUtil::localIpv4Networks().join('\n'));

    // Application Groups
    conf.addDefaultAppGroup();

    conf.setAppBlockAll(true);
    conf.setAppAllowAll(false);

    ConfUtil confUtil;

    QByteArray buf;
    const int confIoSize = confUtil.write(conf, nullptr, envManager, buf);
    ASSERT_NE(confIoSize, 0);

    ASSERT_TRUE(device.ioctl(FortCommon::ioctlSetConf(), buf.data(), confIoSize));
}

void printLogs(LogBuffer &buf)
{
    for (;;) {
        const LogEntry::LogType logType = buf.peekEntryType();
        if (logType == LogEntry::TypeNone)
            break;

        if (logType == LogEntry::Time) {
            LogEntryTime entry;
            buf.readEntryTime(&entry);
            continue;
        }

        ASSERT_EQ(logType, LogEntry::AppBlockedIp);

        LogEntryBlockedIp entry;
        buf.readEntryBlockedIp(&entry);

        qDebug() << entry.pid() << entry.kernelPath() << entry.path()
                 << NetUtil::ip4ToText(entry.remoteIp()) << entry.remotePort();
    }
}

}

TEST_F(LogReaderTest, LogRead)
{
    Device device;
    ASSERT_TRUE(device.open(FortCommon::deviceName()));

    validateDriver(device);
    setConf(device);

    LogBuffer buf(FortCommon::bufferSize());

    for (;;) {
        int nr;
        QByteArray &array = buf.array();
        ASSERT_TRUE(device.ioctl(
                FortCommon::ioctlGetLog(), nullptr, 0, array.data(), array.size(), &nr));
        buf.reset(nr);
        printLogs(buf);
    }
}
