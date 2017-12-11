#ifndef FIREWALLCONF_H
#define FIREWALLCONF_H

#include <QObject>
#include <QQmlListProperty>
#include <QVariant>

QT_FORWARD_DECLARE_CLASS(AddressGroup)
QT_FORWARD_DECLARE_CLASS(AppGroup)

#define DEFAULT_APP_GROUP_BITS          0xFFFF
#define DEFAULT_TRAF_HOUR_KEEP_DAYS     90  // ~3 months
#define DEFAULT_TRAF_DAY_KEEP_DAYS      356  // ~1 year
#define DEFAULT_TRAF_MONTH_KEEP_MONTHS  360  // ~3 years

class FirewallConf : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool provBoot READ provBoot WRITE setProvBoot NOTIFY provBootChanged)
    Q_PROPERTY(bool filterEnabled READ filterEnabled WRITE setFilterEnabled NOTIFY filterEnabledChanged)
    Q_PROPERTY(bool resolveAddress READ resolveAddress WRITE setResolveAddress NOTIFY resolveAddressChanged)
    Q_PROPERTY(bool logBlocked READ logBlocked WRITE setLogBlocked NOTIFY logBlockedChanged)
    Q_PROPERTY(bool logStat READ logStat WRITE setLogStat NOTIFY logStatChanged)
    Q_PROPERTY(bool appBlockAll READ appBlockAll WRITE setAppBlockAll NOTIFY appBlockAllChanged)
    Q_PROPERTY(bool appAllowAll READ appAllowAll WRITE setAppAllowAll NOTIFY appAllowAllChanged)
    Q_PROPERTY(int trafHourKeepDays READ trafHourKeepDays WRITE setTrafHourKeepDays NOTIFY trafHourKeepDaysChanged)
    Q_PROPERTY(int trafDayKeepDays READ trafDayKeepDays WRITE setTrafDayKeepDays NOTIFY trafDayKeepDaysChanged)
    Q_PROPERTY(int trafMonthKeepMonths READ trafMonthKeepMonths WRITE setTrafMonthKeepMonths NOTIFY trafMonthKeepMonthsChanged)
    Q_PROPERTY(int trafUnit READ trafUnit WRITE setTrafUnit NOTIFY trafUnitChanged)
    Q_PROPERTY(AddressGroup *ipInclude READ ipInclude CONSTANT)
    Q_PROPERTY(AddressGroup *ipExclude READ ipExclude CONSTANT)
    Q_PROPERTY(QQmlListProperty<AppGroup> appGroups READ appGroups NOTIFY appGroupsChanged)
    Q_CLASSINFO("DefaultProperty", "appGroups")

public:
    enum TrafUnit {
        UnitAdaptive = 0,
        UnitBytes,
        UnitKB,
        UnitMB,
        UnitGB,
        UnitTB
    };
    Q_ENUM(TrafUnit)

    explicit FirewallConf(QObject *parent = nullptr);

    bool provBoot() const { return m_provBoot; }
    void setProvBoot(bool provBoot);

    bool filterEnabled() const { return m_filterEnabled; }
    void setFilterEnabled(bool filterEnabled);

    bool resolveAddress() const { return m_resolveAddress; }
    void setResolveAddress(bool resolveAddress);

    bool logBlocked() const { return m_logBlocked; }
    void setLogBlocked(bool logBlocked);

    bool logStat() const { return m_logStat; }
    void setLogStat(bool logStat);

    bool appBlockAll() const { return m_appBlockAll; }
    void setAppBlockAll(bool appBlockAll);

    bool appAllowAll() const { return m_appAllowAll; }
    void setAppAllowAll(bool appAllowAll);

    int trafHourKeepDays() const { return m_trafHourKeepDays; }
    void setTrafHourKeepDays(int trafHourKeepDays);

    int trafDayKeepDays() const { return m_trafDayKeepDays; }
    void setTrafDayKeepDays(int trafDayKeepDays);

    int trafMonthKeepMonths() const { return m_trafMonthKeepMonths; }
    void setTrafMonthKeepMonths(int trafMonthKeepMonths);

    int trafUnit() const { return m_trafUnit; }
    void setTrafUnit(int trafUnit);

    quint32 appGroupBits() const;
    void setAppGroupBits(quint32 groupBits);

    AddressGroup *ipInclude() const { return m_ipInclude; }
    AddressGroup *ipExclude() const { return m_ipExclude; }

    const QList<AppGroup *> &appGroupsList() const { return m_appGroups; }
    QQmlListProperty<AppGroup> appGroups();

    void copyFlags(const FirewallConf &o);
    void copyImmediateValues(const FirewallConf &o);

    QVariant toVariant() const;
    void fromVariant(const QVariant &v);

signals:
    void provBootChanged();
    void filterEnabledChanged();
    void resolveAddressChanged();
    void logBlockedChanged();
    void logStatChanged();
    void appBlockAllChanged();
    void appAllowAllChanged();
    void trafHourKeepDaysChanged();
    void trafDayKeepDaysChanged();
    void trafMonthKeepMonthsChanged();
    void trafUnitChanged();
    void appGroupsChanged();

public slots:
    void addAppGroup(AppGroup *appGroup, int to = -1);
    void addAppGroupByName(const QString &name);
    void moveAppGroup(int from, int to);
    void removeAppGroup(int from, int to);

private:
    uint m_provBoot         : 1;
    uint m_filterEnabled    : 1;

    uint m_resolveAddress   : 1;

    uint m_logBlocked       : 1;
    uint m_logStat          : 1;

    uint m_appBlockAll      : 1;
    uint m_appAllowAll      : 1;

    int m_trafHourKeepDays;
    int m_trafDayKeepDays;
    int m_trafMonthKeepMonths;

    TrafUnit m_trafUnit;

    AddressGroup *m_ipInclude;
    AddressGroup *m_ipExclude;

    QList<AppGroup *> m_appGroups;
};

#endif // FIREWALLCONF_H
