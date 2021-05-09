#ifndef RULESPAGE_H
#define RULESPAGE_H

#include "basepage.h"

class RulesPage : public BasePage
{
    Q_OBJECT

public:
    explicit RulesPage(OptionsController *ctrl = nullptr, QWidget *parent = nullptr);

protected slots:
    void onSaveWindowState(IniOptions *ini) override;
    void onRestoreWindowState(IniOptions *ini) override;

    void onRetranslateUi() override;

private:
    void setupUi();
};

#endif // RULESPAGE_H
