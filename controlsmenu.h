#ifndef CONTROLSMENU_H
#define CONTROLSMENU_H

#include <QWidget>

#include "audioprocessor.h"

class ControlsMenu : public QWidget
{
    Q_OBJECT
public:
    explicit ControlsMenu(AudioProcessor* audioProcessor, QWidget *parent = nullptr);

private:
    AudioProcessor* audioProcessor;
};

#endif // CONTROLSMENU_H
