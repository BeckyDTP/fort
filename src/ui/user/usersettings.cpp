#include "usersettings.h"

#include <fort_version.h>

#include "../fortsettings.h"

UserSettings::UserSettings(QObject *parent) : Settings(parent), m_iniUser(this) { }

void UserSettings::initialize(FortSettings *settings)
{
    setupIni(settings->userPath() + APP_BASE + ".user.ini");

    iniUser().setDefaultLanguage(settings->defaultLanguage());
}

void UserSettings::migrateIniOnStartup()
{
    const int version = iniVersion();
    if (version == appVersion())
        return;

    // COMPAT: v3.4.0: .ini ~> .user.ini
    if (version < 0x030400) {
        setCacheValue("statWindow/trafUnit", ini()->value("stat/trafUnit"));
        setCacheValue("statWindow/geometry", ini()->value("connWindow/geometry"));
        setCacheValue("statWindow/connListHeader", ini()->value("connWindow/connListHeader"));
        setCacheValue("statWindow/connListHeaderVersion",
                ini()->value("connWindow/connListHeaderVersion"));
        setCacheValue("statWindow/autoScroll", ini()->value("connWindow/autoScroll"));
        setCacheValue("statWindow/showHostNames", ini()->value("connWindow/showHostNames"));
        setCacheValue("statWindow/maximized", ini()->value("connWindow/maximized"));
    }
}

void UserSettings::migrateIniOnWrite()
{
    const int version = iniVersion();
    if (version == appVersion())
        return;

    Settings::migrateIniOnWrite();

    // COMPAT: v3.4.0: .ini ~> .user.ini
    if (version < 0x030400) {
        removeIniKey("base/debug");
        removeIniKey("base/console");
        removeIniKey("base/passwordHash");
        removeIniKey("confFlags");
        removeIniKey("stat");
        removeIniKey("quota");
        removeIniKey("graphWindow/alwaysOnTop");
        removeIniKey("graphWindow/frameless");
        removeIniKey("graphWindow/clickThrough");
        removeIniKey("graphWindow/hideOnHover");
        removeIniKey("graphWindow/opacity");
        removeIniKey("graphWindow/hoverOpacity");
        removeIniKey("graphWindow/maxSeconds");
        removeIniKey("graphWindow/color");
        removeIniKey("graphWindow/colorIn");
        removeIniKey("graphWindow/colorOut");
        removeIniKey("graphWindow/axisColor");
        removeIniKey("graphWindow/tickLabelColor");
        removeIniKey("graphWindow/labelColor");
        removeIniKey("graphWindow/gridColor");
        removeIniKey("connWindow");
        ini()->setValue("statWindow/trafUnit", cacheValue("statWindow/trafUnit"));
        ini()->setValue("statWindow/geometry", cacheValue("statWindow/geometry"));
        ini()->setValue("statWindow/connListHeader", cacheValue("statWindow/connListHeader"));
        ini()->setValue(
                "statWindow/connListHeaderVersion", cacheValue("statWindow/connListHeaderVersion"));
        ini()->setValue("statWindow/autoScroll", cacheValue("statWindow/autoScroll"));
        ini()->setValue("statWindow/showHostNames", cacheValue("statWindow/showHostNames"));
        ini()->setValue("statWindow/maximized", cacheValue("statWindow/maximized"));
    }
}
