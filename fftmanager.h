#ifndef FFTMANAGER_H
#define FFTMANAGER_H

#include <QObject>

#include <vector>
#include <fftw3.h>

#include "streammanager.h"

#define FFT_SIZE BUFFER_SIZE
#define FFT_DIRECTION FFTW_FORWARD
#define FFT_FLAGS FFTW_MEASURE

typedef struct
{
    // Floating point PCM data
    // Range: [0, 1]
    std::vector<std::vector<float>>* iVecFFTs;

    // FFT output
    std::vector<std::vector<float>>* oVecFFTs;
}
FFTData;


typedef struct
{
    fftw_plan fftPlan;

    fftw_complex* dataIn;
    fftw_complex* dataOut;

    unsigned int fftSize;
    int direction;
    unsigned int flags;
}
FFTParams;


class FFTManager : public QObject
{
    Q_OBJECT
public:
    explicit FFTManager
    (
        unsigned int fftSize = FFT_SIZE,
        int direction = FFT_DIRECTION,
        unsigned int flags = FFT_FLAGS,
        QObject *parent = nullptr
    );
    ~FFTManager();

    void resize(unsigned int fftSize);

    FFTData fftData;

public slots:
    void update();

private:
    FFTParams fftParams;

    void FFT
    (
        std::vector<std::vector<float>>& iVecFFTs,
        std::vector<std::vector<float>>& oVecFFTs
    );

    void FFT
    (
        std::vector<float>& iVecFFT,
        std::vector<float>& oVecFFT
    );

signals:
};

#endif // FFTMANAGER_H
