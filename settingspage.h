#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>

#include "audioprocessor.h"

class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(AudioProcessor* audioProcessor, QWidget *parent = nullptr);

private:
    AudioProcessor* audioProcessor;
};

#endif // SETTINGSPAGE_H
