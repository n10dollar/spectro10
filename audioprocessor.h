#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <QObject>

#include <q/fx/biquad.hpp>
#include <q/support/literals.hpp>

#include "streammanager.h"
#include "fftmanager.h"

typedef struct
{
    StreamData* streamData;
    FFTData* fftData;

    // Range: [0, 1]
    std::vector<std::vector<float>> iVecBuffersNormalized;
    std::vector<std::vector<float>> oVecBuffersNormalized;

    // Range: [0, 1]
    std::vector<float> iVecBufferAvg;
    std::vector<float> oVecBufferAvg;

    // Range: [0, 1]
    std::vector<float> oVecFiltered;

    // Range: [0, fftSize]
    std::vector<float> oVecFFTAvg;

    // Range: [0, 1]
    std::vector<float> vecOscilliscope;
    std::vector<float> vecSpectrogram;
}
AudioData;


using namespace cycfi::q::literals;
typedef struct
{
    // Filters
    std::vector<cycfi::q::biquad*> filters;
}
FilterData;


class AudioProcessor : public QObject
{
    Q_OBJECT
public:
    explicit AudioProcessor(QObject *parent = nullptr);
    ~AudioProcessor();

    AudioData audioData;
    FilterData filterData;

    StreamManager streamManager;
    FFTManager fftManager;

    void filterStream
    (
        std::vector<float>& iVecFilter,
        std::vector<float>& oVecFilter
    );

public slots:
    void update();

private:
    void streamBuffersNormalize();
    void calculateAverages();
    void processOscilliscope();
    void processSpectrogram();
};

#endif // AUDIOPROCESSOR_H
