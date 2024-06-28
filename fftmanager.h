#ifndef FFTMANAGER_H
#define FFTMANAGER_H

#include <QObject>

#include <vector>
#include <fftw3.h>

#include "streammanager.h"

#define FFT_SIZE BUFFER_SIZE
#define NYQUIST_SIZE FFT_SIZE / 2

#define FFT_DIRECTION FFTW_FORWARD
#define FFT_FLAGS FFTW_MEASURE

typedef struct
{
    std::vector<std::vector<float>> oVecFFTs;

    unsigned int numFFTBuffers;
    unsigned int fftSize;
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
        std::vector<std::vector<float>>* iVecBuffers,
        unsigned int numFFTBuffers,
        unsigned int fftSize = FFT_SIZE,
        int direction = FFT_DIRECTION,
        unsigned int flags = FFT_FLAGS,
        QObject *parent = nullptr
    );
    ~FFTManager();

    void FFT
    (
        std::vector<float>& iVecFFT,
        std::vector<float>& oVecFFT
    );

    FFTData fftData;

public slots:
    void update();

private:
    FFTParams fftParams;

    // Floating point PCM data
    // Range: [0, 1]
    std::vector<std::vector<float>>* iVecFFTs;

signals:
};

#endif // FFTMANAGER_H
