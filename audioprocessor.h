#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>

#include "streammanager.h"
#include "fftmanager.h"

#define SAMPLE_RATE 44100
#define FREQ_NYQUIST SAMPLE_RATE / 2

#define BUFFER_SIZE 1024
#define FFT_SIZE 1024
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
