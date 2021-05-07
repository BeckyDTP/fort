#ifndef CONNECTIONSWINDOW_H
#define CONNECTIONSWINDOW_H

#include "../../util/window/widgetwindow.h"

QT_FORWARD_DECLARE_CLASS(QCheckBox)
QT_FORWARD_DECLARE_CLASS(QPushButton)

class AppInfoCache;
class AppInfoRow;
class ConfManager;
class ConnListModel;
class ConnectionsController;
class FirewallConf;
class FortManager;
class FortSettings;
class IniOptions;
class TableView;
class WidgetWindowStateWatcher;

class ConnectionsWindow : public WidgetWindow
{
    Q_OBJECT

public:
    explicit ConnectionsWindow(FortManager *fortManager, QWidget *parent = nullptr);

    ConnectionsController *ctrl() const { return m_ctrl; }
    FortManager *fortManager() const;
    FortSettings *settings() const;
    ConfManager *confManager() const;
    FirewallConf *conf() const;
    IniOptions *ini() const;
    ConnListModel *connListModel() const;
    AppInfoCache *appInfoCache() const;

    void saveWindowState();
    void restoreWindowState();

private:
    void setupController();
    void setupStateWatcher();

    void retranslateUi();

    void setupUi();
    QLayout *setupHeader();
    void setupLogOptions();
    void setupLogAllowedIp();
    void setupLogBlockedIp();
    void setupAutoScroll();
    void setupShowHostNames();
    void setupTableConnList();
    void setupTableConnListHeader();
    void setupAppInfoRow();
    void setupTableConnsChanged();

    void syncAutoScroll();
    void syncShowHostNames();

    void deleteConn(int row);

    int connListCurrentIndex() const;
    QString connListCurrentPath() const;

private:
    ConnectionsController *m_ctrl = nullptr;
    WidgetWindowStateWatcher *m_stateWatcher = nullptr;

    QPushButton *m_btEdit = nullptr;
    QAction *m_actCopy = nullptr;
    QAction *m_actAddProgram = nullptr;
    QAction *m_actRemoveConn = nullptr;
    QAction *m_actClearConns = nullptr;
    QPushButton *m_btLogOptions = nullptr;
    QCheckBox *m_cbLogAllowedIp = nullptr;
    QCheckBox *m_cbLogBlockedIp = nullptr;
    QCheckBox *m_cbAutoScroll = nullptr;
    QCheckBox *m_cbShowHostNames = nullptr;
    TableView *m_connListView = nullptr;
    AppInfoRow *m_appInfoRow = nullptr;
};

#endif // CONNECTIONSWINDOW_H
