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

void RpcManager::sendResult(ControlWorker *w, bool ok)
{
    w->sendCommand(ok ? Control::Rpc_Result_Ok : Control::Rpc_Result_Error);
}

void RpcManager::invokeOnClients(Control::Command cmd, const QVariantList &args)
{
    const auto clients = controlManager()->clients();
    for (ControlWorker *w : clients) {
        if (!w->isServiceClient())
            continue;

        w->sendCommand(cmd, args);
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
        return true;
    case Control::Rpc_RpcManager_initClient:
        initClientOnServer(w);
        return true;
    case Control::Rpc_AppInfoManager_lookupAppInfo:
        appInfoManager()->lookupAppInfo(args.value(0).toString());
        return true;
    case Control::Rpc_AppInfoManager_checkLookupFinished:
        appInfoManager()->checkLookupFinished(args.value(0).toString());
        return true;
    case Control::Rpc_ConfManager_onConfChanged:
        if (auto cm = qobject_cast<ConfManagerRpc *>(confManager())) {
            cm->onConfChanged(args.value(0));
        }
        return true;
    case Control::Rpc_ConfManager_save: {
        const bool ok = confManager()->saveVariant(args.value(0));
        sendResult(w, ok);
        return true;
    }
    case Control::Rpc_DriverManager_updateState:
        if (auto dm = qobject_cast<DriverManagerRpc *>(driverManager())) {
            dm->updateState(args.value(0).toUInt(), args.value(1).toBool());
        }
        return true;
    case Control::Rpc_DriverManager_reinstallDriver:
        if (checkClientValidated(w)) {
            fortManager()->installDriver();
        }
        return true;
    case Control::Rpc_DriverManager_uninstallDriver:
        if (checkClientValidated(w)) {
            fortManager()->removeDriver();
        }
        return true;
    case Control::Rpc_QuotaManager_alert:
        emit quotaManager()->alert(args.value(0).toInt());
        return true;
    case Control::Rpc_StatManager_:
        statManager();
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
    case Control::Rpc_TaskManager_runTask:
        taskManager()->runTask(args.value(0).toInt());
        return true;
    case Control::Rpc_TaskManager_abortTask:
        taskManager()->abortTask(args.value(0).toInt());
        return true;
    default:
        errorMessage = "Unknown command";
    }

    return false;
}
