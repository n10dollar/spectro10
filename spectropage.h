#ifndef SPECTROPAGE_H
#define SPECTROPAGE_H

#include <QWidget>

#include "controlsmenu.h"
#include "oscilloscope.h"
#include "spectrogram.h"

class SpectroPage : public QWidget
{
    Q_OBJECT
public:
    explicit SpectroPage(QWidget *parent = nullptr);

private:
    ControlsMenu controlsMenu;

    Oscilloscope preOscilliscope;
    Oscilloscope postOscilliscope;

    Spectrogram preSpectrogram;
    Spectrogram postSpectrogram;

signals:
};

#endif // SPECTROPAGE_H
