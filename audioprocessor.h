#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>

#include "streammanager.h"
#include "fftmanager.h"

typedef struct
{
    StreamData* streamData;
    FFTData* fftData;

    std::vector<float> iVecBufferAvg;
    std::vector<float> oVecBufferAvg;

    std::vector<float> iVecFFTAvg;
    std::vector<float> oVecFFTAvg;
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
    void createAverages();
};

#endif // AUDIOPROCESSOR_H
