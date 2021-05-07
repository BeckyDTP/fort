#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include "../../util/window/widgetwindow.h"

class ConfManager;
class FortManager;
class IniOptions;
class MainPage;
class OptionsController;
class WidgetWindowStateWatcher;

class OptionsWindow : public WidgetWindow
{
    Q_OBJECT

public:
    explicit OptionsWindow(FortManager *fortManager, QWidget *parent = nullptr);

    OptionsController *ctrl() const { return m_ctrl; }
    ConfManager *confManager() const;
    IniOptions *ini() const;

    void saveWindowState();
    void restoreWindowState();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupController();
    void setupStateWatcher();

    void retranslateUi();

    void setupUi();

private:
    OptionsController *m_ctrl = nullptr;
    WidgetWindowStateWatcher *m_stateWatcher = nullptr;

    MainPage *m_mainPage = nullptr;
};

#endif // OPTIONSWINDOW_H
