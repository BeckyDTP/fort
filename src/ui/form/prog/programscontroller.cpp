#include "programscontroller.h"

#include <appinfo/appinfocache.h>
#include <conf/confappmanager.h>
#include <manager/windowmanager.h>
#include <model/applistmodel.h>
#include <util/ioc/ioccontainer.h>

namespace {

void showErrorMessage(const QString &errorMessage)
{
    IoC<WindowManager>()->showErrorBox(
            errorMessage, ProgramsController::tr("App Configuration Error"));
}

}

ProgramsController::ProgramsController(QObject *parent) :
    BaseController(parent), m_appListModel(new AppListModel(this))
{
}

AppInfoCache *ProgramsController::appInfoCache() const
{
    return IoC<AppInfoCache>();
}

void ProgramsController::initialize()
{
    appListModel()->initialize();
}

bool ProgramsController::addOrUpdateApp(const App &app, bool onlyUpdate)
{
    return confAppManager()->addOrUpdateApp(app, onlyUpdate);
}

bool ProgramsController::updateApp(const App &app)
{
    return confAppManager()->updateApp(app);
}

bool ProgramsController::updateAppName(qint64 appId, const QString &appName)
{
    return confAppManager()->updateAppName(appId, appName);
}

void ProgramsController::updateAppsBlocked(
        const QVector<qint64> &appIdList, bool blocked, bool killProcess)
{
    if (!confAppManager()->updateAppsBlocked(appIdList, blocked, killProcess)) {
        showErrorMessage(tr("Cannot update program's state"));
    }
}

void ProgramsController::deleteApps(const QVector<qint64> &appIdList)
{
    if (!confAppManager()->deleteApps(appIdList)) {
        showErrorMessage(tr("Cannot delete program"));
    }
}

void ProgramsController::purgeApps()
{
    if (!confAppManager()->purgeApps()) {
        showErrorMessage(tr("Cannot purge obsolete programs"));
    }
}
