#include "mainwindow.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    spectroPage = new SpectroPage(&audioProcessor);
    settingsPage = new SettingsPage(&audioProcessor);

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(spectroPage);
    stackedWidget->addWidget(settingsPage);

    // Switch window controls
    QPushButton* spectroButton = new QPushButton(tr("Spectrogram"));
    QPushButton* settingsButton = new QPushButton(tr("Settings"));

    connect(spectroButton, &QPushButton::clicked, this, &MainWindow::spectroSwitch);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::settingsSwitch);

    QHBoxLayout* buttonBox = new QHBoxLayout;
    buttonBox->addWidget(spectroButton);
    buttonBox->addWidget(settingsButton);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonBox);
    mainLayout->addWidget(stackedWidget);

    // Main widget
    QWidget* mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}


MainWindow::~MainWindow() {}


void MainWindow::spectroSwitch()
{
    stackedWidget->setCurrentWidget(spectroPage);
}


void MainWindow::settingsSwitch()
{
    stackedWidget->setCurrentWidget(settingsPage);
}
