#ifndef FORTSETTINGS_H
#define FORTSETTINGS_H

#include "util/ini/settings.h"
#include "util/ioc/iocobject.h"

class EnvManager;
class FirewallConf;

class FortSettings : public Settings, public IocObject
{
    Q_OBJECT

public:
    explicit FortSettings(QObject *parent = nullptr);

    QString passwordHash() const { return iniText("base/passwordHash"); }
    void setPasswordHash(const QString &v) { setIniValue("base/passwordHash", v); }

    bool isDefaultProfilePath() const { return m_isDefaultProfilePath; }
    bool noCache() const { return m_noCache; }

    bool isService() const { return m_isService; }
    bool hasService() const { return m_hasService; }

    bool isMaster() const { return !hasService() || isService(); }

    bool isUserAdmin() const { return m_isUserAdmin; }

    QString defaultLanguage() const { return m_defaultLanguage; }

    QString profilePath() const { return m_profilePath; }

    QString confFilePath() const;

    QString statPath() const { return m_statPath; }
    QString statFilePath() const;

    QString logsPath() const { return m_logsPath; }

    QString cachePath() const { return m_cachePath; }
    QString cacheFilePath() const;

    QString userPath() const { return m_userPath; }

    QString controlCommand() const { return m_controlCommand; }

    const QStringList &args() const { return m_args; }

    const QStringList &appArguments() const { return m_appArguments; }

    bool passwordChecked() const { return m_passwordChecked; }
    int passwordUnlockType() const { return m_passwordUnlockType; }

    bool hasPassword() const { return !passwordHash().isEmpty(); }
    void setPassword(const QString &password);
    bool checkPassword(const QString &password) const;

    bool isPasswordRequired();
    void setPasswordChecked(bool checked, int unlockType = 0);
    void resetCheckedPassword(int unlockType = 0);

    void setupGlobal();
    void initialize(const QStringList &args, EnvManager *envManager = nullptr);

    bool wasMigrated() const;
    bool canMigrate(QString &viaVersion) const;

    static QString defaultProfilePath(bool hasService);

signals:
    void passwordCheckedChanged();

public slots:
    void readConfIni(FirewallConf &conf) const;
    void writeConfIni(const FirewallConf &conf);

protected:
    void migrateIniOnStartup() override;
    void migrateIniOnWrite() override;

private:
    void processArguments(const QStringList &args);

    void setupPaths(EnvManager *envManager);
    void createPaths();

private:
    uint m_isDefaultProfilePath : 1;
    uint m_noCache : 1;
    uint m_isService : 1;
    uint m_hasService : 1;
    uint m_isUserAdmin : 1;

    uint m_passwordChecked : 1;
    uint m_passwordUnlockType : 3;

    QString m_defaultLanguage;
    QString m_profilePath;
    QString m_statPath;
    QString m_logsPath;
    QString m_cachePath;
    QString m_userPath;
    QString m_controlCommand;
    QStringList m_args;

    QStringList m_appArguments;
};

#endif // FORTSETTINGS_H
