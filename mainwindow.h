#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "audioprocessor.h"
#include "spectropage.h"
#include "settingspage.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    AudioProcessor audioProcessor;

    SpectroPage* spectroPage;
    SettingsPage* settingsPage;

    QStackedWidget* stackedWidget;

private slots:
    void spectroSwitch();
    void settingsSwitch();
};
#endif // MAINWINDOW_H
