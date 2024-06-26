#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QWidget>

#include "audioprocessor.h"

class Spectrogram : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrogram(AudioProcessor* audioProcessor, QWidget *parent = nullptr);

private:
    AudioProcessor* audioProcessor;
};

#endif // SPECTROGRAM_H
