#include "rpcmanager.h"

#include "../conf/firewallconf.h"
#include "../control/controlmanager.h"
#include "../control/controlworker.h"
#include "../fortmanager.h"
#include "../fortsettings.h"
#include "../rpc/appinfomanagerrpc.h"
#include "../rpc/confmanagerrpc.h"
#include "../rpc/drivermanagerrpc.h"
#include "../rpc/quotamanagerrpc.h"
#include "../rpc/statmanagerrpc.h"
#include "../rpc/taskmanagerrpc.h"

RpcManager::RpcManager(FortManager *fortManager, QObject *parent) :
    QObject(parent), m_fortManager(fortManager)
{
}

FortSettings *RpcManager::settings() const
{
    return fortManager()->settings();
}

ControlManager *RpcManager::controlManager() const
{
    return fortManager()->controlManager();
}

AppInfoManager *RpcManager::appInfoManager() const
{
    return fortManager()->appInfoManager();
}

ConfManager *RpcManager::confManager() const
{
    return fortManager()->confManager();
}

DriverManager *RpcManager::driverManager() const
{
    return fortManager()->driverManager();
}

QuotaManager *RpcManager::quotaManager() const
{
    return fortManager()->quotaManager();
}

StatManager *RpcManager::statManager() const
{
    return fortManager()->statManager();
}

TaskManager *RpcManager::taskManager() const
{
    return fortManager()->taskManager();
}

void RpcManager::initialize()
{
    if (settings()->isService()) {
        setupServerSignals();
    } else {
        setupClient();
    }
}

void RpcManager::setupServerSignals()
{
    setupAppInfoManagerSignals();
    setupConfManagerSignals();
    setupDriverManagerSignals();
    setupQuotaManagerSignals();
    setupStatManagerSignals();
    setupTaskManagerSignals();
}

void RpcManager::setupAppInfoManagerSignals()
{
    connect(appInfoManager(), &AppInfoManager::lookupFinished, this,
            [&](const QString &appPath, const AppInfo & /*appInfo*/) {
                invokeOnClients(Control::Rpc_AppInfoManager_checkLookupFinished, { appPath });
            });
}

void RpcManager::setupConfManagerSignals()
{
    connect(confManager(), &ConfManager::confChanged, this, [&](bool /*onlyFlags*/) {
        const QVariant confVar = confManager()->conf()->toVariant(true);
        invokeOnClients(Control::Rpc_ConfManager_onConfChanged, { confVar });
    });

    connect(confManager(), &ConfManager::appEndTimesUpdated, this,
            [&] { invokeOnClients(Control::Rpc_ConfManager_onAppEndTimesUpdated); });
    connect(confManager(), &ConfManager::appAdded, this, [&](bool alerted) {
        invokeOnClients(Control::Rpc_ConfManager_onAppAdded, { alerted });
    });
    connect(confManager(), &ConfManager::appRemoved, this,
            [&] { invokeOnClients(Control::Rpc_ConfManager_onAppRemoved); });
    connect(confManager(), &ConfManager::appUpdated, this,
            [&] { invokeOnClients(Control::Rpc_ConfManager_onAppUpdated); });

    connect(confManager(), &ConfManager::zoneAdded, this,
            [&] { invokeOnClients(Control::Rpc_ConfManager_onZoneAdded); });
    connect(confManager(), &ConfManager::zoneRemoved, this, [&](int zoneId) {
        invokeOnClients(Control::Rpc_ConfManager_onZoneRemoved, { zoneId });
    });
    connect(confManager(), &ConfManager::zoneUpdated, this,
            [&] { invokeOnClients(Control::Rpc_ConfManager_onZoneUpdated); });
}

void RpcManager::setupDriverManagerSignals()
{
    const auto updateClientStates = [&] {
        invokeOnClients(Control::Rpc_DriverManager_updateState, driverManager_updateState_args());
    };
    connect(driverManager(), &DriverManager::errorCodeChanged, this, updateClientStates);
    connect(driverManager(), &DriverManager::isDeviceOpenedChanged, this, updateClientStates);
}

void RpcManager::setupQuotaManagerSignals()
{
    connect(quotaManager(), &QuotaManager::alert, this, [&](qint8 alertType) {
        invokeOnClients(Control::Rpc_QuotaManager_alert, { alertType });
    });
}

void RpcManager::setupStatManagerSignals()
{
    connect(statManager(), &StatManager::appCreated, this,
            [&](qint64 appId, const QString &appPath) {
                invokeOnClients(Control::Rpc_StatManager_appCreated, { appId, appPath });
            });
    connect(statManager(), &StatManager::trafficAdded, this,
            [&](qint64 unixTime, quint32 inBytes, quint32 outBytes) {
                invokeOnClients(
                        Control::Rpc_StatManager_trafficAdded, { unixTime, inBytes, outBytes });
            });
}

void RpcManager::setupTaskManagerSignals()
{
    connect(taskManager(), &TaskManager::taskStarted, this, [&](qint8 taskType) {
        invokeOnClients(Control::Rpc_TaskManager_taskStarted, { taskType });
    });
    connect(taskManager(), &TaskManager::taskFinished, this, [&](qint8 taskType) {
        invokeOnClients(Control::Rpc_TaskManager_taskFinished, { taskType });
    });
}

void RpcManager::setupClient()
{
    m_client = controlManager()->newServiceClient(this);
    invokeOnServer(Control::Rpc_RpcManager_initClient);
}

void RpcManager::invokeOnServer(Control::Command cmd, const QVariantList &args)
{
    client()->sendCommand(cmd, args);
}

bool RpcManager::waitResult()
{
    m_resultCommand = Control::CommandNone;

    do {
        if (!client()->waitForRead())
            return false;
    } while (m_resultCommand == Control::CommandNone);

    return true;
}

void RpcManager::sendResult(ControlWorker *w, bool ok, const QVariantList &args)
{
    w->sendCommand(ok ? Control::Rpc_Result_Ok : Control::Rpc_Result_Error, args);
}

void RpcManager::invokeOnClients(Control::Command cmd, const QVariantList &args)
{
    const QByteArray buffer = ControlWorker::buildCommandData(cmd, args);

    const auto clients = controlManager()->clients();
    for (ControlWorker *w : clients) {
        if (!w->isServiceClient())
            continue;

        w->sendCommandData(buffer);
    }
}

bool RpcManager::checkClientValidated(ControlWorker *w) const
{
    return !settings()->isPasswordRequired() || w->isClientValidated();
}

void RpcManager::initClientOnServer(ControlWorker *w) const
{
    w->setIsServiceClient(true);

    w->sendCommand(Control::Rpc_DriverManager_updateState, driverManager_updateState_args());
}

QVariantList RpcManager::driverManager_updateState_args() const
{
    return { driverManager()->errorCode(), driverManager()->isDeviceOpened() };
}

bool RpcManager::processCommandRpc(
        ControlWorker *w, Control::Command cmd, const QVariantList &args, QString &errorMessage)
{
    switch (cmd) {
    case Control::Rpc_Result_Ok:
    case Control::Rpc_Result_Error:
        m_resultCommand = cmd;
        m_resultArgs = args;
        return true;

    case Control::Rpc_RpcManager_initClient:
        initClientOnServer(w);
        return true;

    case Control::Rpc_AppInfoManager_lookupAppInfo:
    case Control::Rpc_AppInfoManager_checkLookupFinished:
        return processAppInfoManagerRpc(cmd, args);

    case Control::Rpc_ConfManager_save:
        if (!checkClientValidated(w))
            return false;
        Q_FALLTHROUGH();
    case Control::Rpc_ConfManager_onConfChanged:
    case Control::Rpc_ConfManager_onAppEndTimesUpdated:
    case Control::Rpc_ConfManager_onAppAdded:
    case Control::Rpc_ConfManager_onAppRemoved:
    case Control::Rpc_ConfManager_onAppUpdated:
    case Control::Rpc_ConfManager_onZoneAdded:
    case Control::Rpc_ConfManager_onZoneRemoved:
    case Control::Rpc_ConfManager_onZoneUpdated:
        return processConfManagerRpc(w, cmd, args);

    case Control::Rpc_DriverManager_reinstallDriver:
    case Control::Rpc_DriverManager_uninstallDriver:
        if (!checkClientValidated(w))
            return false;
        Q_FALLTHROUGH();
    case Control::Rpc_DriverManager_updateState:
        return processDriverManagerRpc(cmd, args);

    case Control::Rpc_QuotaManager_alert:
        return processQuotaManagerRpc(cmd, args);

    case Control::Rpc_StatManager_appCreated:
    case Control::Rpc_StatManager_trafficAdded:
        return processStatManagerRpc(cmd, args);

    case Control::Rpc_TaskManager_runTask:
    case Control::Rpc_TaskManager_abortTask:
        if (!checkClientValidated(w))
            return false;
        Q_FALLTHROUGH();
    case Control::Rpc_TaskManager_taskStarted:
    case Control::Rpc_TaskManager_taskFinished:
        return processTaskManagerRpc(cmd, args);

    default:
        errorMessage = "Unknown command";
    }

    return false;
}

bool RpcManager::processAppInfoManagerRpc(Control::Command cmd, const QVariantList &args)
{
    switch (cmd) {
    case Control::Rpc_AppInfoManager_lookupAppInfo:
        appInfoManager()->lookupAppInfo(args.value(0).toString());
        return true;
    case Control::Rpc_AppInfoManager_checkLookupFinished:
        appInfoManager()->checkLookupFinished(args.value(0).toString());
        return true;
    default:
        return false;
    }
}

bool RpcManager::processConfManagerRpc(
        ControlWorker *w, Control::Command cmd, const QVariantList &args)
{
    switch (cmd) {
    case Control::Rpc_ConfManager_save: {
        const bool ok = confManager()->saveVariant(args.value(0));
        sendResult(w, ok);
        return true;
    }
    case Control::Rpc_ConfManager_onConfChanged:
        if (auto cm = qobject_cast<ConfManagerRpc *>(confManager())) {
            cm->onConfChanged(args.value(0));
        }
        return true;
    case Control::Rpc_ConfManager_onAppEndTimesUpdated:
        emit confManager()->appEndTimesUpdated();
        return true;
    case Control::Rpc_ConfManager_onAppAdded:
        emit confManager()->appAdded(args.value(0).toBool());
        return true;
    case Control::Rpc_ConfManager_onAppRemoved:
        emit confManager()->appRemoved();
        return true;
    case Control::Rpc_ConfManager_onAppUpdated:
        emit confManager()->appUpdated();
        return true;
    case Control::Rpc_ConfManager_onZoneAdded:
        emit confManager()->zoneAdded();
        return true;
    case Control::Rpc_ConfManager_onZoneRemoved:
        emit confManager()->zoneRemoved(args.value(0).toInt());
        return true;
    case Control::Rpc_ConfManager_onZoneUpdated:
        emit confManager()->zoneUpdated();
        return true;
    default:
        return false;
    }
}

bool RpcManager::processDriverManagerRpc(Control::Command cmd, const QVariantList &args)
{
    switch (cmd) {
    case Control::Rpc_DriverManager_reinstallDriver:
        fortManager()->installDriver();
        return true;
    case Control::Rpc_DriverManager_uninstallDriver:
        fortManager()->removeDriver();
        return true;
    case Control::Rpc_DriverManager_updateState:
        if (auto dm = qobject_cast<DriverManagerRpc *>(driverManager())) {
            dm->updateState(args.value(0).toUInt(), args.value(1).toBool());
        }
        return true;
    default:
        return false;
    }
}

bool RpcManager::processQuotaManagerRpc(Control::Command cmd, const QVariantList &args)
{
    switch (cmd) {
    case Control::Rpc_QuotaManager_alert:
        emit quotaManager()->alert(args.value(0).toInt());
        return true;
    default:
        return false;
    }
}

bool RpcManager::processStatManagerRpc(Control::Command cmd, const QVariantList &args)
{
    switch (cmd) {
    case Control::Rpc_StatManager_appCreated:
        if (auto sm = qobject_cast<StatManagerRpc *>(statManager())) {
            emit sm->appCreated(args.value(0).toLongLong(), args.value(1).toString());
        }
        return true;
    case Control::Rpc_StatManager_trafficAdded:
        if (auto sm = qobject_cast<StatManagerRpc *>(statManager())) {
            emit sm->trafficAdded(
                    args.value(0).toLongLong(), args.value(1).toUInt(), args.value(2).toUInt());
        }
        return true;
    default:
        return false;
    }
}

bool RpcManager::processTaskManagerRpc(Control::Command cmd, const QVariantList &args)
{
    switch (cmd) {
    case Control::Rpc_TaskManager_runTask:
        taskManager()->runTask(args.value(0).toInt());
        return true;
    case Control::Rpc_TaskManager_abortTask:
        taskManager()->abortTask(args.value(0).toInt());
        return true;
    case Control::Rpc_TaskManager_taskStarted:
        if (auto tm = qobject_cast<TaskManagerRpc *>(taskManager())) {
            tm->onTaskStarted(args.value(0).toInt());
        }
        return true;
    case Control::Rpc_TaskManager_taskFinished:
        if (auto tm = qobject_cast<TaskManagerRpc *>(taskManager())) {
            tm->onTaskFinished(args.value(0).toInt());
        }
        return true;
    default:
        return false;
    }
}
