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
    // Floating point PCM data
    // Range: [0, 1]
    std::vector<float>* iVecBuffer;

    // Floating point FFT data
    // Range: [0, 1]
    std::vector<float> vecFFT;
}
FFTData;


class FFTManager : public QObject
{
    Q_OBJECT
public:
    explicit FFTManager
    (
        std::vector<float>* iVecBuffer,
        unsigned int fftSize = FFT_SIZE,
        int direction = FFT_DIRECTION,
        unsigned int flags = FFT_FLAGS,
        QObject *parent = nullptr
    );
    ~FFTManager();

    void FFT();

    FFTData fftData;

public slots:
    void update();

private:
    fftw_plan fftPlan;

    fftw_complex* dataIn;
    fftw_complex* dataOut;

    unsigned int fftSize;
    int direction;
    unsigned int flags;

signals:
};

#endif // FFTMANAGER_H
