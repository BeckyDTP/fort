#include "mainpage.h"

#include <QDesktopServices>
#include <QIcon>
#include <QPushButton>
#include <QTabWidget>
#include <QUrl>
#include <QVBoxLayout>

#include "../../../fortsettings.h"
#include "../../../task/taskinfoupdatechecker.h"
#include "../../../task/taskmanager.h"
#include "../optionscontroller.h"
#include "addressespage.h"
#include "applicationspage.h"
#include "optionspage.h"
#include "schedulepage.h"
#include "statisticspage.h"

namespace {

bool openUrlExternally(const QUrl &url)
{
    return QDesktopServices::openUrl(url);
}

}

MainPage::MainPage(OptionsController *ctrl,
                   QWidget *parent) :
    BasePage(ctrl, parent)
{
    setupUi();
}

void MainPage::onRetranslateUi()
{
    m_tabBar->setTabText(0, tr("Options"));
    m_tabBar->setTabText(1, tr("IPv4 Addresses"));
    m_tabBar->setTabText(2, tr("Application Groups"));
    m_tabBar->setTabText(3, tr("Statistics"));
    m_tabBar->setTabText(4, tr("Schedule"));

    m_logsButton->setText(tr("Logs"));
    m_profileButton->setText(tr("Profile"));
    m_statButton->setText(tr("Statistics"));
    m_releasesButton->setText(tr("Releases"));
    m_newVersionButton->setText(tr("New Version!"));

    m_okButton->setText(tr("OK"));
    m_applyButton->setText(tr("Apply"));
    m_cancelButton->setText(tr("Cancel"));
}

void MainPage::onLinkClicked()
{
    auto button = qobject_cast<QAbstractButton *>(sender());
    if (button) {
        openUrlExternally(QUrl::fromLocalFile(button->windowFilePath()));
    }
}

void MainPage::setupUi()
{
    auto layout = new QVBoxLayout();
    layout->setContentsMargins(6, 6, 6, 6);

    // Main Tab Bar
    m_tabBar = new QTabWidget();
    layout->addWidget(m_tabBar);

    m_optionsPage = new OptionsPage(ctrl());
    m_addressesPage = new AddressesPage(ctrl());
    m_applicationsPage = new ApplicationsPage(ctrl());
    m_statisticsPage = new StatisticsPage(ctrl());
    m_schedulePage = new SchedulePage(ctrl());

    m_tabBar->addTab(m_optionsPage, QIcon(":/images/cog.png"), QString());
    m_tabBar->addTab(m_addressesPage, QIcon(":/images/link.png"), QString());
    m_tabBar->addTab(m_applicationsPage, QIcon(":/images/application_double.png"), QString());
    m_tabBar->addTab(m_statisticsPage, QIcon(":/images/chart_line.png"), QString());
    m_tabBar->addTab(m_schedulePage, QIcon(":/images/clock.png"), QString());

    // Dialog butons
    auto buttonsLayout = setupDialogButtons();
    layout->addLayout(buttonsLayout);

    this->setLayout(layout);
}

QLayout *MainPage::setupDialogButtons()
{
    auto buttonsLayout = new QHBoxLayout();

    m_logsButton = createLinkButton(":/images/folder_error.png", settings()->logsPath());
    m_profileButton = createLinkButton(":/images/folder_user.png", settings()->profilePath());
    m_statButton = createLinkButton(":/images/folder_database.png", settings()->statPath());
    m_releasesButton = createLinkButton(":/images/server_go.png", settings()->appUpdatesUrl());
    m_newVersionButton = createLinkButton(":/images/server_compressed.png");

    connect(m_logsButton, &QAbstractButton::clicked, this, &MainPage::onLinkClicked);
    connect(m_profileButton, &QAbstractButton::clicked, this, &MainPage::onLinkClicked);
    connect(m_statButton, &QAbstractButton::clicked, this, &MainPage::onLinkClicked);
    connect(m_releasesButton, &QAbstractButton::clicked, this, &MainPage::onLinkClicked);
    connect(m_newVersionButton, &QAbstractButton::clicked, this, &MainPage::onLinkClicked);

    setupNewVersionButton();

    buttonsLayout->addWidget(m_logsButton);
    buttonsLayout->addWidget(m_profileButton);
    buttonsLayout->addWidget(m_statButton);
    buttonsLayout->addWidget(m_releasesButton);
    buttonsLayout->addWidget(m_newVersionButton);

    buttonsLayout->addStretch(1);

    m_okButton = new QPushButton(QIcon(":/images/tick.png"), QString());
    m_applyButton = new QPushButton(QIcon(":/images/accept.png"), QString());
    m_cancelButton = new QPushButton(QIcon(":/images/cancel.png"), QString());

    connect(m_okButton, &QAbstractButton::clicked, ctrl(), &OptionsController::saveChanges);
    connect(m_applyButton, &QAbstractButton::clicked, ctrl(), &OptionsController::applyChanges);
    connect(m_cancelButton, &QAbstractButton::clicked, ctrl(), &OptionsController::closeWindow);

    setupOkApplyButtons();

    buttonsLayout->addWidget(m_okButton);
    buttonsLayout->addWidget(m_applyButton);
    buttonsLayout->addWidget(m_cancelButton);

    return buttonsLayout;
}

void MainPage::setupNewVersionButton()
{
    const auto refreshNewVersionButton = [&] {
        auto updateChecker = taskManager()->taskInfoUpdateChecker();
        m_newVersionButton->setVisible(!updateChecker->version().isEmpty());
        m_newVersionButton->setWindowFilePath(updateChecker->downloadUrl());
        m_newVersionButton->setToolTip(updateChecker->releaseText());
    };

    refreshNewVersionButton();

    connect(taskManager()->taskInfoUpdateChecker(), &TaskInfoUpdateChecker::versionChanged,
            this, refreshNewVersionButton);
}

void MainPage::setupOkApplyButtons()
{
    const auto refreshOkApplyButtons = [&] {
        const bool anyEdited = ctrl()->anyEdited();
        m_okButton->setEnabled(anyEdited);
        m_applyButton->setEnabled(anyEdited);
    };

    refreshOkApplyButtons();

    connect(ctrl(), &OptionsController::editedChanged, this, refreshOkApplyButtons);
}
