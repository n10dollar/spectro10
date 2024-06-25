#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "spectropage.h"
#include "settingspage.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    SpectroPage spectroPage;
    SettingsPage settingsPage;
};
#endif // MAINWINDOW_H
