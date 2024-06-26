#ifndef SPECTROPAGE_H
#define SPECTROPAGE_H

#include <QWidget>

#include "audioprocessor.h"
#include "controlsmenu.h"
#include "oscilloscope.h"
#include "spectrogram.h"

#define UPDATE_INTERVAL 10

class SpectroPage : public QWidget
{
    Q_OBJECT
public:
    explicit SpectroPage(AudioProcessor* audioProcessor, QWidget *parent = nullptr);

private:
    AudioProcessor* audioProcessor;

    ControlsMenu controlsMenu;

    Oscilloscope preOscilliscope;
    Oscilloscope postOscilliscope;

    Spectrogram preSpectrogram;
    Spectrogram postSpectrogram;
};

#endif // SPECTROPAGE_H
