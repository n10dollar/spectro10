#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>

#include "streammanager.h"
#include "fftmanager.h"

#define FREQ_NYQUIST 44100 / 2

#define FFT_SIZE 512
#define NYQUIST_SIZE FFT_SIZE / 2

class AudioProcessor : public QObject
{
    Q_OBJECT
public:
    explicit AudioProcessor(QObject *parent = nullptr);

    StreamManager streamManager;
    FFTManager fftManager;

signals:
};

#endif // AUDIOPROCESSOR_H
