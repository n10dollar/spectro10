#include "mainwindow.h"

#include <QStackedWidget>

#include "spectropage.h"
#include "settingspage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SpectroPage* spectroPage = new SpectroPage(&audioProcessor);
    SettingsPage* settingsPage = new SettingsPage(&audioProcessor);

    QStackedWidget* stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(&spectroPage);
}

MainWindow::~MainWindow() {}
