#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include "../../util/window/widgetwindow.h"

QT_FORWARD_DECLARE_CLASS(FortManager)
QT_FORWARD_DECLARE_CLASS(MainPage)
QT_FORWARD_DECLARE_CLASS(OptionsController)

class OptionsWindow : public WidgetWindow
{
    Q_OBJECT

public:
    explicit OptionsWindow(FortManager *fortManager,
                           QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    OptionsController *ctrl() { return m_ctrl; }

    void setupUi();

private:
    OptionsController *m_ctrl;

    MainPage *m_mainPage = nullptr;
};

#endif // OPTIONSWINDOW_H
