#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>

#include "streammanager.h"
#include "fftmanager.h"

typedef struct
{
    StreamData* streamData;
    FFTData* fftData;

    // Range: [-1, 1]
    std::vector<float> iVecBufferAvg;
    std::vector<float> oVecBufferAvg;

    // Range: [0, fftSize]
    std::vector<float> oVecFFTAvg;

    // Range: [0, 1]
    std::vector<float> vecOscilliscope;
    std::vector<float> vecSpectrogram;
}
AudioData;


class AudioProcessor : public QObject
{
    Q_OBJECT
public:
    explicit AudioProcessor(QObject *parent = nullptr);

    StreamManager streamManager;
    FFTManager fftManager;

    AudioData audioData;

public slots:
    void update();

private:
    void calculateAverages();
    void processOscilliscope();
    void processSpectrogram();
};

#endif // AUDIOPROCESSOR_H
