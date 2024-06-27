#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>

#include "streammanager.h"
#include "fftmanager.h"

typedef struct
{
    StreamData* streamData;

    std::vector<float> iVecAvg;
    std::vector<float> oVecAvg;
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
};

#endif // AUDIOPROCESSOR_H
